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

#include <metashell/engine/auto/entry.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

#include <metashell/data/exception.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <cassert>
#include <set>
#include <vector>

namespace metashell
{
  namespace engine
  {
    namespace auto_
    {
      namespace
      {
        void insert(std::set<data::feature>& dst_,
                    const std::vector<data::feature>& src_)
        {
          dst_.insert(src_.begin(), src_.end());
        }

        std::vector<data::feature> supported_features(
            const std::map<std::string, core::engine_entry>& engines_)
        {
          std::set<data::feature> result;

          for (const auto& engine : engines_)
          {
            if (engine.second.usable_by_auto())
            {
              insert(result, engine.second.features());
            }
          }

          return std::vector<data::feature>(result.begin(), result.end());
        }

        std::unique_ptr<iface::engine>
        choose_engine(const data::config& config_,
                      const boost::filesystem::path& internal_dir_,
                      const boost::filesystem::path& temp_dir_,
                      const boost::filesystem::path& env_filename_,
                      const std::map<std::string, core::engine_entry>& engines_,
                      iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      core::logger* logger_)
        {
          const data::shell_config& cfg = config_.active_shell_config();

          const std::string args = boost::algorithm::join(cfg.engine_args, " ");

          METASHELL_LOG(
              logger_,
              "auto engine determining engine to use. Arguments: " + args);

          std::vector<std::string> usable;

          for (const auto& engine : engines_)
          {
            if (engine.second.this_engine(cfg.engine_args))
            {
              METASHELL_LOG(
                  logger_, "Engine " + engine.first + " is suitable.");
              usable.push_back(engine.first);
            }
            else
            {
              METASHELL_LOG(
                  logger_, "Engine " + engine.first + " is not suitable.");
            }
          }

          if (usable.empty())
          {
            throw data::exception(
                "\"--engine auto\" could not use any of the available engines "
                "with the following arguments: " +
                args);
          }
          else if (usable.size() == 1)
          {
            METASHELL_LOG(logger_, "auto engine selected " + usable.front());

            const auto engine = engines_.find(usable.front());
            assert(engine != engines_.end());
            return engine->second.build(config_, internal_dir_, temp_dir_,
                                        env_filename_, engines_, env_detector_,
                                        displayer_, logger_);
          }
          else
          {
            throw data::exception(
                "\"--engine auto\" found multiple usabe engines for the "
                "following arguments " +
                args + ". Please use one of " +
                boost::algorithm::join(
                    usable | boost::adaptors::transformed(
                                 [](const std::string& e_) -> std::string {
                                   return "\"--engine " + e_ + "\"";
                                 }),
                    ", ") +
                "instead of \"--engine auto\".");
          }
        }
      }

      core::engine_entry
      entry(const std::map<std::string, core::engine_entry>& engines_)
      {
        return core::engine_entry(
            choose_engine, "",
            data::markdown_string("Chooses an engine based on the compiler "
                                  "specified in the arguments."),
            supported_features(engines_), core::never_used_by_auto());
      }
    }
  }
}
