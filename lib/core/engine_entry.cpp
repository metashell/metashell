// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine_entry.hpp>

using namespace metashell;

engine_entry::engine_entry(engine_factory factory_,
                           std::string args_,
                           std::string description_)
  : _factory(move(factory_)),
    _args(move(args_)),
    _description(move(description_))
{
}

std::unique_ptr<iface::engine>
engine_entry::build(const data::config& config_,
                    const boost::filesystem::path& internal_dir_,
                    const boost::filesystem::path& temp_dir_,
                    const boost::filesystem::path& env_filename_,
                    iface::environment_detector& env_detector_,
                    iface::displayer& displayer_,
                    logger* logger_) const
{
  return _factory(config_, internal_dir_, temp_dir_, env_filename_,
                  env_detector_, displayer_, logger_);
}

const std::string& engine_entry::args() const { return _args; }

const std::string& engine_entry::description() const { return _description; }
