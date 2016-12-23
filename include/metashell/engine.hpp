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

#include <metashell/if_supported.hpp>
#include <metashell/iface/engine.hpp>
#include <metashell/make_unique.hpp>
#include <metashell/supported.hpp>

#include <memory>
#include <string>
#include <type_traits>

namespace metashell
{
  template <class TypeShell,
            class PreprocessorShell,
            class CodeCompleter,
            class HeaderDiscoverer,
            class TemplateTracer>
  class engine : public iface::engine
  {
  public:
    static_assert(!supported<TypeShell>::value ||
                      std::is_base_of<iface::type_shell, TypeShell>::value,
                  "Type shell is needed");
    static_assert(!supported<PreprocessorShell>::value ||
                      std::is_base_of<iface::preprocessor_shell,
                                      PreprocessorShell>::value,
                  "Preprocessor shell is needed");
    static_assert(
        !supported<CodeCompleter>::value ||
            std::is_base_of<iface::code_completer, CodeCompleter>::value,
        "Code completer is needed");
    static_assert(
        !supported<HeaderDiscoverer>::value ||
            std::is_base_of<iface::header_discoverer, HeaderDiscoverer>::value,
        "Header discoverer is needed");
    static_assert(
        !supported<TemplateTracer>::value ||
            std::is_base_of<iface::template_tracer, TemplateTracer>::value,
        "Template tracer is needed");

    engine(std::string name_,
           TypeShell type_shell_,
           PreprocessorShell preprocessor_shell_,
           CodeCompleter code_completer_,
           HeaderDiscoverer header_discoverer_,
           TemplateTracer template_tracer_)
      : _name(std::move(name_)),
        _type_shell(std::move(type_shell_)),
        _preprocessor_shell(std::move(preprocessor_shell_)),
        _code_completer(std::move(code_completer_)),
        _header_discoverer(std::move(header_discoverer_)),
        _template_tracer(std::move(template_tracer_))
    {
    }

    virtual iface::type_shell& type_shell() override
    {
      return if_supported<iface::type_shell>(_type_shell, _name);
    }

    virtual const iface::type_shell& type_shell() const override
    {
      return if_supported<iface::type_shell>(_type_shell, _name);
    }

    virtual iface::preprocessor_shell& preprocessor_shell() override
    {
      return if_supported<iface::preprocessor_shell>(
          _preprocessor_shell, _name);
    }

    virtual const iface::preprocessor_shell& preprocessor_shell() const override
    {
      return if_supported<iface::preprocessor_shell>(
          _preprocessor_shell, _name);
    }

    virtual iface::code_completer& code_completer() override
    {
      return if_supported<iface::code_completer>(_code_completer, _name);
    }

    virtual const iface::code_completer& code_completer() const override
    {
      return if_supported<iface::code_completer>(_code_completer, _name);
    }

    virtual iface::header_discoverer& header_discoverer() override
    {
      return if_supported<iface::header_discoverer>(_header_discoverer, _name);
    }

    virtual const iface::header_discoverer& header_discoverer() const override
    {
      return if_supported<iface::header_discoverer>(_header_discoverer, _name);
    }

    virtual iface::template_tracer& template_tracer() override
    {
      return if_supported<iface::template_tracer>(_template_tracer, _name);
    }

    virtual const iface::template_tracer& template_tracer() const override
    {
      return if_supported<iface::template_tracer>(_template_tracer, _name);
    }

  private:
    std::string _name;
    TypeShell _type_shell;
    PreprocessorShell _preprocessor_shell;
    CodeCompleter _code_completer;
    HeaderDiscoverer _header_discoverer;
    TemplateTracer _template_tracer;
  };

  template <class TypeShell,
            class PreprocessorShell,
            class CodeCompleter,
            class HeaderDiscoverer,
            class TemplateTracer>
  std::unique_ptr<engine<TypeShell,
                         PreprocessorShell,
                         CodeCompleter,
                         HeaderDiscoverer,
                         TemplateTracer>>
  make_engine(std::string name_,
              TypeShell&& type_shell_,
              PreprocessorShell&& preprocessor_shell_,
              CodeCompleter&& code_completer_,
              HeaderDiscoverer&& header_discoverer_,
              TemplateTracer&& template_tracer_)
  {
    return metashell::make_unique<
        engine<TypeShell, PreprocessorShell, CodeCompleter, HeaderDiscoverer,
               TemplateTracer>>(
        std::move(name_), std::forward<TypeShell>(type_shell_),
        std::forward<PreprocessorShell>(preprocessor_shell_),
        std::forward<CodeCompleter>(code_completer_),
        std::forward<HeaderDiscoverer>(header_discoverer_),
        std::forward<TemplateTracer>(template_tracer_));
  }
}

#endif
