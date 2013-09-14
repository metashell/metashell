// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/token_iterator.hpp>
#include <metashell/shell.hpp>

using namespace metashell;

token_iterator metashell::begin_tokens(const std::string& s_)
{
  return
    token_iterator(
      s_.begin(),
      s_.end(),
      token_iterator::value_type::position_type(shell::input_filename()),
      boost::wave::language_support(
        boost::wave::support_cpp
        | boost::wave::support_option_long_long
      )
    );
}

