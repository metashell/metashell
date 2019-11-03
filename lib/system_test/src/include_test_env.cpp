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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/include_test_env.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <metashell/data/unsupported_standard_headers_allowed.hpp>

#include <just/file.hpp>

#include <boost/filesystem.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

using pattern::_;

namespace metashell
{
  namespace system_test
  {
    namespace
    {
      std::string to_string_arg(data::include_argument_type arg_)
      {
        if (using_msvc() && arg_ == data::include_argument_type::capital_i)
        {
          return "/I";
        }
        else if (using_wave() && arg_ == data::include_argument_type::isystem)
        {
          return "-S";
        }
        else
        {
          return to_string(arg_);
        }
      }

      std::string encode(data::include_argument_type arg_)
      {
        // An extra character (eg. X) is needed to avoid confusion about having
        // "-I" in the path of the include files.
        return "X" + to_string(arg_) + "X";
      }
    }

    /*
     * include_test_env::test_filename
     */

    boost::filesystem::path to_path(include_test_env::test_filename fn_)
    {
      switch (fn_)
      {
      case include_test_env::test_filename::nonstandard_header:
        return "test.hpp";
      case include_test_env::test_filename::standard_c_header:
        return "stdlib.h";
      case include_test_env::test_filename::standard_cpp_header:
        return "vector";
      }
      assert(!"Invalid test filename");
      return boost::filesystem::path();
    }

    /*
     * include_test_env::result
     */

    include_test_env::result::result(data::include_argument_type value_)
      : _value(value_)
    {
    }

    include_test_env::result::result(none_t value_) : _value(value_) {}

    include_test_env::result::result(system_used_t value_) : _value(value_) {}

    const mpark::variant<include_test_env::none_t,
                         include_test_env::system_used_t,
                         data::include_argument_type>&
    include_test_env::result::value() const
    {
      return _value;
    }

    include_test_env::result include_test_env::result::none()
    {
      return result(none_t());
    }

    include_test_env::result include_test_env::result::system_used()
    {
      return result(system_used_t());
    }

    bool operator==(const include_test_env::result& lhs_,
                    const include_test_env::result& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    std::string to_string(const include_test_env::result& res_)
    {
      return mpark::visit([](auto v) { return to_string(v); }, res_.value());
    }

    std::ostream& operator<<(std::ostream& out_,
                             const include_test_env::result& res_)
    {
      return out_ << to_string(res_);
    }

    /*
     * include_test_env
     */

    include_test_env::include_test_env()
    {
      create_directories(cwd());

      for (data::include_argument_type arg : data::all_include_argument_types())
      {
        const boost::filesystem::path dir = include_dir(arg);
        create_directories(dir);
        for (test_filename filename : all_test_filenames())
        {
          just::file::write((dir / to_path(filename)).string(), encode(arg));
        }
      }
    }

    std::array<include_test_env::test_filename, 3>
    include_test_env::all_test_filenames()
    {
      return {{test_filename::nonstandard_header,
               test_filename::standard_c_header,
               test_filename::standard_cpp_header}};
    }

    boost::filesystem::path
    include_test_env::include_dir(data::include_argument_type arg_) const
    {
      return tmp() / encode(arg_);
    }

    boost::filesystem::path include_test_env::tmp() const
    {
      return boost::filesystem::path(_tmp_dir.path());
    }

    boost::filesystem::path include_test_env::cwd() const
    {
      return tmp() / "cwd";
    }

    include_test_env::result
    include_test_env::check(std::vector<data::include_argument_type> args_,
                            test_filename filename_,
                            data::include_type include_type_,
                            data::standard_headers_allowed std_headers_)
    {
      data::command_line_argument_list args{"--"};

      switch (std_headers_)
      {
      case data::standard_headers_allowed::none:
        if (using_msvc())
        {
          args.push_back("/X");
        }
        else if (using_wave())
        {
          args.push_back("--nostdinc++");
        }
        else
        {
          args.push_back("-nostdinc++");
          args.push_back("-nostdinc");
        }
        break;
      case data::standard_headers_allowed::c:
      case data::standard_headers_allowed::cpp:
        if (using_msvc())
        {
          throw data::unsupported_standard_headers_allowed(
              data::real_engine_name::msvc, std_headers_);
        }
        else if (using_wave())
        {
          throw data::unsupported_standard_headers_allowed(
              data::real_engine_name::wave, std_headers_);
        }
        else if (using_internal())
        {
          throw data::unsupported_standard_headers_allowed(
              data::real_engine_name::internal, std_headers_);
        }
        else
        {
          args.push_back(std_headers_ == data::standard_headers_allowed::c ?
                             "-nostdinc++" :
                             "-nostdinc");
        }
        break;
      case data::standard_headers_allowed::all:
        break;
      }

      for (data::include_argument_type arg : args_)
      {
        args.push_back(to_string_arg(arg) + include_dir(arg).string());
      }

      metashell_instance mi(args, cwd(), true, true);

      mi.command("#msh preprocessor mode");

      const std::vector<json_string> out = mi.command(
          "#include " + include_code(include_type_, to_path(filename_)));

      if (!out.empty() && error(_) != out.front())
      {
        const auto i = std::find_if(
            args_.begin(), args_.end(),
            [&out](data::include_argument_type arg_) {
              return out.front().get().find(encode(arg_)) != std::string::npos;
            });

        if (i == args_.end())
        {
          // Assuming there are only the system headers available beyond the
          // auto-generated ones
          return result::system_used();
        }
        else
        {
          return *i;
        }
      }

      return result::none();
    }

    include_test_env::result include_test_env::include_dir_used(
        std::set<data::include_argument_type> args_,
        test_filename filename_,
        data::include_type include_type_,
        data::standard_headers_allowed std_headers_)
    {
      std::vector<data::include_argument_type> args(args_.begin(), args_.end());

      const include_test_env::result original =
          check(args, filename_, include_type_, std_headers_);

      if (args.size() > 1)
      {
        std::reverse(args.begin(), args.end());

        const include_test_env::result reversed =
            check(args, filename_, include_type_, std_headers_);
        if (original != reversed)
        {
          throw std::runtime_error(
              "Include result of " +
              include_code(include_type_, to_path(filename_)) +
              " depends on argument order. Arguments: " +
              boost::algorithm::join(
                  args | boost::adaptors::transformed(
                             [](auto arg) { return to_string(arg); }),
                  " ") +
              ". Original result: " + to_string(original) +
              ". Reversed result: " + to_string(reversed) +
              ". Standard headers allowed: " + to_string(std_headers_));
        }
      }

      return original;
    }
  }
}
