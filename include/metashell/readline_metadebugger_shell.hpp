#ifndef METASHELL_READLINE_METADEBUGGER_SHELL_HPP
#define METASHELL_READLINE_METADEBUGGER_SHELL_HPP

#include <just/console.hpp>

#include <metashell/metadebugger_shell.hpp>
#include <metashell/readline_environment.hpp>

namespace metashell {

class readline_metadebugger_shell : public metadebugger_shell {
public:

  readline_metadebugger_shell(
      const config& conf,
      environment& env,
      const std::string& args);

  virtual void run();

  virtual void add_history(const std::string& str);

  virtual void display(
      const std::string& str,
      optional_color color = boost::none) const;

  virtual unsigned width() const;
private:

  readline_environment readline_env;
};

}

#endif
