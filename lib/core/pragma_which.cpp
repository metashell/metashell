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

#include <metashell/pragma_which.hpp>
#include <metashell/exception.hpp>
#include <metashell/shell.hpp>

#include <boost/filesystem.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  void throw_more_than_one_arguments(const data::command::iterator& name_begin_,
                                     const data::command::iterator& name_end_)
  {
    throw exception("More than one arguments provided to " +
                    data::tokens_to_string(name_begin_, name_end_) + ".");
  }

  template <char Closing, class Pred>
  boost::filesystem::path
  parse_path_until_token(const data::command::iterator& name_begin_,
                         const data::command::iterator& name_end_,
                         data::command::iterator begin_,
                         data::command::iterator end_,
                         Pred is_closing_token_)
  {
    const auto path_end = std::find_if(begin_, end_, is_closing_token_);
    if (path_end == end_)
    {
      throw exception(std::string("closing ") + Closing + " is missing.");
    }
    else if (std::find_if(path_end + 1, end_, [](const data::token& token_)
                          {
                            return token_.category() !=
                                   data::token_category::whitespace;
                          }) != end_)
    {
      throw_more_than_one_arguments(name_begin_, name_end_);
    }
    return data::tokens_to_string(begin_, path_end);
  }

  bool include_quote_token(const data::token& token_)
  {
    return token_.type() == data::token_type::unknown && token_.value() == "\"";
  }
}

pragma_which::pragma_which(shell& shell_) : _shell(shell_) {}

iface::pragma_handler* pragma_which::clone() const
{
  return new pragma_which(_shell);
}

std::string pragma_which::arguments() const
{
  return "[<include file>|\"include file\"]";
}

std::string pragma_which::description() const
{
  return "Searches the given include file on the include path.";
}

void pragma_which::run(const data::command::iterator& name_begin_,
                       const data::command::iterator& name_end_,
                       const data::command::iterator& args_begin_,
                       const data::command::iterator& args_end_,
                       iface::displayer& displayer_) const
{
  using boost::filesystem::path;

  const std::pair<data::include_type, path> include_file =
      parse_arguments(name_begin_, name_end_, args_begin_, args_end_);
  const auto include_path = _shell.engine().include_path(include_file.first);
  const auto include_dir =
      std::find_if(include_path.begin(), include_path.end(),
                   [&include_file](const path& path_)
                   {
                     return exists(path_ / include_file.second);
                   });

  if (include_dir == include_path.end())
  {
    throw exception("Include file " +
                    data::tokens_to_string(args_begin_, args_end_) +
                    " not found.");
  }
  else
  {
    displayer_.show_filename_list({*include_dir / include_file.second});
  }
}

std::pair<data::include_type, boost::filesystem::path>
pragma_which::parse_arguments(const data::command::iterator& name_begin_,
                              const data::command::iterator& name_end_,
                              const data::command::iterator& args_begin_,
                              const data::command::iterator& args_end_)
{
  if (args_begin_ == args_end_)
  {
    throw exception("No arguments provided to " +
                    data::tokens_to_string(name_begin_, name_end_) + ".");
  }
  if (args_begin_->type() == data::token_type::operator_less)
  {
    return {data::include_type::sys,
            parse_path_until_token<'>'>(
                name_begin_, name_end_, args_begin_ + 1, args_end_,
                [](const data::token& token_)
                {
                  return token_.type() == data::token_type::operator_greater;
                })};
  }
  else if (args_begin_->type() == data::token_type::string_literal)
  {
    if (args_begin_ + 1 != args_end_)
    {
      throw_more_than_one_arguments(name_begin_, name_end_);
    }
    else
    {
      return {data::include_type::quote, string_literal_value(*args_begin_)};
    }
  }
  else if (include_quote_token(*args_begin_))
  {
    return {data::include_type::quote,
            parse_path_until_token<'"'>(name_begin_, name_end_, args_begin_ + 1,
                                        args_end_, include_quote_token)};
  }

  const std::string arguments = data::tokens_to_string(args_begin_, args_end_);
  throw exception("Argument of " +
                  data::tokens_to_string(name_begin_, name_end_) +
                  " is not a header to include. Did you mean <" + arguments +
                  "> or \"" + arguments + "\"?");
}
