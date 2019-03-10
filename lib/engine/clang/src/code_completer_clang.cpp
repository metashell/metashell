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

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        boost::optional<std::string> remove_prefix(const std::string& s_,
                                                   const std::string& prefix_)
        {
          if (boost::starts_with(s_, prefix_))
          {
            return std::string(s_.begin() + prefix_.length(), s_.end());
          }
          else
          {
            return boost::none;
          }
        }

        boost::optional<std::string> parse_completion(std::string line_)
        {
          if (const auto without_completion =
                  remove_prefix(line_, "COMPLETION: "))
          {
            if (const auto pattern =
                    remove_prefix(*without_completion, "Pattern : "))
            {
              const auto prefix_end = pattern->find("<#");
              return std::string(
                  pattern->begin(), prefix_end == std::string::npos ?
                                        pattern->end() :
                                        pattern->begin() + prefix_end);
            }
            else
            {
              return std::string(without_completion->begin(),
                                 std::find(without_completion->begin(),
                                           without_completion->end(), ' '));
            }
          }
          else
          {
            return boost::none;
          }
        }

        std::pair<std::string, std::string>
        find_completion_start(const std::string& s_)
        {
          const data::command cmd = core::to_command(data::cpp_code(s_));

          if (cmd.empty())
          {
            return {"", ""};
          }
          else
          {
            using boost::adaptors::sliced;
            using boost::adaptors::transformed;

            const std::string prefix = boost::algorithm::join(
                cmd | sliced(0, cmd.size() - 1) |
                    transformed([](const data::token& t_) {
                      return value(t_).value();
                    }),
                "");

            const data::token& last = *(cmd.end() - 1);

            if (category(last) == data::token_category::identifier ||
                category(last) == data::token_category::keyword)
            {
              return {prefix, value(last).value()};
            }
            else
            {
              return {prefix + value(last).value(), ""};
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

      void code_completer::code_complete(const iface::environment& env_,
                                         const std::string& src_,
                                         std::set<std::string>& out_,
                                         bool use_precompiled_headers_)
      {
        using boost::starts_with;

        using std::pair;
        using std::string;
        using std::set;

        METASHELL_LOG(_logger, "Code completion of " + src_);

        const pair<string, string> completion_start =
            find_completion_start(src_);

        METASHELL_LOG(_logger, "Part kept for code completion: " +
                                   completion_start.first);

        const data::unsaved_file src(
            _temp_dir / "code_complete.cpp",
            env_.get_appended(data::cpp_code(completion_start.first)).value());

        core::generate(src);

        const core::source_position sp =
            core::source_position_of(src.content());

        std::vector<std::string> clang_args{"-fsyntax-only", "-Xclang",
                                            "-code-completion-at=" +
                                                src.filename().string() + ":" +
                                                to_string(sp),
                                            src.filename().string()};

        if (use_precompiled_headers_)
        {
          clang_args.push_back("-include");
          clang_args.push_back(_env_path.string());
        }

        const data::process_output o = _binary.run(clang_args, "");

        METASHELL_LOG(_logger, "Exit code of clang: " + to_string(o.exit_code));

        const std::string out = o.standard_output;
        out_.clear();
        const int prefix_len = completion_start.second.length();
        core::for_each_line(out, [&out_, &completion_start,
                                  prefix_len](const std::string& line_) {
          if (const boost::optional<std::string> comp = parse_completion(line_))
          {
            if (starts_with(*comp, completion_start.second) &&
                *comp != completion_start.second)
            {
              out_.insert(string(comp->begin() + prefix_len, comp->end()));
            }
          }
        });
      }
    }
  }
}
