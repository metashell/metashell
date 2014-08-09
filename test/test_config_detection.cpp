// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/config.hpp>
#include <metashell/user_config.hpp>

#include "mock_environment_detector.hpp"

#include <just/test.hpp>

#include <algorithm>
#include <sstream>

using namespace metashell;

namespace
{
  using metashell::detect_config;

  metashell::config detect_config(
    const metashell::user_config& ucfg_,
    metashell::iface::environment_detector& env_detector_
  )
  {
    std::ostringstream s;
    return detect_config(ucfg_, env_detector_, s);
  }

  template <class T, class Ts>
  bool contains(const T& item_, const Ts& container_)
  {
    const typename Ts::const_iterator e = container_.end();
    return std::find(container_.begin(), e, item_) != e;
  }

  void check_flag_is_kept(
    bool user_config::* ucfg_field_,
    bool config::* cfg_field_
  )
  {
    mock_environment_detector dstub;

    user_config cfg1;
    cfg1.*ucfg_field_ = true;

    user_config cfg2;
    cfg2.*ucfg_field_ = false;

    JUST_ASSERT(detect_config(cfg1, dstub).*cfg_field_);
    JUST_ASSERT(!(detect_config(cfg2, dstub).*cfg_field_));
  }
}

JUST_TEST_CASE(test_include_path_entry_is_kept)
{
  mock_environment_detector dstub;

  user_config ucfg;
  ucfg.include_path.push_back("/foo/bar");

  JUST_ASSERT(contains("/foo/bar", detect_config(ucfg, dstub).include_path));
}

JUST_TEST_CASE(test_verbosity_is_kept)
{
  check_flag_is_kept(&user_config::verbose, &config::verbose);
}

JUST_TEST_CASE(test_syntax_highlighting_is_kept)
{
  check_flag_is_kept(&user_config::syntax_highlight, &config::syntax_highlight);
}

JUST_TEST_CASE(test_indent_is_kept)
{
  check_flag_is_kept(&user_config::indent, &config::indent);
}

JUST_TEST_CASE(test_warnings_enabled_is_kept)
{
  check_flag_is_kept(&user_config::warnings_enabled, &config::warnings_enabled);
}

JUST_TEST_CASE(test_macro_definition_is_kept)
{
  mock_environment_detector dstub;

  user_config ucfg;
  ucfg.macros.push_back("FOO=bar");

  JUST_ASSERT(contains("FOO=bar", detect_config(ucfg, dstub).macros));
}

JUST_TEST_CASE(test_extra_clang_arg_is_kept)
{
  mock_environment_detector dstub;

  user_config ucfg;
  ucfg.extra_clang_args.push_back("--foo");

  JUST_ASSERT(contains("--foo", detect_config(ucfg, dstub).extra_clang_args));
}

JUST_TEST_CASE(test_clang_path_is_kept)
{
  mock_environment_detector dstub;

  user_config ucfg;
  ucfg.clang_path = "/foo/clang";

  JUST_ASSERT_EQUAL("/foo/clang", detect_config(ucfg, dstub).clang_path);
}

JUST_TEST_CASE(test_standard_to_use_is_kept)
{
  mock_environment_detector dstub;

  user_config ucfg;
  ucfg.standard_to_use = standard::cpp14;

  JUST_ASSERT_EQUAL(
    standard::cpp14,
    detect_config(ucfg, dstub).standard_to_use
  );
}

JUST_TEST_CASE(test_clang_binary_is_searched_when_not_specified)
{
  mock_environment_detector envd;
  envd.search_clang_binary_returns("/foo/bar/clang");
  
  const config cfg = detect_config(user_config(), envd);

  JUST_ASSERT_EQUAL(1, envd.search_clang_binary_called_times());
  JUST_ASSERT_EQUAL("/foo/bar/clang", cfg.clang_path);
}

JUST_TEST_CASE(test_clang_binary_is_not_searched_when_specified)
{
  mock_environment_detector envd;
  
  user_config ucfg;
  ucfg.clang_path = "/foo/clang";

  const config cfg = detect_config(ucfg, envd);

  JUST_ASSERT_EQUAL(0, envd.search_clang_binary_called_times());
}

JUST_TEST_CASE(
  test_clang_binarys_existence_is_checked_when_path_is_manually_specified
)
{
  mock_environment_detector envd;
  
  user_config ucfg;
  ucfg.clang_path = "/foo/clang";

  const config cfg = detect_config(ucfg, envd);

  JUST_ASSERT_EQUAL(1, envd.file_exists_called_times());
  JUST_ASSERT_EQUAL("/foo/clang", envd.file_exists_last_arg());
}

JUST_TEST_CASE(test_custom_clang_binary_is_not_used_when_does_not_exist)
{
  mock_environment_detector envd;
  envd.file_exists_returns(false);
  
  user_config ucfg;
  ucfg.clang_path = "/foo/clang";

  JUST_ASSERT_EQUAL("", detect_config(ucfg, envd).clang_path);
}

JUST_TEST_CASE(test_error_is_displayed_when_custom_clang_binary_is_not_found)
{
  mock_environment_detector envd;
  envd.file_exists_returns(false);
  
  user_config ucfg;
  ucfg.clang_path = "/foo/clang";

  std::ostringstream err;
  detect_config(ucfg, envd, err);

  JUST_ASSERT(!err.str().empty());
}

JUST_TEST_CASE(
  test_error_is_displayed_clang_binary_is_not_found_at_default_locations
)
{
  mock_environment_detector envd;
  
  std::ostringstream err;
  detect_config(user_config(), envd, err);

  JUST_ASSERT(!err.str().empty());
}

JUST_TEST_CASE(
  test_no_error_is_displayed_clang_binary_is_found_at_default_location
)
{
  mock_environment_detector envd;
  envd.search_clang_binary_returns("/foo/bar/clang");
  
  std::ostringstream err;
  detect_config(user_config(), envd, err);

  JUST_ASSERT_EQUAL("", err.str());
}

JUST_TEST_CASE(test_precompiled_headers_are_disabled_when_no_clang_is_found)
{
  mock_environment_detector envd;

  user_config ucfg;
  ucfg.use_precompiled_headers = true;
  
  std::ostringstream err;
  const config cfg = detect_config(ucfg, envd, err);

  JUST_ASSERT(!cfg.use_precompiled_headers);
  JUST_ASSERT(
    err.str().find("Disabling precompiled headers") != std::string::npos
  );
}

JUST_TEST_CASE(test_precompiled_headers_are_enabled_when_clang_is_found)
{
  mock_environment_detector envd;
  envd.search_clang_binary_returns("/foo/bar/clang");

  user_config ucfg;
  ucfg.use_precompiled_headers = true;
  
  std::ostringstream err;
  const config cfg = detect_config(ucfg, envd, err);

  JUST_ASSERT(cfg.use_precompiled_headers);
}

JUST_TEST_CASE(test_directory_of_binary_is_added_to_path_on_windows)
{
  mock_environment_detector envd;
  envd.on_windows_returns(true);
  envd.search_clang_binary_returns("/foo/bar/clang");

  std::ostringstream err;
  detect_config(user_config(), envd, err);

  JUST_ASSERT_EQUAL(1, envd.append_to_path_called_times());
  JUST_ASSERT_EQUAL("/foo/bar", envd.append_to_path_last_arg());
}

JUST_TEST_CASE(test_when_clang_is_not_found_nothing_is_added_to_path_on_windows)
{
  mock_environment_detector envd;
  envd.on_windows_returns(true);

  std::ostringstream err;
  detect_config(user_config(), envd, err);

  JUST_ASSERT_EQUAL(0, envd.append_to_path_called_times());
}

JUST_TEST_CASE(
  test_default_clang_sysinclude_is_preprended_to_include_path_when_clang_binary_is_available
)
{
  mock_environment_detector envd;
  envd.search_clang_binary_returns("/foo/bar/clang");
  envd.default_clang_sysinclude_returns_append("/foo/include");
  envd.default_clang_sysinclude_returns_append("/bar/include");

  user_config ucfg;
  ucfg.include_path.push_back("/user/1");

  std::ostringstream err;
  const config cfg = detect_config(ucfg, envd, err);
  
  JUST_ASSERT_EQUAL(3, cfg.include_path.size());
  JUST_ASSERT_EQUAL("/foo/include", cfg.include_path[0]);
  JUST_ASSERT_EQUAL("/bar/include", cfg.include_path[1]);
  JUST_ASSERT_EQUAL("/user/1", cfg.include_path[2]);
}

JUST_TEST_CASE(
  test_default_clang_sysinclude_is_not_called_when_clang_is_not_available
)
{
  mock_environment_detector envd;

  std::ostringstream err;
  detect_config(user_config(), envd, err);
  
  JUST_ASSERT_EQUAL(0, envd.default_clang_sysinclude_called_times());
}

JUST_TEST_CASE(
  test_adding_standard_headers_next_to_the_binary_to_include_path_on_windows
)
{
  mock_environment_detector envd;
  envd.on_windows_returns(true);
  envd.path_of_executable_returns("c:/program files/metashell.exe");

  std::ostringstream err;
  const config cfg = detect_config(user_config(), envd, err);
  
  JUST_ASSERT_EQUAL(2, cfg.include_path.size());
  JUST_ASSERT_EQUAL("c:/program files\\windows_headers", cfg.include_path[0]);
  JUST_ASSERT_EQUAL(
    "c:/program files\\windows_headers\\mingw32",
    cfg.include_path[1]
  );
}

JUST_TEST_CASE(
  test_standard_headers_next_to_the_binary_are_prepended_to_include_path
)
{
  mock_environment_detector envd;
  envd.on_windows_returns(true);
  envd.path_of_executable_returns("c:/program files/metashell.exe");

  user_config ucfg;
  ucfg.include_path.push_back("c:\\foo\\bar");

  std::ostringstream err;
  const config cfg = detect_config(ucfg, envd, err);
  
  JUST_ASSERT(!cfg.include_path.empty());
  JUST_ASSERT_EQUAL("c:\\foo\\bar", cfg.include_path.back());
}

JUST_TEST_CASE(test_mingw_header_path_follows_clang_sysinclude_path)
{
  mock_environment_detector envd;
  envd.search_clang_binary_returns("/foo/bar/clang");
  envd.default_clang_sysinclude_returns_append("/foo/include");
  envd.on_windows_returns(true);
  envd.path_of_executable_returns("c:/program files/metashell.exe");

  std::ostringstream err;
  const config cfg = detect_config(user_config(), envd, err);
  
  JUST_ASSERT_EQUAL(3, cfg.include_path.size());
  JUST_ASSERT_EQUAL("/foo/include", cfg.include_path[0]);
  JUST_ASSERT_EQUAL("c:/program files\\windows_headers", cfg.include_path[1]);
  JUST_ASSERT_EQUAL(
    "c:/program files\\windows_headers\\mingw32",
    cfg.include_path[2]
  );
}

JUST_TEST_CASE(
  test_when_no_clang_binary_is_available_extra_sysinclude_is_added_to_include_path
)
{
  mock_environment_detector envd;
  envd.extra_sysinclude_returns_append("/foo/include");
  envd.extra_sysinclude_returns_append("/bar/include");

  user_config ucfg;
  ucfg.include_path.push_back("/user/1");

  std::ostringstream err;
  const config cfg = detect_config(ucfg, envd, err);
  
  JUST_ASSERT_EQUAL(3, cfg.include_path.size());
  JUST_ASSERT_EQUAL("/foo/include", cfg.include_path[0]);
  JUST_ASSERT_EQUAL("/bar/include", cfg.include_path[1]);
  JUST_ASSERT_EQUAL("/user/1", cfg.include_path[2]);
}

