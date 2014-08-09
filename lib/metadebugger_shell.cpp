
#include <metashell/metadebugger_shell.hpp>

namespace metashell {

void metadebugger_shell::run() {
  for (boost::optional<std::string> line;
      !stopped() && (line = _readline_environment.readline(prompt())); )
  {
    line_available(*line);
  }
  std::cout << std::endl;
}

std::string metadebugger_shell::prompt() const {
  return "(mdb) ";
}

bool metadebugger_shell::stopped() const {
  return false;
}

void metadebugger_shell::line_available(const std::string& line) {

}

}

