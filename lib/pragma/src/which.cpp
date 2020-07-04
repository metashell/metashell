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

#include <metashell/pragma/which.hpp>

#include <metashell/core/code_complete.hpp>

#include <metashell/data/exception.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

namespace metashell
{
  namespace pragma
  {
    std::string which::arguments() const
    {
      return "[-all] <include file>|\"include file\"";
    }

    std::string which::description() const
    {
      return "Searches the given include file on the include path. When -all "
             "is provided, it displays all headers found, not just the first "
             "one.";
    }

    void which::run(const data::command::iterator& name_begin_,
                    const data::command::iterator& name_end_,
                    const data::command::iterator& args_begin_,
                    const data::command::iterator& args_end_,
                    iface::main_shell& shell_,
                    iface::displayer& displayer_) const
    {
      using boost::filesystem::path;
      using boost::adaptors::transformed;
      using boost::adaptors::filtered;

      const parsed_arguments args = parse_arguments(
          data::tokens_to_string(name_begin_, name_end_).value(), args_begin_,
          args_end_);
      const auto include_path =
          shell_.engine().header_discoverer().include_path(
              args.header.type, data::standard_headers_allowed::all);
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

    which::parsed_arguments
    which::parse_arguments(const std::string& name_,
                           const data::command::iterator& args_begin_,
                           const data::command::iterator& args_end_)
    {
      if (args_begin_ == args_end_)
      {
        throw data::exception("No header is provided.");
      }

      bool all = false;

      data::command::iterator i = args_begin_;
      while (i != args_end_ && type_of(*i) == data::token_type::operator_minus)
      {
        ++i;

        if (i == args_end_)
        {
          throw data::exception("Invalid argument: -");
        }
        else if (type_of(*i) == data::token_type::identifier &&
                 value(*i) == "all")
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

    which::parsed_arguments::parsed_arguments(
        const data::include_argument& header_, bool all_)
      : header(header_), all(all_)
    {
    }

    data::code_completion
    which::code_complete(data::command::const_iterator begin_,
                         data::command::const_iterator end_,
                         iface::main_shell& shell_) const
    {
      using data::token_type;

      if (begin_ == end_)
      {
        return data::code_completion{
            data::user_input{" -all <"}, data::user_input{" -all \""},
            data::user_input{" <"}, data::user_input{" \""}};
      }

      auto beg = skip_all_whitespace(begin_, end_);
      if (beg == end_)
      {
        return data::code_completion{
            data::user_input{"-all <"}, data::user_input{"-all \""},
            data::user_input{"<"}, data::user_input{"\""}};
      }

      if (type_of(*beg) == token_type::operator_minus)
      {
        ++beg;
        if (beg == end_)
        {
          return data::code_completion{
              data::user_input{"all <"}, data::user_input{"all \""}};
        }

        if (type_of(*beg) == token_type::identifier)
        {
          const std::string all{"all"};
          const std::string val = to_string(value(*beg));
          ++beg;
          if (beg == end_)
          {
            if (boost::algorithm::starts_with(all, val))
            {
              const std::string ext = all.substr(val.size());
              return data::code_completion{
                  data::user_input{ext + " <"}, data::user_input{ext + " \""}};
            }
            else
            {
              return data::code_completion{};
            }
          }

          if (val != all)
          {
            return data::code_completion{};
          }
        }

        beg = skip_all_whitespace(beg, end_);
        if (beg == end_)
        {
          return data::code_completion{
              data::user_input{"<"}, data::user_input{"\""}};
        }
      }

      return core::code_complete::include(
          beg, end_, shell_.engine().header_discoverer());
    }

    std::ostream& operator<<(std::ostream& out_,
                             const which::parsed_arguments& args_)
    {
      if (args_.all)
      {
        out_ << "-all ";
      }
      return out_ << args_.header;
    }

    bool operator==(const which::parsed_arguments& a_,
                    const which::parsed_arguments& b_)
    {
      return a_.header == b_.header && a_.all == b_.all;
    }
  }
}
