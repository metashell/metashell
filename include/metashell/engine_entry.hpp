#ifndef METASHELL_ENGINE_ENTRY_HPP
#define METASHELL_ENGINE_ENTRY_HPP

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

#include <metashell/data/feature.hpp>
#include <metashell/engine_factory.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  class engine_entry
  {
  public:
    engine_entry(engine_factory factory_,
                 std::string args_,
                 std::string description_,
                 std::vector<data::feature> features_);

    std::unique_ptr<iface::engine>
    build(const data::config& config_,
          const boost::filesystem::path& internal_dir_,
          const boost::filesystem::path& temp_dir_,
          const boost::filesystem::path& env_filename_,
          iface::environment_detector& env_detector_,
          iface::displayer& displayer_,
          logger* logger_) const;

    const std::string& args() const;
    const std::string& description() const;

    const std::vector<data::feature>& features() const;

  private:
    engine_factory _factory;
    std::string _args;
    std::string _description;
    std::vector<data::feature> _features;
  };
}

#endif
