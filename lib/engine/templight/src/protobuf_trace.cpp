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

#include <metashell/engine/templight/protobuf_trace.hpp>

#include <metashell/data/exception.hpp>

#include <fstream>
#include <string>

namespace metashell
{
  namespace engine
  {
    namespace templight
    {
      namespace
      {
        data::event_kind instantiation_kind_from_protobuf(int kind)
        {
          using data::event_kind;

          switch (kind)
          {
          case 0:
            return event_kind::template_instantiation;
          case 1:
            return event_kind::default_template_argument_instantiation;
          case 2:
            return event_kind::default_function_argument_instantiation;
          case 3:
            return event_kind::explicit_template_argument_substitution;
          case 4:
            return event_kind::deduced_template_argument_substitution;
          case 5:
            return event_kind::prior_template_argument_substitution;
          case 6:
            return event_kind::default_template_argument_checking;
          case 7:
            return event_kind::exception_spec_evaluation;
          case 8:
            return event_kind::exception_spec_instantiation;
          case 9:
            return event_kind::declaring_special_member;
          case 10:
            return event_kind::defining_synthesized_function;
          case 11:
            return event_kind::memoization;
          default:
            throw data::exception(
                "templight xml parse failed (invalid instantiation kind " +
                std::to_string(kind) + ")");
          }
        }
      }

      protobuf_trace::protobuf_trace(
          const boost::filesystem::path& src,
          data::type_or_code_or_error evaluation_result,
          data::cpp_code root_name_,
          data::metaprogram_mode mode_)
          // Opening in binary mode, because some platforms interpret some
          // characters
          // specially in text mode, which caused parsing to fail.
          : _src(new std::ifstream(src.string(),
                                   std::ios_base::in | std::ios_base::binary)),
            _evaluation_result(
                data::event_details<data::event_kind::evaluation_end>{
                    {evaluation_result}}),
            _root_name(std::move(root_name_)),
            _mode(mode_)
      {
        if (!*_src)
        {
          if (evaluation_result.is_error())
          {
            throw data::exception(evaluation_result.get_error());
          }
          else
          {
            // Shouldn't happen
            throw data::exception(
                "Unexpected type type_or_code_or_error result");
          }
        }
        _reader.startOnBuffer(*_src);
      }

      std::optional<data::event_data> protobuf_trace::next()
      {
        while (true)
        {
          switch (_reader.LastChunk)
          {
          case ::templight::ProtobufReader::BeginEntry:
          {
            auto begin_entry = _reader.LastBeginEntry;
            _reader.next();
            return template_begin(
                instantiation_kind_from_protobuf(begin_entry.InstantiationKind),
                data::type(begin_entry.Name),
                data::file_location(
                    begin_entry.FileName, begin_entry.Line, begin_entry.Column),
                data::file_location(begin_entry.TempOri_FileName,
                                    begin_entry.TempOri_Line,
                                    begin_entry.TempOri_Column),
                begin_entry.TimeStamp);
          }
          case ::templight::ProtobufReader::EndEntry:
          {
            auto end_entry = _reader.LastEndEntry;
            _reader.next();
            return data::event_data(
                data::event_details<data::event_kind::template_end>{
                    {}, end_entry.TimeStamp});
          }
          case ::templight::ProtobufReader::EndOfFile:
            if (_evaluation_result)
            {
              data::event_data result = *_evaluation_result;
              _evaluation_result = std::nullopt;
              return result;
            }
            else
            {
              return std::nullopt;
            }
          case ::templight::ProtobufReader::Other:
          case ::templight::ProtobufReader::Header:
          default:
            _reader.next();
            break;
          }
        }
      }

      const data::cpp_code& protobuf_trace::root_name() const
      {
        return _root_name;
      }

      data::metaprogram_mode protobuf_trace::mode() const { return _mode; }
    }
  }
}
