// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/engine_constant.hpp>

using namespace metashell;

namespace
{
  class engine_constant : public iface::engine
  {
  public:
    explicit engine_constant(
        data::result result_,
        std::vector<boost::filesystem::path> sysincludes_,
        std::vector<boost::filesystem::path> quoteincludes_)
      : _result(std::move(result_)),
        _sysincludes(move(sysincludes_)),
        _quoteincludes(move(quoteincludes_))
    {
    }

    virtual data::result precompile(const std::string&) override
    {
      return _result;
    }

    virtual data::result eval(const iface::environment&,
                              const boost::optional<std::string>&,
                              const boost::optional<boost::filesystem::path>&,
                              bool) override
    {
      return _result;
    }

    virtual data::result validate_code(const std::string&,
                                       const data::config&,
                                       const iface::environment&,
                                       bool) override
    {
      return _result;
    }

    virtual void code_complete(const iface::environment&,
                               const std::string&,
                               std::set<std::string>&,
                               bool) override
    {
      // ignore
    }

    virtual void
    generate_precompiled_header(const boost::filesystem::path&) override
    {
      // ignore
    }

    virtual std::string macros(const iface::environment&) override
    {
      return "";
    }

    virtual std::vector<boost::filesystem::path>
    include_path(data::include_type type_) override
    {
      switch (type_)
      {
      case data::include_type::sys:
        return _sysincludes;
      case data::include_type::quote:
        return _quoteincludes;
      }
      return {}; // avoid control reaches end of non-void function warnings on
      // some compilers
    }

    virtual std::set<boost::filesystem::path>
    files_included_by(const std::string&) override
    {
      return std::set<boost::filesystem::path>();
    }

  private:
    data::result _result;
    std::vector<boost::filesystem::path> _sysincludes;
    std::vector<boost::filesystem::path> _quoteincludes;
  };

  std::unique_ptr<engine_constant>
  create(data::result result_,
         std::vector<boost::filesystem::path> sysincludes_,
         std::vector<boost::filesystem::path> quoteincludes_)
  {
    return std::unique_ptr<engine_constant>(new engine_constant(
        std::move(result_), move(sysincludes_), move(quoteincludes_)));
  }
}

std::unique_ptr<iface::engine>
metashell::create_failing_engine(const std::string& msg_)
{
  return create(data::result(false, "", msg_, ""), {}, {});
}

std::unique_ptr<iface::engine>
metashell::create_engine_returning_type(const std::string& type_)
{
  return create(data::result(true, type_, "", ""), {}, {});
}

std::unique_ptr<iface::engine> metashell::create_engine_with_include_path(
    data::include_type type_, std::vector<boost::filesystem::path> path_)
{
  const data::result result(true, "int", "", "");
  switch (type_)
  {
  case data::include_type::sys:
    return create(result, move(path_), {});
  case data::include_type::quote:
    return create(result, {}, move(path_));
  }
  return nullptr; // avoid control reaches end of non-void function warnings on
  // some compilers
}
