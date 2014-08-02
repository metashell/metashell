// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma_help.hpp>
#include <metashell/shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/version.hpp>
#include "indenter.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>

using namespace metashell;

namespace
{
  void display_all(shell& shell_)
  {
    using boost::algorithm::join;
  
    indenter ind(shell_.width(), " * ");
    ind
      .raw("/*")
      .left_align("Metashell has the following built-in pragmas:");
  
    typedef std::pair<const std::vector<std::string>, pragma_handler> sp;
    BOOST_FOREACH(const sp& p, shell_.pragma_handlers())
    {
      const std::string args = p.second.arguments();
      ind.left_align(
        "#msh "
          + join(p.first, " ") + (args.empty() ? std::string() : " " + args),
        " *   "
      );
    }
  
    ind
      .empty_line()
      .left_align(
        "#msh is the short form of #pragma metashell. Both forms are accepted"
      )
      .left_align(
        "To quit Metashell run \"#msh quit\""
      )
      .empty_line()
      .raw(" */");
  
    shell_.display_normal(ind.str());
  }

  template <class It1, class It2>
  bool prefix_of(It1 prefix_begin_, It1 prefix_end_, It2 begin_, It2 end_)
  {
    while (
      prefix_begin_ != prefix_end_ && begin_ != end_
        && *prefix_begin_ == *begin_
    )
    {
      ++prefix_begin_;
      ++begin_;
    }
    return prefix_begin_ == prefix_end_;
  }
}

pragma_help::pragma_help(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_help::clone() const
{
  return new pragma_help(_shell);
}

std::string pragma_help::arguments() const
{
  return "[<command>]";
}

std::string pragma_help::description() const
{
  return "Displays a help message.";
}

void pragma_help::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  using boost::algorithm::join;

  if (args_begin_ == args_end_)
  {
    display_all(_shell);
  }
  else
  {
    std::vector<std::string> args;
    for (
      token_iterator i = args_begin_;
      i != args_end_;
      i = skip_whitespace(skip(i))
    )
    {
      args.push_back(std::string(i->get_value().begin(), i->get_value().end()));
    }
    
    indenter ind(_shell.width(), " * ");
    ind .raw("/*");
    bool was_pragma = false;

    for (
      std::map<std::vector<std::string>, pragma_handler>::const_iterator
        i = _shell.pragma_handlers().begin(),
        e = _shell.pragma_handlers().end();
      i != e;
      ++i
    )
    {
      if (prefix_of(args.begin(), args.end(), i->first.begin(), i->first.end()))
      {
        if (was_pragma)
        {
          ind.empty_line();
        }
        else
        {
          was_pragma = true;
        }
        const std::string p_args = i->second.arguments();
        ind
          .left_align(
            "#msh "
              + join(i->first, " ")
              + (p_args.empty() ? std::string() : " " + p_args)
          )
          .left_align(i->second.description(), " *     ");
      }
    }
    if (was_pragma)
    {
      _shell.display_normal(ind.raw(" */").str());
    }
    else
    {
      _shell.display_error("Pragma " + join(args, " ") + " not found.");
    }
  }
}

