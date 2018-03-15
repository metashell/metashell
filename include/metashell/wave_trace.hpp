#ifndef METASHELL_WAVE_TRACE_HPP
#define METASHELL_WAVE_TRACE_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/event_data_sequence.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/include_argument.hpp>
#include <metashell/data/wave_config.hpp>

#include <metashell/wave_context.hpp>

#include <boost/optional.hpp>

#include <deque>
#include <sstream>
#include <vector>

namespace metashell
{
  class wave_trace : public data::event_data_sequence<wave_trace>
  {
  public:
    wave_trace(const data::cpp_code& env_,
               const boost::optional<data::cpp_code>& exp_,
               const data::wave_config& config_);

    boost::optional<data::event_data> next();

  private:
    data::cpp_code _env;
    bool _full_trace;
    bool _ignore_macro_redefinition;

    std::vector<data::file_location> _macro_loc_stack;
    data::file_location _point_of_event;

    data::cpp_code _input;
    int _num_tokens_from_macro_call;

    wave_context _ctx;
    boost::optional<wave_context::iterator_type> _pos;

    std::ostringstream _output;
    std::deque<data::event_data> _events;

    void on_macro_expansion_begin(
        const data::cpp_code& name_,
        const boost::optional<std::vector<data::cpp_code>>& args_,
        const data::file_location& point_of_event_,
        const data::file_location& source_location_);

    void on_rescanning(const data::cpp_code& c_);

    void on_macro_expansion_end(const data::cpp_code& c_, int num_tokens_);

    void on_token_generated(const data::token& t_,
                            const data::file_location& source_location_);

    void on_token_skipped(const data::token& t_,
                          const data::file_location& source_location_);

    void on_include_begin(const data::include_argument& arg_,
                          const data::file_location& point_of_event_);

    void on_include_end();

    void on_define(const data::cpp_code& name_,
                   const boost::optional<std::vector<data::cpp_code>>& args_,
                   const data::cpp_code& body_,
                   const data::file_location& point_of_event_);

    void on_undefine(const data::cpp_code& name_,
                     const data::file_location& point_of_event_);

    void on_conditional(const data::cpp_code& expression_,
                        const data::file_location& point_of_event_);

    void on_evaluated_conditional_expression(bool result_);

    void on_else(const data::file_location& point_of_event_);

    void on_endif(const data::file_location& point_of_event_);

    void on_error(const std::string& message_,
                  const data::file_location& point_of_event_);

    void on_line(const data::cpp_code& arg_,
                 const data::file_location& point_of_event_,
                 const data::file_location& source_location_);

    void record_point_of_event(const data::file_location& point_of_event_);
  };
}

#endif
