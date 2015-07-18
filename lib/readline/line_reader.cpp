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

#include <metashell/readline/line_reader.hpp>
#include <metashell/line_reader.hpp>

#ifdef USE_EDITLINE
#  include <editline/readline.h>
#else
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

#include <cassert>
#include <algorithm>

using namespace metashell;
using namespace metashell::readline;

namespace
{
  int completion_end = 0;

  // not owning
  command_processor_queue* processor_queue;

#ifdef _WIN32
  template <class T>
  stdext::checked_array_iterator<T*> array_begin(T* array_, int len_)
  {
    return stdext::checked_array_iterator<T*>(array_, len_);
  }
#else
  template <class T>
  T* array_begin(T* array_, int)
  {
    return array_;
  }
#endif

  int line_length()
  {
#ifdef _WIN32
    return int(wcslen(_el_line_buffer));
#else
    return rl_end;
#endif
  }

  std::string get_edited_text()
  {
    return std::string(rl_line_buffer, rl_line_buffer + line_length());
  }

  char* tab_generator(const char* text_, int state_)
  {
    assert(processor_queue != nullptr);

    static std::set<std::string> values;
    static std::set<std::string>::const_iterator pos;

    if (!state_) // init
    {
      const std::string edited_text = get_edited_text();

      const auto eb = edited_text.begin();
      processor_queue->code_complete(
        std::string(eb, eb + completion_end),
        values
      );
      pos = values.begin();
    }

    if (pos == values.end())
    {
      return nullptr;
    }
    else
    {
      const std::string str = text_ + *pos;
      char* s = new char[str.length() + 1];
      std::copy(str.begin(), str.end(), array_begin(s, str.length() + 1));
      s[str.length()] = 0;
      ++pos;
      return s;
    }
    return nullptr;
  }

  char** tab_completion(const char* text_, int, int end_)
  {
    completion_end = end_;
    return rl_completion_matches(const_cast<char*>(text_), &tab_generator);
  }

  boost::optional<std::string> read_next_line(
    const std::string& prompt_,
    command_processor_queue& processor_queue_
  )
  {
    processor_queue = &processor_queue_;
    rl_attempted_completion_function = tab_completion;

    if (char *line = ::readline(prompt_.c_str()))
    {
      const std::string str(line);

#if defined __FreeBSD__ || defined __OpenBSD__ || defined __APPLE__
      free(line);
#else
      rl_free(line);
#endif
      return str;
    }
    else
    {
      return boost::none;
    }
  }
}

metashell::line_reader metashell::readline::line_reader(
  command_processor_queue& processor_queue_
)
{
  return
    [&processor_queue_](const std::string& prompt_)
    {
      return read_next_line(prompt_ + " ", processor_queue_);
    };
}

