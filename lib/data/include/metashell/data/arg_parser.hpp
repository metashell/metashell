#ifndef METASHELL_DATA_ARG_PARSER_HPP
#define METASHELL_DATA_ARG_PARSER_HPP

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

#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/exception.hpp>

#include <functional>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class arg_parser
    {
    public:
      template <size_t Len>
      arg_parser& flag(const char (&name_)[Len],
                       std::string desc_,
                       std::function<void()> action_)
      {
        _flags.emplace_back(
            command_line_argument(name_), std::move(desc_), std::move(action_));
        return *this;
      }

      template <size_t Len1, size_t Len2>
      arg_parser& with_value(const char (&short_name_)[Len1],
                             const char (&long_name_)[Len2],
                             std::string desc_,
                             std::function<void(command_line_argument)> action_)
      {
        _args_with_value.emplace_back(command_line_argument(short_name_),
                                      command_line_argument(long_name_),
                                      std::move(desc_), std::move(action_));
        return *this;
      }

      template <size_t Len>
      arg_parser& with_value(const char (&short_name_)[Len],
                             std::string desc_,
                             std::function<void(command_line_argument)> action_)
      {
        _args_with_value.emplace_back(command_line_argument(short_name_),
                                      std::move(desc_), std::move(action_));
        return *this;
      }

      template <size_t Len1, size_t Len2, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len1],
                             const char (&long_name_)[Len2],
                             std::string desc_,
                             std::vector<ValueType>& out_)
      {
        return with_value(short_name_, long_name_, desc_,
                          [&out_](const command_line_argument& item_) {
                            out_.emplace_back(item_.value());
                          });
      }

      template <size_t Len, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len],
                             std::string desc_,
                             std::vector<ValueType>& out_)
      {
        return with_value(
            short_name_, desc_, [&out_](const command_line_argument& item_) {
              out_.emplace_back(item_.value());
            });
      }

      void parse(const command_line_argument_list& args_);

      std::string description() const;

    private:
      struct flag_definition
      {
        command_line_argument name;
        std::string description;
        std::function<void()> action;

        flag_definition(command_line_argument name_,
                        std::string description_,
                        std::function<void()> action_);
      };

      struct arg_with_value_definition
      {
        command_line_argument short_name;
        boost::optional<command_line_argument> long_name;
        std::string description;
        std::function<void(command_line_argument)> action;

        arg_with_value_definition(
            command_line_argument short_name_,
            command_line_argument long_name_,
            std::string description_,
            std::function<void(command_line_argument)> action_);

        arg_with_value_definition(
            command_line_argument short_name_,
            std::string description_,
            std::function<void(command_line_argument)> action_);

        std::string format_name() const;
      };

      std::vector<flag_definition> _flags;
      std::vector<arg_with_value_definition> _args_with_value;

      boost::optional<std::vector<arg_with_value_definition>::iterator>
      parse(boost::optional<std::vector<arg_with_value_definition>::iterator>,
            const command_line_argument&);
    };
  }
}

#endif
