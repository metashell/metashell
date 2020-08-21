
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/pragma/mdb.hpp>

#include <metashell/mdb/shell.hpp>

#include <metashell/data/mdb_usage.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <cassert>

namespace metashell
{
  namespace pragma
  {
    mdb::mdb(core::command_processor_queue* cpq_,
             const boost::filesystem::path& mdb_temp_dir_,
             bool preprocessor_,
             core::logger* logger_)
      : _preprocessor(preprocessor_),
        _cpq(cpq_),
        _mdb_temp_dir(mdb_temp_dir_),
        _logger(logger_)
    {
    }

    std::string mdb::arguments() const
    {
      return data::mdb_usage(_preprocessor);
    }

    std::string mdb::description() const
    {
      const std::string name =
          _preprocessor ? "preprocessor debugger" : "metadebugger";
      return "Starts the " + name +
             ". For more information see evaluate in the " + name +
             " command reference.";
    }

    void mdb::run(const data::command::iterator&,
                  const data::command::iterator&,
                  const data::command::iterator& args_begin_,
                  const data::command::iterator& args_end_,
                  iface::main_shell& shell_,
                  iface::displayer& displayer_) const
    {
      assert(_cpq != nullptr);

      const data::mdb_command::arguments_type args(
          tokens_to_string(args_begin_, args_end_).value());

      core::command_processor_queue::cleanup_function restore;
      if (shell_.enabled(data::shell_flag::use_precompiled_headers))
      {
        shell_.enabled(data::shell_flag::use_precompiled_headers, false);
        restore = [&shell_](iface::displayer& displayer_) {
          try
          {
            shell_.enabled(data::shell_flag::use_precompiled_headers, true);
          }
          catch (const std::exception& e_)
          {
            displayer_.show_error(e_.what());
          }
        };
      }

      auto sh = std::make_unique<metashell::mdb::shell>(
          shell_.env(), shell_.engine(), shell_.env_path(), _mdb_temp_dir,
          _preprocessor, _logger);

      if (shell_.get_config().splash_enabled)
      {
        sh->display_splash(displayer_);
      }

      if (!args.empty())
      {
        sh->command_evaluate(args, displayer_);
      }

      _cpq->push(move(sh), move(restore));
    }

    data::code_completion
    mdb::code_complete(data::command::const_iterator begin_,
                       data::command::const_iterator end_,
                       iface::main_shell& shell_) const
    {
      using data::token_type;
      using data::token_category;

      std::set<std::string> allowed{"", "profile", "nocache"};
      if (!_preprocessor)
      {
        allowed.insert("full");
      }

      if (begin_ == end_)
      {
        data::code_completion result;
        for (const std::string& all : allowed)
        {
          result.insert(data::user_input{" -" + all});
        }
        return result;
      }

      auto beg = skip_all_whitespace(begin_, end_);
      if (beg == end_)
      {
        data::code_completion result;
        for (const std::string& all : allowed)
        {
          result.insert(data::user_input{"-" + all});
        }
        return result;
      }

      while (beg != end_ && type_of(*beg) == token_type::operator_minus &&
             !allowed.empty())
      {
        const auto dash_at = beg;

        ++beg;
        if (beg == end_)
        {
          data::code_completion result;
          for (const std::string& all : allowed)
          {
            result.insert(data::user_input{all});
          }
          return result;
        }

        if (type_of(*beg) == token_type::identifier)
        {
          const std::string val = to_string(value(*beg));
          ++beg;
          if (beg == end_)
          {
            for (const std::string& all : allowed)
            {
              if (boost::algorithm::starts_with(all, val))
              {
                return data::code_completion{
                    data::user_input{all.substr(val.size())}};
              }
            }
          }
          else if (allowed.find(val) == allowed.end())
          {
            beg = dash_at;
            break;
          }
          else if (val == "profile" || (val == "full" && !_preprocessor))
          {
            allowed.erase("profile");
            allowed.erase("full");
          }
          else
          {
            allowed.erase(val);
          }
        }
        else if (category(*beg) == token_category::whitespace)
        {
          return data::code_completion{};
        }
        else
        {
          beg = dash_at;
          break;
        }

        beg = skip_whitespace(beg, end_);
      }

      if (skip_whitespace(beg, end_) == end_)
      {
        return data::code_completion{};
      }
      else
      {
        return shell_.code_complete(
            data::user_input{data::join_tokens(beg, end_)}, false);
      }
    }
  }
}
