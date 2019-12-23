// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/all_config.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace metashell
{
  namespace pragma
  {
    std::string all_config::description() const
    {
      return "Lists all available configs.";
    }

    void all_config::run(iface::main_shell& shell_,
                         iface::displayer& displayer_) const
    {
      using boost::algorithm::join;
      using boost::adaptors::transformed;

      const data::config& cfg = shell_.get_config();
      const data::shell_config* active = &cfg.active_shell_config();

      displayer_.show_comment(data::text(
          join(cfg.shell_configs() |
                   transformed([active](const data::shell_config& cfg_) {
                     return (&cfg_ == active ? " * " : "   ") + cfg_.name;
                   }),
               "\n")));
    }
  }
}
