#ifndef METASHELL_PROCESS_INPUT_FILE_HPP
#define METASHELL_PROCESS_INPUT_FILE_HPP

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

#include <metashell/process/file.hpp>

#include <boost/operators.hpp>

#include <array>
#include <string>
#include <tuple>

namespace metashell
{
  namespace process
  {
    class input_file : public file<input_file>
    {
    public:
      class iterator : public boost::forward_iterator_helper<iterator, char>
      {
      public:
        explicit iterator(input_file* input_ = nullptr);

        bool operator==(const iterator& it_) const;
        bool operator<(const iterator& it_) const;

        char operator*() const;
        iterator& operator++();

      private:
        input_file* _input;
        std::array<char, 1024> _buffer;
        int _buffer_begin;
        int _buffer_end;

        bool is_end() const;
      };

      typedef iterator const_iterator;

      explicit input_file(fd_t fd_);

      // Adding move operations for implementing ownership transfer
      input_file(input_file&& f_);
      input_file& operator=(input_file&& f_);

      template <std::size_t Count>
      size_type read(std::array<char, Count>& out_)
      {
        return read(out_.data(), out_.size());
      }

      bool eof() const;

      iterator begin() const;
      iterator end() const;

    private:
      bool _eof;

      size_type read(char* buf_, size_t count_);
    };

    void read_all(std::tuple<input_file&, std::string&> io1_);
    void read_all(std::tuple<input_file&, std::string&> io1_,
                  std::tuple<input_file&, std::string&> io2_);
  } // namespace process
} // namespace metashell

#endif
