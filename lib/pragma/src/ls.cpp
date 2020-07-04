// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/ls.hpp>

#include <metashell/data/exception.hpp>
#include <metashell/data/include_argument.hpp>

#include <metashell/core/code_complete.hpp>
#include <metashell/core/include_path_cache.hpp>
#include <metashell/core/util.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

#include <algorithm>
#include <set>
#include <vector>

namespace metashell
{
  namespace pragma
  {
    namespace
    {
      std::vector<data::include_argument>
      parse_arguments(const std::string& name_,
                      data::command::iterator begin_,
                      const data::command::iterator& end_)
      {
        std::vector<data::include_argument> result;

        while (begin_ != end_)
        {
          const auto include_arg = data::include_argument::parse(begin_, end_);
          if (include_arg.first)
          {
            result.push_back(*include_arg.first);
            begin_ = include_arg.second;
          }
          else
          {
            const std::string arguments =
                data::tokens_to_string(begin_, end_).value();
            throw data::exception(
                "Argument of " + name_ +
                " is not a header to include. Did you mean <" + arguments +
                "> or \"" + arguments + "\"?");
          }
        }

        if (result.empty())
        {
          const boost::filesystem::path empty;
          result.push_back({data::include_type::sys, empty});
          result.push_back({data::include_type::quote, empty});
        }

        return result;
      }

      void display(iface::displayer& displayer_,
                   const std::string& type_,
                   const std::set<data::include_argument>& paths_,
                   bool extra_new_line_ = false)
      {
        if (!paths_.empty())
        {
          displayer_.show_comment(data::text{type_ + ":"});

          displayer_.show_cpp_code(data::cpp_code(
              boost::algorithm::join(
                  paths_ | boost::adaptors::transformed(
                               [](const data::include_argument& a_) {
                                 return "#include " + data::include_code(a_);
                               }),
                  "\n") +
              (extra_new_line_ ? "\n" : "")));
        }
      }

      data::command::const_iterator
      last_include_start(data::command::const_iterator begin_,
                         data::command::const_iterator end_)
      {
        data::command::const_iterator result = begin_;

        for (auto i = begin_; i != end_; ++i)
        {
          if (type_of(*i) == data::token_type::operator_less)
          {
            const auto j = std::find_if(i, end_, [](const data::token& token_) {
              return type_of(token_) == data::token_type::operator_greater;
            });
            if (j == end_)
            {
              return i;
            }
            else
            {
              i = j;
              result = j + 1;
            }
          }
          else if (type_of(*i) == data::token_type::string_literal)
          {
            result = i + 1;
          }
          else if (data::include_quote_token(*begin_))
          {
            const auto j = std::find_if(i, end_, data::include_quote_token);
            if (j == end_)
            {
              return i;
            }
            else
            {
              i = j;
              result = j + 1;
            }
          }
        }

        return result;
      }
    }

    std::string ls::arguments() const
    {
      return "{<include file>|\"include file\"}";
    }

    std::string ls::description() const
    {
      return "Lists the available header files in the directories specified by "
             "the "
             "arguments.";
    }

    void ls::run(const data::command::iterator& name_begin_,
                 const data::command::iterator& name_end_,
                 const data::command::iterator& args_begin_,
                 const data::command::iterator& args_end_,
                 iface::main_shell& shell_,
                 iface::displayer& displayer_) const
    {
      std::set<data::include_argument> dirs;
      std::set<data::include_argument> headers;

      core::include_path_cache paths(shell_.engine().header_discoverer());

      const boost::filesystem::directory_iterator end;

      for (const data::include_argument& arg : parse_arguments(
               data::tokens_to_string(name_begin_, name_end_).value(),
               args_begin_, args_end_))
      {
        for (const boost::filesystem::path& p :
             paths[{arg.type, data::standard_headers_allowed::all}])
        {
          const boost::filesystem::path path =
              core::resolve_symlink(p / arg.path);

          if (is_regular_file(path))
          {
            headers.insert({arg.type, arg.path});
          }
          else if (is_directory(path))
          {
            for (boost::filesystem::directory_iterator i(path); i != end; ++i)
            {
              const boost::filesystem::path f =
                  core::resolve_symlink(i->path());

              const data::include_argument entry{
                  arg.type, arg.path / i->path().filename()};

              if (is_regular_file(*i))
              {
                headers.insert(entry);
              }
              else if (is_directory(*i))
              {
                dirs.insert(entry);
              }
            }
          }
        }
      }

      display(displayer_, "Directories", dirs, !headers.empty());
      display(displayer_, "Header files", headers);
    }

    data::code_completion
    ls::code_complete(data::command::const_iterator begin_,
                      data::command::const_iterator end_,
                      iface::main_shell& shell_) const
    {
      return core::code_complete::include(last_include_start(begin_, end_),
                                          end_,
                                          shell_.engine().header_discoverer());
    }
  }
}
