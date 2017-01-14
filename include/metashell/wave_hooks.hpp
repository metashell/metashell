#ifndef METASHELL_WAVE_HOOKS_HPP
#define METASHELL_WAVE_HOOKS_HPP

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

#include <metashell/wave_token.hpp>

#include <boost/wave.hpp>

#include <boost/filesystem/path.hpp>

#include <set>

namespace metashell
{
  class wave_hooks
      : public boost::wave::context_policies::eat_whitespace<wave_token>
  {
  public:
    wave_hooks() : _included_files(nullptr) {}

    explicit wave_hooks(std::set<boost::filesystem::path>& included_files_)
      : _included_files(&included_files_)
    {
    }

    template <typename ContextT>
    void opened_include_file(const ContextT&,
                             const std::string&,
                             const std::string& absname_,
                             bool)
    {
      if (_included_files)
      {
        _included_files->insert(absname_);
      }
    }

  private:
    std::set<boost::filesystem::path>* _included_files;
  };
}

#endif
