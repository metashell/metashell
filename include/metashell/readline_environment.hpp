#ifndef READLINE_ENVIRONMENT_HPP
#define READLINE_ENVIRONMENT_HPP

#include <string>

#include <boost/optional.hpp>

#ifdef USE_EDITLINE
#  include <editline/readline.h>
#else
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

namespace metashell {

class readline_environment {
public:
  readline_environment();
  ~readline_environment();

  boost::optional<std::string> readline(const std::string& prompt);
  std::string get_edited_text();
  void add_history(const std::string& line);
  void set_rl_attempted_completion_function(rl_completion_func_t func);
  unsigned int width() const;

private:
  rl_completion_func_t *last_rl_attempted_completion_function;
};

}

#endif
