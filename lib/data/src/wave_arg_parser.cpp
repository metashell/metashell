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

#include <metashell/data/wave_arg_parser.hpp>

namespace metashell
{
  namespace data
  {
    wave_arg_parser::wave_arg_parser(bool use_templight_headers_)
    {
      _config.config.standard = use_templight_headers_ ?
                                    language_standard::cpp11 :
                                    language_standard::cpp98;
      _config.ignore_macro_redefinition = use_templight_headers_;
      if (use_templight_headers_)
      {
        _config.config.includes.iquote.emplace_back(".");
      }

      // clang-format off
      _parser
        .with_value(
          "-I", "--include",
          "specify an additional include directory",
          _includes.capital_i
        )
        .with_value(
          "-S", "--sysinclude",
          "specify an additional system include directory",
          _includes.isystem
        )
        .with_value(
          "-iquote",
          "specify an additional quote include directory",
          _includes.iquote
        )
        .with_value(
          "-idirafter",
          "specify an additional include directory to be checked after system headers",
          _includes.idirafter
        )
        .with_value(
          "-isysroot",
          "Specify the root of the includes",
          [this](const command_line_argument& path_)
          {
            this->_includes.isysroot = path_.value();
          }
        )
        .with_value(
          "--sysroot",
          "Specify the root of the includes",
          [this](const command_line_argument& path_)
          {
            if (!this->_includes.isysroot)
            {
              this->_includes.isysroot = path_.value();
            }
          }
        )
        .with_value(
          "-D", "--define",
          "specify a macro to define (as `macro[=[value]]`)",
          [this](const command_line_argument& def_)
          { this->_config.config.macros.push_back(data::macro_definition(def_)); }
        )
        .with_value(
          "-U",
          "specify a macro to undefine",
          [this](const command_line_argument& name_)
          { this->_config.config.macros.push_back(data::macro_undefinition(name_)); }
        )
        .flag(
          "--long_long",
          "enable long long support in C++ mode",
          [this] { this->_config.long_long = true; }
        )
        .flag(
          "--variadics",
          "enable certain C99 extensions in C++ mode",
          [this] { this->_config.variadics = true; }
        )
        .flag(
          "--c99",
          "enable C99 mode (implies `--variadics`)",
          [this] {
            this->_config.config.standard = language_standard::c99;
            this->_config.long_long = true;
            this->_standards.emplace_back("c99");
          }
        )
        .flag(
          "--c++11",
          "enable C++11 mode (implies `--variadics` and `--long_long`)",
          [this] {
            this->_config.config.standard = language_standard::cpp11;
            this->_config.long_long = true;
            this->_standards.emplace_back("c++11");
          }
        )
        .flag(
          "--nostdinc++",
          use_templight_headers_ ?
            "don't add standard headers to the include path" :
            "ignored (accepted to be compatible with the `wave` engine)",
          [this] { this->_config.config.use_standard_headers = data::standard_headers_allowed::none; }
        )
      ;
      // clang-format on
    }

    void wave_arg_parser::parse(const command_line_argument_list& args_,
                                std::vector<macro_definition> sysmacros_)
    {
      for (macro_definition& m : sysmacros_)
      {
        _config.config.macros.push_back(std::move(m));
      }

      _parser.parse(args_);

      if (_standards.size() > 1)
      {
        throw exception("Multiple standards (" +
                        join(_standards, command_line_argument(", ")) +
                        ") specified");
      }

      _config.config.includes += _includes;
    }

    const wave_config& wave_arg_parser::result() const { return _config; }

    std::string wave_arg_parser::description() const
    {
      return "Wave options:<br />" + _parser.description();
    }
  }
}
