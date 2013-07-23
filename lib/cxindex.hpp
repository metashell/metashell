#ifndef METASHELL_CXINDEX_HPP
#define METASHELL_CXINDEX_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxtranslationunit.hpp"

#include <metashell/config.hpp>

#include <clang-c/Index.h>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace metashell
{
  class cxindex : boost::noncopyable
  {
  public:
    cxindex();
    ~cxindex();

    boost::shared_ptr<cxtranslationunit> parse_code(
      const std::string& src_,
      const config& config_
    );
  private:
    CXIndex _index;
  };
}

#endif

