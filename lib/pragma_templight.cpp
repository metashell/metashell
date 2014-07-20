
#include <string>
#include <sstream>

#include <metashell/pragma_templight.hpp>
#include <metashell/shell.hpp>

#include "templight_trace.hpp"

using namespace metashell;

pragma_templight::pragma_templight(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_templight::clone() const
{
  return new pragma_templight(_shell);
}

std::string pragma_templight::arguments() const
{
  return "";
}

std::string pragma_templight::description() const
{
  return "Prints the last template stack trace templight produced.";
}

void pragma_templight::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  std::stringstream command_stream;
  for (token_iterator it = args_begin_; it != args_end_; ++it) {
    command_stream << it->get_value();
  }

  std::string command = command_stream.str();

  templight_trace trace =
    metashell::templight_trace::create_from_xml("templight.xml");

  //TODO better command parsing
  if (command == "print") {
    trace.print_graph();
  } else if (command == "print dot" || command == "print graphviz") {
    trace.print_graphviz();
  }

}

