// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/metaprogram_node.hpp>

#include <iostream>
#include <variant>

namespace metashell
{
  namespace data
  {
    namespace
    {
      struct to_string_visitor
      {
        template <class T>
        std::string operator()(const T& value_) const
        {
          using std::to_string;
          return to_string(value_);
        }

        std::string operator()(const boost::filesystem::path& value_) const
        {
          return value_.string();
        }
      };

      class ostream_visitor
      {
      public:
        explicit ostream_visitor(std::ostream& out_) : _out(out_) {}

        template <class T>
        void operator()(const T& value_) const
        {
          _out << value_;
        }

      private:
        std::ostream& _out;
      };
    } // namespace

    metaprogram_node::metaprogram_node() : _value(type()) {}

    metaprogram_node::metaprogram_node(type value_) : _value(std::move(value_))
    {
    }

    metaprogram_node::metaprogram_node(token value_) : _value(std::move(value_))
    {
    }

    metaprogram_node::metaprogram_node(cpp_code value_)
      : _value(std::move(value_))
    {
    }

    metaprogram_node::metaprogram_node(boost::filesystem::path value_)
      : _value(std::move(value_))
    {
    }

    const std::variant<type, token, cpp_code, boost::filesystem::path>&
    metaprogram_node::variant() const
    {
      return _value;
    }

    std::string to_string(const metaprogram_node& n_)
    {
      to_string_visitor v;
      return std::visit(v, n_.variant());
    }

    std::ostream& operator<<(std::ostream& out_, const metaprogram_node& n_)
    {
      ostream_visitor v(out_);
      std::visit(v, n_.variant());
      return out_;
    }

    bool operator==(const metaprogram_node& a_, const metaprogram_node& b_)
    {
      return a_.variant() == b_.variant();
    }
  } // namespace data
} // namespace metashell
