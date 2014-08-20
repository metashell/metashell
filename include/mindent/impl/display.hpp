#ifndef MINDENT_IMPL_DISPLAY_HPP
#define MINDENT_IMPL_DISPLAY_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/impl/length_calculator.hpp>
#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>
#include <mindent/token_traits.hpp>

#include <boost/algorithm/string/trim.hpp>

#include <algorithm>
#include <iterator>
#include <cassert>

namespace mindent
{
  namespace impl
  {
    template <class DisplayF, class TokenType>
    DisplayF display_in_one_line(
      const syntax_node<TokenType>& n_,
      DisplayF show_
    );

    template <class DisplayF, class TokenType>
    DisplayF display_in_one_line(
      const syntax_node_list<TokenType>& l_,
      DisplayF show_
    );

    template <class DisplayF, class TokenType>
    DisplayF display(
      const syntax_node_list<TokenType>& l_,
      int width_,
      int indent_step_,
      int indented_,
      int current_line_left_,
      DisplayF show_
    );

    template <class TokenType, class DisplayF>
    DisplayF display_name_in_one_line(
      const syntax_node<TokenType>& n_,
      DisplayF show_
    )
    {
      bool first = true;
      for (
        typename syntax_node<TokenType>::name_iterator
          i = n_.name_begin(),
          e = n_.name_end();
        i != e;
        ++i
      )
      {
        if (first)
        {
          first = false;
        }
        else
        {
          show_(token_traits<TokenType>::space_token(1));
        }
        show_(*i);
      }
      return show_;
    }

    template <class TokenType, class DisplayF>
    void display_new_line(
      int width_,
      int indented_,
      int& current_line_left_,
      DisplayF& show_
    )
    {
      show_(token_traits<TokenType>::new_line_token());
      if (indented_ > 0)
      {
        show_(token_traits<TokenType>::space_token(indented_));
      }
      current_line_left_ = width_ - indented_;
    }

    inline bool is_newline_char(char c_)
    {
      return c_ == '\n' || c_ == '\r';
    }

    template <class TokenType, class DisplayF>
    DisplayF display_name(
      const syntax_node<TokenType>& n_,
      int width_,
      int indented_,
      int current_line_left_,
      DisplayF show_
    )
    {
      using boost::algorithm::trim_right_copy_if;

      typedef typename token_traits<TokenType>::string_type string_t;
      typedef typename string_t::const_iterator string_it;

      int current_line_left = current_line_left_;
      bool first = true;
      for (
        typename syntax_node<TokenType>::name_iterator
          i = n_.name_begin(),
          e = n_.name_end();
        i != e;
        ++i
      )
      {
        const int space_len = first ? 0 : 1;
        const int len = token_traits<TokenType>::value(*i).length();
        if (len + space_len > current_line_left && width_ - indented_ >= len)
        {
          display_new_line<TokenType>(
            width_,
            indented_,
            current_line_left,
            show_
          );
        }
        else if (!first)
        {
          show_(token_traits<TokenType>::space_token(1));
          --current_line_left;
        }

        const string_t
          value = trim_right_copy_if(
            token_traits<TokenType>::value(*i),
            is_newline_char
          );

        string_t buff;
        for (string_it j = value.begin(), je = value.end(); j != je; )
        {
          typedef
            typename std::iterator_traits<string_it>::difference_type
            diff_t;

          const string_it j2 =
            std::find_if(
              j,
              j + std::min<diff_t>(je - j, current_line_left),
              is_newline_char
            );
          const bool j2_is_newline = (j2 != je && is_newline_char(*j2));
          if (token_traits<TokenType>::is_c_comment(*i))
          {
            buff += string_t(j, j2);
          }
          else
          {
            show_(token_traits<TokenType>::change_value(string_t(j, j2), *i));
          }
          current_line_left -= j2 - j;
          if (j2_is_newline || (current_line_left <= 0 && j2 != je))
          {
            if (token_traits<TokenType>::is_c_comment(*i))
            {
              buff += '\n' + string_t(indented_, ' ');
              current_line_left = width_ - indented_;
            }
            else
            {
              display_new_line<TokenType>(
                width_,
                indented_,
                current_line_left,
                show_
              );
            }
          }
          j = j2;
          if (j2_is_newline)
          {
            ++j;
          }
        }
        if (!buff.empty())
        {
          show_(token_traits<TokenType>::change_value(buff, *i));
        }
        first = false;
      }
      return show_;
    }

    template <class DisplayF, class TokenType>
    DisplayF display(
      const syntax_node<TokenType>& n_,
      int width_,
      int indent_step_,
      int indented_,
      int current_line_left_,
      DisplayF show_
    )
    {
      if (
        display_in_one_line(n_, length_calculator<TokenType>()).length()
        <=
        current_line_left_
      )
      {
        return display_in_one_line(n_, show_);
      }
      else
      {
        show_ =
          display_name<TokenType>(
            n_,
            width_,
            indented_,
            current_line_left_,
            show_
          );
        if (n_.is_template())
        {
          show_(n_.template_begin());
          show_(token_traits<TokenType>::new_line_token());
          const int indented = indented_ + indent_step_;
          const int l = width_ - indented;
          
          for (
            typename syntax_node<TokenType>::arg_iterator
              i = n_.begin_args(),
              e = n_.end_args();
            i != e;
            ++i
          )
          {
            if (indented > 0)
            {
              show_(token_traits<TokenType>::space_token(indented));
            }
            show_ =
              display(
                i->first,
                width_,
                indent_step_,
                indented,
                l - token_traits<TokenType>::value(i->second).length(),
                show_
              );
            show_(i->second);
            show_(token_traits<TokenType>::new_line_token());
          }
          if (indented_ > 0)
          {
            show_(token_traits<TokenType>::space_token(indented_));
          }
          show_(n_.template_end());
        }
        return show_;
      }
    }

    template <class DisplayF, class TokenType>
    DisplayF display_in_one_line(
      const syntax_node<TokenType>& n_,
      DisplayF show_
    )
    {
      show_ = display_name_in_one_line<TokenType>(n_, show_);
      if (n_.is_template())
      {
        show_(n_.template_begin());
        for (
          typename syntax_node<TokenType>::arg_iterator
            i = n_.begin_args(),
            e = n_.end_args();
          i != e;
          ++i
        )
        {
          show_ = display_in_one_line(i->first, show_);
          show_(i->second);
          if (!token_traits<TokenType>::value(i->second).empty())
          {
            show_(token_traits<TokenType>::space_token(1));
          }
        }
        show_(n_.template_end());
      }
      return show_;
    }

    template <class DisplayF, class TokenType>
    DisplayF display(
      const syntax_node_list<TokenType>& l_,
      int width_,
      int indent_step_,
      int indented_,
      int current_line_left_,
      DisplayF show_
    )
    {
      if (
        display_in_one_line(l_, length_calculator<TokenType>()).length()
        <= current_line_left_
      )
      {
        return display_in_one_line(l_, show_);
      }
      else
      {
        const int l = width_ - indented_;
        for (
          typename syntax_node_list<TokenType>::const_iterator
            i = l_.begin(),
            e = l_.end();
          i != e;
          ++i
        )
        {
          show_ = display(i->first, width_, indent_step_, indented_, l, show_);
          if (!token_traits<TokenType>::value(i->second).empty())
          {
            show_(token_traits<TokenType>::new_line_token());
            if (token_traits<TokenType>::is_double_colon(i->second))
            {
              show_(i->second);
            }
          }
        }
        return show_;
      }
    }

    template <class DisplayF, class TokenType>
    DisplayF display_in_one_line(
      const syntax_node_list<TokenType>& l_,
      DisplayF show_
    )
    {
      for (
        typename syntax_node_list<TokenType>::const_iterator
          i = l_.begin(),
          e = l_.end();
        i != e;
        ++i
      )
      {
        show_ = display_in_one_line(i->first, show_);
        if (token_traits<TokenType>::is_double_colon(i->second))
        {
          show_(i->second);
        }
      }
      return show_;
    }
  }
}

#endif

