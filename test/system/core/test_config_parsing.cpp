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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/metashell_terminated.hpp>
#include <metashell/system_test/paragraph.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/path.hpp>

#include <just/file.hpp>
#include <just/temp.hpp>

#include <gtest/gtest.h>

#include <cassert>
#include <tuple>
#include <vector>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  class config_parse_test
  {
  public:
    std::string error_with_configs(const std::vector<json_string>& configs_)
    {
      try
      {
        metashell_instance(
            write_configs(configs_), boost::filesystem::path(), false);
      }
      catch (const metashell_terminated& e_)
      {
        return e_.standard_error();
      }

      return std::string();
    }

    std::vector<json_string>
    cmd_with_configs(const std::vector<json_string>& configs_,
                     const std::vector<std::string>& cmds_)
    {
      metashell_instance mi(write_configs(configs_), {}, false);
      if (cmds_.empty())
      {
        return {};
      }
      else
      {
        for (auto i = cmds_.begin(), e = cmds_.end() - 1; i != e; ++i)
        {
          mi.command(*i);
        }
        return mi.command(cmds_.back());
      }
    }

  private:
    just::temp::directory _tmp;

    std::vector<std::string>
    write_configs(const std::vector<json_string>& configs_)
    {
      const boost::filesystem::path tmp(_tmp.path());

      std::vector<std::string> args;
      for (std::vector<json_string>::size_type i = 0; i != configs_.size(); ++i)
      {
        const std::string config = (tmp / std::to_string(i)).string();

        just::file::write(config, configs_[i].get());

        args.emplace_back("--load_configs");
        args.emplace_back(config);
      }

      return args;
    }
  };

  enum class value_type
  {
    string_,
    bool_,
    list_
  };

  std::string to_string(value_type v_)
  {
    switch (v_)
    {
    case value_type::string_:
      return "string";
    case value_type::bool_:
      return "bool";
    case value_type::list_:
      return "list of strings";
    }
    assert(!"This point should not be reached");
    return "";
  }
}

TEST(config_parsing, errors)
{
  const std::string nl = new_line() + new_line();
  config_parse_test t;

  typedef std::pair<std::string, json_string> sp;
  typedef std::pair<std::string, value_type> sv;

  for (sp p :
       {sp("integer", json_string("13")), sp("double", json_string("13.1")),
        sp("bool", json_string("true")),
        sp("string", json_string("\"hello\""))})
  {
    ASSERT_EQ("JSON parsing failed: Unexpected " + p.first + " element: " +
                  p.second.get() + nl,
              t.error_with_configs({p.second}));
  }

  ASSERT_EQ("JSON parsing failed: Invalid key: hello" + nl,
            t.error_with_configs({json_string("{\"hello\":13}")}));

  typedef std::tuple<value_type, std::string, std::string> tup;
  const std::vector<tup> sample_value{
      tup{value_type::string_, "\"hello\"", "\"hello\""},
      tup{value_type::bool_, "true", "true"},
      tup{value_type::list_, "[\"foo\", \"bar\"]", "A list"}};

  for (sv p :
       {sv("name", value_type::string_), sv("engine", value_type::string_),
        sv("engine_args", value_type::list_),
        sv("use_precompiled_headers", value_type::bool_),
        sv("preprocessor_mode", value_type::bool_)})
  {
    for (auto smp : sample_value)
    {
      if (std::get<0>(smp) != p.second)
      {
        ASSERT_EQ("JSON parsing failed: " + std::get<2>(smp) +
                      " is not a valid value for " + p.first +
                      ", which should be a " + to_string(p.second) + nl,
                  t.error_with_configs({json_string("{\"" + p.first + "\":" +
                                                    std::get<1>(smp) + "}")}));
      }
    }
  }

  ASSERT_EQ("JSON parsing failed: Unexpected integer element: 13" + nl,
            t.error_with_configs({json_string("{\"engine_args\":[13]}")}));

  ASSERT_EQ("JSON parsing failed: Unexpected double element: 13.1" + nl,
            t.error_with_configs({json_string("{\"engine_args\":[13.1]}")}));

  ASSERT_EQ(
      "JSON parsing failed: false is not a valid value for engine_args, which "
      "should be a list of strings" +
          nl,
      t.error_with_configs({json_string("{\"engine_args\":[false]}")}));

  ASSERT_EQ(
      "JSON parsing failed: A list containing a list is not a valid value for "
      "engine_args, which should be a list of strings" +
          nl,
      t.error_with_configs({json_string("{\"engine_args\":[[]]}")}));

  ASSERT_EQ("JSON parsing failed: The name of a config is missing" + nl,
            t.error_with_configs({json_string("{}")}));

  ASSERT_EQ(
      "JSON parsing failed: More than one config provided with the name "
      "foo" +
          nl,
      t.error_with_configs(
          {json_string("[{\"name\":\"foo\"},{\"name\":\"foo\"}]")}));

  ASSERT_EQ(
      "JSON parsing failed: More than one config provided with the name "
      "foo" +
          nl,
      t.error_with_configs({json_string("{\"name\":\"foo\"}"),
                            json_string("{\"name\":\"foo\"}")}));

  ASSERT_TRUE(boost::starts_with(t.error_with_configs({test_config("")}),
                                 "Empty shell config name value"));
}

TEST(config_parsing, parsed_config)
{
  config_parse_test t;

  ASSERT_EQ(comment(" * default"),
            t.cmd_with_configs({}, {"#msh all config"}).front());

  ASSERT_EQ(
      comment(" * default\n   test"),
      t.cmd_with_configs({array({test_config("test")})}, {"#msh all config"})
          .front());

  ASSERT_EQ(
      comment(" * default\n   test1\n   test2"),
      t.cmd_with_configs({array({test_config("test1"), test_config("test2")})},
                         {"#msh all config"})
          .front());

  ASSERT_EQ(comment(" * default\n   test1\n   test2"),
            t.cmd_with_configs(
                 {array({test_config("test1")}), array({test_config("test2")})},
                 {"#msh all config"})
                .front());

  ASSERT_EQ(comment(test_config("test").get()),
            t.cmd_with_configs(
                 {array({test_config("test")})}, {"#msh config show test"})
                .front());

  ASSERT_EQ(error("Error: Config test2 not found."),
            t.cmd_with_configs(
                 {array({test_config("test1")})}, {"#msh config show test2"})
                .front());

  ASSERT_EQ(comment(" * default\n   1"),
            t.cmd_with_configs({array({test_config("1")})}, {"#msh all config"})
                .front());
}

TEST(config_parsing, switching_config)
{
  const std::vector<json_string> configs{
      json_string("[{\"name\":\"wave\",\"engine\":\"wave\"}]")};
  config_parse_test t;

  ASSERT_EQ(
      prompt(">"), t.cmd_with_configs(configs, {"typedef int x;"}).front());
  ASSERT_EQ(comment(_), t.cmd_with_configs(configs, {"typedef int x;",
                                                     "#msh config load wave"})
                            .front());
  ASSERT_EQ(error(_), t.cmd_with_configs(configs, {"#msh config load wave",
                                                   "typedef foo bar;",
                                                   "#msh config load default"})
                          .front());
  ASSERT_EQ(comment("   default\n * wave"),
            t.cmd_with_configs(
                 configs, {"#msh config load wave", "typedef foo bar;",
                           "#msh config load default", "#msh all config"})
                .front());
}
