// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/clang/code_completer.hpp>

#include <metashell/core/command.hpp>
#include <metashell/core/for_each_line.hpp>
#include <metashell/core/source_position.hpp>
#include <metashell/core/unsaved_file.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/token_category.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <optional>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        std::optional<std::string> remove_prefix(const std::string& s_,
                                                 const std::string& prefix_)
        {
          if (boost::starts_with(s_, prefix_))
          {
            return std::string(s_.begin() + prefix_.length(), s_.end());
          }
          else
          {
            return std::nullopt;
          }
        }

        std::optional<data::cpp_code> parse_completion(std::string line_)
        {
          if (const auto without_completion =
                  remove_prefix(line_, "COMPLETION: "))
          {
            if (const auto pattern =
                    remove_prefix(*without_completion, "Pattern : "))
            {
              const auto prefix_end = pattern->find("<#");
              return data::cpp_code(
                  pattern->begin(), prefix_end == std::string::npos ?
                                        pattern->end() :
                                        pattern->begin() + prefix_end);
            }
            else
            {
              return data::cpp_code(without_completion->begin(),
                                    std::find(without_completion->begin(),
                                              without_completion->end(), ' '));
            }
          }
          else
          {
            return std::nullopt;
          }
        }

        std::pair<data::cpp_code, data::cpp_code>
        find_completion_start(const data::cpp_code& s_)
        {
          const data::command cmd = core::to_command(s_);

          if (cmd.empty())
          {
            return {data::cpp_code(), data::cpp_code()};
          }
          else
          {
            using boost::adaptors::sliced;
            using boost::adaptors::transformed;

            const data::cpp_code prefix =
                join(cmd | sliced(0, cmd.size() - 1) |
                         transformed(
                             [](const data::token& t_) { return value(t_); }),
                     data::cpp_code());

            const data::token& last = *(cmd.end() - 1);

            if (category(last) == data::token_category::identifier ||
                category(last) == data::token_category::keyword)
            {
              return {prefix, value(last)};
            }
            else
            {
              return {prefix + value(last), data::cpp_code()};
            }
          }
        }
      }

      code_completer::code_completer(
          const boost::filesystem::path& internal_dir_,
          const boost::filesystem::path& temp_dir_,
          const boost::filesystem::path& env_filename_,
          binary binary_,
          core::logger* logger_)
        : _binary(binary_),
          _temp_dir(temp_dir_),
          _env_path(internal_dir_ / env_filename_),
          _logger(logger_)
      {
      }

      data::code_completion
      code_completer::code_complete(const iface::environment& env_,
                                    const data::user_input& src_,
                                    bool use_precompiled_headers_)
      {
        using boost::starts_with;

        METASHELL_LOG(_logger, "Code completion of " + src_.value());

        const std::pair<data::cpp_code, data::cpp_code> completion_start =
            find_completion_start(data::cpp_code(src_));

        METASHELL_LOG(_logger, "Part kept for code completion: " +
                                   completion_start.first.value());

        const data::unsaved_file src(
            _temp_dir / "code_complete.cpp",
            env_.get_appended(completion_start.first).value());

        core::generate(src);

        const core::source_position sp =
            core::source_position_of(src.content());

        data::command_line_argument_list clang_args{
            "-fsyntax-only", "-Xclang",
            "-code-completion-at=" + src.filename().string() + ":" +
                to_string(sp),
            src.filename().string()};

        if (use_precompiled_headers_)
        {
          clang_args.push_back("-include", _env_path);
        }

        const data::process_output o = _binary.run(clang_args, "");

        METASHELL_LOG(_logger, "Exit code of clang: " + to_string(o.status));

        const std::string out = o.standard_output;
        const auto prefix_len = completion_start.second.size();
        data::code_completion result;
        core::for_each_line(out, [&result, &completion_start,
                                  prefix_len](const std::string& line_) {
          if (const auto comp = parse_completion(line_))
          {
            if (starts_with(*comp, completion_start.second) &&
                *comp != completion_start.second)
            {
              result.insert(data::user_input{comp->substr(prefix_len)});
            }
          }
        });
        return result;
      }
    }
  }
}
