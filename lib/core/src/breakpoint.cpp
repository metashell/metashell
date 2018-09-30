// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/core/breakpoint.hpp>

#include <sstream>

namespace metashell
{
  namespace core
  {
    namespace
    {
      class match_visitor : public boost::static_visitor<>
      {
      public:
        explicit match_visitor(const boost::regex& regex_) : _regex(regex_) {}

        template <class T>
        void operator()(const T& t_)
        {
          _last_result = match(t_);
        }

        bool last_result() const { return _last_result; }
      private:
        boost::regex _regex;
        bool _last_result;

        bool match(const std::string& s_) const
        {
          return boost::regex_search(s_, _regex);
        }

        bool match(const data::cpp_code& code_) const
        {
          return match(code_.value());
        }

        bool match(const data::type& type_) const
        {
          return match(type_.name());
        }

        bool match(const data::token& token_) const
        {
          return match(token_.value());
        }

        bool match(const boost::filesystem::path& path_) const
        {
          return match(path_.string());
        }
      };
    }
    breakpoint::breakpoint(int id, const boost::regex& name_regex)
      : id(id), name_regex(name_regex)
    {
    }

    bool breakpoint::match(const data::metaprogram_node& node) const
    {
      if (name_regex)
      {
        match_visitor v(*name_regex);
        boost::apply_visitor(v, node);
        return v.last_result();
      }
      else
      {
        return false;
      }
    }

    int breakpoint::get_id() const { return id; }

    std::string breakpoint::to_string() const
    {
      std::stringstream ss;
      ss << "Breakpoint " << id << ": ";
      if (name_regex)
      {
        ss << "regex(\"" + name_regex->str() + "\")";
      }
      else
      {
        ss << "unknown()";
      }
      return ss.str();
    }
  }
} // namespace metashell
