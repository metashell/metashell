// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/templight/entry.hpp>
#include <metashell/engine/templight/metaprogram_tracer.hpp>

#include <metashell/engine/clang/binary.hpp>
#include <metashell/engine/clang/code_completer.hpp>
#include <metashell/engine/clang/cpp_validator.hpp>
#include <metashell/engine/clang/header_discoverer.hpp>
#include <metashell/engine/clang/macro_discovery.hpp>
#include <metashell/engine/clang/preprocessor_shell.hpp>
#include <metashell/engine/clang/this_engine.hpp>
#include <metashell/engine/clang/type_shell.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>

namespace metashell
{
  namespace engine
  {
    namespace templight
    {
      namespace
      {
        std::vector<data::feature> supported_features()
        {
          return {data::feature::type_shell(),
                  data::feature::preprocessor_shell(),
                  data::feature::code_completer(),
                  data::feature::header_discoverer(),
                  data::feature::metaprogram_tracer(),
                  data::feature::cpp_validator(),
                  data::feature::macro_discovery()};
        }

        bool this_engine_internal_templight(
            const data::command_line_argument_list& args_)
        {
          if (const auto first = args_.front())
          {
            return boost::starts_with(first->value(), "-");
          }
          else
          {
            return true;
          }
        }

        bool this_engine_external_templight(
            const data::command_line_argument_list& args_)
        {
          if (const auto first = args_.front())
          {
            const data::executable_path exe(*first);
            return clang::is_clang(exe) && clang::is_templight(exe);
          }
          else
          {
            return false;
          }
        }

        bool is_number(const std::string_view& str_)
        {
          return !str_.empty() &&
                 str_.find_first_not_of("0123456789") == std::string_view::npos;
        }

        std::vector<boost::filesystem::path>
        search_for_standard_headers(const boost::filesystem::path& base_,
                                    core::logger* logger_)
        {
          METASHELL_LOG(logger_, "Checking subdirs of " + base_.string() +
                                     " for standard headers.");

          const boost::filesystem::directory_iterator e;

          std::optional<boost::filesystem::path> subdir = std::nullopt;
          for (boost::filesystem::directory_iterator i{base_}; i != e; ++i)
          {
            if (exists(i->path() / "iostream"))
            {
              METASHELL_LOG(logger_, "Found subdir " + i->path().string());
              if (!subdir || smaller(i->path(), *subdir))
              {
                subdir = i->path();
              }
            }
            else
            {
              METASHELL_LOG(logger_, "Skipping " + i->path().string());
            }
          }

          std::vector<boost::filesystem::path> result;

          if (subdir)
          {
            METASHELL_LOG(logger_, "Choosing " + subdir->string());
            result.push_back(*subdir);

            const boost::filesystem::directory_iterator i =
                std::find_if(boost::filesystem::directory_iterator{*subdir}, e,
                             [](const boost::filesystem::directory_entry& e_) {
                               return boost::starts_with(
                                   e_.path().filename().string(), "x86_64-");
                             });

            if (i != e)
            {
              METASHELL_LOG(logger_, "Also adding " + i->path().string());
              result.push_back(i->path());
            }
          }
          else
          {
            METASHELL_LOG(logger_, "No subdir found.");
          }

          return result;
        }

        template <bool UseInternalTemplight>
        std::unique_ptr<iface::engine>
        create_templight_engine(const data::shell_config& config_,
                                const data::executable_path& metashell_binary_,
                                const boost::filesystem::path& internal_dir_,
                                const boost::filesystem::path& temp_dir_,
                                const boost::filesystem::path& env_filename_,
                                iface::environment_detector& env_detector_,
                                iface::displayer& displayer_,
                                core::logger* logger_)
        {
          using core::not_supported;

          data::command_line_argument_list extra_clang_args =
              UseInternalTemplight ? config_.engine->args :
                                     config_.engine->args.tail();

          const data::executable_path binary_path = clang::find_clang(
              UseInternalTemplight, *config_.engine, metashell_binary_,
              env_detector_, displayer_, logger_);

          clang::binary cbin{UseInternalTemplight,
                             binary_path,
                             extra_clang_args,
                             config_.cwd,
                             internal_dir_,
                             env_detector_,
                             logger_};

          const data::command_line_argument stdinc{"-nostdinc"};
          const data::command_line_argument stdincpp{"-nostdinc++"};
          if (std::find_if(
                  config_.engine->args.begin(), config_.engine->args.end(),
                  [&stdinc,
                   &stdincpp](const data::command_line_argument& arg_) {
                    return arg_ == stdinc || arg_ == stdincpp;
                  }) == config_.engine->args.end())
          {
            try
            {
              METASHELL_LOG(
                  logger_, "Checking if the standard headers are available.");
              const boost::filesystem::path test_header{"iostream"};
              const std::vector<boost::filesystem::path> found =
                  clang::header_discoverer{cbin}.which(data::include_argument{
                      data::include_type::sys, test_header});

              if (found.empty())
              {
                METASHELL_LOG(logger_, "<" + test_header.string() +
                                           "> not found on include path.");
                extra_clang_args.append_with_prefix(
                    "-isystem",
                    search_for_standard_headers("/usr/include/c++", logger_));
              }
              else
              {
                METASHELL_LOG(
                    logger_,
                    "Found " + found.front().string() +
                        ". Assuming that all standard headers are available.");
              }
            }
            catch (const std::exception& err)
            {
              METASHELL_LOG(
                  logger_, std::string{"Checking standard headers failed: "} +
                               err.what());
            }
          }

          cbin = clang::binary{UseInternalTemplight,
                               binary_path,
                               extra_clang_args,
                               config_.cwd,
                               internal_dir_,
                               env_detector_,
                               logger_};

          return core::make_engine(
              name(UseInternalTemplight), config_.engine->name,
              clang::type_shell(internal_dir_, env_filename_, cbin, logger_),
              clang::preprocessor_shell(cbin),
              clang::code_completer(
                  internal_dir_, temp_dir_, env_filename_, cbin, logger_),
              clang::header_discoverer(cbin), metaprogram_tracer(cbin),
              clang::cpp_validator(internal_dir_, env_filename_, cbin, logger_),
              clang::macro_discovery(cbin), not_supported(),
              supported_features(), [extra_clang_args] {
                return parse_clang_arguments(extra_clang_args);
              });
        }
      } // anonymous namespace

      data::real_engine_name name(bool use_internal_templight_)
      {
        return use_internal_templight_ ? data::real_engine_name::internal :
                                         data::real_engine_name::templight;
      }

      core::engine_entry entry(bool use_internal_templight_,
                               data::executable_path metashell_binary_)
      {
        auto factory_fun = use_internal_templight_ ?
                               &create_templight_engine<true> :
                               &create_templight_engine<false>;

        auto factory = [factory_fun, metashell_binary_](
                           const data::shell_config& config_,
                           const boost::filesystem::path& internal_dir_,
                           const boost::filesystem::path& temp_dir_,
                           const boost::filesystem::path& env_filename_,
                           iface::environment_detector& env_detector_,
                           iface::displayer& displayer_,
                           core::logger* logger_) {
          return factory_fun(config_, metashell_binary_, internal_dir_,
                             temp_dir_, env_filename_, env_detector_,
                             displayer_, logger_);
        };
        return use_internal_templight_ ?
                   core::engine_entry(
                       factory, "[<Clang args>]",
                       data::markdown_string(
                           "Uses the "
                           "[Templight](https://github.com/mikael-s-persson/"
                           "templight) shipped with Metashell. `<Clang "
                           "args>` "
                           "are passed to the compiler as command "
                           "line-arguments."),
                       supported_features(), this_engine_internal_templight) :
                   core::engine_entry(
                       factory,
                       "<Templight binary> -std=<standard to use> [<Clang "
                       "args>]",
                       data::markdown_string(
                           "Uses "
                           "[Templight](https://github.com/mikael-s-persson/"
                           "templight). `<Clang args>` are passed to the "
                           "compiler as command line-arguments. Note that "
                           "Metashell requires C++11 or above. If your "
                           "Templight uses such a standard by default, you "
                           "can "
                           "omit the `-std` argument."),
                       supported_features(), this_engine_external_templight);
      }

      bool smaller(boost::filesystem::path lhs_, boost::filesystem::path rhs_)
      {
        lhs_ = lhs_.filename();
        rhs_ = rhs_.filename();

        if (is_number(lhs_.string()))
        {
          return !is_number(rhs_.string()) ||
                 std::stoll(lhs_.string()) < std::stoll(rhs_.string());
        }
        else
        {
          return !is_number(rhs_.string()) && lhs_ < rhs_;
        }
      }

    } // namespace templight
  } // namespace engine
} // namespace metashell
