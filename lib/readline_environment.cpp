
#include <metashell/readline_environment.hpp>

#include <cstdlib>

#ifndef _WIN32
#  include <sys/ioctl.h>
#endif

namespace metashell {

readline_environment::readline_environment() :
  last_rl_attempted_completion_function(rl_attempted_completion_function)
{

}

readline_environment::~readline_environment() {
  rl_attempted_completion_function = last_rl_attempted_completion_function;
}

boost::optional<std::string> readline_environment::readline(
    const std::string& prompt)
{
  char *line = ::readline(prompt.c_str());

  if (line) {
    std::string str(line);
#ifndef _WIN32
    // It breaks on Windows. The library owns the buffer?
    std::free(line);
#endif
    return str;
  } else {
    return boost::none;
  }
}

std::string readline_environment::get_edited_text() {
#ifndef _WIN32
  return std::string(rl_line_buffer, rl_line_buffer + rl_end);
#else
  return std::string();
#endif
}

void readline_environment::add_history(const std::string& line) {
  //TODO save/restore history
  ::add_history(line.c_str());
}

void readline_environment::set_rl_attempted_completion_function(
    rl_completion_func_t func)
{
  rl_attempted_completion_function = func;
}


// This has nothing to do with readline, but it is better to have this
// in one common place
unsigned int readline_environment::width() const {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO info;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.srWindow.Right - info.srWindow.Left + 1;
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
#endif
}

}
