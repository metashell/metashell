// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/engine_switch.hpp>

#include <metashell/core/code_complete.hpp>
#include <metashell/core/engine.hpp>

#include <stdexcept>

namespace metashell
{
  namespace pragma
  {
    std::string engine_switch::arguments() const { return "<engine name>"; }

    std::string engine_switch::description() const
    {
      return "Switches the engine being used keeping the environment and "
             "config. The engine arguments are transformed (eg. -I arguments "
             "of Clang are interpreted as /I for Visual C++)";
    }

    void engine_switch::run(const data::command::iterator& name_begin_,
                            const data::command::iterator& name_end_,
                            const data::command::iterator& args_begin_,
                            const data::command::iterator& args_end_,
                            iface::main_shell& shell_,
                            iface::displayer& displayer_) const
    {
      const data::cpp_code arg = tokens_to_string(args_begin_, args_end_);
      if (arg.empty())
      {
        displayer_.show_error("Usage: " +
                              tokens_to_string(name_begin_, name_end_) + " " +
                              arguments());
        return;
      }

      const data::real_engine_name name =
          data::parse_real_engine_name(arg.value());

      if (name == shell_.engine().name())
      {
        displayer_.show_comment(data::text("Already using engine " + name));
        return;
      }

      try
      {
        shell_.switch_to(name);

        data::text comment;
        comment.paragraphs.emplace_back("Switched to engine " + name);

        if (const auto limit = core::limitation(shell_.engine()))
        {
          comment.paragraphs.emplace_back();
          comment.paragraphs.emplace_back(*limit);
        }

        displayer_.show_comment(comment);
      }
      catch (const std::exception& e)
      {
        displayer_.show_error("Error switching to engine " + name + ": " +
                              e.what());
      }
      catch (...)
      {
        displayer_.show_error("Error switching to engine " + name +
                              ": unknown exception");
      }
    }

    data::code_completion
    engine_switch::code_complete(data::command::const_iterator begin_,
                                 data::command::const_iterator end_,
                                 iface::main_shell& shell_) const
    {
      return core::code_complete::fixed_values(
          begin_, end_, shell_.available_engines());
    }
  }
}
