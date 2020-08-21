// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/code_complete.hpp>
#include <metashell/core/include_path_cache.hpp>
#include <metashell/core/util.hpp>

#include <metashell/data/file_completion_entry.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

#include <algorithm>
#include <cassert>
#include <map>
#include <vector>

namespace metashell
{
  namespace core
  {
    namespace code_complete
    {
      namespace
      {
        bool keep_all(const boost::filesystem::path&) { return true; }

        data::user_input join_with_space(
            std::vector<data::identifier>::const_iterator name_begin_,
            std::vector<data::identifier>::const_iterator name_end_)
        {
          data::user_input result;
          bool first = true;
          for (; name_begin_ != name_end_; ++name_begin_)
          {
            if (first)
            {
              first = false;
            }
            else
            {
              result += data::user_input{" "};
            }
            result += data::user_input{name_begin_->value()};
          }
          return result;
        }

        bool ends_with_sep(const data::user_input& s_)
        {
          return !s_.empty() &&
                 (s_.value().back() == '/' || s_.value().back() == '\\');
        }

        std::set<data::pragma_name>
        real_prefixes(const std::set<data::pragma_name>& names_)
        {
          std::set<data::pragma_name> result;

          for (const data::pragma_name& name : names_)
          {
            data::pragma_name prefix;
            for (const auto& element : name.tokens())
            {
              result.insert(prefix);
              prefix.push_back(element);
            }
          }

          return result;
        }

        void complete_files(
            const boost::filesystem::path& base_path_,
            const data::user_input& to_complete_,
            const data::user_input& prefix_,
            const std::function<bool(const boost::filesystem::path&)>& filter_,
            std::map<data::user_input,
                     std::vector<data::file_completion_entry>>& out_)
        {
          boost::system::error_code ignore_error;

          const boost::filesystem::directory_iterator end;

          const boost::filesystem::path path_to_complete{
              ends_with_sep(to_complete_) ?
                  to_complete_.value().substr(0, to_complete_.size() - 1) :
                  to_complete_.value()};

          const boost::filesystem::path parent_path =
              path_to_complete.parent_path();
          const std::string filename = path_to_complete.filename().string();

          const boost::filesystem::path path =
              resolve_symlink(base_path_ / to_complete_.value());

          const bool path_is_directory = is_directory(path);

          if (path_is_directory)
          {
            for (boost::filesystem::directory_iterator i{path, ignore_error};
                 i != end; ++i)
            {
              const auto real_path = resolve_symlink(i->path());
              if (is_directory(real_path) || filter_(i->path()))
              {
                const data::user_input comp{
                    (to_complete_.empty() || ends_with_sep(to_complete_) ?
                         "" :
                         "/") +
                    i->path().filename().string()};
                out_[prefix_ + comp].push_back(
                    data::file_completion_entry{base_path_, real_path, comp});
              }
            }
          }

          if (!ends_with_sep(to_complete_))
          {
            for (boost::filesystem::directory_iterator i{
                     resolve_symlink(base_path_ / parent_path), ignore_error};
                 i != end; ++i)
            {
              const std::string fn = i->path().filename().string();
              const auto real_path = resolve_symlink(i->path());
              if (boost::algorithm::starts_with(fn, filename) &&
                  (!path_is_directory || fn != filename) &&
                  (is_directory(real_path) || filter_(i->path())))
              {
                const data::user_input comp{fn.substr(filename.size())};
                out_[prefix_ + comp].push_back(
                    data::file_completion_entry{base_path_, real_path, comp});
              }
            }
          }
        }

        data::code_completion post_process(
            const data::user_input& to_complete_,
            std::map<data::user_input, std::vector<data::file_completion_entry>>
                result_,
            const data::user_input& suffix_,
            const std::function<bool(const boost::filesystem::path&)>& filter_)
        {
          while (result_.size() == 1 &&
                 std::any_of(result_.begin()->second.begin(),
                             result_.begin()->second.end(),
                             [](const data::file_completion_entry& e_) {
                               return e_.is_directory();
                             }))
          {
            std::map<data::user_input, std::vector<data::file_completion_entry>>
                new_result;
            for (const data::file_completion_entry& e : result_.begin()->second)
            {
              if (e.is_directory())
              {
                complete_files(e.base_path(),
                               to_complete_ + e.completion_no_prefix() +
                                   data::user_input{"/"},
                               result_.begin()->first + data::user_input{"/"},
                               filter_, new_result);
              }
              else
              {
                new_result[result_.begin()->first].push_back(e);
              }
            }

            if (new_result.empty())
            {
              break;
            }

            result_.swap(new_result);
          }

          data::code_completion result;
          for (auto& p : result_)
          {
            for (const data::file_completion_entry& e : p.second)
            {
              result.insert(p.first + (e.is_directory() ?
                                           data::user_input{"/"} :
                                           suffix_));
            }
          }
          return result;
        }

        data::code_completion
        files(const data::user_input& to_complete_,
              const data::user_input& prefix_,
              const data::user_input& suffix_,
              const std::vector<boost::filesystem::path>& bases_)
        {
          std::map<data::user_input, std::vector<data::file_completion_entry>>
              out;
          for (const boost::filesystem::path& base_path : bases_)
          {
            complete_files(base_path, to_complete_, prefix_, keep_all, out);
          }
          return post_process(to_complete_, std::move(out), suffix_, keep_all);
        }
      }

      namespace impl
      {
        data::user_input to_user_input(std::string value_)
        {
          return data::user_input{std::move(value_)};
        }
      }

      data::code_completion
      files(const data::user_input& to_complete_,
            const data::user_input& prefix_,
            const std::function<bool(const boost::filesystem::path&)>& filter_)
      {
        std::map<data::user_input, std::vector<data::file_completion_entry>>
            out;
        complete_files(boost::filesystem::current_path(), to_complete_, prefix_,
                       filter_, out);
        return post_process(
            to_complete_, std::move(out), data::user_input{}, filter_);
      }

      data::code_completion
      include(data::command::const_iterator begin_,
              data::command::const_iterator end_,
              iface::header_discoverer& header_discoverer_)
      {
        if (begin_ == end_)
        {
          return data::code_completion{
              data::user_input{" <"}, data::user_input{" \""}};
        }

        core::include_path_cache paths{header_discoverer_};

        const boost::filesystem::directory_iterator end;

        const auto start = data::skip_all_whitespace(begin_, end_);

        if (start == end_)
        {
          return data::code_completion{
              data::user_input{"<"}, data::user_input{"\""}};
        }
        else if (type_of(*start) == data::token_type::operator_less &&
                 std::find_if(start + 1, end_, [](const data::token& token_) {
                   return type_of(token_) == data::token_type::operator_greater;
                 }) == end_)
        {
          return files(data::user_input{tokens_to_string(start + 1, end_)},
                       data::user_input{}, data::user_input{">"},
                       paths[{data::include_type::sys,
                              data::standard_headers_allowed::all}]);
        }
        else if (data::include_quote_token(*start) &&
                 std::find_if(start + 1, end_, data::include_quote_token) ==
                     end_)
        {
          return files(data::user_input{tokens_to_string(start + 1, end_)},
                       data::user_input{}, data::user_input{"\""},
                       paths[{data::include_type::quote,
                              data::standard_headers_allowed::all}]);
        }
        else
        {
          return data::code_completion{};
        }
      }

      std::pair<data::code_completion,
                std::vector<std::pair<iface::pragma_handler*,
                                      data::command::const_iterator>>>
      pragma_metashell(
          data::command::const_iterator begin_,
          data::command::const_iterator end_,
          const std::map<data::pragma_name,
                         std::unique_ptr<iface::pragma_handler>>& handlers_)
      {
        std::set<data::pragma_name> possible_commands;
        for (const auto& cmd : handlers_)
        {
          possible_commands.insert(cmd.first);
        }

        std::set<data::pragma_name> prefixes = real_prefixes(possible_commands);

        std::pair<data::code_completion,
                  std::vector<std::pair<iface::pragma_handler*,
                                        data::command::const_iterator>>>
            result;

        if (begin_ == end_)
        {
          for (const auto& handler : handlers_)
          {
            result.first.insert(
                data::user_input{" "} +
                join_with_space(handler.first.tokens().begin(),
                                handler.first.tokens().end()) +
                data::user_input{
                    prefixes.find(handler.first) == prefixes.end() ? "" : " "});
          }
          return result;
        }

        data::token last_token = begin_ == end_ ? data::token{} : *begin_;
        size_t prefix_len = 0;
        for (; begin_ != end_; ++begin_)
        {
          auto next = begin_;
          ++next;

          if (!whitespace_or_comment(category(*begin_)))
          {
            for (auto c = possible_commands.begin();
                 c != possible_commands.end();)
            {
              const auto& token_in_cmd = c->tokens()[prefix_len];
              const auto& token_in_code_to_complete = *begin_;

              const bool cmd_fully_present_in_code_to_complete =
                  prefix_len == c->tokens().size() - 1 &&
                  value(token_in_cmd) == value(*begin_);

              if (cmd_fully_present_in_code_to_complete)
              {
                const auto& handler = handlers_.find(*c);
                assert(handler != handlers_.end());
                result.second.push_back({handler->second.get(), next});
                c = possible_commands.erase(c);
              }
              else if (next == end_)
              {
                const data::cpp_code code_in_cmd = value(token_in_cmd);
                const data::cpp_code code_in_code_to_complete =
                    value(token_in_code_to_complete);

                if (starts_with(code_in_cmd, code_in_code_to_complete))
                {
                  result.first.insert(
                      data::user_input{
                          code_in_cmd.substr(code_in_code_to_complete.size()) +
                          data::cpp_code{
                              prefix_len < c->tokens().size() - 1 ? " " : ""}} +
                      join_with_space(c->tokens().begin() + prefix_len + 1,
                                      c->tokens().end()) +
                      data::user_input{
                          prefixes.find(*c) == prefixes.end() ? "" : " "});
                }
                c = possible_commands.erase(c);
              }
              else if (token_in_cmd == token_in_code_to_complete)
              {
                ++c;
              }
              else
              {
                c = possible_commands.erase(c);
              }
            }
            ++prefix_len;
          }
          last_token = *begin_;
        }

        for (const data::pragma_name& name : possible_commands)
        {
          assert(prefix_len < name.tokens().size());

          result.first.insert(
              join_with_space(
                  name.tokens().begin() + prefix_len, name.tokens().end()) +
              data::user_input{prefixes.find(name) == prefixes.end() ? "" :
                                                                       " "});
        }

        return result;
      }
    }
  }
}
