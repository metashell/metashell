#ifndef METASHELL_MOCK_ENVIRONMENT_DETECTOR_HPP
#define METASHELL_MOCK_ENVIRONMENT_DETECTOR_HPP

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

#include <metashell/iface/environment_detector.hpp>

#include <functional>

class mock_environment_detector : public metashell::iface::environment_detector
{
public:
  mock_environment_detector();

  virtual std::string search_clang_binary();
  void search_clang_binary_returns(const std::string& result_);
  int search_clang_binary_called_times() const;

  virtual bool file_exists(const std::string& path_);
  void file_exists_returns(bool result_);
  int file_exists_called_times() const;
  const std::string& file_exists_last_arg() const;

  virtual bool on_windows();
  void on_windows_returns(bool result_);
  int on_windows_called_times() const;

  virtual bool on_osx();
  void on_osx_returns(bool result_);
  int on_osx_called_times() const;

  virtual std::vector<std::string> default_clang_sysinclude(
    const std::string& clang_path_,
    metashell::stdlib stdlib_
  );
  int default_clang_sysinclude_called_times() const;
  void default_clang_sysinclude_returns_append(const std::string& path_);

  virtual std::vector<std::string> extra_sysinclude();
  int extra_sysinclude_called_times() const;
  void extra_sysinclude_returns_append(const std::string& path_);

  virtual std::string path_of_executable();
  int path_of_executable_called_times() const;
  void path_of_executable_returns(const std::string& result_);
private:
  std::string _search_clang_binary_returns;
  int _search_clang_binary_called_times;

  bool _file_exists_returns;
  int _file_exists_called_times;
  std::string _file_exists_last_arg;

  bool _on_windows_returns;
  int _on_windows_called_times;

  bool _on_osx_returns;
  int _on_osx_called_times;

  int _default_clang_sysinclude_called_times;
  std::vector<std::string> _default_clang_sysinclude_returns;

  int _extra_sysinclude_called_times;
  std::vector<std::string> _extra_sysinclude_returns;

  int _path_of_executable_called_times;
  std::string _path_of_executable_returns;
};

#endif

