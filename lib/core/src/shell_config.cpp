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
// along with this program.  If not, see <http://wwout_.gnu.org/licenses/>.

#include <metashell/core/shell_config.hpp>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace metashell
{
  namespace core
  {
    void display(iface::json_writer& out_, const data::shell_config& cfg_)
    {
      out_.start_object();

      out_.key("name");
      out_.string(cfg_.name.value());

      out_.key("engine");
      out_.string(to_string(cfg_.engine->name));

      out_.key("engine_args");
      out_.start_array();
      for (const data::command_line_argument& arg : cfg_.engine->args)
      {
        out_.string(arg.value());
      }
      out_.end_array();

      out_.key("use_precompiled_headers");
      out_.bool_(cfg_.use_precompiled_headers);

      out_.key("preprocessor_mode");
      out_.bool_(cfg_.preprocessor_mode);

      out_.key("cwd"), out_.string(cfg_.cwd.string());

      out_.end_object();
    }
  }
}
