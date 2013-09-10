#ifndef METASHELL_EDITLINE_TAB_COMPLETION_OVERRIDE_HPP
#define METASHELL_EDITLINE_TAB_COMPLETION_OVERRIDE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/utility.hpp>

class editline_tab_completion_override : boost::noncopyable
{
private:
  typedef char** (*callback)(const char*, int, int);
public:
  editline_tab_completion_override(callback cb_);
  ~editline_tab_completion_override();
private:
  char** (*_old)(const char*, int, int);
};

#endif

