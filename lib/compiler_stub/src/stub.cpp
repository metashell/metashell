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

#include <metashell/compiler_stub/stub.hpp>

#include <metashell/data/exception.hpp>

#include <boost/algorithm/string/join.hpp>

#include <iostream>

namespace metashell
{
  namespace compiler_stub
  {
    stub& stub::on_args(std::vector<std::string> args_,
                        data::process_output out_)
    {
      if (_actions.find(args_) == _actions.end())
      {
        _actions.insert({std::move(args_), std::move(out_)});
        return *this;
      }
      else
      {
        throw data::exception("Arguments " +
                              boost::algorithm::join(args_, " ") +
                              " already specified.");
      }
    }

    data::proc_exit stub::run(int argc_, const char* argv_[]) const
    {
      const auto action = argc_ > 0 ?
                              _actions.find({argv_ + 1, argv_ + argc_}) :
                              _actions.end();
      if (action == _actions.end())
      {
        std::cerr << "NOT SUPPORTED\n";
        return data::exit_failure();
      }
      else
      {
        std::cerr << action->second.standard_error;
        std::cout << action->second.standard_output;
        if (auto e = mpark::get_if<data::proc_exit>(&action->second.status))
        {
          return *e;
        }
        else
        {
          std::cerr << to_string(action->second.status) << "\n";
          return data::exit_failure();
        }
      }
    }
  }
}
