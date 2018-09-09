// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/preprocessor_shell_wave.hpp>
#include <metashell/core/wave_context.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/grammars/cpp_chlit_grammar.hpp>
#include <boost/wave/grammars/cpp_defined_grammar.hpp>
#include <boost/wave/grammars/cpp_expression_grammar.hpp>
#include <boost/wave/grammars/cpp_grammar.hpp>
#include <boost/wave/grammars/cpp_intlit_grammar.hpp>
#include <boost/wave/grammars/cpp_predef_macros_grammar.hpp>

#include <sstream>
#include <stdexcept>

namespace metashell
{
  preprocessor_shell_wave::preprocessor_shell_wave(data::wave_config config_)
    : _config(std::move(config_))
  {
  }

  data::result preprocessor_shell_wave::precompile(const data::cpp_code& exp_)
  {
    try
    {
      wave_context ctx(exp_.begin(), exp_.end(), "<stdin>");
      apply(ctx, _config);

      std::ostringstream s;
      display(s, ctx, _config.ignore_macro_redefinition);
      return data::result{true, s.str(), "", ""};
    }
    catch (const boost::wave::cpp_exception& error_)
    {
      return data::result{false, "", to_string(error_), ""};
    }
    catch (const std::exception& error_)
    {
      return data::result{false, "", error_.what(), ""};
    }
  }
}
