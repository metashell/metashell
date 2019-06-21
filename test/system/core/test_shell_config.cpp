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

#include <boost/optional.hpp>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  void write_shell_config(
      metashell::iface::json_writer& out_,
      const std::string& name_,
      const std::string& engine_,
      const boost::optional<std::string>& engine_arg_ = boost::none)
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
