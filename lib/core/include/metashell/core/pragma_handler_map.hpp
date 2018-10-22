#ifndef METASHELL_PRAGMA_HANDLER_MAP_HPP
#define METASHELL_PRAGMA_HANDLER_MAP_HPP

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

#include <metashell/core/command_processor_queue.hpp>
#include <metashell/core/logger.hpp>
#include <metashell/core/make_unique.hpp>

#include <metashell/data/command.hpp>

#include <metashell/iface/pragma_handler.hpp>

#include <boost/filesystem/path.hpp>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace metashell
{
  namespace core
  {
    class shell;

    class pragma_handler_map
    {
    public:
      pragma_handler_map() = default;

      pragma_handler_map(pragma_handler_map&&) = default;
      pragma_handler_map& operator=(pragma_handler_map&&) = default;

      pragma_handler_map(const pragma_handler_map&) = delete;
      pragma_handler_map& operator=(const pragma_handler_map&) = delete;

      template <class Handler>
      // requires: Handler implements iface::pragma_handler
      pragma_handler_map&& add(const std::string& name_, Handler handler_) &&
      {
        return std::move(*this).add(std::vector<std::string>{name_}, handler_);
      }

      template <class Handler>
      // requires: Handler implements iface::pragma_handler
      pragma_handler_map&& add(const std::string& name1_,
                               const std::string& name2_,
                               Handler handler_) &&
      {
        return std::move(*this).add({name1_, name2_}, handler_);
      }

      template <class Handler>
      // requires: Handler implements iface::pragma_handler
      pragma_handler_map&& add(const std::string& name1_,
                               const std::string& name2_,
                               const std::string& name3_,
                               Handler handler_) &&
      {
        return std::move(*this).add({name1_, name2_, name3_}, handler_);
      }

      template <class Handler>
      // requires: Handler implements iface::pragma_handler
      pragma_handler_map&& add(const std::vector<std::string>& names_,
                               Handler handler_) &&
      {
        assert(!names_.empty());

        _handlers.insert(
            std::make_pair(names_, core::make_unique<Handler>(handler_)));
        return std::move(*this);
      }

      void process(const data::command::iterator& p_,
                   const data::command::iterator& end_,
                   iface::displayer& displayer_) const;

      typedef std::map<std::vector<std::string>,
                       std::unique_ptr<iface::pragma_handler>>::const_iterator
          iterator;
      typedef iterator const_iterator;

      iterator begin() const;
      iterator end() const;
      iterator find(const std::vector<std::string>& p_) const;

      static pragma_handler_map
      build_default(shell& shell_,
                    command_processor_queue* cpq_,
                    const boost::filesystem::path& mdb_temp_dir_,
                    logger* logger_);

    private:
      std::map<std::vector<std::string>, std::unique_ptr<iface::pragma_handler>>
          _handlers;
    };
  }
}

#endif
