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

#include <metashell/pragma_environment_save.hpp>
#include <metashell/shell.hpp>

#include <boost/algorithm/string/trim.hpp>

#include <fstream>

using namespace metashell;

pragma_environment_save::pragma_environment_save(const data::config& config_,
                                                 const iface::environment& env_)
  : _config(config_), _env(env_)
{
}

iface::pragma_handler* pragma_environment_save::clone() const
{
  return new pragma_environment_save(_config, _env);
}

std::string pragma_environment_save::arguments() const { return "<path>"; }

std::string pragma_environment_save::description() const
{
  return "Saves the environment into a file. This is disabled by default. It "
         "can be"
         " enabled using the --enable_saving command line argument.";
}

void pragma_environment_save::run(const data::command::iterator&,
                                  const data::command::iterator&,
                                  const data::command::iterator& args_begin_,
                                  const data::command::iterator& args_end_,
                                  iface::displayer& displayer_) const
{
  if (_config.saving_enabled)
  {
    const std::string fn =
        boost::algorithm::trim_copy(tokens_to_string(args_begin_, args_end_));

    if (fn.empty())
    {
      displayer_.show_error(
          "Filename to save the environment into is missing.");
    }
    else
    {
      std::ofstream f(fn.c_str());
      f << _env.get_all() << std::endl;
      if (f.fail() || f.bad())
      {
        displayer_.show_error("Failed to save the environment into file " + fn);
      }
    }
  }
  else
  {
    displayer_.show_error(
        "Saving is disabled. You can enable it using the --enable_saving"
        " command line argument.");
  }
}
