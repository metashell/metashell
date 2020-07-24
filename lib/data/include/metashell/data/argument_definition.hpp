#ifndef METASHELL_DATA_ARGUMENT_DEFINITON_HPP
#define METASHELL_DATA_ARGUMENT_DEFINITON_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/arg_number.hpp>
#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/no_short_name.hpp>

#include <cassert>
#include <functional>
#include <optional>
#include <string>
#include <type_traits>

namespace metashell
{
  namespace data
  {
    enum class argument_definition_processing_result_with_value
    {
      not_this_argument,
      argument_processed,
      value_comes_next
    };

    enum class argument_definition_processing_result_without_value
    {
      not_this_argument,
      argument_processed
    };

    template <bool WithArg>
    class argument_definition
    {
    public:
      using processing_result = typename std::conditional<
          WithArg,
          argument_definition_processing_result_with_value,
          argument_definition_processing_result_without_value>::type;

      using callback_t =
          typename std::conditional<WithArg,
                                    std::function<void(command_line_argument)>,
                                    std::function<void()>>::type;

      argument_definition(command_line_argument short_name_,
                          command_line_argument long_name_,
                          std::string description_,
                          arg_number number_,
                          callback_t action_)
        : _short_name{std::move(short_name_)},
          _long_name{std::move(long_name_)},
          _description{std::move(description_)},
          _action{std::move(action_)},
          _number{number_}
      {
        assert(_action);
        assert(invariant());
      }

      argument_definition(command_line_argument short_name_,
                          std::string description_,
                          arg_number number_,
                          callback_t action_)
        : _short_name{std::move(short_name_)},
          _long_name{},
          _description{std::move(description_)},
          _action{std::move(action_)},
          _number{number_}
      {
        assert(_action);
        assert(invariant());
      }

      argument_definition(no_short_name_t,
                          command_line_argument long_name_,
                          std::string description_,
                          arg_number number_,
                          callback_t action_)
        : _short_name{},
          _long_name{std::move(long_name_)},
          _description{std::move(description_)},
          _action{std::move(action_)},
          _number{number_}
      {
        assert(_action);
        assert(invariant());
      }

      std::string format_name() const
      {
        if (_short_name)
        {
          return _long_name ?
                     _short_name->value() + " [ " + _long_name->value() + " ]" :
                     _short_name->value();
        }
        else
        {
          assert(_long_name);
          return _long_name->value();
        }
      }

      const command_line_argument& name() const
      {
        assert(invariant());
        return _long_name ? *_long_name : *_short_name;
      }

      void action() const { _action(); }

      template <class CounterT>
      processing_result process(const command_line_argument& arg_,
                                basic_counter<CounterT>& occured_) const
      {
        auto argument_used = [this, &occured_] {
          if (!allow_one_more(this->_number, occured_))
          {
            throw exception{this->format_name() + " occured too many times."};
          }
          ++occured_;
        };

        if (_short_name == arg_ || _long_name == arg_)
        {
          argument_used();
          if
            constexpr(WithArg) { return processing_result::value_comes_next; }
          else
          {
            _action();
            return processing_result::argument_processed;
          }
        }

        if
          constexpr(WithArg)
          {
            if (_short_name)
            {
              if (const auto value = arg_.remove_prefix(*_short_name))
              {
                argument_used();
                _action(*value);
                return processing_result::argument_processed;
              }
            }
            if (_long_name)
            {
              if (const auto value = arg_.remove_prefix(
                      *_long_name + command_line_argument{"="}))
              {
                argument_used();
                _action(*value);
                return processing_result::argument_processed;
              }
            }
          }
        return processing_result::not_this_argument;
      }

      void action(command_line_argument value_) const
      {
        _action(std::move(value_));
      }

      const std::string& description() const { return _description; }

    private:
      std::optional<command_line_argument> _short_name;
      std::optional<command_line_argument> _long_name;
      std::string _description;
      callback_t _action;
      arg_number _number;

      bool invariant() const { return _short_name || _long_name; }
    };
  }
}

#endif
