#ifndef METASHELL_METADEBUGGER_SHELL_HPP
#define METASHELL_METADEBUGGER_SHELL_HPP

#include <string>

#include <boost/optional.hpp>

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/templight_trace.hpp>
#include <metashell/readline_environment.hpp>

namespace metashell {

class metadebugger_shell {
public:
  typedef boost::optional<just::console::color> optional_color;

  metadebugger_shell(
      const config& conf,
      const environment& env,
      const std::string& args);

  virtual ~metadebugger_shell();

  virtual void run() = 0;

  virtual void add_history(const std::string& str) = 0;

  virtual void display(
      const std::string& str,
      optional_color color = boost::none) const = 0;

  virtual unsigned width() const = 0;

protected:
  std::string prompt() const;
  bool stopped() const;
  void line_available(const std::string& line);

  // TODO put this method somewhere global (shell has it too)
  void run_metaprogram(const std::string& str);

  const config& conf;
  const environment& env;

  templight_trace trace;

  bool is_stopped;
};

}

#endif
