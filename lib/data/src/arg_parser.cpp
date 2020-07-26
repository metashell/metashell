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

#include <metashell/data/arg_parser.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/indenter.hpp>

#include <boost/algorithm/string/replace.hpp>

#include <numeric>

namespace metashell
{
  namespace data
  {
    namespace
    {
      class decommissioned
      {
      public:
        explicit decommissioned(const std::string& msg_,
                                const command_line_argument& name_)
          : _error{boost::replace_all_copy(msg_, "{NAME}", name_.value())}
        {
        }

        template <class... Ts>
        void operator()(Ts&&...) const
        {
          throw _error;
        }

      private:
        exception _error;
      };

      std::string deco_description(const std::string& suggestion_)
      {
        return "DECOMMISSIONED argument." +
               (suggestion_.empty() ? std::string{} : std::string{" "}) +
               suggestion_;
      }

      template <int N>
      std::string repeat(const std::string& s_)
      {
        std::string result;
        for (int i = 0; i < N; ++i)
        {
          result += s_;
        }
        return result;
      }
    }

    arg_parser::state::state(
        const std::vector<flag_definition>& flags_,
        const std::vector<arg_with_value_definition>& with_values_)
      : _flags(flags_.size()), _with_values(with_values_.size())
    {
    }

    arg_parser::state::iterator arg_parser::state::begin_flags()
    {
      return _flags.begin();
    }

    arg_parser::state::iterator arg_parser::state::begin_with_values()
    {
      return _with_values.begin();
    }

    arg_parser::arg_parser()
      : arg_parser{[](const command_line_argument& arg_) {
          return "Invalid argument: " + arg_;
        }}
    {
    }

    arg_parser::arg_parser(
        std::function<std::string(const command_line_argument&)>
            invalid_argument_,
        std::function<bool(const command_line_argument&)> unknown_arg_handler_)
      : _invalid_argument{std::move(invalid_argument_)},
        _unknown_arg_handler{std::move(unknown_arg_handler_)}
    {
    }

    arg_parser& arg_parser::flag(command_line_argument short_name_,
                                 std::string desc_,
                                 arg_number num_,
                                 std::function<void()> action_)
    {
      _flags.emplace_back(
          std::move(short_name_), std::move(desc_), num_, std::move(action_));
      return *this;
    }

    arg_parser& arg_parser::flag(no_short_name_t short_name_,
                                 command_line_argument long_name_,
                                 std::string desc_,
                                 arg_number num_,
                                 std::function<void()> action_)
    {
      _flags.emplace_back(short_name_, std::move(long_name_), std::move(desc_),
                          num_, std::move(action_));
      return *this;
    }

    arg_parser& arg_parser::flag(command_line_argument short_name_,
                                 command_line_argument long_name_,
                                 std::string desc_,
                                 arg_number num_,
                                 std::function<void()> action_)
    {
      _flags.emplace_back(std::move(short_name_), std::move(long_name_),
                          std::move(desc_), num_, std::move(action_));
      return *this;
    }

    void arg_parser::parse(const command_line_argument_list& args_)
    {
      state parsing_state{_flags, _args_with_value};
      const arg_with_value_definition* no_prev = nullptr;
      if (const arg_with_value_definition* prev = std::accumulate(
              args_.begin(), args_.end(), no_prev,
              [this, &parsing_state](const arg_with_value_definition* prev_,
                                     const command_line_argument& arg_) {
                return this->parse(prev_, arg_, parsing_state);
              }))
      {
        throw exception("Missing value of " + prev->name());
      }
    }

    const arg_parser::arg_with_value_definition*
    arg_parser::parse(const arg_with_value_definition* prev_,
                      const command_line_argument& arg_,
                      state& parsing_state_)
    {
      if (prev_)
      {
        prev_->action(arg_);
        return nullptr;
      }

      auto flag_i = parsing_state_.begin_flags();
      for (const auto& flag : _flags)
      {
        switch (flag.process(arg_, *flag_i))
        {
        case flag_definition::processing_result::not_this_argument:
          break;
        case flag_definition::processing_result::argument_processed:
          return nullptr;
        }

        ++flag_i;
      }

      auto with_value_i = parsing_state_.begin_with_values();
      for (const auto& arg : _args_with_value)
      {
        switch (arg.process(arg_, *with_value_i))
        {
        case arg_with_value_definition::processing_result::not_this_argument:
          break;
        case arg_with_value_definition::processing_result::argument_processed:
          return nullptr;
        case arg_with_value_definition::processing_result::value_comes_next:
          return &arg;
        }

        ++with_value_i;
      }

      if (_unknown_arg_handler(arg_))
      {
        return nullptr;
      }
      else
      {
        throw exception{_invalid_argument(arg_)};
      }
    }

    markdown_string arg_parser::description(int console_width_) const
    {
      constexpr int prefix_len = 2;
      constexpr int column_sep = 5;

      std::vector<std::pair<std::string, std::string>> args;
      args.reserve(_args_with_value.size() + _flags.size());

      size_t width = 0;
      for (const auto& arg : _args_with_value)
      {
        args.emplace_back(arg.format_name(), arg.description());
        width = std::max(args.back().first.size(), width);
      }
      for (const auto& flag : _flags)
      {
        args.emplace_back(flag.format_name(), flag.description());
        width = std::max(args.back().first.size(), width);
      }

      width += prefix_len + column_sep;

      const std::string line_sep{"<br />"};

      std::ostringstream s;
      for (const auto& arg : args)
      {
        const std::string first_line =
            repeat<prefix_len>("&nbsp;") + "`" + arg.first + "`" +
            std::string(width - arg.first.size() - prefix_len, ' ');
        const auto extra_chars_in_first_line = first_line.size() - width;
        s << first_line.substr(0, extra_chars_in_first_line)
          << data::indenter{console_width_, line_sep}
                 .left_align(arg.second, std::string(width, ' '),
                             first_line.substr(extra_chars_in_first_line))
                 .str();
      }

      return markdown_string{s.str()};
    }

    arg_parser&
    arg_parser::decommissioned_flag(const command_line_argument& short_name_,
                                    const std::string& suggestion_,
                                    const std::string& error_msg_)
    {
      const exception ex{
          boost::replace_all_copy(error_msg_, "{NAME}", short_name_.value())};

      return flag(short_name_, deco_description(suggestion_), arg_number::any,
                  decommissioned{error_msg_, short_name_});
    }

    arg_parser& arg_parser::decommissioned_with_value(
        no_short_name_t short_name_,
        const command_line_argument& long_name_,
        const std::string& suggestion_,
        const std::string& error_msg_)
    {
      return flag(short_name_, long_name_, deco_description(suggestion_),
                  arg_number::any, decommissioned{error_msg_, long_name_});
    }
  }
}
