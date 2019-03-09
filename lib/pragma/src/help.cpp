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

#include <metashell/pragma/help.hpp>

#include <metashell/core/version.hpp>

#include <boost/algorithm/string/join.hpp>

#include <cassert>

namespace metashell
{
  namespace pragma
  {
    namespace
    {
      void display_all(iface::displayer& displayer_,
                       const std::map<data::pragma_name,
                                      std::unique_ptr<iface::pragma_handler>>&
                           pragma_handlers_)
      {
        using boost::algorithm::join;

        data::text t;
        t.paragraphs.push_back(
            data::paragraph("Metashell has the following built-in pragmas:"));

        for (const auto& p : pragma_handlers_)
        {
          assert(p.second);

          const std::string args = p.second->arguments();
          t.paragraphs.push_back(
              data::paragraph("#msh " + to_string(p.first) +
                                  (args.empty() ? std::string() : " " + args),
                              "  "));
        }

        const data::paragraph empty_line("");

        t.paragraphs.push_back(empty_line);
        t.paragraphs.push_back(
            data::paragraph("#msh is the short form of #pragma metashell. Both "
                            "forms are accepted"));
        t.paragraphs.push_back(
            data::paragraph("To quit Metashell run \"#msh quit\""));
        t.paragraphs.push_back(empty_line);

        displayer_.show_comment(t);
      }
    }

    std::string help::arguments() const { return "[<command>]"; }

    std::string help::description() const { return "Displays a help message."; }

    void help::run(const data::command::iterator&,
                   const data::command::iterator&,
                   const data::command::iterator& args_begin_,
                   const data::command::iterator& args_end_,
                   iface::shell& shell_,
                   iface::displayer& displayer_) const
    {
      using boost::algorithm::join;

      if (args_begin_ == args_end_)
      {
        display_all(displayer_, shell_.pragma_handlers());
      }
      else
      {
        std::vector<std::string> args;
        for (auto i = args_begin_; i != args_end_; ++i)
        {
          switch (i->category())
          {
          case data::token_category::whitespace:
          case data::token_category::comment:
            // skip token
            break;
          default:
            args.push_back(i->value().value());
          }
        }

        data::text help_text;
        bool was_pragma = false;

        for (const auto& h : shell_.pragma_handlers())
        {
          assert(h.second);

          if (prefix_of(args, h.first))
          {
            if (was_pragma)
            {
              help_text.paragraphs.push_back(data::paragraph(""));
            }
            else
            {
              was_pragma = true;
            }
            const std::string p_args = h.second->arguments();
            help_text.paragraphs.push_back(data::paragraph(
                "#msh " + to_string(h.first) +
                (p_args.empty() ? std::string() : " " + p_args)));
            help_text.paragraphs.push_back(
                data::paragraph(h.second->description(), "    "));
          }
        }
        if (was_pragma)
        {
          displayer_.show_comment(help_text);
        }
        else
        {
          displayer_.show_error("Pragma " + join(args, " ") + " not found.");
        }
      }
    }
  }
}
