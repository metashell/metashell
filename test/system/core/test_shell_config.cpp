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
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>
#include <metashell/system_test/util.hpp>

#include <metashell/core/rapid_json_writer.hpp>

#include <gtest/gtest.h>
#include <just/temp.hpp>

#include <optional>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  void write_shell_config(
      metashell::iface::json_writer& out_,
      const std::string& name_,
      const std::string& engine_,
      const std::optional<std::string>& engine_arg_ = std::nullopt,
      const std::vector<std::string>& warnings_ = {})
  {
    out_.start_object();

    out_.key("name");
    out_.string(name_);

    out_.key("engine");
    out_.string(engine_);

    out_.key("engine_args");
    out_.start_array();
    if (engine_arg_)
    {
      out_.string(*engine_arg_);
    }
    out_.end_array();

    out_.key("use_precompiled_headers");
    out_.bool_(true);

    out_.key("preprocessor_mode");
    out_.bool_(false);

    out_.key("warnings");
    out_.start_array();
    for (const std::string& warning : warnings_)
    {
      out_.string(warning);
    }
    out_.end_array();

    out_.end_object();
  }
}

TEST(shell_config, engine_arguments)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  write_file(temp / "foobar.hpp", "typedef int foo;\n");
  {
    std::ofstream f((temp / "shell_config.json").string());
    metashell::core::rapid_json_writer out(f);

    out.start_array();
    write_shell_config(out, "foo", "auto");
    write_shell_config(out, "bar", "auto", "-I" + temp.string());
    out.end_array();
  }

  metashell_instance mi(
      {"--load_configs", (temp / "shell_config.json").string()},
      boost::filesystem::path(), false);

  mi.command("#msh config load foo");
  ASSERT_EQ(error(_), mi.command("#include <foobar.hpp>").front());

  mi.command("#msh config load bar");
  ASSERT_EQ(prompt(_), mi.command("#include <foobar.hpp>").front());
  ASSERT_EQ(type("int"), mi.command("foo").front());
}

TEST(shell_config, warning_about_null)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path shell_config =
      boost::filesystem::path{temp_dir.path()} / "shell_config.json";

  {
    std::ofstream f(shell_config.string());
    metashell::core::rapid_json_writer out(f);

    out.start_array();
    write_shell_config(out, "foo", "null");
    out.end_array();
  }

  metashell_instance mi({"--load_configs", shell_config.string()},
                        boost::filesystem::path(), false);

  const std::vector<json_string> load = mi.command("#msh config load foo");
  ASSERT_GT(load.size(), 1);
  ASSERT_EQ(comment(_), load.front());
  ASSERT_TRUE(
      load.front().get().find("Note that you are using the null engine, which "
                              "means that you can not debug anything. "
                              "Metashell supports different features if you "
                              "use different compilers.") != std::string::npos);
}

TEST(shell_config, user_defined_warning)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path shell_config =
      boost::filesystem::path{temp_dir.path()} / "shell_config.json";

  {
    std::ofstream f(shell_config.string());
    metashell::core::rapid_json_writer out(f);

    out.start_array();
    write_shell_config(out, "w1", "auto", std::nullopt, {"This is a warning."});
    write_shell_config(
        out, "w2", "auto", std::nullopt,
        {"This is the first warning.", "This is the second warning."});
    out.end_array();
  }

  metashell_instance mi({"--load_configs", shell_config.string()},
                        boost::filesystem::path{}, false);

  const std::vector<json_string> w1 = mi.command("#msh config show w1");
  ASSERT_GT(w1.size(), 1);
  ASSERT_EQ(comment(_), w1.front());
  ASSERT_TRUE(
      w1.front().get().find("\\\"warnings\\\":[\\\"This is a warning.\\\"]") !=
      std::string::npos);

  const std::vector<json_string> w2 = mi.command("#msh config show w2");
  ASSERT_GT(w2.size(), 1);
  ASSERT_EQ(comment(_), w2.front());
  ASSERT_TRUE(w2.front().get().find("\\\"warnings\\\":[\\\"This is the first "
                                    "warning.\\\",\\\"This is the second "
                                    "warning.\\\"]") != std::string::npos);
}
