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
#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/metashell_terminated.hpp>
#include <metashell/system_test/paragraph.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/system_test_config.hpp>
#include <metashell/system_test/util.hpp>

#include <metashell/core/rapid_json_writer.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

#include <just/file.hpp>
#include <just/temp.hpp>

#include <gtest/gtest.h>

#include <cassert>
#include <tuple>
#include <vector>

using namespace metashell;
using namespace metashell::system_test;
using pattern::_;

namespace
{
  class config_parse_test
  {
  public:
    explicit config_parse_test(bool long_arg_form_,
                               boost::filesystem::path cwd_ = {})
      : _long_arg_form{long_arg_form_}, _cwd{std::move(cwd_)}
    {
    }

    std::string error_with_configs(const std::vector<json_string>& configs_)
    {
      try
      {
        metashell_instance(write_configs(configs_), _cwd, false);
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
      metashell_instance mi(write_configs(configs_), _cwd, false);
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
    bool _long_arg_form;
    just::temp::directory _tmp;
    boost::filesystem::path _cwd;

    metashell::data::command_line_argument_list
    write_configs(const std::vector<json_string>& configs_)
    {
      const boost::filesystem::path tmp(_tmp.path());

      metashell::data::command_line_argument_list args;
      for (std::vector<json_string>::size_type i = 0; i != configs_.size(); ++i)
      {
        const boost::filesystem::path config = tmp / std::to_string(i);

        just::file::write(config.string(), configs_[i].get());

        if (_long_arg_form)
        {
          args.push_back("--load_configs", config);
        }
        else
        {
          args.push_back(config);
        }
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

  void use_current_engine(iface::json_writer& out_)
  {
    out_.key("engine");
    out_.string(current_engine());

    out_.key("engine_args");
    out_.start_array();
    for (const data::command_line_argument& arg :
         system_test_config::engine_args())
    {
      out_.string(to_string(arg));
    }
    out_.end_array();
  }

  json_string generate_cwd_related_configs(const boost::filesystem::path& cwd_)
  {
    std::ostringstream s;
    core::rapid_json_writer w(s);

    w.start_array();

    w.start_object();
    w.key("name");
    w.string("no_cwd");
    use_current_engine(w);
    w.end_object();

    w.start_object();
    w.key("name");
    w.string("with_cwd");
    w.key("cwd");
    w.string(cwd_.string());
    use_current_engine(w);
    w.end_object();

    w.end_array();
    w.end_document();

    return json_string(s.str());
  }
}

TEST(config_parsing, errors)
{
  const std::string nl = new_line() + new_line();
  config_parse_test t{true};

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
  for (bool long_arg_form : {true, false})
  {
    config_parse_test t{long_arg_form};

    ASSERT_EQ(comment(" * default"),
              t.cmd_with_configs({}, {"#msh all config"}).front());

    ASSERT_EQ(
        comment(" * default\n   test"),
        t.cmd_with_configs({array({test_config("test")})}, {"#msh all config"})
            .front());

    ASSERT_EQ(comment(" * default\n   test1\n   test2"),
              t.cmd_with_configs(
                   {array({test_config("test1"), test_config("test2")})},
                   {"#msh all config"})
                  .front());

    ASSERT_EQ(comment(" * default\n   test1\n   test2"),
              t.cmd_with_configs({array({test_config("test1")}),
                                  array({test_config("test2")})},
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

    ASSERT_EQ(
        comment(" * default\n   1"),
        t.cmd_with_configs({array({test_config("1")})}, {"#msh all config"})
            .front());
  }
}

TEST(config_parsing, switching_config)
{
  const std::vector<json_string> configs{
      json_string("[{\"name\":\"wave\",\"engine\":\"wave\"}]")};

  for (bool long_arg_form : {true, false})
  {
    config_parse_test t{long_arg_form};

    ASSERT_EQ(
        prompt(">"), t.cmd_with_configs(configs, {"typedef int x;"}).front());
    ASSERT_EQ(comment(_), t.cmd_with_configs(configs, {"typedef int x;",
                                                       "#msh config load wave"})
                              .front());
    ASSERT_EQ(
        error(_), t.cmd_with_configs(
                       configs, {"#msh config load wave", "typedef foo bar;",
                                 "#msh config load default"})
                      .front());
    ASSERT_EQ(comment("   default\n * wave"),
              t.cmd_with_configs(
                   configs, {"#msh config load wave", "typedef foo bar;",
                             "#msh config load default", "#msh all config"})
                  .front());
  }
}

TEST(config_parsing, cwd)
{
  just::temp::directory tmp;
  const boost::filesystem::path d1 = tmp.path() + "/d1";
  const boost::filesystem::path d2 = tmp.path() + "/d2";

  create_directory(d1);
  create_directory(d2);

  just::file::write((d1 / "d1.hpp").string(), "#define FOO int");
  just::file::write((d2 / "d2.hpp").string(), "#define FOO double");

  const std::vector<json_string> configs{generate_cwd_related_configs(d2)};

  for (bool long_arg_form : {true, false})
  {
    config_parse_test t{long_arg_form, d1};

    ASSERT_EQ(cpp_code("int"),
              t.cmd_with_configs(configs, {"#msh preprocessor mode",
                                           "#include \"d1.hpp\"", "FOO"})
                  .front());

    ASSERT_EQ(cpp_code("int"),
              t.cmd_with_configs(configs, {"#msh preprocessor mode",
                                           "#msh config load no_cwd",
                                           "#include \"d1.hpp\"", "FOO"})
                  .front());

    ASSERT_EQ(cpp_code("double"),
              t.cmd_with_configs(configs, {"#msh preprocessor mode",
                                           "#msh config load with_cwd",
                                           "#include \"d2.hpp\"", "FOO"})
                  .front());

    ASSERT_EQ(
        cpp_code("int"),
        t.cmd_with_configs(
             configs, {"#msh preprocessor mode", "#msh config load with_cwd",
                       "#msh config load no_cwd", "#include \"d1.hpp\"", "FOO"})
            .front());
  }
}
