#include <metashell/in_memory_environment.hpp>

using namespace metashell;

namespace
{
  const std::vector<std::string> no_extra_arguments;
}

void in_memory_environment::append(const std::string& s_)
{
  _buffer = get_appended(s_);
}

std::string in_memory_environment::get() const
{
  return _buffer;
}

std::string in_memory_environment::get_appended(const std::string& s_) const
{
  return _buffer.empty() ? s_ : (_buffer + '\n' + s_);
}

const std::vector<std::string>&
  in_memory_environment::extra_clang_arguments() const
{
  return no_extra_arguments;
}

std::string in_memory_environment::internal_dir() const
{
  return "__metashell_internal";
}


