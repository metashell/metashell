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

#include <metashell/core/pragma_which.hpp>
#include <metashell/core/shell.hpp>

#include <metashell/data/exception.hpp>

#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

namespace metashell
{
  namespace core
  {
    pragma_which::pragma_which(shell& shell_) : _shell(shell_) {}

    std::string pragma_which::arguments() const
    {
      return "[-all] <include file>|\"include file\"";
    }

    std::string pragma_which::description() const
    {
      return "Searches the given include file on the include path. When -all "
             "is "
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

      const parsed_arguments args = parse_arguments(
          data::tokens_to_string(name_begin_, name_end_).value(), args_begin_,
          args_end_);
      const auto include_path =
          _shell.engine().header_discoverer().include_path(args.header.type);
      const auto files =
          include_path |
          transformed(std::function<path(const path&)>([&args](
              const path& path_) { return path_ / args.header.path; })) |
          filtered([](const path& path_) { return exists(path_); });

      if (files.empty())
      {
        throw data::exception(
            "Include file " +
            data::tokens_to_string(args_begin_, args_end_).value() +
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
    pragma_which::parse_arguments(const std::string& name_,
                                  const data::command::iterator& args_begin_,
                                  const data::command::iterator& args_end_)
    {
      if (args_begin_ == args_end_)
      {
        throw data::exception("No header is provided.");
      }

      bool all = false;

      data::command::iterator i = args_begin_;
      while (i != args_end_ && i->type() == data::token_type::operator_minus)
      {
        ++i;

        if (i == args_end_)
        {
          throw data::exception("Invalid argument: -");
        }
        else if (i->type() == data::token_type::identifier &&
                 i->value() == "all")
        {
          i = data::skip_all_whitespace(i + 1, args_end_);
          all = true;
        }
        else
        {
          throw data::exception(
              "Invalid argument: -" +
              data::tokens_to_string(
                  i, data::skip_all_whitespace(i + 1, args_end_))
                  .value());
        }
      }

      if (i == args_end_)
      {
        throw data::exception("No header is provided.");
      }
      else
      {
        const auto include_arg = data::include_argument::parse(i, args_end_);
        if (!include_arg.first)
        {
          const std::string arguments =
              data::tokens_to_string(i, args_end_).value();
          throw data::exception("Argument of " + name_ +
                                " is not a header to include. Did you mean <" +
                                arguments + "> or \"" + arguments + "\"?");
        }
        else if (include_arg.second == args_end_)
        {
          return {*include_arg.first, all};
        }
        else
        {
          throw data::exception("More than one arguments provided.");
        }
      }
    }

    pragma_which::parsed_arguments::parsed_arguments(
        const data::include_argument& header_, bool all_)
      : header(header_), all(all_)
    {
    }

    std::ostream& operator<<(std::ostream& out_,
                             const pragma_which::parsed_arguments& args_)
    {
      if (args_.all)
      {
        out_ << "-all ";
      }
      return out_ << args_.header;
    }

    bool operator==(const pragma_which::parsed_arguments& a_,
                    const pragma_which::parsed_arguments& b_)
    {
      return a_.header == b_.header && a_.all == b_.all;
    }
  }
}
