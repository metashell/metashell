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
    namespace
    {
      void append(std::vector<boost::filesystem::path>& out_,
                  const std::vector<boost::filesystem::path>& extension_)
      {
        out_.insert(out_.end(), extension_.begin(), extension_.end());
      }
    }

    wave_arg_parser::wave_arg_parser(bool use_templight_headers_)
    {
      _config.standard = use_templight_headers_ ?
                             boost::make_optional(wave_standard::cpp11) :
                             boost::none;
      _config.ignore_macro_redefinition = use_templight_headers_;

      // clang-format off
      _parser
        .with_value(
          "-I", "--include",
          "specify an additional include directory",
          _capital_i
        )
        .with_value(
          "-S", "--sysinclude",
          "specify an additional system include directory",
          _capital_s
        )
        .with_value(
          "-D", "--define",
          "specify a macro to define (as `macro[=[value]]`)",
          _config.macros
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
            this->_config.standard = wave_standard::c99;
            this->_config.long_long = true;
            this->_standards.emplace_back("c99");
          }
        )
        .flag(
          "--c++11",
          "enable C++11 mode (implies `--variadics` and `--long_long`)",
          [this] {
            this->_config.standard = wave_standard::cpp11;
            this->_config.long_long = true;
            this->_standards.emplace_back("c++11");
          }
        )
        .flag(
          "--nostdinc++",
          use_templight_headers_ ?
            "don't add standard headers to the include path" :
            "ignored (accepted to be compatible with the `wave` engine)",
          [this] { this->_use_stdincpp = false; }
        )
      ;
      // clang-format on
    }

    void wave_arg_parser::parse(const command_line_argument_list& args_,
                                includes sysincludes_,
                                std::vector<std::string> sysmacros_)
    {
      _config.macros = std::move(sysmacros_);

      _parser.parse(args_);

      if (_standards.size() > 1)
      {
        throw exception("Multiple standards (" +
                        join(_standards, command_line_argument(", ")) +
                        ") specified");
      }

      if (_use_stdincpp)
      {
        _config.includes = std::move(sysincludes_);
      }
      else if (std::find(sysincludes_.quote.begin(), sysincludes_.quote.end(),
                         ".") != sysincludes_.quote.end())
      {
        _config.includes.quote.emplace_back(".");
      }

      append(_config.includes.quote, _capital_i);
      append(_config.includes.sys, _capital_i);
      append(_config.includes.sys, _capital_s);
    }

    const wave_config& wave_arg_parser::result() const { return _config; }

    std::string wave_arg_parser::description() const
    {
      return "Wave options:<br />" + _parser.description();
    }
  }
}
