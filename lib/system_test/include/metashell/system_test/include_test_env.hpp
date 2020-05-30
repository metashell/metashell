#ifndef METASHELL_SYSTEM_TEST_INCLUDE_TEST_ENV_HPP
#define METASHELL_SYSTEM_TEST_INCLUDE_TEST_ENV_HPP

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

#include <metashell/data/include_argument_type.hpp>
#include <metashell/data/include_type.hpp>
#include <metashell/data/standard_headers_allowed.hpp>
#include <metashell/data/unit_type.hpp>

#include <just/temp.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <variant.hpp>

#include <array>
#include <iosfwd>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    class include_test_env
    {
    private:
      struct system_used_t : data::unit_type<system_used_t>
      {
        constexpr static const char* name() { return "<system>"; }
      };

      struct none_t : data::unit_type<none_t>
      {
        constexpr static const char* name() { return "<none>"; }
      };

    public:
      enum class test_filename
      {
        nonstandard_header,
        standard_c_header,
        standard_cpp_header
      };

      class result : boost::equality_comparable<result>
      {
      public:
        result(data::include_argument_type value_);

        const mpark::variant<none_t,
                             system_used_t,
                             data::include_argument_type>&
        value() const;

        static result none();
        static result system_used();

      private:
        mpark::variant<none_t, system_used_t, data::include_argument_type>
            _value;

        result(none_t value_);
        result(system_used_t value_);
      };

      include_test_env();

      result include_dir_used(
          std::set<data::include_argument_type>,
          test_filename,
          data::include_type,
          data::standard_headers_allowed = data::standard_headers_allowed::all);

      void run_before_all_checks(std::string);

      static std::array<test_filename, 3> all_test_filenames();

    private:
      just::temp::directory _tmp_dir;
      std::optional<std::string> _init_command;

      boost::filesystem::path tmp() const;

      boost::filesystem::path cwd() const;
      boost::filesystem::path include_dir(data::include_argument_type) const;

      result check(std::vector<data::include_argument_type>,
                   test_filename,
                   data::include_type,
                   data::standard_headers_allowed);
    };

    bool operator==(const include_test_env::result&,
                    const include_test_env::result&);

    std::string to_string(const include_test_env::result& res_);

    std::ostream& operator<<(std::ostream& out_,
                             const include_test_env::result& res_);

    boost::filesystem::path to_path(include_test_env::test_filename);
  }
}

#endif
