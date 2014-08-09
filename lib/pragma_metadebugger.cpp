
#include <metashell/shell.hpp>
#include <metashell/token_iterator.hpp>
#include <metashell/pragma_metadebugger.hpp>
#include <metashell/metadebugger_shell.hpp>

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
  if (!args.empty()) {
    _shell.display_error("Warning: arguments are ignored");
  }

  metadebugger_shell mdb_shell;
  mdb_shell.run();
}

