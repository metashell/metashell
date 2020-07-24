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

#include "auto_engine_test.hpp"

#include <metashell/data/arg_parser.hpp>

#include <boost/filesystem/path.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{
  struct parsed_arguments
  {
    ::metashell::data::executable_path metashell;
    std::vector<boost::filesystem::path> clang;
    std::vector<boost::filesystem::path> templight;
    std::vector<boost::filesystem::path> gcc;
    std::vector<boost::filesystem::path> msvc;
  };

  parsed_arguments parse_arguments(int argc_, const char* argv_[])
  {
    using metashell::data::no_short_name;

    boost::filesystem::path metashell_path;
    std::vector<boost::filesystem::path> clang;
    std::vector<boost::filesystem::path> templight;
    std::vector<boost::filesystem::path> gcc;
    std::vector<boost::filesystem::path> msvc;

    metashell::data::arg_parser parser{};
    // clang-format off
    parser
      .with_value(
        no_short_name, "--metashell",
        "Path of metashell binary to test",
        metashell_path
      )
      .with_value(no_short_name, "--clang", "path of clang stub", clang)
      .with_value(
        no_short_name, "--templight",
        "Path of templight binary to test",
        templight
      )
      .with_value(no_short_name, "--gcc", "path of gcc stub", gcc)
      .with_value(no_short_name, "--msvc", "path of cl.exe stub", msvc)
    ;
    // clang-format on

    parser.parse(metashell::data::command_line_argument_list{argc_, argv_});

    if (metashell_path.empty())
    {
      throw std::runtime_error{"Missing mandatory --metashell argument."};
    }

    return parsed_arguments{
        metashell::data::executable_path{std::move(metashell_path)},
        std::move(clang), std::move(templight), std::move(gcc),
        std::move(msvc)};
  }

  void test_engine(auto_engine_test& test_,
                   const std::string& name_,
                   const std::vector<boost::filesystem::path>& stubs_)
  {
    for (const auto& stub : stubs_)
    {
      test_.test_engine_selection({stub.string()}, name_);
    }
  }
}

int main(int argc_, const char* argv_[])
{
  try
  {
    const parsed_arguments args = parse_arguments(argc_, argv_);

    auto_engine_test test{args.metashell};

    test.test_engine_selection({}, std::string("internal"));
    test.test_engine_selection({"-I."}, std::string("internal"));
    test.test_engine_selection({"asd"}, std::nullopt);

    test_engine(test, "clang", args.clang);
    test_engine(test, "gcc", args.gcc);
    test_engine(test, "msvc", args.msvc);
    test_engine(test, "templight", args.templight);

    std::cout << "All tests passed\n";
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
