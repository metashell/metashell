// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/core/console_displayer.hpp>
#include <metashell/core/pager.hpp>

#include "mock_console.hpp"

#include <gtest/gtest.h>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;

TEST(pager, one_line)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(100));

  pager p(c);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("first")));
    EXPECT_CALL(c, new_line());
  }

  p.show("first");

  ASSERT_TRUE(p.new_line());
}

TEST(pager, non_full_page)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(5));

  pager p(c);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("first")));
    EXPECT_CALL(c, new_line());
  }

  p.show("first");

  ASSERT_TRUE(p.new_line());

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("second")));
    EXPECT_CALL(c, new_line());
  }

  p.show("second");

  ASSERT_TRUE(p.new_line());

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("third")));
    EXPECT_CALL(c, new_line());
  }

  p.show("third");

  ASSERT_TRUE(p.new_line());
}

TEST(pager, almost_full_page)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(4));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("first")));
  p.show("first");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("second")));
  p.show("second");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("third")));
  p.show("third");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());
}

TEST(pager, full_page_by_one_line)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(4));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("first")));
  p.show("first");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("second")));
  p.show("second");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("third")));
  p.show("third");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("forth")));
  p.show("forth");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());
}

TEST(pager, multi_page_next_page_answer)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(3));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("first")));
  p.show("first");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("second")));
  p.show("second");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("third")));
  p.show("third");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("forth")));
  p.show("forth");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("fifth")));
  p.show("fifth");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());
}

TEST(pager, multi_page_multiline_shows)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(3));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("first\nsecond")));
  p.show("first\nsecond");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("third")));
  p.show("third");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("forth\nfifth")));
  p.show("forth\nfifth");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());
}

TEST(pager, multi_page_narrow_terminal)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(5));
  ON_CALL(c, height()).WillByDefault(Return(3));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("firstsecond")));
  p.show("first" /*\n*/ "second");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("third")));
  p.show("third");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("forthfifth")));
  p.show("forth" /*\n*/ "fifth");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::next_page));
  ASSERT_TRUE(p.new_line());
}

TEST(pager, multi_page_show_all_answer)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(3));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("first")));
  p.show("first");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("second")));
  p.show("second");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::show_all));
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("third")));
  p.show("third");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("forth")));
  p.show("forth");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("fifth")));
  p.show("fifth");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());
}

TEST(pager, quit_answer)
{
  NiceMock<mock_console> c;

  ON_CALL(c, width()).WillByDefault(Return(80));
  ON_CALL(c, height()).WillByDefault(Return(3));

  pager p(c);

  EXPECT_CALL(c, show(data::colored_string("first")));
  p.show("first");
  EXPECT_CALL(c, new_line());
  ASSERT_TRUE(p.new_line());

  EXPECT_CALL(c, show(data::colored_string("second")));
  p.show("second");
  EXPECT_CALL(c, new_line());
  EXPECT_CALL(c, ask_for_continuation())
      .WillOnce(Return(iface::console::user_answer::quit));
  ASSERT_FALSE(p.new_line());
}
