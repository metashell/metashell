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
    explicit engine_constant(data::result result_) :
      _result(std::move(result_))
    {}

    virtual data::result eval_tmp_formatted(
      const iface::environment&,
      const std::string&,
      bool
    ) override
    {
      return _result;
    }

    virtual data::result eval(
      const iface::environment&,
      const boost::optional<std::string>&,
      const boost::optional<std::string>&,
      bool
    ) override
    {
      return _result;
    }

    virtual data::result validate_code(
      const std::string&,
      const data::config&,
      const iface::environment&,
      bool
    ) override
    {
      return _result;
    }

    virtual void code_complete(
      const iface::environment&,
      const std::string&,
      std::set<std::string>&,
      bool
    ) override
    {
      // ignore
    }

    virtual void precompile(const std::string&) override
    {
      // ignore
    }
  private:
    data::result _result;
  };

  std::unique_ptr<engine_constant> create(data::result result_)
  {
    return
      std::unique_ptr<engine_constant>(new engine_constant(std::move(result_)));
  }
}

std::unique_ptr<iface::engine> metashell::create_failing_engine(
  const std::string& msg_
)
{
  return create(data::result(false, "", msg_, ""));
}

std::unique_ptr<iface::engine> metashell::create_engine_returning_type(
  const std::string& type_
)
{
  return create(data::result(true, type_, "", ""));
}

