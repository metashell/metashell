#ifndef METASHELL_SHELL_HPP
#define METASHELL_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/pragma_handler_map.hpp>

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/history.hpp>

#include <just/console.hpp>

#include <boost/optional.hpp>

#include <string>
#include <set>
#include <map>
#include <stack>
#include <memory>

namespace metashell
{
  class shell
  {
  public:
    shell(const config& config_, iface::displayer& displayer_);

    shell(
      const config& config_,
      std::unique_ptr<environment> env_,
      iface::displayer& displayer_
    );

    iface::displayer& displayer();

    void display_splash();
    void line_available(const std::string& s_);
    std::string prompt() const;

    void cancel_operation();

    bool store_in_buffer(const std::string& s_);
    void run_metaprogram(const std::string& s_);

    static const char* input_filename();

    void code_complete(
      const std::string& s_,
      std::set<std::string>& out_
    ) const;

    const pragma_handler_map& pragma_handlers() const;

    void verbose(bool enabled_);
    bool verbose() const;

    bool stopped() const;
    void stop();

    void using_precompiled_headers(bool enabled_);
    bool using_precompiled_headers() const;

    const environment& env() const;
    environment& env();

    void reset_environment();
    void push_environment();
    void pop_environment();
    void display_environment_stack_size();
    void rebuild_environment();

    const config& get_config() const;

    void history(iface::history& h_);
    iface::history* history();
  private:
    std::string _line_prefix;
    std::unique_ptr<environment> _env;
    config _config;
    std::string _prev_line;
    pragma_handler_map _pragma_handlers;
    bool _stopped;
    std::stack<std::string> _environment_stack;
    iface::displayer& _displayer;
    iface::history* _history; // not owning

    void init();
    void rebuild_environment(const std::string& content_);
  };
}

#endif

