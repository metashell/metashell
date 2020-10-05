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

#include <metashell/core/input_loop.hpp>
#include <metashell/core/interrupt_handler_override.hpp>

#include <cassert>

namespace metashell
{
  namespace core
  {
    namespace
    {
      // single threaded
      class single_entry_guard
      {
      public:
        single_entry_guard()
        {
          assert(!_active);
          _active = true;
        }

        ~single_entry_guard() { _active = false; }

      private:
        static bool _active;
      };

      bool single_entry_guard::_active = false;
    } // namespace

    void input_loop(command_processor_queue& processor_queue_,
                    iface::displayer& displayer_,
                    const data::line_reader& line_reader_)
    {
      single_entry_guard g;

      interrupt_handler_override ovr3(
          [&processor_queue_]() { processor_queue_.cancel_operation(); });

      while (!processor_queue_.empty())
      {
        processor_queue_.pop_stopped_processors(displayer_);

        if (!processor_queue_.empty())
        {
          try
          {
            if (const auto line = line_reader_(processor_queue_.prompt()))
            {
              processor_queue_.line_available(*line, displayer_);
            }
            else
            {
              processor_queue_.pop(displayer_);
            }
          }
          catch (const std::exception& e)
          {
            displayer_.show_error(e.what());
          }
          catch (...)
          {
            displayer_.show_error("Unknown error");
          }
        }
      }
    }
  } // namespace core
} // namespace metashell
