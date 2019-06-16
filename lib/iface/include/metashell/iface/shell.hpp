#ifndef METASHELL_IFACE_SHELL_HPP
#define METASHELL_IFACE_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/command_processor.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/engine.hpp>
#include <metashell/iface/environment.hpp>

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/pragma_name.hpp>
#include <metashell/data/shell_flag.hpp>

#include <boost/filesystem/path.hpp>

#include <map>
#include <memory>

namespace metashell
{
  namespace iface
  {
    class pragma_handler;

    class shell : public command_processor
    {
    public:
      virtual const data::config& get_config() const = 0;
      virtual data::config& get_config() = 0;

      virtual iface::engine& engine() = 0;

      virtual bool store_in_buffer(const data::cpp_code& s_,
                                   displayer& displayer_) = 0;

      virtual const environment& env() const = 0;
      virtual environment& env() = 0;

      virtual void reset_environment() = 0;
      virtual void push_environment() = 0;
      virtual void pop_environment() = 0;
      virtual void display_environment_stack_size(displayer& displayer_) = 0;
      virtual void rebuild_environment() = 0;

      virtual bool preprocess(displayer& displayer_,
                              const data::cpp_code& exp_,
                              bool process_directives_) = 0;

      virtual void run_metaprogram(const data::cpp_code& s_,
                                   displayer& displayer_) = 0;

      virtual const std::map<data::pragma_name,
                             std::unique_ptr<iface::pragma_handler>>&
      pragma_handlers() const = 0;

      virtual boost::filesystem::path env_path() const = 0;

      virtual void stop() = 0;

      virtual bool enabled(data::shell_flag) const = 0;
      virtual void enabled(data::shell_flag, bool) = 0;
    };
  }
}

#endif
