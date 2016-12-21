#ifndef METASHELL_ENGINE_HPP
#define METASHELL_ENGINE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/engine.hpp>
#include <metashell/make_unique.hpp>

#include <memory>
#include <type_traits>

namespace metashell
{
  template <class TypeShell,
            class PreprocessorShell,
            class CodeCompleter,
            class HeaderDiscoverer>
  class engine : public iface::engine
  {
  public:
    static_assert(std::is_base_of<iface::type_shell, TypeShell>::value,
                  "Type shell is needed");
    static_assert(
        std::is_base_of<iface::preprocessor_shell, PreprocessorShell>::value,
        "Preprocessor shell is needed");
    static_assert(std::is_base_of<iface::code_completer, CodeCompleter>::value,
                  "Code completer is needed");
    static_assert(
        std::is_base_of<iface::header_discoverer, HeaderDiscoverer>::value,
        "Header discoverer is needed");

    engine(TypeShell type_shell_,
           PreprocessorShell preprocessor_shell_,
           CodeCompleter code_completer_,
           HeaderDiscoverer header_discoverer_)
      : _type_shell(std::move(type_shell_)),
        _preprocessor_shell(std::move(preprocessor_shell_)),
        _code_completer(std::move(code_completer_)),
        _header_discoverer(std::move(header_discoverer_))
    {
    }

    virtual iface::type_shell& type_shell() override { return _type_shell; }

    virtual const iface::type_shell& type_shell() const override
    {
      return _type_shell;
    }

    virtual iface::preprocessor_shell& preprocessor_shell() override
    {
      return _preprocessor_shell;
    }

    virtual const iface::preprocessor_shell& preprocessor_shell() const override
    {
      return _preprocessor_shell;
    }

    virtual iface::code_completer& code_completer() override
    {
      return _code_completer;
    }

    virtual const iface::code_completer& code_completer() const override
    {
      return _code_completer;
    }

    virtual iface::header_discoverer& header_discoverer() override
    {
      return _header_discoverer;
    }

    virtual const iface::header_discoverer& header_discoverer() const override
    {
      return _header_discoverer;
    }

  private:
    TypeShell _type_shell;
    PreprocessorShell _preprocessor_shell;
    CodeCompleter _code_completer;
    HeaderDiscoverer _header_discoverer;
  };

  template <class TypeShell,
            class PreprocessorShell,
            class CodeCompleter,
            class HeaderDiscoverer>
  std::unique_ptr<
      engine<TypeShell, PreprocessorShell, CodeCompleter, HeaderDiscoverer>>
  make_engine(TypeShell&& type_shell_,
              PreprocessorShell&& preprocessor_shell_,
              CodeCompleter&& code_completer_,
              HeaderDiscoverer&& header_discoverer_)
  {
    return make_unique<
        engine<TypeShell, PreprocessorShell, CodeCompleter, HeaderDiscoverer>>(
        std::forward<TypeShell>(type_shell_),
        std::forward<PreprocessorShell>(preprocessor_shell_),
        std::forward<CodeCompleter>(code_completer_),
        std::forward<HeaderDiscoverer>(header_discoverer_));
  }
}

#endif
