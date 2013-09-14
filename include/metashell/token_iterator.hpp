#ifndef METASHELL_TOKEN_ITERATOR_HPP
#define METASHELL_TOKEN_ITERATOR_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <string>

namespace metashell
{
  typedef
    boost::wave::cpplexer::lex_iterator<
      boost::wave::cpplexer::lex_token<>
    >
    token_iterator;

  token_iterator begin_tokens(const std::string& s_);
}

#endif

