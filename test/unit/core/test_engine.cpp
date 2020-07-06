// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/engine.hpp>

#include <metashell/engine/constant/builder.hpp>

#include <metashell/data/feature_not_supported.hpp>

#include <gtest/gtest.h>

using namespace metashell;

namespace
{
  template <class... SupportedFeatures>
  class test_engine : public iface::engine
  {
  public:
    explicit test_engine(iface::engine& base_) : _base(base_) {}

    data::real_engine_name name() const override
    {
      return data::real_engine_name::null;
    }

    data::engine_name display_name() const override
    {
      return data::real_engine_name::internal;
    }

    data::engine_config config() const override
    {
      return data::engine_config();
    }

    iface::type_shell& type_shell() override
    {
      return return_if_works(_base.type_shell());
    }

    const iface::type_shell& type_shell() const override
    {
      return return_if_works(_base.type_shell());
    }

    iface::preprocessor_shell& preprocessor_shell() override
    {
      return return_if_works(_base.preprocessor_shell());
    }

    const iface::preprocessor_shell& preprocessor_shell() const override
    {
      return return_if_works(_base.preprocessor_shell());
    }

    iface::code_completer& code_completer() override
    {
      return return_if_works(_base.code_completer());
    }

    const iface::code_completer& code_completer() const override
    {
      return return_if_works(_base.code_completer());
    }

    iface::header_discoverer& header_discoverer() override
    {
      return return_if_works(_base.header_discoverer());
    }

    const iface::header_discoverer& header_discoverer() const override
    {
      return return_if_works(_base.header_discoverer());
    }

    iface::metaprogram_tracer& metaprogram_tracer() override
    {
      return return_if_works(_base.metaprogram_tracer());
    }

    const iface::metaprogram_tracer& metaprogram_tracer() const override
    {
      return return_if_works(_base.metaprogram_tracer());
    }

    iface::cpp_validator& cpp_validator() override
    {
      return return_if_works(_base.cpp_validator());
    }

    const iface::cpp_validator& cpp_validator() const override
    {
      return return_if_works(_base.cpp_validator());
    }

    iface::macro_discovery& macro_discovery() override
    {
      return return_if_works(_base.macro_discovery());
    }

    const iface::macro_discovery& macro_discovery() const override
    {
      return return_if_works(_base.macro_discovery());
    }

    iface::preprocessor_tracer& preprocessor_tracer() override
    {
      return return_if_works(_base.preprocessor_tracer());
    }

    const iface::preprocessor_tracer& preprocessor_tracer() const override
    {
      return return_if_works(_base.preprocessor_tracer());
    }

  private:
    iface::engine& _base;

    template <class Feature>
    Feature& return_if_works(Feature& feature_) const
    {
      if (std::disjunction_v<std::is_same<
              typename std::remove_cv<Feature>::type, SupportedFeatures>...>)
      {
        return feature_;
      }
      else
      {
        throw data::feature_not_supported<Feature>(name());
      }
    }
  };
}

TEST(engine, limitation_results)
{
  using iface::type_shell;
  using iface::preprocessor_shell;
  using iface::code_completer;
  using iface::header_discoverer;
  using iface::metaprogram_tracer;
  using iface::cpp_validator;
  using iface::macro_discovery;
  using iface::preprocessor_tracer;

  std::unique_ptr<iface::engine> base =
      metashell::engine::constant::create_failing()(data::shell_config(
          data::shell_config_name("test"), data::shell_config_data{}));

  // complete
  ASSERT_EQ(
      std::nullopt,
      core::limitation(
          test_engine<type_shell, preprocessor_shell, code_completer,
                      header_discoverer, metaprogram_tracer, cpp_validator,
                      macro_discovery, preprocessor_tracer>(*base)));

  // clang, internal, templight
  ASSERT_EQ(
      std::string("Note that you are using the null engine, which means that "
                  "you can not use the preprocessor tracer. Metashell supports "
                  "different features if you use different compilers."),
      core::limitation(
          test_engine<type_shell, preprocessor_shell, code_completer,
                      header_discoverer, metaprogram_tracer, cpp_validator,
                      macro_discovery>(*base)));

  // wave, pure_wave
  ASSERT_EQ(
      std::string("Note that you are using the null engine, which means that "
                  "you can not debug types or metaprograms, only the "
                  "preprocessor. Metashell supports different features if you "
                  "use different compilers."),
      core::limitation(
          test_engine<preprocessor_shell, code_completer, header_discoverer,
                      cpp_validator, macro_discovery, preprocessor_tracer>(
              *base)));

  // gcc
  ASSERT_EQ(
      std::string("Note that you are using the null engine, which means that "
                  "you can not debug types or metaprograms, only the "
                  "preprocessor (but not trace it). Metashell supports "
                  "different features if you use different compilers."),
      core::limitation(
          test_engine<preprocessor_shell, code_completer, header_discoverer,
                      cpp_validator, macro_discovery>(*base)));

  // msvc
  ASSERT_EQ(
      std::string("Note that you are using the null engine, which means that "
                  "you can not debug types or metaprograms, only the "
                  "preprocessor (but not trace it or query the defined "
                  "macros). Metashell supports different features if you use "
                  "different compilers."),
      core::limitation(test_engine<preprocessor_shell, code_completer,
                                   header_discoverer, cpp_validator>(*base)));

  // null
  ASSERT_EQ(std::string("Note that you are using the null engine, which means "
                        "that you can not debug anything. Metashell supports "
                        "different features if you use different compilers."),
            core::limitation(test_engine<>(*base)));
}
