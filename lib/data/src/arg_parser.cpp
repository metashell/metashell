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

#include <numeric>

namespace metashell
{
  namespace data
  {
    arg_parser::arg_parser()
      : _invalid_argument([](const command_line_argument& arg_) {
          return "Invalid argument: " + arg_;
        })
    {
    }

    arg_parser::arg_parser(std::function<std::string(
                               const command_line_argument&)> invalid_argument_)
      : _invalid_argument(std::move(invalid_argument_))
    {
    }

    arg_parser::flag_definition::flag_definition(command_line_argument name_,
                                                 std::string description_,
                                                 std::function<void()> action_)
      : name(std::move(name_)),
        description(std::move(description_)),
        action(std::move(action_))
    {
    }

    arg_parser::arg_with_value_definition::arg_with_value_definition(
        command_line_argument short_name_,
        command_line_argument long_name_,
        std::string description_,
        std::function<void(command_line_argument)> action_)
      : short_name(std::move(short_name_)),
        long_name(std::move(long_name_)),
        description(std::move(description_)),
        action(std::move(action_))
    {
    }

    arg_parser::arg_with_value_definition::arg_with_value_definition(
        command_line_argument short_name_,
        std::string description_,
        std::function<void(command_line_argument)> action_)
      : short_name(std::move(short_name_)),
        long_name(boost::none),
        description(std::move(description_)),
        action(std::move(action_))
    {
    }

    std::string arg_parser::arg_with_value_definition::format_name() const
    {
      std::string result = short_name.value();
      if (long_name)
      {
        result += " [ " + long_name->value() + " ]";
      }
      return result;
    }

    void arg_parser::parse(const command_line_argument_list& args_)
    {
      using prev_t =
          boost::optional<std::vector<arg_with_value_definition>::iterator>;

      if (const prev_t prev = std::accumulate(
              args_.begin(), args_.end(), prev_t(),
              [this](prev_t prev_, const command_line_argument& arg_) {
                return this->parse(prev_, arg_);
              }))
      {
        throw exception("Missing value of " + (*prev)->short_name);
      }
    }

    boost::optional<
        std::vector<arg_parser::arg_with_value_definition>::iterator>
    arg_parser::parse(
        boost::optional<std::vector<arg_with_value_definition>::iterator> prev_,
        const command_line_argument& arg_)
    {
      if (prev_)
      {
        (*prev_)->action(arg_);
        return boost::none;
      }

      const auto flag = std::find_if(
          _flags.begin(), _flags.end(),
          [&arg_](const flag_definition& flag_) { return arg_ == flag_.name; });
      if (flag != _flags.end())
      {
        flag->action();
        return boost::none;
      }

      for (auto i = _args_with_value.begin(); i != _args_with_value.end(); ++i)
      {
        if (arg_ == i->short_name || arg_ == i->long_name)
        {
          return i;
        }
        else if (const auto value = arg_.remove_prefix(i->short_name))
        {
          i->action(*value);
          return boost::none;
        }
      }

      throw exception(_invalid_argument(arg_));
    }

    std::string arg_parser::description() const
    {
      std::vector<std::pair<std::string, std::string>> args;
      args.reserve(_args_with_value.size() + _flags.size());

      size_t width = 0;
      for (const auto& arg : _args_with_value)
      {
        args.emplace_back(arg.format_name(), arg.description);
        width = std::max(args.back().first.size(), width);
      }
      for (const auto& flag : _flags)
      {
        args.emplace_back(flag.name.value(), flag.description);
        width = std::max(args.back().first.size(), width);
      }

      std::ostringstream s;
      for (const auto& arg : args)
      {
        s << "&nbsp;&nbsp;`" << arg.first << "`"
          << std::string(width - arg.first.size() + 5, ' ') << arg.second
          << "<br />";
      }

      return s.str();
    }
  }
}
