#include <metashell/in_memory_environment.hpp>
#include <metashell/standard.hpp>
#include <metashell/config.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <algorithm>
#include <iterator>

using namespace metashell;

namespace
{
  const std::vector<std::string> no_extra_arguments;

  template <class Cont>
  void add_with_prefix(
    const std::string& prefix_,
    const Cont& cont_,
    std::vector<std::string>& v_
  )
  {
    using boost::phoenix::arg_names::arg1;

    std::transform(
      cont_.begin(),
      cont_.end(),
      std::back_insert_iterator<std::vector<std::string> >(v_),
      prefix_ + arg1
    );
  }

}

in_memory_environment::in_memory_environment(
  const std::string& internal_dir_,
  const config& config_,
  const std::string& clang_extra_arg_
) :
  _buffer(),
  _headers(internal_dir_),
  _clang_args()
{
  _clang_args.push_back("-x");
  _clang_args.push_back("c++-header");
  _clang_args.push_back(clang_argument(config_.standard_to_use));
  _clang_args.push_back("-I" + internal_dir());

  add_with_prefix("-I", config_.include_path, _clang_args);
  add_with_prefix("-D", config_.macros, _clang_args);

  if (!config_.warnings_enabled)
  {
    _clang_args.push_back("-w");
  }

  _clang_args.insert(
    _clang_args.end(),
    config_.extra_clang_args.begin(),
    config_.extra_clang_args.end()
  );

  if (!clang_extra_arg_.empty())
  {
    _clang_args.push_back(clang_extra_arg_);
  }
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
  in_memory_environment::clang_arguments() const
{
  return _clang_args;
}

std::string in_memory_environment::internal_dir() const
{
  return _headers.internal_dir();
}

const headers& in_memory_environment::get_headers() const
{
  return _headers;
}

void in_memory_environment::add_clang_arg(const std::string& arg_)
{
  _clang_args.push_back(arg_);
}


