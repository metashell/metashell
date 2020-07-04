#ifndef METASHELL_PRAGMA_WHICH_HPP
#define METASHELL_PRAGMA_WHICH_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/include_argument.hpp>
#include <metashell/iface/pragma_handler.hpp>

#include <boost/operators.hpp>

#include <iosfwd>

namespace metashell
{
  namespace pragma
  {
    class which : public iface::pragma_handler
    {
    public:
      struct parsed_arguments : boost::equality_comparable<parsed_arguments>
      {
        parsed_arguments(const data::include_argument& header_, bool all_);
        data::include_argument header;
        bool all;
      };

      std::string arguments() const override;
      std::string description() const override;

      void run(const data::command::iterator& name_begin_,
               const data::command::iterator& name_end_,
               const data::command::iterator& args_begin_,
               const data::command::iterator& args_end_,
               iface::main_shell& shell_,
               iface::displayer& displayer_) const override;

      data::code_completion code_complete(data::command::const_iterator,
                                          data::command::const_iterator,
                                          iface::main_shell&) const override;

      static parsed_arguments
      parse_arguments(const std::string& name_,
                      const data::command::iterator& args_begin_,
                      const data::command::iterator& args_end_);
    };

    std::ostream& operator<<(std::ostream& out_,
                             const which::parsed_arguments& args_);
    bool operator==(const which::parsed_arguments& a_,
                    const which::parsed_arguments& b_);
  }
}

#endif
