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

#include <metashell/engine/clang/binary.hpp>
#include <metashell/engine/clang/get_type_from_ast_string.hpp>

#include <metashell/data/default_clang_search_path.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/language_standard.hpp>

#include <metashell/process/run.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <vector>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        bool uses_c(const data::command_line_argument_list& args_)
        {
          for (const data::command_line_argument& arg : args_)
          {
            if (const auto standard = arg.remove_prefix("-std="))
            {
              try
              {
                if (c_standard(parse_standard(
                        data::real_engine_name::clang, standard->value())))
                {
                  return true;
                }
              }
              catch (const std::exception&)
              {
                // skip
              }
            }
          }
          return false;
        }

        data::executable_path
        extract_clang_binary(const data::engine_arguments& engine_,
                             iface::environment_detector& env_detector_,
                             const data::executable_path& metashell_path_)
        {
          if (const auto first_arg = engine_.args.front())
          {
            const boost::filesystem::path path = first_arg->value();
            if (env_detector_.file_exists(path))
            {
              return data::executable_path(path);
            }
            else
            {
              throw std::runtime_error(
                  "The path specified as the Clang binary to use (" +
                  path.string() + ") does not exist.");
            }
          }
          else
          {
            const std::string sample_path =
                env_detector_.on_windows() ?
                    "c:\\Program Files\\LLVM\\bin\\clang++.exe" :
                    "/usr/bin/clang++";
            throw std::runtime_error(
                "The engine requires that you specify the path to the clang "
                "compiler after --. For example: " +
                metashell_path_ + " --engine " + engine_.name + " -- " +
                sample_path + " -std=c++11");
          }
        }

        data::executable_path templight_shipped_with_metashell(
            iface::environment_detector& env_detector_)
        {
          return data::executable_path(env_detector_.directory_of_executable() /
                                       (env_detector_.on_windows() ?
                                            "\\templight\\templight.exe" :
                                            "/templight_metashell"));
        }

        std::optional<data::executable_path>
        detect_clang_binary(iface::environment_detector& env_detector_,
                            iface::displayer& displayer_,
                            core::logger* logger_)
        {
          METASHELL_LOG(logger_, "Searching Clang binary");

          const data::executable_path clang_metashell =
              templight_shipped_with_metashell(env_detector_);

          METASHELL_LOG(logger_, "Path of Clang shipped with Metashell: " +
                                     to_string(clang_metashell));

          if (env_detector_.file_exists(clang_metashell))
          {
            METASHELL_LOG(
                logger_,
                "Clang shipped with Metashell is there. Choosing that.");
            return clang_metashell;
          }
          else
          {
            METASHELL_LOG(
                logger_,
                "Clang binary shipped with Metashell is missing. Searching for"
                " another Clang binary at the following locations: " +
                    join(data::default_clang_search_path(), ", "));
            const std::optional<data::executable_path> clang =
                env_detector_.search_clang_binary();

            if (clang)
            {
              METASHELL_LOG(
                  logger_, "Clang binary found: " + to_string(*clang));
            }
            else
            {
              METASHELL_LOG(logger_, "No Clang binary found.");

              displayer_.show_error(
                  "clang++ not found. Checked:\n" + to_string(clang_metashell) +
                  "\n" + join(data::default_clang_search_path(), "\n") + "\n");
            }

            return clang;
          }
        }

        data::command_line_argument set_max_template_depth(int v_)
        {
          return data::command_line_argument("-ftemplate-depth=" +
                                             std::to_string(v_));
        }

        bool cpp_standard_set(const data::command_line_argument_list& args_)
        {
          return std::any_of(args_.begin(), args_.end(),
                             [](const data::command_line_argument& arg_) {
                               return arg_.starts_with("-std") ||
                                      arg_.starts_with("--std");
                             });
        }

        bool
        max_template_depth_set(const data::command_line_argument_list& args_)
        {
          return std::any_of(args_.begin(), args_.end(),
                             [](const data::command_line_argument& arg_) {
                               return arg_.starts_with("-ftemplate-depth");
                             });
        }

        bool stdinc_allowed(
            const data::command_line_argument_list& extra_clang_args_)
        {
          return find_if(
                     extra_clang_args_.begin(), extra_clang_args_.end(),
                     [](const data::command_line_argument& s_) {
                       return s_ == data::command_line_argument("-nostdinc") ||
                              s_ == data::command_line_argument("-nostdinc++");
                     }) == extra_clang_args_.end();
        }

        std::vector<boost::filesystem::path> determine_include_path(
            const std::optional<data::executable_path>& clang_binary_path_,
            iface::environment_detector& env_detector_,
            core::logger* logger_)
        {
          if (clang_binary_path_)
          {
            METASHELL_LOG(logger_, "Determining include path of Clang: " +
                                       to_string(*clang_binary_path_));
          }
          else
          {
            METASHELL_LOG(logger_, "Determining include path of Clang.");
          }

          std::vector<boost::filesystem::path> result;

          const boost::filesystem::path dir_of_executable =
              env_detector_.directory_of_executable();

          if (!env_detector_.on_windows())
          {
            if (env_detector_.on_osx())
            {
              result.push_back(dir_of_executable / ".." / "include" /
                               "metashell" / "libcxx");
            }
            result.push_back(dir_of_executable / ".." / "include" /
                             "metashell" / "templight");
          }

          METASHELL_LOG(
              logger_, "Include path determined: " +
                           boost::algorithm::join(
                               result | boost::adaptors::transformed([](
                                            const boost::filesystem::path& p_) {
                                 return p_.string();
                               }),
                               ";"));

          return result;
        }

        data::command_line_argument_list
        clang_args(bool use_internal_templight_,
                   const data::command_line_argument_list& extra_clang_args_,
                   const boost::filesystem::path& internal_dir_,
                   iface::environment_detector& env_detector_,
                   core::logger* logger_,
                   const data::executable_path& clang_path_)
        {
          data::command_line_argument_list args{
              "-iquote", ".", "-x",
              uses_c(extra_clang_args_) ? "c-header" : "c++-header"};

          if (stdinc_allowed(extra_clang_args_))
          {
            args.push_back("-I", internal_dir_);
          }

          if (use_internal_templight_)
          {
            args.push_back("-Wfatal-errors");

            if (env_detector_.on_windows())
            {
              args.push_back("-fno-ms-compatibility");
              args.push_back("-U_MSC_VER");
            }

            if (!cpp_standard_set(extra_clang_args_))
            {
              args.push_back("-std=c++14");
            }

            if (!max_template_depth_set(extra_clang_args_))
            {
              args.push_back(set_max_template_depth(256));
            }

            if (stdinc_allowed(extra_clang_args_))
            {
              args.append_with_prefix(
                  "-idirafter",
                  determine_include_path(clang_path_, env_detector_, logger_));
            }
          }

          return args + extra_clang_args_;
        }

        data::result
        preprocess(const iface::environment& env_,
                   const std::optional<data::cpp_code>& tmp_exp_,
                   const std::optional<boost::filesystem::path>& env_path_,
                   binary& binary_)
        {
          return binary_.precompile(
              env_path_ ?
                  data::command_line_argument_list{
                      data::command_line_argument("-include"),
                      data::command_line_argument(*env_path_)} :
                  data::command_line_argument_list{},
              tmp_exp_ ?
                  env_.get_appended(
                      data::cpp_code("::metashell::impl::wrap< ") + *tmp_exp_ +
                      data::cpp_code(" > __metashell_v;\n")) :
                  env_.get());
        }

        data::command_line_argument_list dump_ast()
        {
          return data::command_line_argument_list{"-Xclang", "-ast-dump"};
        }

        data::command_line_argument_list dump_templight_to_file(
            data::command_line_argument_list clang_args_,
            const std::optional<boost::filesystem::path>& templight_dump_path_)
        {
          if (templight_dump_path_)
          {
            clang_args_.push_back("-Xtemplight", "-profiler");
            clang_args_.push_back("-Xtemplight", "-safe-mode");

            // templight can't be forced to generate output file with
            // -Xtemplight -output=<file> for some reason
            // A workaround is to specify a standard output location with -o
            // then append ".trace.pbf" to the specified file (on the calling
            // side)
            clang_args_.push_back("-o", *templight_dump_path_);
          }

          return clang_args_;
        }

        data::process_output
        compile(const data::cpp_code& preprocessed_code_,
                data::command_line_argument_list clang_args_,
                binary& binary_)
        {
          clang_args_.push_back("-c", "-x", "c++-cpp-output");

          return run_clang(binary_, std::move(clang_args_), preprocessed_code_);
        }

        data::result compile(
            const std::optional<data::cpp_code>& tmp_exp_,
            data::cpp_code precompiled_exp_,
            const std::optional<boost::filesystem::path>& templight_dump_path_,
            binary& binary_)
        {
          const data::process_output output =
              compile(std::move(precompiled_exp_),
                      dump_templight_to_file(dump_ast(), templight_dump_path_),
                      binary_);

          const bool success = exit_success(output);

          return data::result{
              success, success && tmp_exp_ ?
                           get_type_from_ast_string(output.standard_output) :
                           "",
              success ? "" : output.standard_error, ""};
        }
      }

      binary::binary(data::executable_path clang_path_,
                     data::command_line_argument_list base_args_,
                     boost::filesystem::path cwd_,
                     core::logger* logger_)
        : _clang_path(std::move(clang_path_)),
          _base_args(std::move(base_args_)),
          _cwd(std::move(cwd_)),
          _logger(logger_)
      {
      }

      binary::binary(bool use_internal_templight_,
                     data::executable_path clang_path_,
                     const data::command_line_argument_list& extra_clang_args_,
                     boost::filesystem::path cwd_,
                     const boost::filesystem::path& internal_dir_,
                     iface::environment_detector& env_detector_,
                     core::logger* logger_)
        : binary(std::move(clang_path_),
                 clang_args(use_internal_templight_,
                            extra_clang_args_,
                            internal_dir_,
                            env_detector_,
                            logger_,
                            clang_path_),
                 std::move(cwd_),
                 logger_)
      {
      }

      data::process_output
      binary::run(const data::command_line_argument_list& args_,
                  const std::string& stdin_) const
      {
        const data::command_line cmd(_clang_path, _base_args + args_);

        METASHELL_LOG(_logger, "Running Clang: " + to_string(cmd) + " [cwd=" +
                                   _cwd.string() + "]");

        const data::process_output o = process::run(cmd, stdin_, _cwd);

        METASHELL_LOG(_logger, "Clang's exit code: " + to_string(o.status));
        METASHELL_LOG(_logger, "Clang's stdout: " + o.standard_output);
        METASHELL_LOG(_logger, "Clang's stderr: " + o.standard_error);

        return o;
      }

      data::result binary::precompile(data::command_line_argument_list args_,
                                      const data::cpp_code& exp_) const
      {
        args_.push_back("-E");

        const data::process_output output =
            run_clang(*this, std::move(args_), exp_);

        const bool success = exit_success(output);

        return data::result{success, success ? output.standard_output : "",
                            success ? "" : output.standard_error, ""};
      }

      data::process_output
      run_clang(const iface::executable& binary_,
                data::command_line_argument_list clang_args_,
                const data::cpp_code& input_)
      {
        const data::command_line_argument compile_from_stdin("-");
        clang_args_.push_back(compile_from_stdin);

        return binary_.run(clang_args_, input_.value());
      }

      std::tuple<data::result, std::string> eval_with_templight_dump_on_stdout(
          const iface::environment& env_,
          const std::optional<data::cpp_code>& tmp_exp_,
          const std::optional<boost::filesystem::path>& env_path_,
          binary& binary_,
          data::metaprogram_mode mode_)
      {
        data::result precompile_result =
            preprocess(env_, tmp_exp_, env_path_, binary_);

        if (precompile_result.successful)
        {
          const data::cpp_code precompiled_code(
              std::move(precompile_result.output));

          data::command_line_argument_list args{"-Xclang", "-templight-dump"};
          if (mode_ == data::metaprogram_mode::profile)
          {
            args += {"-Xclang", "-templight-profile"};
          }

          const data::process_output templight_output =
              compile(precompiled_code, args, binary_);

          return std::make_tuple(
              exit_success(templight_output) ?
                  compile(tmp_exp_, precompiled_code, std::nullopt, binary_) :
                  data::result{false, "", templight_output.standard_error, ""},
              templight_output.standard_output);
        }
        else
        {
          return std::make_tuple(precompile_result, "");
        }
      }

      data::result
      eval(const iface::environment& env_,
           const std::optional<data::cpp_code>& tmp_exp_,
           const std::optional<boost::filesystem::path>& env_path_,
           const std::optional<boost::filesystem::path>& templight_dump_path_,
           binary& binary_)
      {
        const data::result precompile_result =
            preprocess(env_, tmp_exp_, env_path_, binary_);

        return precompile_result.successful ?
                   compile(tmp_exp_, data::cpp_code(precompile_result.output),
                           templight_dump_path_, binary_) :
                   precompile_result;
      }

      std::optional<data::executable_path>
      find_clang_nothrow(bool use_internal_templight_,
                         const data::engine_arguments& engine_,
                         const data::executable_path& metashell_binary_,
                         iface::environment_detector& env_detector_,
                         iface::displayer& displayer_,
                         core::logger* logger_)
      {
        return use_internal_templight_ ?
                   detect_clang_binary(env_detector_, displayer_, logger_) :
                   extract_clang_binary(
                       engine_, env_detector_, metashell_binary_);
      }

      data::executable_path
      find_clang(bool use_internal_templight_,
                 const data::engine_arguments& engine_,
                 const data::executable_path& metashell_binary_,
                 iface::environment_detector& env_detector_,
                 iface::displayer& displayer_,
                 core::logger* logger_)
      {
        if (const auto result = find_clang_nothrow(
                use_internal_templight_, engine_, metashell_binary_,
                env_detector_, displayer_, logger_))
        {
          return *result;
        }
        else
        {
          throw data::exception("Clang binary not found.");
        }
      }
    }
  }
}
