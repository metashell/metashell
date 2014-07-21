
#include <string>
#include <fstream>
#include <sstream>

#include <metashell/shell.hpp>
#include <metashell/pragma_templight.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

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
  return "Get templight info about the last evaluation.";
}

void pragma_templight::command_print(const templight_trace& trace) const {
  trace.print_graph();
}

void pragma_templight::command_print_dot(const templight_trace& trace) const {
  std::ofstream file("graph.dot");
  trace.print_graphviz(file);
}

void pragma_templight::command_forward_trace(
    const templight_trace& trace,
    const std::vector<char>& type) const
{
  trace.print_forwardtrace(std::string(type.begin(), type.end()));
}

void pragma_templight::command_unknown() const {
  std::cout << "Unknown command" << std::endl;
}

void pragma_templight::run_command(
    const std::string& command, templight_trace& trace) const
{

  namespace spirit = boost::spirit;
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  namespace phx = boost::phoenix;

  using ascii::space;
  using ascii::print;
  using phx::bind;
  using phx::ref;
  using qi::lexeme;
  using qi::lit;
  using qi::eoi;
  using qi::_1;
  using qi::_2;

  std::string::const_iterator begin = command.begin();

  bool success =
    qi::phrase_parse(
      begin,
      command.end(),

      (lit("print") >> eoi)
        [bind(&pragma_templight::command_print, *this, ref(trace))] |
      (lexeme[lit("print") >> space] >> "dot" >> eoi)
        [bind(&pragma_templight::command_print_dot, *this, ref(trace))] |
      (lexeme[lit("ft") >> space] >> *print >> eoi)
        [bind(&pragma_templight::command_forward_trace, *this, ref(trace), _2)]

      ,
      space);

  if (!success || begin != command.end()) {
    std::cout << std::string(begin, command.end()) << std::endl;
    command_unknown();
  }
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

  run_command(command, trace);
}

