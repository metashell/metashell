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

#include <metashell/engine/vc/binary.hpp>

#include <metashell/data/exception.hpp>

#include <metashell/process/run.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <just/lines.hpp>

#include <algorithm>
#include <fstream>
#include <optional>
#include <regex>
#include <sstream>

namespace metashell
{
  namespace engine
  {
    namespace vc
    {
      namespace
      {
        void save(const boost::filesystem::path& filename_,
                  const data::cpp_code& s_)
        {
          const std::string fn = filename_.string();
          std::ofstream f(fn.c_str());
          if (!f)
          {
            throw data::exception("Failed to create file " + fn);
          }
          else if (!(f << s_))
          {
            throw data::exception("Failed to write content to file " + fn);
          }
        }

        std::optional<std::string> output_line_of(const std::string& filename_,
                                                  const std::string& line_)
        {
          const std::regex error_report("^\\([0-9]+\\): ");

          for (auto i = line_.find(filename_); i != std::string::npos;
               i = line_.find(filename_, i + 1))
          {
            if (i == 0 || line_[i - 1] == '\\' || line_[i - 1] == '/')
            {
              const std::string after_filename =
                  line_.substr(i + filename_.size());
              std::smatch what;
              if (regex_search(after_filename, what, error_report))
              {
                return what.suffix().str() + "\n";
              }
            }
          }
          return std::nullopt;
        }

        template <class LineIt>
        std::string error_report(LineIt begin_, LineIt end_)
        {
          const std::regex is_filename("([a-zA-Z0-9]:[\\\\/])?[^:]*");

          std::ostringstream o;
          std::string filename;
          for (LineIt i = begin_; i != end_; ++i)
          {
            if (!i->empty())
            {
              if (regex_match(*i, is_filename))
              {
                if (filename.empty())
                {
                  filename = *i;
                }
                else
                {
                  throw data::exception("Multiple filenames (" + filename +
                                        ", " + *i + ") in Visual C++ output.");
                }
              }
              else if (const std::optional<std::string> l =
                           output_line_of(filename, *i))
              {
                o << *l;
              }
              else
              {
                throw data::exception("Unexpected output from Visual C++: " +
                                      *i);
              }
            }
          }
          return o.str();
        }
      }

      binary::binary(data::executable_path cl_path_,
                     data::command_line_argument_list base_args_,
                     boost::filesystem::path cwd_,
                     boost::filesystem::path temp_dir_,
                     core::logger* logger_)
        : _cl_path(std::move(cl_path_)),
          _base_args(std::move(base_args_)),
          _cwd(std::move(cwd_)),
          _temp_dir(std::move(temp_dir_)),
          _logger(logger_)
      {
      }

      data::process_output
      binary::run(const data::command_line_argument_list& args_,
                  const std::string& stdin_) const
      {
        const data::command_line cmd(_cl_path, _base_args + args_);

        METASHELL_LOG(_logger, "Running cl.exe: " + to_string(cmd));

        const data::process_output o = dos2unix(process::run(cmd, stdin_));

        METASHELL_LOG(_logger, "cl.exe's exit code: " + to_string(o.status));
        METASHELL_LOG(_logger, "cl.exe's stdout: " + o.standard_output);
        METASHELL_LOG(_logger, "cl.exe's stderr: " + o.standard_error);

        return o;
      }

      const boost::filesystem::path& binary::temp_dir() const
      {
        return _temp_dir;
      }

      const data::command_line_argument_list& binary::base_args() const
      {
        return _base_args;
      }

      data::process_output run(const binary& binary_,
                               data::command_line_argument_list args_,
                               const data::cpp_code& input_)
      {
        const boost::filesystem::path temp_path =
            binary_.temp_dir() / "msvc.cpp";
        save(temp_path, input_);
        args_.push_back(temp_path.string());
        return binary_.run(args_, "");
      }

      std::string error_report_on_stdout(const data::process_output& output_)
      {
        return error_report(just::lines::begin_lines(output_.standard_output),
                            just::lines::end_lines(output_.standard_output));
      }

      std::string error_report_on_stderr(const data::process_output& output_)
      {
        const just::lines::view err(output_.standard_error);
        auto empty_line = std::find(err.begin(), err.end(), "");
        if (empty_line != err.end())
        {
          ++empty_line;
        }
        return error_report(empty_line, err.end());
      }
    }
  }
}
