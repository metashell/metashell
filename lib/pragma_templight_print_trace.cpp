
#include <metashell/pragma_templight_print_trace.hpp>
#include <metashell/shell.hpp>

#include "templight_trace.hpp"

using namespace metashell;

pragma_templight_print_trace::pragma_templight_print_trace(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_templight_print_trace::clone() const
{
  return new pragma_templight_print_trace(_shell);
}

std::string pragma_templight_print_trace::arguments() const
{
  return "";
}

std::string pragma_templight_print_trace::description() const
{
  return "Prints the last template stack trace templight produced";
}

void pragma_templight_print_trace::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  templight_trace trace =
    metashell::templight_trace::create_from_xml("templight.xml");

  trace.print_graph();
}

