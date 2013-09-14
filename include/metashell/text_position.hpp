#ifndef METASHELL_TEXT_POSITION_HPP
#define METASHELL_TEXT_POSITION_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/operators.hpp>

#include <string>
#include <iosfwd>

namespace metashell
{
  class text_position : boost::equality_comparable<text_position>
  {
  public:
    text_position();
    text_position(int line_, int column_);

    int line() const;
    int column() const;

    void next_column();
    void next_line();

    bool operator==(const text_position& pos_) const;
  private:
    int _line;
    int _column;
  };

  /*
   * Character sequences treated as one new line:
   *   10
   *   13
   *   13 10
   */
  text_position operator+(text_position pos_, const std::string& s_);

  std::ostream& operator<<(std::ostream& o_, const text_position& pos_);
}

#endif

