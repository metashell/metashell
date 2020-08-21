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

#include <metashell/data/engine_name.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/feature.hpp>

#include <metashell/core/feature_validator.hpp>
#include <metashell/core/if_supported.hpp>
#include <metashell/core/supported.hpp>

#include <metashell/iface/engine.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

namespace metashell
{
  namespace core
  {
    template <class TypeShell,
              class PreprocessorShell,
              class CodeCompleter,
              class HeaderDiscoverer,
              class MetaprogramTracer,
              class CppValidator,
              class MacroDiscovery,
              class PreprocessorTracer>
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
      static_assert(!supported<HeaderDiscoverer>::value ||
                        std::is_base_of<iface::header_discoverer,
                                        HeaderDiscoverer>::value,
                    "Header discoverer is needed");
      static_assert(!supported<MetaprogramTracer>::value ||
                        std::is_base_of<iface::metaprogram_tracer,
                                        MetaprogramTracer>::value,
                    "Metaprogram tracer is needed");
      static_assert(
          !supported<CppValidator>::value ||
              std::is_base_of<iface::cpp_validator, CppValidator>::value,
          "C++ validator is needed");
      static_assert(
          !supported<MacroDiscovery>::value ||
              std::is_base_of<iface::macro_discovery, MacroDiscovery>::value,
          "Macro discovery is needed");
      static_assert(!supported<PreprocessorTracer>::value ||
                        std::is_base_of<iface::preprocessor_tracer,
                                        PreprocessorTracer>::value,
                    "Preprocessor tracer is needed");

      engine(data::real_engine_name name_,
             data::engine_name display_name_,
             TypeShell type_shell_,
             PreprocessorShell preprocessor_shell_,
             CodeCompleter code_completer_,
             HeaderDiscoverer header_discoverer_,
             MetaprogramTracer metaprogram_tracer_,
             CppValidator cpp_validator_,
             MacroDiscovery macro_discovery_,
             PreprocessorTracer preprocessor_tracer_,
             std::function<data::engine_config()> parse_config_)
        : _name(std::move(name_)),
          _display_name(std::move(display_name_)),
          _parse_config(std::move(parse_config_)),
          _type_shell(std::move(type_shell_)),
          _preprocessor_shell(std::move(preprocessor_shell_)),
          _code_completer(std::move(code_completer_)),
          _header_discoverer(std::move(header_discoverer_)),
          _metaprogram_tracer(std::move(metaprogram_tracer_)),
          _cpp_validator(std::move(cpp_validator_)),
          _macro_discovery(std::move(macro_discovery_)),
          _preprocessor_tracer(std::move(preprocessor_tracer_))
      {
      }

      virtual data::real_engine_name name() const override { return _name; }

      virtual data::engine_name display_name() const override
      {
        return _display_name;
      }

      virtual data::engine_config config() const override
      {
        if (_parse_config)
        {
          return _parse_config();
        }
        else
        {
          throw data::exception("Switching from engine " + to_string(_name) +
                                " is not supported.");
        }
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

      virtual const iface::preprocessor_shell&
      preprocessor_shell() const override
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
        return if_supported<iface::header_discoverer>(
            _header_discoverer, _name);
      }

      virtual const iface::header_discoverer& header_discoverer() const override
      {
        return if_supported<iface::header_discoverer>(
            _header_discoverer, _name);
      }

      virtual iface::metaprogram_tracer& metaprogram_tracer() override
      {
        return if_supported<iface::metaprogram_tracer>(
            _metaprogram_tracer, _name);
      }

      virtual const iface::metaprogram_tracer&
      metaprogram_tracer() const override
      {
        return if_supported<iface::metaprogram_tracer>(
            _metaprogram_tracer, _name);
      }

      virtual iface::cpp_validator& cpp_validator() override
      {
        return if_supported<iface::cpp_validator>(_cpp_validator, _name);
      }

      virtual const iface::cpp_validator& cpp_validator() const override
      {
        return if_supported<iface::cpp_validator>(_cpp_validator, _name);
      }

      virtual iface::macro_discovery& macro_discovery() override
      {
        return if_supported<iface::macro_discovery>(_macro_discovery, _name);
      }

      virtual const iface::macro_discovery& macro_discovery() const override
      {
        return if_supported<iface::macro_discovery>(_macro_discovery, _name);
      }

      virtual iface::preprocessor_tracer& preprocessor_tracer() override
      {
        return if_supported<iface::preprocessor_tracer>(
            _preprocessor_tracer, _name);
      }

      virtual const iface::preprocessor_tracer&
      preprocessor_tracer() const override
      {
        return if_supported<iface::preprocessor_tracer>(
            _preprocessor_tracer, _name);
      }

    private:
      data::real_engine_name _name;
      data::engine_name _display_name;
      std::function<data::engine_config()> _parse_config;
      TypeShell _type_shell;
      PreprocessorShell _preprocessor_shell;
      CodeCompleter _code_completer;
      HeaderDiscoverer _header_discoverer;
      MetaprogramTracer _metaprogram_tracer;
      CppValidator _cpp_validator;
      MacroDiscovery _macro_discovery;
      PreprocessorTracer _preprocessor_tracer;
    };

    template <class TypeShell,
              class PreprocessorShell,
              class CodeCompleter,
              class HeaderDiscoverer,
              class MetaprogramTracer,
              class CppValidator,
              class MacroDiscovery,
              class PreprocessorTracer>
    std::unique_ptr<engine<TypeShell,
                           PreprocessorShell,
                           CodeCompleter,
                           HeaderDiscoverer,
                           MetaprogramTracer,
                           CppValidator,
                           MacroDiscovery,
                           PreprocessorTracer>>
    make_engine(data::real_engine_name name_,
                data::engine_name display_name_,
                TypeShell&& type_shell_,
                PreprocessorShell&& preprocessor_shell_,
                CodeCompleter&& code_completer_,
                HeaderDiscoverer&& header_discoverer_,
                MetaprogramTracer&& metaprogram_tracer_,
                CppValidator&& cpp_validator_,
                MacroDiscovery&& macro_discovery_,
                PreprocessorTracer&& preprocessor_tracer_,
                const std::vector<data::feature>& supported_features_,
                std::function<data::engine_config()> parse_config_ =
                    std::function<data::engine_config()>())
    {
      // clang-format off

    feature_validator(name_, supported_features_)
      .check(data::feature::type_shell(), type_shell_)
      .check(data::feature::preprocessor_shell(), preprocessor_shell_)
      .check(data::feature::code_completer(), code_completer_)
      .check(data::feature::header_discoverer(), header_discoverer_)
      .check(data::feature::metaprogram_tracer(), metaprogram_tracer_)
      .check(data::feature::cpp_validator(), cpp_validator_)
      .check(data::feature::macro_discovery(), macro_discovery_)
      .check(data::feature::preprocessor_tracer(), preprocessor_tracer_)

      .all_checked()
    ;

      // clang-format on

      return std::make_unique<engine<
          TypeShell, PreprocessorShell, CodeCompleter, HeaderDiscoverer,
          MetaprogramTracer, CppValidator, MacroDiscovery, PreprocessorTracer>>(
          std::move(name_), std::move(display_name_),
          std::forward<TypeShell>(type_shell_),
          std::forward<PreprocessorShell>(preprocessor_shell_),
          std::forward<CodeCompleter>(code_completer_),
          std::forward<HeaderDiscoverer>(header_discoverer_),
          std::forward<MetaprogramTracer>(metaprogram_tracer_),
          std::forward<CppValidator>(cpp_validator_),
          std::forward<MacroDiscovery>(macro_discovery_),
          std::forward<PreprocessorTracer>(preprocessor_tracer_),
          std::move(parse_config_));
    }

    std::optional<std::string> limitation(const iface::engine& engine_);
  }
}

#endif
