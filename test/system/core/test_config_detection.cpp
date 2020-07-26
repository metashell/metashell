// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>
#include <just/file.hpp>
#include <just/temp.hpp>

#include <boost/filesystem/path.hpp>

#include <map>

using namespace metashell::system_test;

TEST(config_detection, invalid_files)
{
  const std::string compile_command_item =
      "{\"directory\":\".\",\"command\":\"foo bar\",\"file\":\"test.cpp\"}";
  const std::string config_item =
      "{\"name\":\"test\", \"engine\":\"auto\", \"engine_args\":[]}";

  just::temp::directory temp_dir;
  const boost::filesystem::path temp{temp_dir.path()};

  const std::pair<boost::filesystem::path, std::string> bad_files[] = {
      {temp / "empty.json", ""},
      {temp / "invalid.json", "Not really a JSON file"},
      {temp / "not_object.json", "13"},
      {temp / "empty_object.json", "[{}]"},
      {temp / "other_list.json", "[{\"foo\":\"bar\"}]"},
      {temp / "bad_compile_command.json",
       "[" + compile_command_item + ", {\"foo\":\"bar\"}]"},
      {temp / "bad_config.json", "[" + config_item + ", {\"foo\":\"bar\"}]"},
      {temp / "mixed_list.json",
       "[" + compile_command_item + ", " + config_item + "]"}};

  for (const auto& file : bad_files)
  {
    just::file::write(file.first.string(), file.second);

    ASSERT_THROW(metashell_instance(
                     {file.first.string()}, boost::filesystem::path{}, false),
                 std::exception);
  }

  const boost::filesystem::path empty_list = temp / "empty_list.json";
  just::file::write(empty_list.string(), "[]");

  // expected not to throw
  metashell_instance{{empty_list.string()}, boost::filesystem::path{}, false};
  metashell_instance{{empty_list.string(), empty_list.string()},
                     boost::filesystem::path{},
                     false};
}
