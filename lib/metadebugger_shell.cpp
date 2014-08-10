
#include <metashell/metashell.hpp>

#include <metashell/metadebugger_shell.hpp>

namespace metashell {

metadebugger_shell::metadebugger_shell(
    const config& conf,
    const environment& env,
    const std::string& args) :
  conf(conf),
  env(env),
  is_stopped(false)
{
  if (!args.empty()) {
    run_metaprogram(args);
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

  templight_trace trace =
    templight_trace::create_from_xml("templight.xml");


}

void metadebugger_shell::run_metaprogram(const std::string& str) {
  result res = eval_tmp(env, str, conf, "<mdb-stdin>");

  if (!res.info.empty()) {
    std::cout << res.info;
  }

  for (const std::string& e : res.errors)
  {
    display(e, just::console::color::red);
  }
  if (!res.has_errors())
  {
    std::cout << res.output << std::endl;
  }
}

}

