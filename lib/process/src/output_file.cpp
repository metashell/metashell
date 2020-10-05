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

#include <metashell/process/exception.hpp>
#include <metashell/process/output_file.hpp>

#include <metashell/data/signal.hpp>

#include <cassert>
#include <optional>

#ifndef _WIN32
#include <signal.h>
#include <unistd.h>
#endif

#include <errno.h>

namespace metashell
{
  namespace process
  {
    namespace
    {
#ifndef _WIN32
      class signal_set
      {
      public:
        signal_set() { sigemptyset(&_set); }

        explicit signal_set(std::initializer_list<data::signal> signals_)
          : signal_set{}
        {
          for (data::signal sig : signals_)
          {
            sigaddset(&_set, signum(sig));
          }
        }

        template <data::signal Signal>
        bool contains() const
        {
          const int member = sigismember(&_set, signum(Signal));
          if (member == -1)
          {
            throw exception::from_errno();
          }
          return member != 0;
        }

        static signal_set pending()
        {
          signal_set result;
          if (sigpending(&result._set) == -1)
          {
            throw exception::from_errno();
          }
          return result;
        }

        const sigset_t& value() const { return _set; }

      private:
        sigset_t _set;
      };
#endif

#ifndef _WIN32
      data::signal wait(const signal_set& set_)
      {
        int sig;
        if (const int err = sigwait(&set_.value(), &sig))
        {
          throw exception::from_errno(err);
        }
        return data::from_signum(sig);
      }
#endif

#ifndef _WIN32
      template <data::signal... Signal>
      class thread_level_signal_blocking
      {
      public:
        thread_level_signal_blocking()
        {
          const signal_set sig_block{Signal...};

          if (const int err =
                  pthread_sigmask(SIG_BLOCK, &sig_block.value(), &_sig_restore))
          {
            throw exception::from_errno(err);
          }
        }

        thread_level_signal_blocking(const thread_level_signal_blocking&) =
            delete;
        thread_level_signal_blocking(thread_level_signal_blocking&&) = delete;

        thread_level_signal_blocking&
        operator=(const thread_level_signal_blocking&) = delete;
        thread_level_signal_blocking&
        operator=(thread_level_signal_blocking&&) = delete;

        ~thread_level_signal_blocking()
        {
          // Suppressing any error to avoid throwing from the destructor
          pthread_sigmask(SIG_SETMASK, &_sig_restore, NULL);
        }

      private:
        sigset_t _sig_restore;
      };
#endif

#ifndef _WIN32
      template <data::signal... Signal>
      std::optional<data::signal> peek_signal()
      {
        const signal_set pending = signal_set::pending();
        if ((pending.contains<Signal>() || ...))
        {
          return wait(signal_set{Signal...});
        }
        else
        {
          return std::nullopt;
        }
      }
#endif
    } // namespace

    output_file::output_file(fd_t fd_) : file<output_file>(fd_) {}

    output_file::output_file(output_file&& f_)
      : file<output_file>(std::move(f_))
    {
    }

    output_file& output_file::operator=(output_file&& f_)
    {
      file<output_file>::operator=(std::move(f_));
      return *this;
    }

#ifdef _WIN32
    output_file::size_type output_file::write(const char* buff_, size_t count_)
    {
      DWORD len = 0;
      if (WriteFile(fd(), buff_, count_, &len, NULL))
      {
        return len;
      }
      else
      {
        return 0;
      }
    }
#endif

#ifndef _WIN32
    output_file::size_type output_file::write(const char* buff_, size_t count_)
    {
      // based on
      // https://riptutorial.com/posix/example/17424/handle-sigpipe-generated-by-write---in-a-thread-safe-manner
      thread_level_signal_blocking<data::signal::sigpipe> block_sig_pipe;

      const bool prior_sigpipe_pending =
          signal_set::pending().contains<data::signal::sigpipe>();

      const auto ret = ::write(fd(), buff_, count_);

      if (ret == -1 && errno == EPIPE && !prior_sigpipe_pending)
      {
        while (peek_signal<data::signal::sigpipe>())
          ;
      }

      return ret;
    }
#endif

    output_file::size_type output_file::write(const std::string& s_)
    {
      return write(s_.c_str(), s_.length());
    }
  } // namespace process
} // namespace metashell
