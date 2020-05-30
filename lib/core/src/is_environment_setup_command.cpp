// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/for_each_line.hpp>
#include <metashell/core/is_environment_setup_command.hpp>
#include <metashell/core/source_position.hpp>
#include <metashell/core/unsaved_file.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/exception.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <memory>
#include <optional>
#include <regex>

namespace metashell
{
  namespace core
  {
    namespace
    {
      bool has_typedef(const data::command::iterator& begin_,
                       const data::command::iterator& end_)
      {
        return std::find_if(begin_, end_, [](const data::token& t_) {
                 return type_of(t_) == data::token_type::keyword_typedef;
               }) != end_;
      }

      data::token_type
      last_non_whitespace_token_type(data::command::iterator begin_,
                                     data::command::iterator end_)
      {
        data::token_type t = data::token_type::unknown;
        for (data::command::iterator i = begin_; i != end_; ++i)
        {
          const data::token_category c = category(*i);
          if (c != data::token_category::whitespace &&
              c != data::token_category::comment)
          {
            t = type_of(*i);
          }
        }
        return t;
      }
    } // anonymous namespace

    bool is_environment_setup_command(data::command::iterator begin_,
                                      const data::command::iterator& end_)
    {
      try
      {
        begin_ = skip_whitespace(begin_, end_);

        if (begin_ == end_)
        {
          // empty input is not a query
          return true;
        }
        else
        {
          const data::token t = *begin_;
          switch (category(t))
          {
          case data::token_category::keyword:
            switch (type_of(t))
            {
            case data::token_type::keyword_bool:
            case data::token_type::keyword_char:
            case data::token_type::keyword_const:
            case data::token_type::keyword_double:
            case data::token_type::keyword_float:
            case data::token_type::keyword_int:
            case data::token_type::keyword_long:
            case data::token_type::keyword_short:
            case data::token_type::keyword_signed:
            case data::token_type::keyword_unsigned:
            case data::token_type::keyword_void:
            case data::token_type::keyword_volatile:
            case data::token_type::keyword_wchar_t:
              if (has_typedef(begin_, end_))
              {
                return true;
              }
              else
              {
                const data::token_type lt =
                    last_non_whitespace_token_type(begin_, end_);
                return lt == data::token_type::operator_semicolon ||
                       lt == data::token_type::operator_right_brace;
              }
            case data::token_type::keyword_sizeof:
            case data::token_type::keyword_const_cast:
            case data::token_type::keyword_static_cast:
            case data::token_type::keyword_dynamic_cast:
            case data::token_type::keyword_reinterpret_cast:
              return false;
            default:
              return true;
            }
            assert(false);
          case data::token_category::identifier:
            return has_typedef(begin_, end_) ||
                   last_non_whitespace_token_type(begin_, end_) ==
                       data::token_type::operator_semicolon;
          case data::token_category::preprocessor:
            return true;
          default:
            return false;
          }
        }
      }
      catch (...)
      {
        return false;
      }
    }
  }
}
