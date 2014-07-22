
#include <string>
#include <fstream>

#include <metashell/shell.hpp>
#include <metashell/templight_trace.hpp>
#include <metashell/pragma_templight_print_dot.hpp>

using namespace metashell;

pragma_templight_print_dot::pragma_templight_print_dot(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_templight_print_dot::clone() const
{
  return new pragma_templight_print_dot(_shell);
}

std::string pragma_templight_print_dot::arguments() const
{
  return "";
}

std::string pragma_templight_print_dot::description() const
{
  return "Output the templight graph in dot/graphviz format.";
}

void pragma_templight_print_dot::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{

  templight_trace trace =
    metashell::templight_trace::create_from_xml("templight.xml");

  std::fstream out("graph.dot");
  trace.print_graphviz(out);
}

