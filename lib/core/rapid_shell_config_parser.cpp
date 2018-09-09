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

#include <metashell/core/rapid_shell_config_parser.hpp>

#include <boost/optional.hpp>

#include <algorithm>
#include <cassert>
#include <map>

namespace metashell
{
  namespace core
  {
    namespace
    {
      enum class field_type
      {
        string_,
        bool_,
        list_
      };

      std::string to_string(field_type v_)
      {
        switch (v_)
        {
        case field_type::string_:
          return "string";
        case field_type::bool_:
          return "bool";
        case field_type::list_:
          return "list of strings";
        }
        assert(!"This point should not be reached");
        return "";
      }

      std::string to_string(boost::optional<field_type> v_)
      {
        return v_ ? to_string(*v_) : "";
      }

      boost::optional<field_type> type_of_field(const std::string& field_)
      {
        static const std::map<std::string, field_type> fields{
            {"name", field_type::string_},
            {"engine", field_type::string_},
            {"engine_args", field_type::list_},
            {"use_precompiled_headers", field_type::bool_},
            {"preprocessor_mode", field_type::bool_}};

        const auto i = fields.find(field_);
        return i == fields.end() ? boost::none :
                                   boost::make_optional(i->second);
      }
    }

    bool rapid_shell_config_parser::StartArray()
    {
      not_empty();

      if (_config && _key)
      {
        const auto t = type_of_field(*_key);

        if (_in_engine_args)
        {
          fail("A list containing a list is not a valid value for " + *_key +
               ", which should be a " + to_string(t));
          return false;
        }
        else if (t == field_type::list_)
        {
          _in_engine_args = true;
          return true;
        }
        else
        {
          fail("A list is not a valid value for " + *_key +
               ", which should be a " + to_string(t));
          return false;
        }
      }
      else if (_in_list || _config)
      {
        fail("Unexpected array");
        return false;
      }
      else
      {
        _in_list = true;
        return true;
      }
    }

    bool rapid_shell_config_parser::end_array()
    {
      not_empty();

      _in_list = false;
      _in_engine_args = false;
      _key = boost::none;

      return true;
    }

    bool rapid_shell_config_parser::StartObject()
    {
      not_empty();

      if (_config || _in_engine_args)
      {
        fail("Unexpected object");
        return false;
      }

      _config = data::shell_config();
      return true;
    }

    bool rapid_shell_config_parser::end_object()
    {
      assert(bool(_config));

      not_empty();

      if (_config->name.empty())
      {
        fail("The name of a config is missing");
        return false;
      }
      else
      {
        parsed_config_callback(std::move(*_config));
        _config = boost::none;

        return true;
      }
    }

    bool rapid_shell_config_parser::key(const std::string& str_)
    {
      assert(bool(_config));

      not_empty();

      if (type_of_field(str_))
      {
        _key = str_;
        return true;
      }
      else
      {
        fail("Invalid key: " + str_);
        return false;
      }
    }

    bool rapid_shell_config_parser::string(const std::string& str_)
    {
      not_empty();

      if (_config && _key)
      {
        const auto t = type_of_field(*_key);

        if (t == field_type::string_)
        {
          if (*_key == "name")
          {
            _config->name = data::shell_config_name(str_);
          }
          else if (*_key == "engine")
          {
            _config->engine = str_;
          }
          else
          {
            assert(false);
          }

          return true;
        }
        else if (t == field_type::list_ && _in_engine_args)
        {
          _config->engine_args.push_back(str_);
          return true;
        }
        else
        {
          fail("\"" + str_ + "\" is not a valid value for " + *_key +
               ", which should be a " + to_string(t));
          return false;
        }
      }
      else
      {
        fail("Unexpected string element: \"" + str_ + "\"");
        return false;
      }
    }

    bool rapid_shell_config_parser::Bool(bool b_)
    {
      not_empty();

      const std::string value = b_ ? "true" : "false";

      if (_config && _key)
      {
        const auto t = type_of_field(*_key);

        if (t == field_type::bool_)
        {
          if (*_key == "use_precompiled_headers")
          {
            _config->use_precompiled_headers = b_;
          }
          else if (*_key == "preprocessor_mode")
          {
            _config->preprocessor_mode = b_;
          }
          else
          {
            assert(false);
          }

          return true;
        }
        else
        {
          fail(value + " is not a valid value for " + *_key +
               ", which should be a " + to_string(t));
          return false;
        }
      }
      else
      {
        fail("Unexpected bool element: " + value);
        return false;
      }
    }
  }
}
