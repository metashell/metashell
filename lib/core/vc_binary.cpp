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

#include <metashell/core/metashell.hpp>
#include <metashell/core/vc_binary.hpp>

#include <metashell/data/exception.hpp>

#include <metashell/process/run.hpp>
#include <metashell/process/util.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <just/lines.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>

namespace metashell
{
  namespace core
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

      boost::optional<std::string> output_line_of(const std::string& filename_,
                                                  const std::string& line_)
      {
        using boost::xpressive::bos;
        using boost::xpressive::_d;

        const boost::xpressive::sregex error_report =
            bos >> '(' >> +_d >> "): ";

        for (auto i = line_.find(filename_); i != std::string::npos;
             i = line_.find(filename_, i + 1))
        {
          if (i == 0 || line_[i - 1] == '\\' || line_[i - 1] == '/')
          {
            const std::string after_filename =
                line_.substr(i + filename_.size());
            boost::xpressive::smatch what;
            if (regex_search(after_filename, what, error_report))
            {
              return what.suffix().str() + "\n";
            }
          }
        }
        return boost::none;
      }

      template <class LineIt>
      std::string error_report(LineIt begin_, LineIt end_)
      {
        using boost::xpressive::alpha;
        using boost::xpressive::as_xpr;

        const boost::xpressive::sregex is_filename =
            !(alpha >> ':' >> (as_xpr('\\') | '/')) >> *~as_xpr(':');

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
                throw data::exception("Multiple filenames (" + filename + ", " +
                                      *i + ") in Visual C++ output.");
              }
            }
            else if (const boost::optional<std::string> l =
                         output_line_of(filename, *i))
            {
              o << *l;
            }
            else
            {
              throw data::exception("Unexpected output from Visual C++: " + *i);
            }
          }
        }
        return o.str();
      }
    }

    vc_binary::vc_binary(boost::filesystem::path cl_path_,
                         std::vector<std::string> base_args_,
                         boost::filesystem::path temp_dir_,
                         logger* logger_)
      : _cl_path(std::move(cl_path_)),
        _base_args(std::move(base_args_)),
        _temp_dir(std::move(temp_dir_)),
        _logger(logger_)
    {
      process::quote_arguments(_base_args);
    }

    data::process_output vc_binary::run(const std::vector<std::string>& args_,
                                        const std::string& stdin_) const
    {
      std::vector<std::string> cmd(_base_args.size() + args_.size());

      process::quote_arguments(
          args_.begin(), args_.end(),
          std::copy(_base_args.begin(), _base_args.end(), cmd.begin()));

      METASHELL_LOG(_logger, "Running cl.exe: " + _cl_path.string() + " " +
                                 boost::algorithm::join(cmd, " "));

      const data::process_output o =
          dos2unix(process::run(_cl_path, cmd, stdin_));

      METASHELL_LOG(_logger, "cl.exe's exit code: " + to_string(o.exit_code));
      METASHELL_LOG(_logger, "cl.exe's stdout: " + o.standard_output);
      METASHELL_LOG(_logger, "cl.exe's stderr: " + o.standard_error);

      return o;
    }

    const boost::filesystem::path& vc_binary::temp_dir() const
    {
      return _temp_dir;
    }

    const std::vector<std::string>& vc_binary::base_args() const
    {
      return _base_args;
    }

    data::process_output run_vc(const vc_binary& vc_binary_,
                                std::vector<std::string> vc_args_,
                                const data::cpp_code& input_)
    {
      const boost::filesystem::path temp_path =
          vc_binary_.temp_dir() / "msvc.cpp";
      save(temp_path, input_);
      vc_args_.push_back(temp_path.string());
      return vc_binary_.run(vc_args_, "");
    }

    std::string
    vc_error_report_on_stdout(const data::process_output& vc_output_)
    {
      return error_report(just::lines::begin_lines(vc_output_.standard_output),
                          just::lines::end_lines(vc_output_.standard_output));
    }

    std::string
    vc_error_report_on_stderr(const data::process_output& vc_output_)
    {
      const just::lines::view err(vc_output_.standard_error);
      auto empty_line = std::find(err.begin(), err.end(), "");
      if (empty_line != err.end())
      {
        ++empty_line;
      }
      return error_report(empty_line, err.end());
    }
  }
}
