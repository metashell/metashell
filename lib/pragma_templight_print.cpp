
#include <string>

#include <metashell/shell.hpp>
#include <metashell/templight_trace.hpp>
#include <metashell/pragma_templight_print.hpp>

using namespace metashell;

pragma_templight_print::pragma_templight_print(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_templight_print::clone() const
{
  return new pragma_templight_print(_shell);
}

std::string pragma_templight_print::arguments() const
{
  return "";
}

std::string pragma_templight_print::description() const
{
  return "Print the templight graph for the last evauluation.";
}

void pragma_templight_print::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  templight_trace trace =
    metashell::templight_trace::create_from_xml("templight.xml");

  trace.print_graph();
}

