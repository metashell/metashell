#ifndef MINDENT_IMPL_PARSER_HPP
#define MINDENT_IMPL_PARSER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>
#include <mindent/token_traits.hpp>

#include <algorithm>
#include <map>
#include <list>
#include <cassert>

namespace mindent
{
  namespace impl
  {
    template <class Lexer>
    void skip_spaces(Lexer& lexer_)
    {
      typedef typename Lexer::token_type token_type;

      while (
        lexer_.has_further_tokens()
        && token_traits<token_type>::is_space(lexer_.current_token())
      )
      {
        lexer_.move_to_next_token();
      }
    }

    template <class Lexer>
    syntax_node_list<typename Lexer::token_type>
    parse_syntax_node_list(Lexer& begin_);

    template <class Lexer>
    bool still_in_name(const Lexer& lexer_)
    {
      if (lexer_.has_further_tokens())
      {
        typedef typename Lexer::token_type token_type;
        const token_type t = lexer_.current_token();
        return
            !token_traits<token_type>::is_comma(t)
            && !token_traits<token_type>::is_double_colon(t)
            && !token_traits<token_type>::is_less(t)
            && !token_traits<token_type>::is_greater(t);
      }
      else
      {
        return false;
      }
    }

    template <class Lexer>
    std::vector<typename Lexer::token_type> parse_name(Lexer& lexer_)
    {
      typedef typename Lexer::token_type token_type;

      std::vector<token_type> result;
      for (; still_in_name(lexer_); lexer_.move_to_next_token())
      {
        if (
          !lexer_.has_further_tokens() ||
          !token_traits<token_type>::is_space(lexer_.current_token())
        )
        {
          result.push_back(lexer_.current_token());
        }
      }
      return result;
    }

    template <class Lexer>
    syntax_node<typename Lexer::token_type> parse_syntax_node(Lexer& lexer_)
    {
      using std::make_pair;
      using std::list;
      using std::pair;
      using std::vector;

      typedef typename Lexer::token_type token_type;

      const vector<token_type> node_name = parse_name(lexer_);

      if (
        lexer_.has_further_tokens()
        && token_traits<token_type>::is_less(lexer_.current_token())
      )
      {
        const token_type template_begin = lexer_.current_token();

        assert(lexer_.has_further_tokens());
        assert(token_traits<token_type>::is_less(lexer_.current_token()));
        lexer_.move_to_next_token();

        assert(lexer_.has_further_tokens());
        list<pair<syntax_node_list<token_type>, token_type> > args;
        for (;;)
        {
          const syntax_node_list<token_type>
            p = impl::parse_syntax_node_list(lexer_);
          if (
            lexer_.has_further_tokens()
            && token_traits<token_type>::is_comma(lexer_.current_token())
          )
          {
            args.push_back(make_pair(p, lexer_.current_token()));
            lexer_.move_to_next_token();
          }
          else
          {
            args.push_back(
              make_pair(p, token_traits<token_type>::empty_token())
            );
            break;
          }
        }

        const token_type template_end = lexer_.current_token();
        assert(lexer_.has_further_tokens());
        assert(token_traits<token_type>::is_greater(lexer_.current_token()));
        lexer_.move_to_next_token();

        return
          syntax_node<token_type>(
            node_name.begin(),
            node_name.end(),
            template_begin,
            args.begin(),
            args.end(),
            template_end
          );
      }
      else
      {
        return syntax_node<token_type>(node_name.begin(), node_name.end());
      }
    }

    template <class Lexer>
    bool at_syntax_node_list_separator(const Lexer& lexer_)
    {
      if (lexer_.has_further_tokens())
      {
        typedef typename Lexer::token_type token_type;
        const token_type t = lexer_.current_token();
        return
          !token_traits<token_type>::is_comma(t)
          && !token_traits<token_type>::is_greater(t);
      }
      else
      {
        return false;
      }
    }

    template <class Lexer>
    syntax_node_list<typename Lexer::token_type>
    parse_syntax_node_list(Lexer& lexer_)
    {
      using std::list;
      using std::pair;
      using std::make_pair;

      typedef typename Lexer::token_type token_type;
      typedef syntax_node<token_type> syn_node;

      skip_spaces(lexer_);

      list<pair<syn_node, token_type> > l;
      while (at_syntax_node_list_separator(lexer_))
      {
        const syn_node p = parse_syntax_node(lexer_);
        skip_spaces(lexer_);
        if (
          lexer_.has_further_tokens()
          && token_traits<token_type>::is_double_colon(lexer_.current_token())
        )
        {
          l.push_back(make_pair(p, lexer_.current_token()));
          // we know that it is a :: token, no need to assert
          lexer_.move_to_next_token();
        }
        else
        {
          l.push_back(make_pair(p, token_traits<token_type>::empty_token()));
          break;
        }
      }
      return syntax_node_list<token_type>(l.begin(), l.end());
    }
  }
}

#endif

