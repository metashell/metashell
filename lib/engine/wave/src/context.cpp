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

#include <sstream>
#include <stdexcept>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      namespace
      {
        std::optional<boost::filesystem::path>
        canonical_path(const boost::filesystem::path& path)
        {
          boost::system::error_code ec;
          auto res = boost::filesystem::canonical(path, ec);
          if (ec.value() == boost::system::errc::success)
          {
            return res;
          }
          return std::nullopt;
        }

        void apply(context& ctx_,
                   const data::include_config& includes_,
                   const std::vector<boost::filesystem::path>& system_includes_)
        {
          for (const boost::filesystem::path& p :
               includes_.get(data::include_type::sys, system_includes_))
          {
            if (auto cp = canonical_path(p))
            {
              ctx_.add_sysinclude_path(cp->string().c_str());
            }
          }
          for (const boost::filesystem::path& p :
               includes_.get(data::include_type::quote, system_includes_))
          {
            if (auto cp = canonical_path(p))
            {
              ctx_.add_include_path(cp->string().c_str());
            }
          }
        }

        boost::wave::language_support
        language_support(data::language_standard standard_)
        {
          switch (standard_)
          {
          case data::language_standard::c89:
          case data::language_standard::gnu89:
          case data::language_standard::c90:
          case data::language_standard::iso9899_199409:
          case data::language_standard::gnu90:
            return boost::wave::language_support(
                boost::wave::support_option_convert_trigraphs |
                boost::wave::support_option_emit_line_directives |
                boost::wave::support_option_include_guard_detection |
                boost::wave::support_option_emit_pragma_directives |
                boost::wave::support_option_insert_whitespace);
          case data::language_standard::c9x:
          case data::language_standard::gnu9x:
          case data::language_standard::c99:
          case data::language_standard::gnu99:
          case data::language_standard::c1x:
          case data::language_standard::gnu1x:
          case data::language_standard::c11:
          case data::language_standard::gnu11:
            return boost::wave::language_support(
                boost::wave::support_c99 |
                boost::wave::support_option_convert_trigraphs |
                boost::wave::support_option_emit_line_directives |
                boost::wave::support_option_include_guard_detection |
                boost::wave::support_option_emit_pragma_directives |
                boost::wave::support_option_insert_whitespace);
          case data::language_standard::cpp98:
          case data::language_standard::gnu98:
          case data::language_standard::cpp03:
          case data::language_standard::gnu03:
            return boost::wave::language_support(
                boost::wave::support_option_convert_trigraphs |
                boost::wave::support_option_emit_line_directives |
                boost::wave::support_option_include_guard_detection |
                boost::wave::support_option_emit_pragma_directives |
                boost::wave::support_option_insert_whitespace);
          case data::language_standard::cpp0x:
          case data::language_standard::gnupp0x:
          case data::language_standard::cpp11:
          case data::language_standard::gnupp11:
          case data::language_standard::cpp1y:
          case data::language_standard::gnupp1y:
          case data::language_standard::cpp14:
          case data::language_standard::gnupp14:
          case data::language_standard::cpp1z:
          case data::language_standard::gnupp1z:
          case data::language_standard::cpp17:
          case data::language_standard::gnupp17:
          case data::language_standard::cpp2a:
          case data::language_standard::gnupp2a:
          case data::language_standard::cpp20:
          case data::language_standard::gnupp20:
            return boost::wave::language_support(
                boost::wave::support_cpp0x |
                boost::wave::support_option_convert_trigraphs |
                boost::wave::support_option_long_long |
                boost::wave::support_option_emit_line_directives |
                boost::wave::support_option_include_guard_detection |
                boost::wave::support_option_emit_pragma_directives |
                boost::wave::support_option_insert_whitespace);
          }
          assert(!"Invalid language standard");
          return boost::wave::language_support();
        }

        boost::wave::language_support
        language_support(const data::wave_config& cfg_)
        {
          boost::wave::language_support result =
              language_support(cfg_.config.standard);
          if (cfg_.long_long)
          {
            result = boost::wave::enable_long_long(result);
          }
          if (cfg_.variadics)
          {
            result = boost::wave::enable_variadics(result);
          }
          return result;
        }
      }

      void apply(context& ctx_, const data::macro_definition& def_)
      {
        ctx_.add_macro_definition(to_string(def_));
      }

      void apply(context& ctx_, const data::macro_undefinition& undef_)
      {
        ctx_.remove_macro_definition(to_string(undef_), true);
      }

      void apply(context& ctx_,
                 const data::wave_config& cfg_,
                 const std::vector<boost::filesystem::path>& system_includes_)
      {
        apply(ctx_, cfg_.config.includes, system_includes_);

        ctx_.set_language(language_support(cfg_));

        for (const auto& macro : cfg_.config.macros)
        {
          mpark::visit([&ctx_](const auto& m_) { apply(ctx_, m_); }, macro);
        }
      }

      std::string to_string(const boost::wave::cpp_exception& error_)
      {
        std::ostringstream result;
        result << error_.file_name() << ":" << error_.line_no() << ":"
               << error_.column_no() << ": " << error_.description();
        return result.str();
      }

      void preprocess(context& ctx_)
      {
        try
        {
          const auto e = ctx_.end();
          for (auto i = ctx_.begin(); i != e; ++i)
            ;
        }
        catch (const boost::wave::cpp_exception& error_)
        {
          throw std::runtime_error(to_string(error_));
        }
      }
    }
  }
}
