#include <metashell/header_file_environment.hpp>
#include <metashell/headers.hpp>

#include <iostream>
#include <fstream>

using namespace metashell;

namespace
{
  const char env_fn[] = "metashell_environment.hpp";
}

header_file_environment::header_file_environment() :
  _dir(),
  _extra_clang_args(1, "-I" + _dir.path()),
  _buffer(_dir.path())
{
  save();
  get_headers().generate();
}

void header_file_environment::append(const std::string& s_)
{
  _buffer.append(s_);
  save();
}

std::string header_file_environment::get() const
{
  return "#include <" + std::string(env_fn) + ">\n";
}

std::string header_file_environment::get_appended(const std::string& s_) const
{
  return get() + s_;
}

const std::vector<std::string>&
  header_file_environment::extra_clang_arguments() const
{
  return _extra_clang_args;
}

void header_file_environment::save()
{
  const std::string fn = _dir.path() + "/" + env_fn;
  std::ofstream f(fn.c_str());
  f << _buffer.get();
}

std::string header_file_environment::internal_dir() const
{
  return _dir.path();
}

const headers& header_file_environment::get_headers() const
{
  return _buffer.get_headers();
}


