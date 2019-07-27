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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

namespace
{
  std::vector<json_string> with_null_engine(const std::string& init_,
                                            const std::string& cmd_)
  {
    metashell_instance mi({"--engine", "null"}, {}, false);
    if (!init_.empty())
    {
      mi.command(init_);
    }
    else if (cmd_.empty())
    {
      return mi.initial_responses();
    }

    return mi.command(cmd_);
  }

  std::vector<json_string> with_null_engine(const std::string& cmd_ = "")
  {
    return with_null_engine("", cmd_);
  }
}

TEST(null_engine, startup)
{
  ASSERT_EQ(std::vector<json_string>{to_json_string(prompt(">"))},
            with_null_engine());
}

TEST(null_engine, type_shell)
{
  ASSERT_EQ(
      error("Error: Feature type shell is not supported by the null engine."),
      with_null_engine("int").front());
}

TEST(null_engine, preprocessor_shell)
{
  ASSERT_EQ(error("Error: Feature preprocessor shell is not supported by the "
                  "null engine."),
            with_null_engine("#msh preprocessor mode", "int").front());
}

TEST(null_engine, header_discoverer)
{
  ASSERT_EQ(error("Error: Feature header discoverer is not supported by the "
                  "null engine."),
            with_null_engine("#msh sysincludes").front());
}

TEST(null_engine, metaprogram_tracer)
{
  ASSERT_EQ(error("Error: Feature metaprogram tracer is not supported by the "
                  "null engine."),
            with_null_engine("#msh mdb int").front());
}

TEST(null_engine, cpp_validator)
{
  ASSERT_EQ(error("Error: Feature cpp validator is not supported by the "
                  "null engine."),
            with_null_engine("typedef int x;").front());
}

TEST(null_engine, configs)
{
  ASSERT_EQ(comment(" * default"), with_null_engine("#msh config").front());

  ASSERT_EQ(comment("{\"name\":\"default\",\"engine\":\"null\","
                    "\"engine_args\":[],\"use_precompiled_headers\":"
                    "true,\"preprocessor_mode\":false}"),
            with_null_engine("#msh config show default").front());
}
