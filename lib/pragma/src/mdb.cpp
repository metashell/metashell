
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
                  iface::shell& shell_,
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
  }
}
