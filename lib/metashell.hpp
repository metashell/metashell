#ifndef METASHELL_METASHELL_HPP
#define METASHELL_METASHELL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/config.hpp>

#include "result.hpp"

#include <set>
#include <string>

namespace metashell
{
  std::string append_to_buffer(
    const std::string& buffer_,
    const std::string& s_
  );

  result eval_tmp(
    const std::string& buffer_,
    const std::string& tmp_exp_,
    const config& config_
  );

  result validate_code(const std::string& s_, const config& config_);

  void code_complete(
    const std::string& buffer_,
    const std::string& src_,
    const config& config_,
    std::set<std::string>& out_
  );
}

#endif

