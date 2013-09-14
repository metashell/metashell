#ifndef METASHELL_HEADERS_HPP
#define METASHELL_HEADERS_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang-c/Index.h>

#include <boost/iterator/transform_iterator.hpp>

#include <map>
#include <vector>
#include <string>

namespace metashell
{
  class headers
  {
  private:
    typedef std::pair<std::string, std::string> header;
  public:
    explicit headers(const std::string& src_);

    typedef
      boost::transform_iterator<
        CXUnsavedFile (*)(const header&),
        std::vector<header>::const_iterator
      >
      iterator;

    iterator begin() const;
    iterator end() const;

    const std::string& internal_dir() const;

    std::string operator[](const std::string& filename_) const;
  private:
    std::string _internal_dir;
    std::vector<header> _headers;

    void add(const std::string& filename_, const std::string& content_);
  };
}

#endif

