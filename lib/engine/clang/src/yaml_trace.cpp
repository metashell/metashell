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

#include <chrono>
#include <metashell/engine/clang/yaml_trace.hpp>

#include <yaml-cpp/yaml.h>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        std::optional<data::event_kind>
        instantiation_kind_from_yaml_dump(const std::string& s_)
        {
          using data::event_kind;

          if (s_ == "TemplateInstantiation")
          {
            return event_kind::template_instantiation;
          }
          else if (s_ == "DefaultTemplateArgumentInstantiation")
          {
            return event_kind::default_template_argument_instantiation;
          }
          else if (s_ == "DefaultFunctionArgumentInstantiation")
          {
            return event_kind::default_function_argument_instantiation;
          }
          else if (s_ == "ExplicitTemplateArgumentSubstitution")
          {
            return event_kind::explicit_template_argument_substitution;
          }
          else if (s_ == "DeducedTemplateArgumentSubstitution")
          {
            return event_kind::deduced_template_argument_substitution;
          }
          else if (s_ == "PriorTemplateArgumentSubstitution")
          {
            return event_kind::prior_template_argument_substitution;
          }
          else if (s_ == "DefaultTemplateArgumentChecking")
          {
            return event_kind::default_template_argument_checking;
          }
          else if (s_ == "ExceptionSpecInstantiation")
          {
            return event_kind::exception_spec_instantiation;
          }
          else if (s_ == "DeclaringSpecialMember")
          {
            return event_kind::declaring_special_member;
          }
          else if (s_ == "DefiningSynthesizedFunction")
          {
            return event_kind::defining_synthesized_function;
          }
          else if (s_ == "Memoization")
          {
            return event_kind::memoization;
          }
          else
          {
            return std::nullopt;
          }
        }
      }

      yaml_trace::yaml_trace(const std::string& trace_,
                             data::type_or_code_or_error evaluation_result_,
                             data::cpp_code root_name_,
                             data::metaprogram_mode mode_)
        : _nodes(YAML::LoadAll(trace_)),
          _next(_nodes.begin()),
          _evaluation_result(
              data::event_details<data::event_kind::evaluation_end>{
                  {evaluation_result_}}),
          _root_name(std::move(root_name_)),
          _mode(mode_)
      {
      }

      std::optional<data::event_data> yaml_trace::next()
      {
        std::optional<data::event_data> result;

        if (_next != _nodes.end())
        {
          const YAML::Node& node = *_next;

          if (const auto kind = instantiation_kind_from_yaml_dump(
                  node["kind"].as<std::string>()))
          {
            const std::string event = node["event"].as<std::string>();

            double timestamp;
            if (const auto stamp = node["stamp"])
            {
              using namespace std::chrono;
              timestamp = (double)stamp.as<high_resolution_clock::rep>() *
                          nanoseconds::period::num / nanoseconds::period::den;
            }
            else
            {
              timestamp = 0;
            }

            if (event == "Begin")
            {
              result = template_begin(
                  *kind, data::type(node["name"].as<std::string>()),
                  data::file_location::parse(node["poi"].as<std::string>()),
                  data::file_location::parse(node["orig"].as<std::string>()),
                  timestamp);
            }
            else if (event == "End")
            {
              result = data::event_details<data::event_kind::template_end>{
                  {}, timestamp};
            }
          }

          ++_next;
        }
        else if (_evaluation_result)
        {
          result = *_evaluation_result;
          _evaluation_result = std::nullopt;
        }

        return result;
      }

      const data::cpp_code& yaml_trace::root_name() const { return _root_name; }

      data::metaprogram_mode yaml_trace::mode() const { return _mode; }
    }
  }
}
