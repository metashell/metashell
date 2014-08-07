
#include <string>
#include <sstream>

#include <metashell/shell.hpp>
#include <metashell/token_iterator.hpp>
#include <metashell/templight_trace.hpp>
#include <metashell/pragma_templight_forwardtrace.hpp>

using namespace metashell;

pragma_templight_forwardtrace::pragma_templight_forwardtrace(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_templight_forwardtrace::clone() const
{
  return new pragma_templight_forwardtrace(_shell);
}

std::string pragma_templight_forwardtrace::arguments() const
{
  return "";
}

std::string pragma_templight_forwardtrace::description() const
{
  return "Get forwardtrace for type given as parameter.";
}

void pragma_templight_forwardtrace::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  std::string type = tokens_to_string(args_begin_, args_end_);

  templight_trace trace =
    metashell::templight_trace::create_from_xml("templight.xml");

  if (type.empty()) {
    trace.print_full_forwardtrace(_shell);
  } else {
    trace.print_forwardtrace(_shell, type);
  }
}

