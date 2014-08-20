#ifndef MINDENT_IMPL_LENGTH_CALCULATOR_HPP
#define MINDENT_IMPL_LENGTH_CALCULATOR_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/token_traits.hpp>

namespace mindent
{
  namespace impl
  {
    template <class TokenType>
    class length_calculator
    {
    public:
      length_calculator() : _len(0) {}

      void operator()(const TokenType& t_)
      {
        _len += token_traits<TokenType>::value(t_).length();
      }

      int length() const
      {
        return _len;
      }
    private:
      int _len;
    };
  }
}

#endif

