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
#include <boost/range/adaptors.hpp>

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

  template <class InputIt>
  InputIt first_non_whitespace(InputIt begin_, InputIt end_)
  {
    return std::find_if(begin_, end_, [](const data::token& token_)
                        {
                          return token_.category() !=
                                 data::token_category::whitespace;
                        });
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
    else if (first_non_whitespace(path_end + 1, end_) != end_)
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
  return "[-all] <include file>|\"include file\"";
}

std::string pragma_which::description() const
{
  return "Searches the given include file on the include path. When -all is "
         "provided, it displays all headers found, not just the first one.";
}

void pragma_which::run(const data::command::iterator& name_begin_,
                       const data::command::iterator& name_end_,
                       const data::command::iterator& args_begin_,
                       const data::command::iterator& args_end_,
                       iface::displayer& displayer_) const
{
  using boost::filesystem::path;
  using boost::adaptors::transformed;
  using boost::adaptors::filtered;

  const parsed_arguments args =
      parse_arguments(name_begin_, name_end_, args_begin_, args_end_);
  const auto include_path = _shell.engine().include_path(args.header.type);
  const auto files = include_path |
                     transformed(std::function<path(const path&)>(
                         [&args](const path& path_)
                         {
                           return path_ / args.header.path;
                         })) |
                     filtered([](const path& path_)
                              {
                                return exists(path_);
                              });

  if (files.empty())
  {
    throw exception("Include file " +
                    data::tokens_to_string(args_begin_, args_end_) +
                    " not found.");
  }
  else if (args.all)
  {
    displayer_.show_filename_list(
        std::vector<path>(files.begin(), files.end()));
  }
  else
  {
    displayer_.show_filename_list({files.front()});
  }
}

pragma_which::parsed_arguments
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
  else if (args_begin_->type() == data::token_type::operator_minus &&
           args_begin_ + 1 != args_end_ &&
           (args_begin_ + 1)->type() == data::token_type::identifier &&
           (args_begin_ + 1)->value() == "all")
  {
    parsed_arguments args = parse_arguments(
        name_begin_, name_end_,
        first_non_whitespace(args_begin_ + 2, args_end_), args_end_);
    args.all = true;
    return args;
  }
  else if (args_begin_->type() == data::token_type::operator_less)
  {
    return {data::include_argument(
                data::include_type::sys,
                parse_path_until_token<'>'>(
                    name_begin_, name_end_, args_begin_ + 1, args_end_,
                    [](const data::token& token_)
                    {
                      return token_.type() ==
                             data::token_type::operator_greater;
                    })),
            false};
  }
  else if (args_begin_->type() == data::token_type::string_literal)
  {
    if (args_begin_ + 1 != args_end_)
    {
      throw_more_than_one_arguments(name_begin_, name_end_);
    }
    else
    {
      return {data::include_argument(data::include_type::quote,
                                     string_literal_value(*args_begin_)),
              false};
    }
  }
  else if (include_quote_token(*args_begin_))
  {
    return {data::include_argument(data::include_type::quote,
                                   parse_path_until_token<'"'>(
                                       name_begin_, name_end_, args_begin_ + 1,
                                       args_end_, include_quote_token)),
            false};
  }

  const std::string arguments = data::tokens_to_string(args_begin_, args_end_);
  throw exception("Argument of " +
                  data::tokens_to_string(name_begin_, name_end_) +
                  " is not a header to include. Did you mean <" + arguments +
                  "> or \"" + arguments + "\"?");
}

pragma_which::parsed_arguments::parsed_arguments(
    const data::include_argument& header_, bool all_)
  : header(header_), all(all_)
{
}

std::ostream& metashell::operator<<(std::ostream& out_,
                                    const pragma_which::parsed_arguments& args_)
{
  if (args_.all)
  {
    out_ << "-all ";
  }
  return out_ << args_.header;
}

bool metashell::operator==(const pragma_which::parsed_arguments& a_,
                           const pragma_which::parsed_arguments& b_)
{
  return a_.header == b_.header && a_.all == b_.all;
}
