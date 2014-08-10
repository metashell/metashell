
#include <metashell/shell.hpp>
#include <metashell/token_iterator.hpp>
#include <metashell/pragma_metadebugger.hpp>
#include <metashell/readline_metadebugger_shell.hpp>

using namespace metashell;

pragma_metadebugger::pragma_metadebugger(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_metadebugger::clone() const
{
  return new pragma_metadebugger(_shell);
}

std::string pragma_metadebugger::arguments() const
{
  return "";
}

std::string pragma_metadebugger::description() const
{
  return "Starts the metadebugger.";
}

void pragma_metadebugger::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  std::string args = tokens_to_string(args_begin_, args_end_);

  readline_metadebugger_shell mdb_shell(_shell.get_config(), _shell.env(), args);
  mdb_shell.run();
}

