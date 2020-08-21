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

#include <metashell/data/argument_definition.hpp>
#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/counter.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/markdown_string.hpp>

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class arg_parser
    {
    private:
      static constexpr char default_deco_error[] =
          "Argument {NAME} has been decommissioned. Please provide it as a "
          "compiler argument after --";

    public:
      arg_parser();

      explicit arg_parser(
          std::function<std::string(const command_line_argument&)>,
          std::function<bool(const command_line_argument&)>
              unknown_arg_handler_ =
                  [](const command_line_argument&) { return false; });

      arg_parser& flag(command_line_argument short_name_,
                       std::string desc_,
                       arg_number,
                       std::function<void()> action_);

      arg_parser& flag(no_short_name_t,
                       command_line_argument long_name_,
                       std::string desc_,
                       arg_number,
                       std::function<void()> action_);

      arg_parser& flag(command_line_argument short_name_,
                       command_line_argument long_name_,
                       std::string desc_,
                       arg_number,
                       std::function<void()> action_);

      template <arg_number Number, size_t Len>
      arg_parser& flag(const char (&short_name_)[Len],
                       std::string desc_,
                       std::function<void()> action_)
      {
        return flag(command_line_argument{short_name_}, std::move(desc_),
                    Number, std::move(action_));
      }

      template <arg_number Number, size_t Len1, size_t Len2>
      arg_parser& flag(const char (&short_name_)[Len1],
                       const char (&long_name_)[Len2],
                       std::string desc_,
                       std::function<void()> action_)
      {
        return flag(command_line_argument{short_name_},
                    command_line_argument{long_name_}, std::move(desc_), Number,
                    std::move(action_));
      }

      template <bool Default, size_t Len>
      arg_parser&
      flag(const char (&short_name_)[Len], std::string desc_, bool& flag_)
      {
        flag_ = Default;
        return flag<arg_number::at_most_once>(
            short_name_, std::move(desc_), [&flag_] { flag_ = !Default; });
      }

      template <bool Default, size_t Len1, size_t Len2>
      arg_parser& flag(const char (&short_name_)[Len1],
                       const char (&long_name_)[Len2],
                       std::string desc_,
                       bool& flag_)
      {
        flag_ = Default;
        return flag<arg_number::at_most_once>(short_name_, long_name_,
                                              std::move(desc_),
                                              [&flag_] { flag_ = !Default; });
      }

      template <arg_number Number, size_t Len>
      arg_parser& flag(const char (&short_name_)[Len],
                       const std::vector<std::string>& suffixes_,
                       std::string desc_,
                       std::function<void()> action_)
      {
        for (const std::string& suffix : suffixes_)
        {
          flag(command_line_argument{short_name_ + suffix}, desc_, Number,
               action_);
        }
        return flag(command_line_argument{short_name_}, std::move(desc_),
                    Number, std::move(action_));
      }

      template <arg_number Number, size_t Len1, size_t Len2>
      arg_parser& with_value(const char (&short_name_)[Len1],
                             const char (&long_name_)[Len2],
                             std::string desc_,
                             std::function<void(command_line_argument)> action_)
      {
        _args_with_value.emplace_back(command_line_argument(short_name_),
                                      command_line_argument(long_name_),
                                      std::move(desc_), Number,
                                      std::move(action_));
        return *this;
      }

      template <arg_number Number, size_t Len>
      arg_parser& with_value(const char (&short_name_)[Len],
                             std::string desc_,
                             std::function<void(command_line_argument)> action_)
      {
        _args_with_value.emplace_back(command_line_argument(short_name_),
                                      std::move(desc_), Number,
                                      std::move(action_));
        return *this;
      }

      template <arg_number Number, size_t Len>
      arg_parser& with_value(no_short_name_t no_short_name_,
                             const char (&long_name_)[Len],
                             std::string desc_,
                             std::function<void(command_line_argument)> action_)
      {
        _args_with_value.emplace_back(
            no_short_name_, command_line_argument(long_name_), std::move(desc_),
            Number, std::move(action_));
        return *this;
      }

      template <size_t Len1, size_t Len2, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len1],
                             const char (&long_name_)[Len2],
                             std::string desc_,
                             std::vector<ValueType>& out_)
      {
        return with_value<arg_number::any>(
            short_name_, long_name_, desc_,
            [&out_](const command_line_argument& item_) {
              out_.emplace_back(item_.value());
            });
      }

      template <size_t Len, class ValueType>
      arg_parser& with_value(no_short_name_t short_name_,
                             const char (&long_name_)[Len],
                             std::string desc_,
                             std::vector<ValueType>& out_)
      {
        return with_value<arg_number::any>(
            short_name_, long_name_, desc_,
            [&out_](const command_line_argument& item_) {
              out_.emplace_back(item_.value());
            });
      }

      template <size_t Len, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len],
                             std::string desc_,
                             std::vector<ValueType>& out_)
      {
        return with_value<arg_number::any>(
            short_name_, desc_, [&out_](const command_line_argument& item_) {
              out_.emplace_back(item_.value());
            });
      }

      template <size_t Len1, size_t Len2, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len1],
                             const char (&long_name_)[Len2],
                             std::string desc_,
                             std::optional<ValueType>& out_)
      {
        return with_value<arg_number::at_most_once>(
            short_name_, long_name_, desc_,
            [&out_](const command_line_argument& item_) {
              out_ = ValueType(item_);
            });
      }

      template <size_t Len, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len],
                             std::string desc_,
                             std::optional<ValueType>& out_)
      {
        return with_value<arg_number::at_most_once>(
            short_name_, desc_, [&out_](const command_line_argument& item_) {
              out_ = ValueType(item_);
            });
      }

      template <size_t Len1, size_t Len2, class ValueType>
      arg_parser& with_value(const char (&short_name_)[Len1],
                             const char (&long_name_)[Len2],
                             std::string desc_,
                             ValueType& out_)
      {
        return with_value<arg_number::at_most_once>(
            short_name_, long_name_, desc_,
            [&out_](const command_line_argument& item_) {
              out_ = ValueType(item_.value());
            });
      }

      template <size_t Len, class ValueType>
      arg_parser& with_value(no_short_name_t short_name_,
                             const char (&long_name_)[Len],
                             std::string desc_,
                             ValueType& out_)
      {
        return with_value<arg_number::at_most_once>(
            short_name_, long_name_, desc_,
            [&out_](const command_line_argument& item_) {
              out_ = ValueType(item_.value());
            });
      }

      arg_parser& decommissioned_flag(const command_line_argument& short_name_,
                                      const std::string& suggestion_,
                                      const std::string& error_msg_);

      arg_parser&
      decommissioned_with_value(no_short_name_t,
                                const command_line_argument& long_name_,
                                const std::string& suggestion_,
                                const std::string& error_msg_);

      template <size_t Len>
      arg_parser& decommissioned_flag(const char (&short_name_)[Len],
                                      const std::string& suggestion_)
      {
        return decommissioned_flag(command_line_argument{short_name_},
                                   suggestion_, default_deco_error);
      }

      template <size_t Len>
      arg_parser& decommissioned_flag(const char (&short_name_)[Len],
                                      const std::string& suggestion_,
                                      const std::string& error_msg_)
      {
        return decommissioned_flag(
            command_line_argument{short_name_}, suggestion_, error_msg_);
      }

      template <size_t Len>
      arg_parser& decommissioned_with_value(no_short_name_t short_name_,
                                            const char (&long_name_)[Len],
                                            const std::string& suggestion_)
      {
        return decommissioned_with_value(short_name_,
                                         command_line_argument{long_name_},
                                         suggestion_, default_deco_error);
      }

      template <size_t Len>
      arg_parser& decommissioned_with_value(no_short_name_t short_name_,
                                            const char (&long_name_)[Len],
                                            const std::string& suggestion_,
                                            const std::string& error_msg_)
      {
        return decommissioned_with_value(short_name_,
                                         command_line_argument{long_name_},
                                         suggestion_, error_msg_);
      }

      void parse(const command_line_argument_list& args_);

      markdown_string description(int console_width_) const;

    private:
      using flag_definition = argument_definition<false>;
      using arg_with_value_definition = argument_definition<true>;

      class state
      {
      public:
        using counters = std::vector<counter>;
        using iterator = counters::iterator;

        state(const std::vector<flag_definition>&,
              const std::vector<arg_with_value_definition>&);

        iterator begin_flags();
        iterator begin_with_values();

      private:
        counters _flags;
        counters _with_values;
      };

      std::vector<flag_definition> _flags;
      std::vector<arg_with_value_definition> _args_with_value;
      std::function<std::string(const command_line_argument&)>
          _invalid_argument;
      std::function<bool(const command_line_argument&)> _unknown_arg_handler;

      const arg_with_value_definition* parse(const arg_with_value_definition*,
                                             const command_line_argument&,
                                             state&);
    };
  }
}

#endif
