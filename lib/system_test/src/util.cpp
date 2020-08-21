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

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <map>
#include <stdexcept>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    namespace
    {
      std::vector<std::pair<std::string, std::string>> include_definitions()
      {
        if (using_msvc())
        {
          return {{"/I", ""}, {"\"/I", "\""}, {"/I\"", "\""}};
        }
        else
        {
          return {{"-I", ""},
                  {"-iquote", ""},
                  {"-isystem", ""},
                  {"-idirafter", ""}};
        }
      }

      template <class OutputIterator>
      OutputIterator write_to(OutputIterator dest_, char c_)
      {
        *dest_ = c_;
        ++dest_;
        return dest_;
      }
    }

    std::string remove_prefix(const std::string& prefix_, const std::string& s_)
    {
      if (const auto p = try_to_remove_prefix(prefix_, s_))
      {
        return *p;
      }
      else
      {
        throw std::runtime_error("String \"" + s_ + "\" has no \"" + prefix_ +
                                 "\" prefix");
      }
    }

    std::optional<std::string> try_to_remove_prefix(const std::string& prefix_,
                                                    const std::string& s_)
    {
      return try_to_remove_prefix_suffix(prefix_, s_, "");
    }

    std::optional<std::string>
    try_to_remove_prefix_suffix(const std::string& prefix_,
                                const std::string& s_,
                                const std::string& suffix_)
    {
      const auto pre_suff_size = prefix_.size() + suffix_.size();
      if (s_.size() >= pre_suff_size &&
          boost::algorithm::starts_with(s_, prefix_) &&
          boost::algorithm::ends_with(s_, suffix_))
      {
        return s_.substr(prefix_.size(), s_.size() - pre_suff_size);
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<boost::filesystem::path>
    include_path_addition(const data::command_line_argument& arg_)
    {
      for (const auto& def : include_definitions())
      {
        if (const auto path = try_to_remove_prefix_suffix(
                def.first, arg_.value(), def.second))
        {
          return boost::filesystem::path(*path);
        }
      }
      return std::nullopt;
    }

    std::string c_string_literal(const std::string& s_)
    {
      std::vector<char> s(s_.size() * 2 + 2);
      auto end = write_to(s.begin(), '\"');
      for (char c : s_)
      {
        switch (c)
        {
        case '\\':
        case '\"':
        case '\'':
          end = write_to(end, '\\');
          [[fallthrough]];
        default:
          end = write_to(end, c);
        }
      }
      return std::string(s.begin(), write_to(end, '\"'));
    }

    std::string new_line()
    {
#ifdef _WIN32
      return "\r\n";
#else
      return "\n";
#endif
    }

    void write_file(const boost::filesystem::path& p_,
                    const std::string& content_)
    {
      const std::string p = p_.string();
      std::ofstream f(p);
      if (f)
      {
        if (!(f << content_))
        {
          throw std::runtime_error("Failed to write into file " + p);
        }
      }
      else
      {
        throw std::runtime_error("Failed to create file " + p);
      }
    }

    json_string test_config(const std::string& name_)
    {
      rapidjson::StringBuffer buff;
      rapidjson::Writer<rapidjson::StringBuffer> w(buff);

      w.StartObject();

      w.Key("name");
      w.String(name_.c_str());

      w.Key("engine");
      w.String("internal");

      w.Key("engine_args");
      w.StartArray();
      w.String("-I.");
      w.EndArray();

      w.Key("use_precompiled_headers");
      w.Bool(true);

      w.Key("preprocessor_mode");
      w.Bool(false);

      w.Key("cwd");
      w.String("");

      w.EndObject();

      return json_string(buff.GetString());
    }
  }
}
