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

  virtual std::string search_clang_binary() override;
  void search_clang_binary_returns(const std::string& result_);
  int search_clang_binary_called_times() const;

  virtual bool file_exists(const std::string& path_) override;
  void file_exists_returns(bool result_);
  int file_exists_called_times() const;
  const std::string& file_exists_last_arg() const;

  virtual bool on_windows() override;
  void on_windows_returns(bool result_);
  int on_windows_called_times() const;

  virtual bool on_osx() override;
  void on_osx_returns(bool result_);
  int on_osx_called_times() const;

  virtual std::string directory_of_executable() override;
  int directory_of_executable_called_times() const;
  void directory_of_executable_returns(const std::string& result_);
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

  int _directory_of_executable_called_times;
  std::string _directory_of_executable_returns;
};

#endif

