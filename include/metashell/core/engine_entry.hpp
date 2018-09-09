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
#include <metashell/data/markdown_string.hpp>

#include <metashell/core/engine_factory.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace core
  {
    class engine_entry
    {
    public:
      engine_entry(engine_factory factory_,
                   std::string args_,
                   data::markdown_string description_,
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
      const data::markdown_string& description() const;

      const std::vector<data::feature>& features() const;

    private:
      engine_factory _factory;
      std::string _args;
      data::markdown_string _description;
      std::vector<data::feature> _features;
    };

    std::string list_features(const engine_entry& engine_);
    data::markdown_string
    list_features_in_markdown(const engine_entry& engine_);
  }
}

#endif
