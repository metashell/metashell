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

#include <metashell/system_test/read_only_path.hpp>

#include <stdexcept>

#ifdef _WIN32
#include <sddl.h>
#include <windows.h>
#endif

using namespace metashell::system_test;

#ifdef _WIN32
namespace
{
  std::string subpath() { return "nw"; }

  void make_read_only(SECURITY_ATTRIBUTES& sa_)
  {
    if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
            "D:(A;OICI;GR;;;WD)", SDDL_REVISION_1, &sa_.lpSecurityDescriptor,
            nullptr))
    {
      throw std::runtime_error("Failed to create security descriptor");
    }
  }
}
#endif

read_only_path::read_only_path()
{
#ifdef _WIN32
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = FALSE;
  make_read_only(sa);

  const auto p = path();
  if (!CreateDirectory(p.string().c_str(), &sa))
  {
    throw std::runtime_error("Error creating " + p.string());
  }
#endif
}

boost::filesystem::path read_only_path::path() const
{
#ifdef _WIN32
  return boost::filesystem::path(_temp.path()) / subpath();
#else
  return "/";
#endif
}
