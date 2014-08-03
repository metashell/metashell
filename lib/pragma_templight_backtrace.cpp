
#include <string>
#include <sstream>

#include <metashell/shell.hpp>
#include <metashell/token_iterator.hpp>
#include <metashell/templight_trace.hpp>
#include <metashell/pragma_templight_backtrace.hpp>

using namespace metashell;

pragma_templight_backtrace::pragma_templight_backtrace(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_templight_backtrace::clone() const
{
  return new pragma_templight_backtrace(_shell);
}

std::string pragma_templight_backtrace::arguments() const
{
  return "";
}

std::string pragma_templight_backtrace::description() const
{
  return "Get backtrace for type given as parameter.";
}

void pragma_templight_backtrace::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  std::string type = tokens_to_string(args_begin_, args_end_);

  templight_trace trace =
    metashell::templight_trace::create_from_xml("templight.xml");

  if (type.empty()) {
    trace.print_full_backtrace(_shell.width());
  } else {
    trace.print_backtrace(type, _shell.width());
  }
}

