#ifndef METASHELL_WAVE_CONTEXT_HPP
#define METASHELL_WAVE_CONTEXT_HPP

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

#include <metashell/data/wave_config.hpp>
#include <metashell/wave_hooks.hpp>

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  typedef boost::wave::context<
      std::string::const_iterator,
      boost::wave::cpplexer::lex_iterator<wave_token>,
      boost::wave::iteration_context_policies::load_file_to_string,
      wave_hooks>
      wave_context;

  void apply(wave_context& ctx_, const data::wave_config& cfg_);

  void preprocess(wave_context& ctx_);

  template <class Tokens>
  void
  display(std::ostream& out_, Tokens& tokens_, bool ignore_macro_redefinition_)
  {
    const auto e = tokens_.end();
    auto i = tokens_.begin();
    for (bool iterate = true; iterate;)
    {
      try
      {
        if (i == e)
        {
          iterate = false;
        }
        else
        {
          out_ << i->get_value();
          ++i;
        }
      }
      catch (const boost::wave::preprocess_exception& e_)
      {
        if (!ignore_macro_redefinition_ ||
            e_.get_errorcode() != boost::wave::preprocess_exception::
                                      error_code::macro_redefinition)
        {
          throw;
        }
      }
    }
  }

  std::string to_string(const boost::wave::cpp_exception& error_);
}

#endif
