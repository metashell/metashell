#ifndef METASHELL_SHELL_HPP
#define METASHELL_SHELL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/config.hpp>

#include <string>

namespace metashell
{
  class shell
  {
  public:
    explicit shell(const config& config_);
    virtual ~shell();
  
    virtual void display_normal(const std::string& s_) const = 0;
    virtual void display_info(const std::string& s_) const = 0;
    virtual void display_error(const std::string& s_) const = 0;
  
    void display_splash() const;
    void line_available(const std::string& s_);
    std::string prompt() const;
  
    void cancel_operation();

    bool store_in_buffer(const std::string& s_);
  private:
    std::string _buffer;
    config _config;
  };
}

#endif

