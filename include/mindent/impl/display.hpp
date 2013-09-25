#ifndef MINDENT_IMPL_DISPLAY_HPP
#define MINDENT_IMPL_DISPLAY_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/impl/length_calculator.hpp>
#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <algorithm>
#include <iterator>
#include <cassert>

namespace mindent
{
  namespace impl
  {
    template <class TokenType>
    TokenType new_line_token()
    {
      return
        TokenType(
          boost::wave::T_NEWLINE,
          "\n",
          typename TokenType::position_type()
        );
    }

    template <class TokenType>
    TokenType space_token(int len_)
    {
      assert(len_ > 0);
      return
        TokenType(
          boost::wave::T_SPACE,
          typename TokenType::string_type(len_, ' '),
          typename TokenType::position_type()
        );
    }

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
          show_(space_token<TokenType>(1));
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
      show_(new_line_token<TokenType>());
      if (indented_ > 0)
      {
        show_(space_token<TokenType>(indented_));
      }
      current_line_left_ = width_ - indented_;
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
      using boost::algorithm::is_any_of;
      using boost::algorithm::trim_right_copy_if;

      typedef typename TokenType::string_type string_t;
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
        const int len = i->get_value().length();
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
          show_(space_token<TokenType>(1));
          --current_line_left;
        }

        const string_t
          value = trim_right_copy_if(i->get_value(), is_any_of("\n\r"));

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
              is_any_of("\n\r")
            );
          const bool j2_is_newline = (j2 != je && is_any_of("\n\r")(*j2));
          if (*i == boost::wave::T_CCOMMENT)
          {
            buff += string_t(j, j2);
          }
          else
          {
            show_(TokenType(*i, string_t(j, j2), i->get_position()));
          }
          current_line_left -= j2 - j;
          if (j2_is_newline || (current_line_left <= 0 && j2 != je))
          {
            if (*i == boost::wave::T_CCOMMENT)
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
          show_(TokenType(*i, buff, i->get_position()));
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
          show_(new_line_token<TokenType>());
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
              show_(space_token<TokenType>(indented));
            }
            show_ =
              display(
                i->first,
                width_,
                indent_step_,
                indented,
                l - i->second.get_value().length(),
                show_
              );
            show_(i->second);
            show_(new_line_token<TokenType>());
          }
          if (indented_ > 0)
          {
            show_(space_token<TokenType>(indented_));
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
          if (!i->second.get_value().empty())
          {
            show_(space_token<TokenType>(1));
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
          if (!i->second.get_value().empty())
          {
            show_(new_line_token<TokenType>());
            if (i->second == boost::wave::T_COLON_COLON)
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
        if (i->second == boost::wave::T_COLON_COLON)
        {
          show_(i->second);
        }
      }
      return show_;
    }
  }
}

#endif

