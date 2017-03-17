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

#include <metashell/metaprogram_builder.hpp>
#include <metashell/preprocessor_tracer_wave.hpp>
#include <metashell/wave_context.hpp>

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

namespace
{
  metashell::data::cpp_code
  determine_input(metashell::data::cpp_code env_,
                  const boost::optional<metashell::data::cpp_code>& exp_)
  {
    if (exp_)
    {
      return env_ + "\n" + add_markers(*exp_, true) + "\n";
    }
    else
    {
      return env_;
    }
  }
}

namespace metashell
{
  preprocessor_tracer_wave::preprocessor_tracer_wave(data::wave_config config_)
    : _config(std::move(config_))
  {
  }

  data::metaprogram
  preprocessor_tracer_wave::eval(iface::environment& env_,
                                 const boost::optional<data::cpp_code>& exp_)
  {
    const data::cpp_code input = determine_input(env_.get_all(), exp_);

    metaprogram_builder builder(data::metaprogram::mode_t::full,
                                exp_ ? *exp_ : data::cpp_code("<environment>"),
                                data::file_location());

    try
    {
      wave_context ctx(input.begin(), input.end(), "<input>");
      apply(ctx, _config);

      std::ostringstream s;
      display(s, ctx, _config.ignore_macro_redefinition);
      const data::cpp_code output_code(s.str());
      builder.handle_evaluation_end(exp_ ? remove_markers(output_code, true) :
                                           output_code);
    }
    catch (const boost::wave::cpp_exception& error_)
    {
      builder.handle_evaluation_end(
          data::type_or_code_or_error::make_error(to_string(error_)));
    }
    catch (const std::exception& error_)
    {
      builder.handle_evaluation_end(
          data::type_or_code_or_error::make_error(error_.what()));
    }

    return builder.get_metaprogram();
  }
}
