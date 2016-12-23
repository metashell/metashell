#ifndef METASHELL_CLANG_BINARY_HPP
#define METASHELL_CLANG_BINARY_HPP

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

#include <metashell/data/result.hpp>
#include <metashell/iface/environment.hpp>
#include <metashell/iface/executable.hpp>
#include <metashell/logger.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace metashell
{
  class clang_binary : public iface::executable
  {
  public:
    clang_binary(const boost::filesystem::path& path_,
                 const std::vector<std::string>& base_args_,
                 logger* logger_);

    virtual data::process_output run(const std::vector<std::string>& args_,
                                     const std::string& stdin_) const override;

  private:
    std::vector<std::string> _base_args;
    logger* _logger;
  };

  data::process_output run_clang(const iface::executable& clang_binary_,
                                 std::vector<std::string> clang_args_,
                                 const std::string& input_);

  data::result
  eval(const iface::environment& env_,
       const boost::optional<std::string>& tmp_exp_,
       const boost::optional<boost::filesystem::path>& env_path_,
       const boost::optional<boost::filesystem::path>& templight_dump_path_,
       clang_binary& clang_binary_);
}

#endif
