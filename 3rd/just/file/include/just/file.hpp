#ifndef JUST_FILE_HPP
#define JUST_FILE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cerrno>

namespace just
{
  namespace file
  {
    class error : public std::runtime_error
    {
    public:
      explicit error(const std::string& reason_) :
        std::runtime_error(reason_)
      {}
    };

    namespace impl
    {
      inline void throw_errno(const std::string& action_)
      {
        throw error(action_ + ": " + std::strerror(errno));
      }

      class output_file
      {
      public:
        explicit output_file(const std::string& filename_) :
          _filename(filename_),
          _f(std::fopen(filename_.c_str(), "wb"))
        {
          if (!_f)
          {
            throw_errno("Error creating file " + filename_);
          }
        }

        ~output_file()
        {
          fclose(_f);
        }

        void write(const char* content_, std::size_t len_)
        {
          if (len_ > 0 && fwrite(content_, len_, 1, _f) < 1)
          {
            throw_errno("Error writing to " + _filename);
          }
        }
      private:
        std::string _filename;
        std::FILE* _f;

        output_file(const output_file&); // = delete
        output_file& operator=(const output_file&); // = delete
      };

      class input_file
      {
      public:
        explicit input_file(const std::string& filename_) :
          _filename(filename_),
          _f(std::fopen(filename_.c_str(), "rb"))
        {
          if (!_f)
          {
            throw_errno("Error opening " + filename_ + " for reading");
          }
        }

        ~input_file()
        {
          fclose(_f);
        }

        long size()
        {
          if (fseek(_f, 0, SEEK_END))
          {
            throw_errno("Failed to seek to the end of input file " + _filename);
          }
          return ftell(_f);
        }

        void read_from_file_start(std::vector<char>& buffer_)
        {
          if (!buffer_.empty())
          {
            rewind(_f);
            if (fread(buffer_.data(), buffer_.size(), 1, _f) != 1)
            {
              throw_errno("Failed to read file " + _filename);
            }
          }
        }
      private:
        std::string _filename;
        std::FILE* _f;

        input_file(const input_file&); // = delete
        input_file& operator=(const input_file&); // = delete
      };
    }

    inline void write(const std::string& filename_, const std::string& content_)
    {
      impl::output_file(filename_).write(content_.c_str(), content_.size());
    }

    inline void write(
      const std::string& filename_,
      const std::vector<char>& content_
    )
    {
      impl::output_file(filename_).write(content_.data(), content_.size());
    }

    inline void read(const std::string& filename_, std::vector<char>& out_)
    {
      impl::input_file f(filename_);
      out_.resize(f.size());
      f.read_from_file_start(out_);
    }

    inline std::vector<char> read(const std::string& filename_)
    {
      impl::input_file f(filename_);
      std::vector<char> result(f.size());
      f.read_from_file_start(result);
      return result;
    }

    template <class Container>
    inline Container read(const std::string& filename_)
    {
      const std::vector<char> bytes = read<std::vector<char> >(filename_);
      return Container(bytes.begin(), bytes.end());
    }

    template <>
    inline std::vector<char> read<std::vector<char> >(
      const std::string& filename_
    )
    {
      return read(filename_);
    }
  }
}

#endif

