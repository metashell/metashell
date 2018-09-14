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

#include <metashell/process/input_file.hpp>

#include <boost/thread.hpp>

#include <cassert>
#include <sstream>

namespace metashell
{
  namespace process
  {
    input_file::input_file(fd_t fd_) : file<input_file>(fd_), _eof(false) {}

    input_file::input_file(input_file&& f_)
      : file<input_file>(std::move(f_)), _eof(f_._eof)
    {
      f_._eof = true;
    }

    input_file& input_file::operator=(input_file&& f_)
    {
      if (this != &f_)
      {
        file<input_file>::operator=(std::move(f_));

        _eof = f_._eof;
        f_._eof = true;
      }
      return *this;
    }

#ifdef _WIN32
    input_file::size_type input_file::read(char* buf_, size_t count_)
    {
      DWORD len = 0;
      if (ReadFile(fd(), buf_, count_, &len, NULL))
      {
        _eof = (len == 0);
        return len;
      }
      else
      {
        _eof = true;
        return 0;
      }
    }
#endif

#ifndef _WIN32
    input_file::size_type input_file::read(char* buf_, size_t count_)
    {
      const ssize_t len = ::read(fd(), buf_, count_);
      _eof = (len == 0);
      return len;
    }
#endif

    bool input_file::eof() const { return _eof; }

    void read_all(std::tuple<input_file&, std::string&> io1_)
    {
      std::ostringstream s;
      std::array<char, 1024> buff;
      while (!std::get<0>(io1_).eof())
      {
        s.write(buff.data(), std::get<0>(io1_).read(buff));
      }
      std::get<1>(io1_) = s.str();
    }

    void read_all(std::tuple<input_file&, std::string&> io1_,
                  std::tuple<input_file&, std::string&> io2_)
    {
      boost::thread reader2([&io2_]() { read_all(io2_); });
      read_all(io1_);

      reader2.join();
    }

    input_file::iterator input_file::begin() const
    {
      return iterator(const_cast<input_file*>(this));
    }

    input_file::iterator input_file::end() const { return iterator(nullptr); }

    input_file::iterator::iterator(input_file* input_)
      : _input(input_), _buffer_begin(0), _buffer_end(0)
    {
      ++*this;
    }

    bool input_file::iterator::operator==(const iterator& it_) const
    {
      return is_end() == it_.is_end();
    }

    bool input_file::iterator::operator<(const iterator& it_) const
    {
      return !is_end() && it_.is_end();
    }

    char input_file::iterator::operator*() const
    {
      assert(_buffer_begin != _buffer_end);

      return _buffer[_buffer_begin];
    }

    input_file::iterator& input_file::iterator::operator++()
    {
      if (_buffer_begin != _buffer_end)
      {
        ++_buffer_begin;
      }
      if (_buffer_begin == _buffer_end && _input)
      {
        _buffer_begin = 0;
        _buffer_end = _input->read(_buffer);
        if (_buffer_begin == _buffer_end)
        {
          _input = nullptr;
        }
      }
      return *this;
    }

    bool input_file::iterator::is_end() const
    {
      return _buffer_begin == _buffer_end && !_input;
    }
  }
}
