
#include <metashell/metashell.hpp>

#include <metashell/metadebugger_shell.hpp>

namespace metashell {

metadebugger_shell::metadebugger_shell(
    const config& conf,
    environment& env,
    const std::string& args) :
  conf(conf),
  env(env),
  is_stopped(false)
{
  if (!args.empty()) {
    get_templight_trace_from_metaprogram(args);
  }
}

metadebugger_shell::~metadebugger_shell() {}

std::string metadebugger_shell::prompt() const {
  return "(mdb) ";
}

bool metadebugger_shell::stopped() const {
  return is_stopped;
}

void metadebugger_shell::line_available(const std::string& line) {
  get_templight_trace_from_metaprogram(line);
  trace.print_full_forwardtrace(*this);
}

void metadebugger_shell::get_templight_trace_from_metaprogram(
    const std::string& str)
{

  std::vector<std::string>& clang_args = env.clang_arguments();

  clang_args.push_back("-templight");
  clang_args.push_back("-templight-output");
  clang_args.push_back("templight.xml");
  clang_args.push_back("-templight-format");
  clang_args.push_back("xml");

  run_metaprogram(str);

  clang_args.erase(clang_args.end() - 5, clang_args.end());

  trace = templight_trace::create_from_xml("templight.xml");
}

void metadebugger_shell::run_metaprogram(const std::string& str) {
  result res = eval_tmp(env, str, conf, "<mdb-stdin>");

  if (!res.info.empty()) {
    std::cout << res.info;
  }

  for (const std::string& e : res.errors)
  {
    display(e + "\n", just::console::color::red);
  }
  if (!res.has_errors())
  {
    std::cout << res.output << std::endl;
  }
}

}

