#ifndef MINDENT_STREAM_DISPLAY_HPP
#define MINDENT_STREAM_DISPLAY_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/token_traits.hpp>

#include <iostream>

namespace mindent
{
  /**
   * @brief Functor to display tokens in an output stream.
   */
  class stream_display
  {
  public:
    /**
     * @brief Initialise the functor with an std::ostream to display the tokens
     *        on.
     *
     * @param o_ The stream to display the tokens on.
     */
    explicit stream_display(std::ostream& o_) :
      _o(&o_)
    {}

    template <class TokenType>
    void operator()(const TokenType& t_)
    {
      *_o << token_traits<TokenType>::value(t_);
    }
  private:
    std::ostream* _o;
  };
}

#endif

