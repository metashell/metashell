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

#include <metashell/core/rapid_compile_commands_parser.hpp>

#include <metashell/data/shell_command_view.hpp>
#include <metashell/data/shell_config_name.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <cassert>
#include <map>
#include <optional>

namespace metashell
{
  namespace core
  {
    namespace
    {
      template <class CommandLineArgumentInputRange>
      data::command_line_argument_list
      parse_compiler_command(const CommandLineArgumentInputRange& command_)
      {
        data::command_line_argument_list result;

        for (const data::command_line_argument& s : command_)
        {
          const auto last = result.back();

          if (last && (*last == "-c" || *last == "-o"))
          {
            result.pop_back();
          }
          else
          {
            result.push_back(s);
          }
        }

        return result;
      }
    } // namespace

    rapid_compile_commands_parser::rapid_compile_commands_parser(
        bool use_precompiled_headers_, bool preprocessor_mode_)
    {
      _defaults.use_precompiled_headers = use_precompiled_headers_;
      _defaults.engine.default_value().name = data::auto_engine_name(),
      _defaults.preprocessor_mode = preprocessor_mode_;
    }

    bool rapid_compile_commands_parser::StartArray()
    {
      not_empty();

      if (!_in_list)
      {
        _in_list = true;
        return true;
      }
      else if (_data && _key == "arguments")
      {
        _arguments = data::command_line_argument_list{};
        return true;
      }
      else
      {
        fail("Lists are expected as top level elements only");
        return false;
      }
    }

    bool rapid_compile_commands_parser::end_array()
    {
      not_empty();

      if (_arguments)
      {
        compile_command_available(parse_compiler_command(*_arguments));
        _arguments = std::nullopt;
      }
      else
      {
        _in_list = false;

        const auto prefix_len = common_prefix_length(
            _configs |
            boost::adaptors::transformed(
                [](const data::shell_config& cfg_) { return cfg_.name; }));

        const data::shell_config_name default_name("_");

        for (data::shell_config& cfg : _configs)
        {
          cfg.name = remove_prefix(cfg.name, prefix_len).value_or(default_name);
          if (cfg.name.back().value().empty())
          {
            cfg.name += default_name;
          }
        }
      }

      return true;
    }

    bool rapid_compile_commands_parser::StartObject()
    {
      not_empty();

      if (!_in_list)
      {
        fail("The top level element is expected to be a list");
        return false;
      }

      _data = _defaults;
      return true;
    }

    bool rapid_compile_commands_parser::end_object()
    {
      assert(bool(_data));

      not_empty();

      if (!_name)
      {
        fail("The file element of a compilation unit is missing");
        return false;
      }
      else
      {
        _configs.emplace_back(*_name, *_data);
        _name = std::nullopt;
        _data = std::nullopt;

        return true;
      }
    }

    bool rapid_compile_commands_parser::key(const std::string& str_)
    {
      assert(bool(_data));

      not_empty();

      _key = str_;
      return true;
    }

    bool rapid_compile_commands_parser::string(const std::string& str_)
    {
      not_empty();

      if (_data && _key)
      {
        if (*_key == "command")
        {
          compile_command_available(
              parse_compiler_command(data::shell_command_view(str_)));
        }
        else if (*_key == "file")
        {
          _name = data::shell_config_name(str_.empty() ? "_" : str_);
        }
        else if (*_key == "directory")
        {
          _data->cwd = str_;
        }
        else if (_arguments)
        {
          _arguments->push_back(data::command_line_argument{str_});
        }

        return true;
      }
      else
      {
        fail("Unexpected string element: \"" + str_ + "\"");
        return false;
      }
    }

    bool rapid_compile_commands_parser::Bool(bool)
    {
      not_empty();

      // ignore

      return true;
    }

    const std::vector<data::shell_config>&
    rapid_compile_commands_parser::configs() const
    {
      return _configs;
    }

    void rapid_compile_commands_parser::compile_command_available(
        const data::command_line_argument_list& cmd_)
    {
      assert(_data);

      const auto removed = remove_multiple_arch_arguments(cmd_);
      if (removed.first)
      {
        _data->warnings.push_back(*removed.first);
      }
      _data->engine.default_value().args = removed.second;
    }
  } // namespace core
} // namespace metashell
