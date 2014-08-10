
#include <iostream>

#include <boost/optional.hpp>

#include <metashell/readline_metadebugger_shell.hpp>

namespace metashell {

readline_metadebugger_shell::readline_metadebugger_shell(
    const config& conf,
    const environment& env,
    const std::string& args) :
  metadebugger_shell(conf, env, args) {}

void readline_metadebugger_shell::run() {
  for (boost::optional<std::string> line;
      !stopped() && (line = readline_env.readline(prompt())); )
  {
    line_available(*line);
  }
  std::cout << std::endl;
}

void readline_metadebugger_shell::add_history(const std::string& str) {
  readline_env.add_history(str);
}

void readline_metadebugger_shell::display(
    const std::string& str,
    optional_color color) const
{
  if (color) {
    just::console::text_color(*color);
  }
  std::cout << str;
  if (color) {
    just::console::reset();
  }
}

unsigned readline_metadebugger_shell::width() const {
  return readline_env.width();
}

}

