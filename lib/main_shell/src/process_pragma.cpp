// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/main_shell/metashell_pragma.hpp>
#include <metashell/main_shell/process_pragma.hpp>

#include <metashell/data/exception.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/token_type.hpp>

#include <boost/optional.hpp>

namespace metashell
{
  namespace main_shell
  {
    namespace
    {
      bool can_be_part_of_name(const data::token& t_)
      {
        return t_.type() == data::token_type::identifier;
      }

      boost::optional<data::command::iterator>
      is_this_pragma(const std::vector<std::string>& name_,
                     data::command::iterator begin_,
                     const data::command::iterator& end_)
      {
        using std::string;
        using std::vector;
        using boost::optional;

        const vector<string>::const_iterator e = name_.end();
        vector<string>::const_iterator i = name_.begin();
        while (begin_ != end_ && i != e && can_be_part_of_name(*begin_) &&
               *i == begin_->value())
        {
          ++i;
          begin_ = skip_whitespace(skip(begin_), end_);
        }
        return i == e ? optional<data::command::iterator>(begin_) :
                        optional<data::command::iterator>();
      }

      std::string name_of_pragma(data::command::iterator begin_,
                                 const data::command::iterator& end_)
      {
        std::ostringstream s;
        bool first = true;
        for (; begin_ != end_ && can_be_part_of_name(*begin_);
             begin_ = skip_whitespace(skip(begin_), end_))
        {
          s << (first ? "" : " ") << begin_->value();
          first = false;
        }
        return s.str();
      }

      template <class ForwardIterator, class Pred>
      ForwardIterator
      find_last_if(ForwardIterator begin_, ForwardIterator end_, Pred pred_)
      {
        ForwardIterator last = end_;
        for (ForwardIterator i = begin_; i != end_; ++i)
        {
          if (pred_(*i))
          {
            last = i;
          }
        }
        return last;
      }
    }

    void process_pragma(
        const std::map<std::vector<std::string>,
                       std::unique_ptr<iface::pragma_handler>>& handlers_,
        const data::command::iterator& begin_,
        const data::command::iterator& end_,
        iface::shell& shell_,
        iface::displayer& displayer_)
    {
      using boost::optional;

      const data::command::iterator e =
          end_of_pragma_argument_list(begin_, end_);

      auto longest_fit_begin = e;
      const iface::pragma_handler* longest_fit_handler = 0;
      int longest_fit_len = -1;

      for (const auto& p : handlers_)
      {
        assert(p.second);

        if (const optional<data::command::iterator> i =
                is_this_pragma(p.first, begin_, e))
        {
          if (longest_fit_len < int(p.first.size()))
          {
            longest_fit_begin = *i;
            longest_fit_handler = p.second.get();
            longest_fit_len = p.first.size();
          }
        }
      }
      if (longest_fit_handler)
      {
        longest_fit_handler->run(
            begin_, find_last_if(begin_, longest_fit_begin,
                                 [](const data::token& token_) {
                                   return token_.category() !=
                                          data::token_category::whitespace;
                                 }) +
                        1,
            longest_fit_begin, e, shell_, displayer_);
      }
      else
      {
        throw data::exception("Pragma " + name_of_pragma(begin_, e) +
                              " not found");
      }
    }
  }
}
