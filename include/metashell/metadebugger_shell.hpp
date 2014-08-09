#ifndef METASHELL_METADEBUGGER_SHELL_HPP
#define METASHELL_METADEBUGGER_SHELL_HPP

#include <string>

#include <metashell/readline_environment.hpp>

namespace metashell {

class metadebugger_shell {
public:

  void run();
private:

  readline_environment _readline_environment;

  std::string prompt() const;
  bool stopped() const;
  void line_available(const std::string& line);

};

}

#endif
