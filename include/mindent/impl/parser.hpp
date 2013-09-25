#ifndef MINDENT_IMPL_PARSER_HPP
#define MINDENT_IMPL_PARSER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <algorithm>
#include <map>
#include <cassert>

namespace mindent
{
  namespace impl
  {
    inline bool not_space(boost::wave::token_id t_)
    {
      return
        t_ != boost::wave::T_ANY &&
        t_ != boost::wave::T_SPACE &&
        t_ != boost::wave::T_SPACE2 &&
        t_ != boost::wave::T_EOF &&
        t_ != boost::wave::T_CONTLINE &&
        t_ != boost::wave::T_NEWLINE;
    }

    template <class TokenType>
    boost::wave::cpplexer::lex_iterator<TokenType> end()
    {
      return boost::wave::cpplexer::lex_iterator<TokenType>();
    }

    template <class TokenType>
    boost::wave::cpplexer::lex_iterator<TokenType> skip_spaces(
      boost::wave::cpplexer::lex_iterator<TokenType> i_
    )
    {
      return std::find_if(i_, end<TokenType>(), not_space);
    }

    template <class TokenType>
    TokenType empty_token()
    {
      return
        TokenType(
          boost::wave::T_ANY,
          typename TokenType::string_type(),
          typename TokenType::position_type()
        );
    }

    template <boost::wave::token_id T, class TokenType>
    bool at_token(boost::wave::cpplexer::lex_iterator<TokenType> i_)
    {
      return i_ != end<TokenType>() && *i_ == T;
    }

    template <boost::wave::token_id T, class TokenType>
    boost::wave::cpplexer::lex_iterator<TokenType> skip_token(
      boost::wave::cpplexer::lex_iterator<TokenType> i_
    )
    {
      assert(at_token<T>(i_));
      ++i_;
      return i_;
    }

    template <class TokenType>
    std::pair<
      syntax_node_list<TokenType>,
      boost::wave::cpplexer::lex_iterator<TokenType>
    >
    parse_syntax_node_list(
      boost::wave::cpplexer::lex_iterator<TokenType> begin_
    );

    template <class TokenType>
    std::pair<
      std::vector<TokenType>,
      boost::wave::cpplexer::lex_iterator<TokenType>
    >
    parse_name(boost::wave::cpplexer::lex_iterator<TokenType> begin_)
    {
      using std::pair;
      using std::vector;

      using boost::wave::cpplexer::lex_iterator;

      pair<vector<TokenType>, lex_iterator<TokenType> >
        result(vector<TokenType>(), begin_);
      for (
        ;
        result.second != end<TokenType>()
          && *result.second != boost::wave::T_COMMA
          && *result.second != boost::wave::T_COLON_COLON
          && *result.second != boost::wave::T_LESS
          && *result.second != boost::wave::T_GREATER;
        ++result.second
      )
      {
        if (not_space(*result.second))
        {
          result.first.push_back(*result.second);
        }
      }
      return result;
    }

    template <class TokenType>
    std::pair<
      syntax_node<TokenType>,
      boost::wave::cpplexer::lex_iterator<TokenType>
    >
    parse_syntax_node(boost::wave::cpplexer::lex_iterator<TokenType> begin_)
    {
      using std::make_pair;
      using std::list;
      using std::pair;
      using std::vector;

      using boost::wave::T_COLON_COLON;
      using boost::wave::T_COMMA;
      using boost::wave::T_GREATER;
      using boost::wave::T_LESS;

      using boost::wave::cpplexer::lex_iterator;

      const pair<vector<TokenType>, lex_iterator<TokenType> >
        node_name = parse_name(begin_);

      if (at_token<T_LESS>(node_name.second))
      {
        const TokenType template_begin = *node_name.second;
        lex_iterator<TokenType> i = skip_token<T_LESS>(node_name.second);

        assert(i != end<TokenType>());
        list<pair<syntax_node_list<TokenType>, TokenType> > args;
        for (;;)
        {
          const pair<syntax_node_list<TokenType>, lex_iterator<TokenType> >
            p = parse_syntax_node_list(i);
          i = p.second;
          if (at_token<T_COMMA>(i))
          {
            args.push_back(make_pair(p.first, *i));
            ++i; // we know that it is a comma token, no need to assert
          }
          else
          {
            args.push_back(make_pair(p.first, empty_token<TokenType>()));
            break;
          }
        }

        const TokenType template_end = *i;
        i = skip_token<T_GREATER>(i);

        return
          make_pair(
            syntax_node<TokenType>(
              node_name.first.begin(),
              node_name.first.end(),
              template_begin,
              args.begin(),
              args.end(),
              template_end
            ),
            i
          );
      }
      else
      {
        return
          make_pair(
            syntax_node<TokenType>(
              node_name.first.begin(),
              node_name.first.end()
            ),
            node_name.second
          );
      }
    }

    template <class TokenType>
    std::pair<
      syntax_node_list<TokenType>,
      boost::wave::cpplexer::lex_iterator<TokenType>
    >
    parse_syntax_node_list(
      boost::wave::cpplexer::lex_iterator<TokenType> begin_
    )
    {
      using boost::wave::cpplexer::lex_iterator;

      using std::list;
      using std::pair;
      using std::make_pair;

      lex_iterator<TokenType> i = skip_spaces(begin_);
      list<pair<syntax_node<TokenType>, TokenType> > l;
      while (
        i != end<TokenType>()
        && *i != boost::wave::T_COMMA
        && *i != boost::wave::T_GREATER
        && *i != boost::wave::T_EOF
      )
      {
        const pair<syntax_node<TokenType>, lex_iterator<TokenType> >
          p = parse_syntax_node(i);
        assert(i != p.second);
        i = skip_spaces(p.second);
        if (at_token<boost::wave::T_COLON_COLON>(i))
        {
          l.push_back(make_pair(p.first, *i));
          ++i; // we know that it is a :: token, no need to assert
        }
        else
        {
          l.push_back(make_pair(p.first, empty_token<TokenType>()));
          break;
        }
      }
      return make_pair(syntax_node_list<TokenType>(l.begin(), l.end()), i);
    }
  }
}

#endif

