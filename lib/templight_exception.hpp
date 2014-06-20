#ifndef METASHELL_TEMPLIGHT_EXCEPTION_HPP
#define METASHELL_TEMPLIGHT_EXCEPTION_HPP

#include <exception>
#include <boost/exception.hpp>

namespace metashell {

struct templight_exception :
  virtual std::exception, virtual boost::exception {};

}

#endif
