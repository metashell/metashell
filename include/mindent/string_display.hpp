#ifndef MINDENT_STRING_DISPLAY_HPP
#define MINDENT_STRING_DISPLAY_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mindent/token_traits.hpp>

#include <boost/algorithm/string/join.hpp>

#include <list>
#include <string>

namespace mindent
{
  /**
   * @brief Functor to display tokens in a string.
   *
   * This functor collects the tokens in memory and can provide a string
   * containing all tokens.
   */
  class string_display
  {
  public:
    template <class TokenType>
    void operator()(const TokenType& t_)
    {
      store(token_traits<TokenType>::value(t_));
    }

    /// Construct the string of all tokens displayed using this functor.
    std::string str() const
    {
      return boost::algorithm::join(_s, "");
    }
  private:
    std::list<std::string> _s;

    template <class String>
    void store(const String& s_)
    {
      _s.push_back(std::string(s_.begin(), s_.end()));
    }
  };
}

#endif

