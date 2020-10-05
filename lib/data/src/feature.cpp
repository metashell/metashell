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

#include <metashell/data/feature.hpp>

#include <algorithm>
#include <cassert>
#include <ostream>

namespace metashell
{
  namespace data
  {
    namespace
    {
      struct feature_info_t
      {
        const char* name;
        const char* description;
      };

      const feature_info_t& feature_info(int id_)
      {
        static feature_info_t infos[] = {
            {"type shell",
             "It is used for evaluating template metaprograms in Metashell."},
            {"preprocessor shell",
             "It is used for evaluating preprocessor metaprograms in Metashell "
             "in preprocessor mode."},
            {"code completer", "It is used for code-completion in the shell."},
            {"header discoverer",
             "It is used for analysing the included headers and the include "
             "path."},
            {"metaprogram tracer",
             "It is used by MDB to get an insight into template "
             "instantiations."},
            {"cpp validator",
             "It is used for checking that the code entered into Metashell is "
             "valid C++ code."},
            {"macro discovery", "It is used for querying the defined macros."},
            {"preprocessor tracer",
             "It is used by PDB to get an insight into preprocessing (macro "
             "calls, includes, etc)."}};

        assert(id_ >= 0);
        assert(id_ < int(sizeof(infos) / sizeof(infos[0])));

        return infos[id_];
      }

      bool valid(feature f_)
      {
        const auto features = feature::all();
        return std::find(features.begin(), features.end(), f_) !=
               features.end();
      }
    } // namespace

    feature feature::type_shell() { return feature(0); }
    feature feature::preprocessor_shell() { return feature(1); }
    feature feature::code_completer() { return feature(2); }
    feature feature::header_discoverer() { return feature(3); }
    feature feature::metaprogram_tracer() { return feature(4); }
    feature feature::cpp_validator() { return feature(5); }
    feature feature::macro_discovery() { return feature(6); }
    feature feature::preprocessor_tracer() { return feature(7); }

    std::vector<feature> feature::all()
    {
      return {type_shell(),        preprocessor_shell(), code_completer(),
              header_discoverer(), metaprogram_tracer(), cpp_validator(),
              macro_discovery(),   preprocessor_tracer()};
    }

    feature::feature(int id_) : _id(id_) {}

    int feature::id() const { return _id; }

    std::string feature::description() const
    {
      return feature_info(_id).description;
    }

    std::ostream& operator<<(std::ostream& o_, feature f_)
    {
      assert(valid(f_));
      return o_ << to_string(f_);
    }

    bool operator<(feature a_, feature b_) { return a_.id() < b_.id(); }

    bool operator==(feature a_, feature b_) { return a_.id() == b_.id(); }

    std::string to_string(feature f_)
    {
      assert(valid(f_));

      return feature_info(f_.id()).name;
    }
  } // namespace data
} // namespace metashell
