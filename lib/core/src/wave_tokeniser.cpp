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

// Disable warnings coming from Boost.Wave
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <metashell/core/wave_token.hpp>
#include <metashell/core/wave_tokeniser.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>

namespace metashell
{
  namespace core
  {
    namespace
    {
      template <char C>
      std::string remove(const std::string& s_)
      {
        std::vector<char> v;
        v.reserve(s_.size());
        std::copy_if(s_.begin(), s_.end(), std::back_inserter(v),
                     [](char c_) { return c_ != C; });
        return std::string(v.begin(), v.end());
      }

      class wave_tokeniser : public iface::tokeniser
      {
      public:
        typedef boost::wave::cpplexer::lex_iterator<
            boost::wave::cpplexer::lex_token<>>
            token_iterator;

        data::cpp_code src;
        std::string input_filename;
        // Wave's operator!= doesn't seem to accept const object as argument
        mutable token_iterator it;
        mutable bool error_flag; // has_further_tokens might set it
        data::token current_tok;

        wave_tokeniser(data::cpp_code src_, std::string input_filename_)
          : src(std::move(src_)),
            input_filename(std::move(input_filename_)),
            it(),
            error_flag(false)
        {
          try
          {
            it = token_iterator(src.begin(), src.end(),
                                token_iterator::value_type::position_type(
                                    input_filename.c_str()),
                                boost::wave::language_support(
                                    boost::wave::support_cpp |
                                    boost::wave::support_option_long_long));
            update_current_token();
          }
          catch (const boost::wave::cpplexer::lexing_exception&)
          {
            error_flag = true;
            it = token_iterator();
          }
        }

        virtual bool has_further_tokens() const override
        {
          try
          {
            if (it != token_iterator())
            {
              const bool b = *it != boost::wave::T_EOF;
              return b;
            }
            else
            {
              return false;
            }
          }
          catch (const boost::wave::cpplexer::lexing_exception&)
          {
            error_flag = true;
            it = token_iterator();
            return false;
          }
        }

        virtual data::token current_token() const override
        {
          return current_tok;
        }

        virtual void move_to_next_token() override
        {
          try
          {
            // has_further_tokens seems to return different results when called
            // twice in some situations for invalid code
            if (has_further_tokens())
            {
              ++it;
              update_current_token();
            }
          }
          catch (const boost::wave::cpplexer::lexing_exception&)
          {
            error_flag = true;
            it = token_iterator();
          }
        }

        virtual bool was_error() const override { return error_flag; }

        void update_current_token()
        {
          if (has_further_tokens())
          {
            current_tok = token_from_wave_token(*it);
          }
        }
      };
    }

    std::unique_ptr<iface::tokeniser>
    create_wave_tokeniser(data::cpp_code src_, std::string input_filename_)
    {
      return std::make_unique<wave_tokeniser>(
          std::move(src_), std::move(input_filename_));
    }

    std::string wave_version()
    {
      return remove<'"'>(
          boost::wave::context<
              const char*,
              boost::wave::cpplexer::lex_iterator<
                  boost::wave::cpplexer::lex_token<>>>::get_version_string());
    }
  }
}
