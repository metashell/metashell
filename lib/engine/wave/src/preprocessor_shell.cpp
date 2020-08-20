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

#include <metashell/engine/wave/context.hpp>
#include <metashell/engine/wave/preprocessor_shell.hpp>

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
  namespace engine
  {
    namespace wave
    {
      preprocessor_shell::preprocessor_shell(
          data::wave_config config_,
          std::vector<boost::filesystem::path> system_includes_)
        : _config(std::move(config_)),
          _system_includes(std::move(system_includes_))
      {
      }

      data::result preprocessor_shell::precompile(const data::cpp_code& exp_)
      {
        try
        {
          context ctx(exp_.begin(), exp_.end(), "<stdin>");
          wave::apply(ctx, _config, _system_includes);

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
  }
}
