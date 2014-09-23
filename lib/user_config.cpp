#include <metashell/user_config.hpp>

using namespace metashell;

user_config::user_config() :
  include_path(),
  verbose(false),
  syntax_highlight(true),
  indent(true),
  standard_to_use(standard::cpp11),
  warnings_enabled(true),
  use_precompiled_headers(false),
  clang_path(),
  max_template_depth(256)
{}

