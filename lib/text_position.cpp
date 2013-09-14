// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/text_position.hpp>

#include <boost/foreach.hpp>

using namespace metashell;

text_position::text_position() :
  _line(1),
  _column(1)
{}

text_position::text_position(int line_, int column_) :
  _line(line_),
  _column(column_)
{}

int text_position::line() const
{
  return _line;
}

int text_position::column() const
{
  return _column;
}

void text_position::next_column()
{
  ++_column;
}

void text_position::next_line()
{
  _column = 1;
  ++_line;
}

text_position metashell::operator+(text_position pos_, const std::string& s_)
{
  bool was13 = false;

  BOOST_FOREACH(char c, s_)
  {
    switch (c)
    {
    case 10:
      pos_.next_line();
      break;
    case 13:
      if (was13)
      {
        pos_.next_line();
      }
      break;
    default:
      pos_.next_column();
    }
    was13 = c == 13;
  }

  if (was13)
  {
    pos_.next_line();
  }

  return pos_;
}

bool text_position::operator==(const text_position& pos_) const
{
  return _line == pos_._line && _column == pos_._column;
}

std::ostream& metashell::operator<<(std::ostream& o_, const text_position& pos_)
{
  return o_ << pos_.line() << ":" << pos_.column();
}


