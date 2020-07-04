// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/environment_save.hpp>

#include <metashell/core/code_complete.hpp>

#include <boost/filesystem.hpp>

#include <cctype>
#include <fstream>

namespace metashell
{
  namespace pragma
  {
    namespace
    {
      template <char C>
      bool is(char c_)
      {
        return c_ == C || c_ == std::toupper(C);
      }

      bool cpp_source(const boost::filesystem::path& path_)
      {
        const std::string ext = extension(path_);
        const auto len = ext.size();
        if (len > 1 && ext[0] == '.')
        {
          switch (ext[1])
          {
          case 'c':
          case 'C':
          case 'h':
          case 'H':
            if (len == 2)
            {
              return true;
            }
            else if (is<'p'>(ext[2]))
            {
              return len == 4 && is<'p'>(ext[3]);
            }
            else if (is<'x'>(ext[2]))
            {
              return len == 4 && is<'x'>(ext[3]);
            }
            break;
          }
        }
        return false;
      }
    }

    std::string environment_save::arguments() const { return "<path>"; }

    std::string environment_save::description() const
    {
      return "Saves the environment into a file. This is disabled by default. "
             "It can be enabled using the --enable_saving command line "
             "argument.";
    }

    void environment_save::run(const data::command::iterator&,
                               const data::command::iterator&,
                               const data::command::iterator& args_begin_,
                               const data::command::iterator& args_end_,
                               iface::main_shell& shell_,
                               iface::displayer& displayer_) const
    {
      if (shell_.get_config().saving_enabled)
      {
        const std::string fn =
            trim_copy(tokens_to_string(args_begin_, args_end_)).value();

        if (fn.empty())
        {
          displayer_.show_error(
              "Filename to save the environment into is missing.");
        }
        else
        {
          std::ofstream f(fn.c_str());
          f << shell_.env().get_all() << std::endl;
          if (f.fail() || f.bad())
          {
            displayer_.show_error("Failed to save the environment into file " +
                                  fn);
          }
        }
      }
      else
      {
        displayer_.show_error(
            "Saving is disabled by the --disable_saving command line "
            "argument.");
      }
    }

    data::code_completion
    environment_save::code_complete(data::command::const_iterator begin_,
                                    data::command::const_iterator end_,
                                    iface::main_shell&) const
    {
      return core::code_complete::files(
          data::user_input{
              tokens_to_string(data::skip_all_whitespace(begin_, end_), end_)},
          data::user_input{begin_ == end_ ? " " : ""}, cpp_source);
    }
  }
}
