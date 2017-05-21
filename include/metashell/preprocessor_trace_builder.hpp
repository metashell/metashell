#ifndef METASHELL_PREPROCESSOR_TRACE_BUILDER_HPP
#define METASHELL_PREPROCESSOR_TRACE_BUILDER_HPP

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
#include <metashell/data/file_location.hpp>
#include <metashell/data/metaprogram.hpp>
#include <metashell/data/wave_config.hpp>

#include <metashell/metaprogram_builder.hpp>

#include <boost/optional.hpp>

#include <vector>

namespace metashell
{
  class preprocessor_trace_builder
  {
  public:
    preprocessor_trace_builder(data::cpp_code env_,
                               boost::optional<data::cpp_code> exp_,
                               data::metaprogram::mode_t mode_);

    data::metaprogram build(const data::wave_config& config_);

  private:
    data::cpp_code _env;
    boost::optional<data::cpp_code> _exp;

    std::vector<data::file_location> _macro_loc_stack;
    data::file_location _last_macro_call_loc;

    int _num_tokens_from_macro_call;

    metaprogram_builder _builder;

    void on_macro_expansion_begin(
        const data::cpp_code& name_,
        const boost::optional<std::vector<data::cpp_code>>& args_,
        const data::file_location& point_of_event_,
        const data::file_location& source_location_);

    void on_rescanning(const data::cpp_code& c_);

    void on_macro_expansion_end(const data::cpp_code& c_, int num_tokens_);

    void on_token_generated(const data::token& t_,
                            const data::file_location& source_location_);
  };
}

#endif
