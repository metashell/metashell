#ifndef METASHELL_WAVE_HOOKS_HPP
#define METASHELL_WAVE_HOOKS_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/wave_token.hpp>

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/include_argument.hpp>
#include <metashell/data/token.hpp>

#include <boost/wave.hpp>

#include <boost/filesystem/path.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <set>
#include <sstream>

namespace metashell
{
  class wave_hooks
      : public boost::wave::context_policies::eat_whitespace<wave_token>
  {
  public:
    int lines_of_env = 0;
    int lines_to_ignore_after_env = 0;

    std::function<void(const data::cpp_code&,
                       const boost::optional<std::vector<data::cpp_code>>&,
                       const data::file_location&,
                       const data::file_location&)>
        on_macro_expansion_begin;

    std::function<void(data::cpp_code)> on_rescanning;

    std::function<void(data::cpp_code, int)> on_macro_expansion_end;

    std::function<void(data::token, data::file_location)> on_token_generated;

    std::function<void(data::include_argument, data::file_location)>
        on_include_begin;
    std::function<void()> on_include_end;

    wave_hooks() : _included_files(nullptr) {}

    explicit wave_hooks(std::set<boost::filesystem::path>& included_files_)
      : _included_files(&included_files_)
    {
    }

    template <typename ContextT>
    void opened_include_file(const ContextT&,
                             const std::string&,
                             const std::string& absname_,
                             bool is_system_include_)
    {
      if (_included_files)
      {
        _included_files->insert(absname_);
      }
      if (on_include_begin)
      {
        on_include_begin(data::include_argument(is_system_include_ ?
                                                    data::include_type::sys :
                                                    data::include_type::quote,
                                                absname_),
                         _last_directive_location);
      }
    }

    template <typename ContextT>
    void returning_from_include_file(const ContextT&)
    {
      if (on_include_end)
      {
        on_include_end();
      }
    }

    template <typename ContextT,
              typename TokenT,
              typename ContainerT,
              typename IteratorT>
    bool
    expanding_function_like_macro(const ContextT&,
                                  const TokenT& macrodef_,
                                  const std::vector<TokenT>&,
                                  const ContainerT&,
                                  const TokenT& macrocall_,
                                  const std::vector<ContainerT>& arguments_,
                                  const IteratorT&,
                                  const IteratorT&)
    {
      if (on_macro_expansion_begin)
      {
        std::vector<data::cpp_code> args;
        args.reserve(arguments_.size());
        std::transform(arguments_.begin(), arguments_.end(),
                       std::back_inserter(args), &tokens_to_code<ContainerT>);

        on_macro_expansion_begin(token_to_code(macrodef_), args,
                                 to_file_location(macrocall_),
                                 to_file_location(macrodef_));
      }
      return false;
    }

    template <typename ContextT, typename TokenT, typename ContainerT>
    bool expanding_object_like_macro(const ContextT&,
                                     const TokenT& macrodef_,
                                     const ContainerT&,
                                     const TokenT& macrocall_)
    {
      if (on_macro_expansion_begin)
      {
        on_macro_expansion_begin(token_to_code(macrodef_), boost::none,
                                 to_file_location(macrocall_),
                                 to_file_location(macrodef_));
      }
      return false;
    }

    template <typename ContextT, typename ContainerT>
    void expanded_macro(const ContextT&, const ContainerT& result_)
    {
      if (on_rescanning)
      {
        on_rescanning(tokens_to_code(result_));
      }
    }

    template <typename ContextT, typename ContainerT>
    void rescanned_macro(const ContextT&, const ContainerT& result_)
    {
      if (on_macro_expansion_end)
      {
        on_macro_expansion_end(tokens_to_code(result_), result_.size());
      }
    }

    template <typename ContextT, typename TokenT>
    const TokenT& generated_token(const ContextT&, const TokenT& t_)
    {
      if (!IS_CATEGORY(t_, boost::wave::token_category::EOFTokenType) &&
          on_token_generated)
      {
        on_token_generated(token_from_wave_token(t_), to_file_location(t_));
      }
      return t_;
    }

    template <typename ContextT, typename TokenT>
    bool found_directive(const ContextT&, const TokenT& directive_)
    {
      _last_directive_location = to_file_location(directive_);
      return false;
    }

  private:
    std::set<boost::filesystem::path>* _included_files;
    data::file_location _last_directive_location;

    template <class String>
    static std::string to_std_string(const String& s_)
    {
      std::ostringstream result;
      result << s_;
      return result.str();
    }

    template <class Token>
    static std::string token_to_string(const Token& token_)
    {
      return to_std_string(token_.get_value());
    }

    template <class Token>
    static data::cpp_code token_to_code(const Token& token_)
    {
      return data::cpp_code(token_to_string(token_));
    }

    template <class ContainerT>
    static data::cpp_code tokens_to_code(const ContainerT& tokens_)
    {
      std::ostringstream result;
      for (const auto& t : tokens_)
      {
        result << t.get_value();
      }
      return data::cpp_code(result.str());
    }

    template <class Token>
    data::file_location to_file_location(const Token& token_)
    {
      const auto pos = token_.get_position();
      const std::string fn = to_std_string(pos.get_file());
      int line = pos.get_line();
      if (line > lines_of_env)
      {
        line -= lines_to_ignore_after_env;
      }
      return data::file_location(
          boost::filesystem::path(fn).filename() == "<stdin>" ? "<stdin>" : fn,
          line, pos.get_column());
    }
  };
}

#endif
