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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/system_test_config.hpp>
#include <metashell/system_test/util.hpp>

#include <metashell/core/rapid_json_writer.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>
#include <just/temp.hpp>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  void write_compile_commands(metashell::iface::json_writer& out_,
                              const boost::filesystem::path& cpp_path_,
                              const boost::filesystem::path& bin_dir_,
                              const std::string& command_)
  {
    out_.start_object();

    out_.key("directory");
    out_.string(bin_dir_.string());

    out_.key("command");
    out_.string(command_);

    out_.key("file");
    out_.string(cpp_path_.string());

    out_.end_object();
  }

  json_string config_names(const boost::filesystem::path& temp_dir_,
                           const std::vector<std::string>& filenames_)
  {
    const boost::filesystem::path bin_dir = temp_dir_ / "bin";
    const boost::filesystem::path compile_commands =
        temp_dir_ / "compile_commands.json";

    {
      std::ofstream f(compile_commands.string());
      metashell::core::rapid_json_writer out(f);

      out.start_array();
      for (const std::string& filename : filenames_)
      {
        write_compile_commands(
            out, filename, bin_dir, "/foo/bar/g++ -c " + filename);
      }
      out.end_array();
    }

    return metashell_instance(
               {"--load_compile_commands", compile_commands.string()},
               boost::filesystem::path(), false)
        .command("#msh all config")
        .front();
  }
}

TEST(shell_compile_commands, creating_shell_configs)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  const boost::filesystem::path src_dir = temp / "src";
  const boost::filesystem::path bin_dir = temp / "bin";

  {
    std::ofstream f((temp / "compile_commands.json").string());
    metashell::core::rapid_json_writer out(f);

    out.start_array();
    write_compile_commands(out, src_dir / "xa/foo.cpp", bin_dir,
                           "/foo/bar/g++ -Iasd -c xa/foo.cpp");
    write_compile_commands(
        out, src_dir / "xb/bar.cpp", bin_dir, "/foo/bar/g++ bar.cpp");
    out.end_array();
  }

  for (const auto& args : with_and_without_prefix(
           "--load_compile_commands", temp / "compile_commands.json"))
  {
    metashell_instance mi{args, boost::filesystem::path{}, false};

    const std::vector<json_string> a_foo =
        mi.command("#msh config show xa/foo.cpp");
    ASSERT_EQ(a_foo.size(), 2);
    ASSERT_EQ(comment(_), a_foo.front());
    ASSERT_TRUE(a_foo.front().get().find("\\\"engine\\\":\\\"auto\\\",") !=
                std::string::npos);
    ASSERT_TRUE(
        a_foo.front().get().find(
            "\\\"engine_args\\\":[\\\"/foo/bar/g++\\\",\\\"-Iasd\\\"],") !=
        std::string::npos);
    ASSERT_TRUE(a_foo.front().find(json_string_escape(
                    json_string("\"cwd\":") + to_json(bin_dir.string()))) !=
                std::string::npos);
  }
}

TEST(shell_compile_commands, existing_name)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  const boost::filesystem::path src_dir = temp / "src";
  const boost::filesystem::path bin_dir = temp / "bin";

  create_directory(src_dir);
  create_directory(bin_dir);

  {
    std::ofstream f((temp / "compile_commands.json").string());
    metashell::core::rapid_json_writer out(f);

    out.start_array();
    write_compile_commands(
        out, "default", bin_dir, "/foo/bar/g++ -Iasd -c xa/foo.cpp");
    out.end_array();
  }

  for (const auto& args : with_and_without_prefix(
           "--load_compile_commands", temp / "compile_commands.json"))
  {
    metashell_instance mi{args, boost::filesystem::path{}, false};

    const std::vector<json_string> a_foo =
        mi.command("#msh config show default:1");
    ASSERT_EQ(a_foo.size(), 2);
    ASSERT_EQ(comment(_), a_foo.front());
    ASSERT_TRUE(a_foo.front().get().find("\\\"engine\\\":\\\"auto\\\",") !=
                std::string::npos);
    ASSERT_TRUE(
        a_foo.front().get().find(
            "\\\"engine_args\\\":[\\\"/foo/bar/g++\\\",\\\"-Iasd\\\"],") !=
        std::string::npos);
  }
}

TEST(shell_compile_commands, test_path_simplification)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  ASSERT_EQ(comment(" * default"), config_names(temp, {}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {""}));

  ASSERT_EQ(comment(" * default\n   foo"), config_names(temp, {"foo"}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {"/"}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {"//"}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {"\\"}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {"\\\\"}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {"/\\"}));
  ASSERT_EQ(comment(" * default\n   _"), config_names(temp, {"\\/"}));

  ASSERT_EQ(
      comment(" * default\n   _\n   _:1"), config_names(temp, {"/", "\\"}));
  ASSERT_EQ(
      comment(" * default\n   _\n   _:1"), config_names(temp, {"/", "\\_"}));

  ASSERT_EQ(comment(" * default\n   foo\n   foo:1"),
            config_names(temp, {"foo", "foo"}));
  ASSERT_EQ(comment(" * default\n   foo\n   bar"),
            config_names(temp, {"foo", "bar"}));
  ASSERT_EQ(comment(" * default\n   foo\n   foobar"),
            config_names(temp, {"foo", "foobar"}));

  ASSERT_EQ(comment(" * default\n   a.hpp"), config_names(temp, {"foo/a.hpp"}));
  ASSERT_EQ(comment(" * default\n   a.hpp\n   b.hpp"),
            config_names(temp, {"foo/a.hpp", "foo/b.hpp"}));
  ASSERT_EQ(comment(" * default\n   a.hpp\n   b.hpp"),
            config_names(temp, {"/foo/a.hpp", "/foo/b.hpp"}));
  ASSERT_EQ(comment(" * default\n   foox/a.hpp\n   fooy/b.hpp"),
            config_names(temp, {"foox/a.hpp", "fooy/b.hpp"}));
  ASSERT_EQ(comment(" * default\n   barx/a.hpp\n   bary/b.hpp"),
            config_names(temp, {"foo/barx/a.hpp", "foo/bary/b.hpp"}));
  ASSERT_EQ(comment(" * default\n   x/a.hpp\n   y/b.hpp"),
            config_names(temp, {"foo/bar/x/a.hpp", "foo/bar/y/b.hpp"}));

  ASSERT_EQ(comment(" * default\n   foo/b.hpp\n   foo"),
            config_names(temp, {"foo/b.hpp", "foo"}));
  ASSERT_EQ(comment(" * default\n   foo\n   foo/b.hpp"),
            config_names(temp, {"foo", "foo/b.hpp"}));
  ASSERT_EQ(comment(" * default\n   b.hpp\n   _"),
            config_names(temp, {"foo/b.hpp", "foo/"}));
  ASSERT_EQ(comment(" * default\n   _\n   b.hpp"),
            config_names(temp, {"foo/", "foo/b.hpp"}));

  ASSERT_EQ(comment(" * default\n   bar/b.hpp\n   a.hpp"),
            config_names(temp, {"foo/bar/b.hpp", "foo/a.hpp"}));
  ASSERT_EQ(comment(" * default\n   bar/b.hpp\n   a.hpp"),
            config_names(temp, {"foo/bar/b.hpp", "foo\\a.hpp"}));

  ASSERT_EQ(comment(" * default\n   /foo\n   bar"),
            config_names(temp, {"/foo", "bar"}));

  ASSERT_EQ(comment(" * default\n   baz/a.hpp\n   b.hpp\n   _"),
            config_names(
                temp, {"/foo/bar/baz/a.hpp", "/foo/bar/b.hpp", "/foo/bar/"}));
  ASSERT_EQ(
      comment(" * default\n   bar/baz/a.hpp\n   bar/b.hpp\n   bar"),
      config_names(temp, {"/foo/bar/baz/a.hpp", "/foo/bar/b.hpp", "/foo/bar"}));
  ASSERT_EQ(
      comment(" * default\n   bar/baz/a.hpp\n   bar/b.hpp\n   bar\n   bar/_"),
      config_names(temp, {"/foo/bar/baz/a.hpp", "/foo/bar/b.hpp", "/foo/bar",
                          "/foo/bar/"}));
}

TEST(shell_compile_commands, test_multiple_arch_args)
{
  if (using_internal() || !using_clang())
  {
    return;
  }

  just::temp::directory temp_dir;
  const boost::filesystem::path temp = temp_dir.path();

  const boost::filesystem::path src_dir = temp / "src";
  const boost::filesystem::path bin_dir = temp / "bin";

  create_directory(src_dir);
  create_directory(bin_dir);

  const boost::filesystem::path compile_commands_json =
      temp / "compile_commands.json";

  {
    std::ofstream f{compile_commands_json.string()};
    metashell::core::rapid_json_writer out{f};

    out.start_array();
    write_compile_commands(
        out, "foo.cpp", bin_dir,
        boost::algorithm::join(
            system_test_config::engine_args() |
                boost::adaptors::transformed(
                    [](const auto& arg_) { return to_string(arg_); }),
            " ") +
            " -arch armv7 -arch armv7s -c foo.cpp");
    out.end_array();
  }

  for (const auto& args : with_and_without_prefix(
           "--load_compile_commands", compile_commands_json))
  {
    metashell_instance mi{args, boost::filesystem::path{}, false};

    {
      const std::vector<json_string> load =
          mi.command("#msh config load foo.cpp");
      ASSERT_EQ(2, load.size());
      ASSERT_EQ(comment(_), load.front());
      ASSERT_TRUE(
          load.front().get().find("Removed argument -arch armv7s because "
                                  "of earlier argument: -arch armv7.") !=
          std::string::npos);
    }

    ASSERT_EQ(json_string{}, mi.command_fails("#msh config load foo.cpp"));
    ASSERT_EQ(json_string{}, mi.command_fails("#msh engine switch wave"));
    ASSERT_EQ(json_string{}, mi.command_fails("#msh engine switch internal"));
  }
}
