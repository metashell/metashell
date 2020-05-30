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

#include <metashell/system_test/include_test_env.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/system_test_config.hpp>

#include <metashell/data/include_argument_type.hpp>
#include <metashell/data/include_type.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;
using namespace metashell;

namespace
{
  std::optional<data::command_line_argument>
  extends_include_path(const data::command_line_argument& arg_)
  {
    for (const char* prefix : {"-I", "-iquote", "-S", "-isystem", "-idirafter"})
    {
      if (const auto result =
              arg_.remove_prefix(data::command_line_argument(prefix)))
      {
        return result;
      }
    }
    return std::nullopt;
  }

  bool on_include_path(include_test_env::test_filename header_)
  {
    for (const data::command_line_argument& arg :
         system_test_config::metashell_args())
    {
      if (const auto path = extends_include_path(arg))
      {
        if (exists(boost::filesystem::path(path->value()) / to_path(header_)))
        {
          return true;
        }
      }
    }

    return false;
  }

  bool uses_clang(const std::string& engine_)
  {
    return engine_ == "internal" || engine_ == "clang" ||
           engine_ == "templight";
  }

  bool can_engine_use_c_headers_without_cpp_headers(const std::string& engine_)
  {
#ifdef _WIN32
    constexpr bool on_windows = true;
#else
    constexpr bool on_windows = false;
#endif
#ifdef __APPLE__
    constexpr bool on_osx = true;
#else
    constexpr bool on_osx = false;
#endif

    return engine_ != "msvc" && engine_ != "internal" &&
           !((uses_clang(engine_) || engine_ == "wave") &&
             (on_windows || on_osx));
  }

  void test_include_path_extensions(
      include_test_env& env_,
      const std::optional<std::string>& engine_override_ = std::nullopt)
  {
    using data::standard_headers_allowed;

    constexpr auto iquote = data::include_argument_type::iquote;
    constexpr auto capital_i = data::include_argument_type::capital_i;
    constexpr auto isystem = data::include_argument_type::isystem;
    constexpr auto idirafter = data::include_argument_type::idirafter;

    constexpr auto sys = data::include_type::sys;
    constexpr auto quote = data::include_type::quote;

    constexpr auto nonstandard_header =
        include_test_env::test_filename::nonstandard_header;
    constexpr auto standard_c_header =
        include_test_env::test_filename::standard_c_header;
    constexpr auto standard_cpp_header =
        include_test_env::test_filename::standard_cpp_header;

    const bool std_headers_on_include_path =
        on_include_path(standard_c_header) ||
        on_include_path(standard_cpp_header);

    const bool can_use_c_headers_without_cpp_headers =
        !std_headers_on_include_path &&
        can_engine_use_c_headers_without_cpp_headers(current_real_engine()) &&
        (!engine_override_ ||
         can_engine_use_c_headers_without_cpp_headers(*engine_override_));

    const include_test_env::result none_found =
        include_test_env::result::none();
    const include_test_env::result system_found =
        include_test_env::result::system_used();

    for (data::include_type type : {sys, quote})
    {
      /*
       * None
       */

      ASSERT_EQ(
          none_found, env_.include_dir_used({}, nonstandard_header, type));

      ASSERT_EQ(
          system_found, env_.include_dir_used({}, standard_c_header, type));

      ASSERT_EQ(
          system_found, env_.include_dir_used({}, standard_cpp_header, type));

      if (!std_headers_on_include_path)
      {
        ASSERT_EQ(
            none_found, env_.include_dir_used({}, standard_c_header, type,
                                              standard_headers_allowed::none));

        ASSERT_EQ(
            none_found, env_.include_dir_used({}, standard_cpp_header, type,
                                              standard_headers_allowed::none));
      }

      if (can_use_c_headers_without_cpp_headers)
      {
        ASSERT_EQ(
            none_found, env_.include_dir_used({}, standard_cpp_header, type,
                                              standard_headers_allowed::c));

        ASSERT_EQ(
            system_found, env_.include_dir_used({}, standard_c_header, type,
                                                standard_headers_allowed::c));
      }

      /*
       * Single
       */

      // -I

      ASSERT_EQ(capital_i,
                env_.include_dir_used({capital_i}, nonstandard_header, type));

      ASSERT_EQ(capital_i,
                env_.include_dir_used({capital_i}, standard_c_header, type));

      ASSERT_EQ(capital_i,
                env_.include_dir_used({capital_i}, standard_cpp_header, type));

      if (!std_headers_on_include_path)
      {
        ASSERT_EQ(capital_i,
                  env_.include_dir_used({capital_i}, standard_c_header, type,
                                        standard_headers_allowed::none));

        ASSERT_EQ(capital_i,
                  env_.include_dir_used({capital_i}, standard_cpp_header, type,
                                        standard_headers_allowed::none));
      }

      if (can_use_c_headers_without_cpp_headers)
      {
        ASSERT_EQ(capital_i,
                  env_.include_dir_used({capital_i}, standard_cpp_header, type,
                                        standard_headers_allowed::c));

        ASSERT_EQ(capital_i,
                  env_.include_dir_used({capital_i}, standard_c_header, type,
                                        standard_headers_allowed::c));
      }

      // -iquote

      if (!using_msvc())
      {
        ASSERT_EQ(type == sys ? none_found : iquote,
                  env_.include_dir_used({iquote}, nonstandard_header, type));

        ASSERT_EQ(type == sys ? system_found : iquote,
                  env_.include_dir_used({iquote}, standard_c_header, type));

        ASSERT_EQ(type == sys ? system_found : iquote,
                  env_.include_dir_used({iquote}, standard_cpp_header, type));
      }

      if (!using_msvc() && !std_headers_on_include_path)
      {
        ASSERT_EQ(type == sys ? none_found : iquote,
                  env_.include_dir_used({iquote}, standard_c_header, type,
                                        standard_headers_allowed::none));

        ASSERT_EQ(type == sys ? none_found : iquote,
                  env_.include_dir_used({iquote}, standard_cpp_header, type,
                                        standard_headers_allowed::none));
      }

      if (can_use_c_headers_without_cpp_headers)
      {
        ASSERT_EQ(type == sys ? none_found : iquote,
                  env_.include_dir_used({iquote}, standard_cpp_header, type,
                                        standard_headers_allowed::c));

        ASSERT_EQ(type == sys ? system_found : iquote,
                  env_.include_dir_used({iquote}, standard_c_header, type,
                                        standard_headers_allowed::c));
      }

      // -isystem, -S

      if (!using_msvc())
      {
        ASSERT_EQ(isystem,
                  env_.include_dir_used({isystem}, nonstandard_header, type));

        ASSERT_EQ(
            isystem, env_.include_dir_used({isystem}, standard_c_header, type));

        ASSERT_EQ(isystem,
                  env_.include_dir_used({isystem}, standard_cpp_header, type));
      }

      if (!using_msvc() && !std_headers_on_include_path)
      {
        ASSERT_EQ(
            isystem, env_.include_dir_used({isystem}, standard_c_header, type,
                                           standard_headers_allowed::none));

        ASSERT_EQ(
            isystem, env_.include_dir_used({isystem}, standard_cpp_header, type,
                                           standard_headers_allowed::none));
      }

      if (can_use_c_headers_without_cpp_headers)
      {
        ASSERT_EQ(
            isystem, env_.include_dir_used({isystem}, standard_cpp_header, type,
                                           standard_headers_allowed::c));

        ASSERT_EQ(
            isystem, env_.include_dir_used({isystem}, standard_c_header, type,
                                           standard_headers_allowed::c));
      }

      // -idirafter

      if (!using_msvc())
      {
        ASSERT_EQ(idirafter,
                  env_.include_dir_used({idirafter}, nonstandard_header, type));

        ASSERT_EQ(system_found,
                  env_.include_dir_used({idirafter}, standard_c_header, type));

        ASSERT_EQ(system_found, env_.include_dir_used(
                                    {idirafter}, standard_cpp_header, type));
      }

      if (!using_msvc() && !std_headers_on_include_path)
      {
        ASSERT_EQ(idirafter,
                  env_.include_dir_used({idirafter}, standard_c_header, type,
                                        standard_headers_allowed::none));

        ASSERT_EQ(idirafter,
                  env_.include_dir_used({idirafter}, standard_cpp_header, type,
                                        standard_headers_allowed::none));
      }

      if (can_use_c_headers_without_cpp_headers)
      {
        ASSERT_EQ(idirafter,
                  env_.include_dir_used({idirafter}, standard_cpp_header, type,
                                        standard_headers_allowed::c));

        ASSERT_EQ(system_found,
                  env_.include_dir_used({idirafter}, standard_c_header, type,
                                        standard_headers_allowed::c));
      }

      /*
       * Pairs
       */

      if (!using_msvc())
      {
        ASSERT_EQ(type == sys ? capital_i : iquote,
                  env_.include_dir_used(
                      {capital_i, iquote}, nonstandard_header, type));

        ASSERT_EQ(capital_i, env_.include_dir_used({capital_i, isystem},
                                                   nonstandard_header, type));

        ASSERT_EQ(capital_i, env_.include_dir_used({capital_i, idirafter},
                                                   nonstandard_header, type));

        ASSERT_EQ(
            type == sys ? isystem : iquote,
            env_.include_dir_used({iquote, isystem}, nonstandard_header, type));

        ASSERT_EQ(type == sys ? idirafter : iquote,
                  env_.include_dir_used(
                      {iquote, idirafter}, nonstandard_header, type));

        ASSERT_EQ(isystem, env_.include_dir_used(
                               {isystem, idirafter}, nonstandard_header, type));
      }
    }
  }
}

TEST(include_types, tests)
{
  const std::string current_engine = current_real_engine();

  include_test_env env;
  test_include_path_extensions(env);

  env.run_before_all_checks("#msh engine switch " + current_engine);
  test_include_path_extensions(env);

  if (current_engine != "null")
  {
    for (const std::string engine : {"internal", "wave"})
    {
      if (engine != current_engine)
      {
        env.run_before_all_checks("#msh engine switch " + engine);
        test_include_path_extensions(env, engine);
      }
    }
  }
}
