#ifndef METASHELL_ENGINE_WAVE_CONTEXT_HPP
#define METASHELL_ENGINE_WAVE_CONTEXT_HPP

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

#include <metashell/engine/wave/hooks.hpp>

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      typedef boost::wave::context<
          std::string::const_iterator,
          boost::wave::cpplexer::lex_iterator<core::wave_token>,
          boost::wave::iteration_context_policies::load_file_to_string,
          hooks>
          context;

      void apply(context& ctx_,
                 const data::wave_config& cfg_,
                 const std::vector<boost::filesystem::path>& system_includes_);

      void preprocess(context& ctx_);

      template <class TokenIterator>
      bool display_step(std::ostream& out_,
                        TokenIterator& begin_,
                        const TokenIterator& end_,
                        bool ignore_macro_redefinition_)
      {
        try
        {
          if (begin_ == end_)
          {
            return false;
          }
          else
          {
            out_ << begin_->get_value();
            ++begin_;
          }
        }
        catch (const boost::wave::preprocess_exception& e_)
        {
          typedef boost::wave::preprocess_exception::error_code error_code;

          const auto ec = e_.get_errorcode();
          if (ec != error_code::last_line_not_terminated &&
              (!ignore_macro_redefinition_ ||
               ec != error_code::macro_redefinition))
          {
            throw;
          }
        }
        return true;
      }

      template <class Tokens>
      void display(std::ostream& out_,
                   Tokens& tokens_,
                   bool ignore_macro_redefinition_)
      {
        const auto e = tokens_.end();
        auto i = tokens_.begin();
        while (display_step(out_, i, e, ignore_macro_redefinition_))
          ;
      }

      std::string to_string(const boost::wave::cpp_exception& error_);
    }
  }
}

#endif
