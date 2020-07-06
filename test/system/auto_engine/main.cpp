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

#include <boost/filesystem/path.hpp>

#include <boost/program_options.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{
  boost::program_options::variables_map parse_arguments(int argc_,
                                                        char* argv_[])
  {
    using boost::program_options::value;

    boost::program_options::options_description desc("auto engine system test");
    // clang-format off
    desc.add_options()
      ("metashell", value<boost::filesystem::path>(), "Path of metashell binary to test")
      ("clang", value<std::vector<boost::filesystem::path>>()->multitoken(), "path of clang stub")
      ("templight", value<std::vector<boost::filesystem::path>>()->multitoken(), "Path of templight binary to test")
      ("gcc", value<std::vector<boost::filesystem::path>>()->multitoken(), "path of gcc stub")
      ("msvc", value<std::vector<boost::filesystem::path>>()->multitoken(), "path of cl.exe stub");
    // clang-format on

    boost::program_options::variables_map vm;
    store(parse_command_line(argc_, argv_, desc), vm);
    notify(vm);

    if (vm["metashell"].empty())
    {
      throw std::runtime_error("Missing mandatory --metashell argument.");
    }

    return vm;
  }
}

int main(int argc_, char* argv_[])
{
  try
  {
    const boost::program_options::variables_map args =
        parse_arguments(argc_, argv_);

    auto_engine_test test(metashell::data::executable_path(
        args["metashell"].as<boost::filesystem::path>()));

    test.test_engine_selection({}, std::string("internal"));
    test.test_engine_selection({"-I."}, std::string("internal"));
    test.test_engine_selection({"asd"}, std::nullopt);

    for (const std::string engine : {"clang", "gcc", "msvc", "templight"})
    {
      if (!args[engine].empty())
      {
        for (const auto& stub :
             args[engine].as<std::vector<boost::filesystem::path>>())
        {
          test.test_engine_selection({stub.string()}, engine);
        }
      }
    }

    std::cout << "All tests passed\n";
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
