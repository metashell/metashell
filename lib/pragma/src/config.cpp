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

#include <metashell/pragma/config.hpp>

#include <metashell/core/code_complete.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/uniqued.hpp>

#include <algorithm>
#include <optional>
#include <set>

namespace metashell
{
  namespace pragma
  {
    namespace
    {
      std::optional<std::string>
      element_to_show(const std::optional<data::shell_config_name>& prefix_,
                      const data::shell_config_name& name_)
      {
        if (!prefix_)
        {
          return to_string_with_standard_suffix(name_.front());
        }

        if (size(name_) <= size(*prefix_))
        {
          return std::nullopt;
        }

        auto i = name_.begin();
        for (const auto& e : *prefix_)
        {
          if (e.value() != i->value())
          {
            return std::nullopt;
          }
          ++i;
        }

        return to_string_with_standard_suffix(*i);
      };
    }

    std::string config::arguments() const { return "[<name>]"; }

    std::string config::description() const
    {
      return "Lists the available configs and config folders in config folder "
             "<name>.";
    }

    void config::run(const data::command::iterator&,
                     const data::command::iterator&,
                     const data::command::iterator& args_begin_,
                     const data::command::iterator& args_end_,
                     iface::main_shell& shell_,
                     iface::displayer& displayer_) const
    {
      using boost::algorithm::join;
      using boost::adaptors::transformed;
      using boost::adaptors::filtered;

      const data::config& cfg = shell_.get_config();
      const data::shell_config_name active = cfg.active_shell_config().name;

      const auto arg = tokens_to_string(args_begin_, args_end_);
      const std::optional<data::shell_config_name> prefix =
          arg.empty() ?
              std::nullopt :
              std::make_optional(data::shell_config_name(arg.value()));

      const auto active_to_show = element_to_show(prefix, active);

      const auto element_range =
          cfg.shell_configs() |
          transformed([&prefix](const data::shell_config& cfg_) {
            return element_to_show(prefix, cfg_.name);
          }) |
          filtered([](const auto& element_) { return bool(element_); }) |
          transformed([](const auto& element_) { return *element_; });

      const std::set<std::string> elements(
          element_range.begin(), element_range.end());

      displayer_.show_comment(data::text(
          join(elements | transformed([&active_to_show](const auto& element_) {
                 return (element_ == active_to_show ? " * " : "   ") + element_;
               }),
               "\n")));
    }

    data::code_completion
    config::code_complete(data::command::const_iterator begin_,
                          data::command::const_iterator end_,
                          iface::main_shell& shell_) const
    {
      return core::code_complete::fixed_values(
          begin_, end_,
          shell_.get_config().shell_configs() |
              boost::adaptors::transformed([](const data::shell_config& cfg_) {
                return cfg_.name.value();
              }));
    }
  }
}
