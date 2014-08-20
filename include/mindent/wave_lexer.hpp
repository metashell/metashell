#ifndef MINDENT_WAVE_LEXER_HPP
#define MINDENT_WAVE_LEXER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/token_traits.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/utility.hpp>

#include <cassert>

/**
 * @brief Entities of the mIndent library
 */
namespace mindent
{
  /**
   * @brief Makes mIndent use Boost.Wave as its lexer
   *
   * @tparam TokenType the Boost.Wave type used for representeing tokens.
   */
  template <class TokenType>
  class wave_lexer : boost::noncopyable
  {
  public:
    typedef TokenType token_type;

    template <class InputIt>
    wave_lexer(
      InputIt begin_,
      InputIt end_,
      typename TokenType::position_type pos_,
      boost::wave::language_support lng_sup_
    ) :
      _it(begin_, end_, pos_, lng_sup_)
    {}

    bool has_further_tokens() const
    {
      typedef boost::wave::cpplexer::lex_iterator<TokenType> iterator_t;
      iterator_t it(_it);
      return it != iterator_t() && *it != boost::wave::T_EOF;
    }

    TokenType current_token() const
    {
      assert(has_further_tokens());
      return *_it;
    }

    void move_to_next_token()
    {
      assert(has_further_tokens());
      ++_it;
    }
  private:
    boost::wave::cpplexer::lex_iterator<TokenType> _it;
  };

  template <class Position>
  class token_traits<boost::wave::cpplexer::lex_token<Position> >
  {
  public:
    typedef boost::wave::cpplexer::lex_token<Position> token_type;
    typedef typename token_type::string_type string_type;
    
    static bool is_double_colon(const token_type& t_)
    {
      return t_ == boost::wave::T_COLON_COLON;
    }

    static bool is_c_comment(const token_type& t_)
    {
      return t_ == boost::wave::T_CCOMMENT;
    }

    static bool is_less(const token_type& t_)
    {
      return t_ == boost::wave::T_LESS;
    }

    static bool is_greater(const token_type& t_)
    {
      return t_ == boost::wave::T_GREATER;
    }

    static bool is_comma(const token_type& t_)
    {
      return t_ == boost::wave::T_COMMA;
    }

    static bool is_space(const token_type& t_)
    {
      return
        t_ == boost::wave::T_ANY ||
        t_ == boost::wave::T_SPACE ||
        t_ == boost::wave::T_SPACE2 ||
        t_ == boost::wave::T_EOF ||
        t_ == boost::wave::T_CONTLINE ||
        t_ == boost::wave::T_NEWLINE;
    }

    static token_type empty_token()
    {
      return
        token_type(
          boost::wave::T_ANY,
          typename token_type::string_type(),
          typename token_type::position_type()
        );
    }

    static token_type space_token(int len_)
    {
      assert(len_ > 0);
      return
        token_type(
          boost::wave::T_SPACE,
          typename token_type::string_type(len_, ' '),
          typename token_type::position_type()
        );
    }

    static token_type new_line_token()
    {
      return
        token_type(
          boost::wave::T_NEWLINE,
          "\n",
          typename token_type::position_type()
        );
    }

    static token_type change_value(
      const string_type& value_,
      const token_type& t_
    )
    {
      return token_type(t_, value_, t_.get_position());
    }

    static string_type value(const token_type& t_)
    {
      return t_.get_value();
    }
  };
}

#endif

