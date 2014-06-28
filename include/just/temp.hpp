#ifndef JUST_TEMP_HPP
#define JUST_TEMP_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <stdexcept>

#ifdef _WIN32
#  include <windows.h>
#  include <vector>
#else
#  include <stdlib.h>
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <dirent.h>
#endif


namespace just
{
  namespace temp
  {
    class directory
    {
    public:
      directory()
      {
#ifdef _WIN32
        const std::string temp_dir = get_temp_path();
        char tmp[MAX_PATH + 1];
        if (GetTempFileName(temp_dir.c_str(), "just-", 0, tmp))
        {
          if (DeleteFile(tmp))
          {
            if (CreateDirectory(tmp, NULL))
            {
              _path = tmp;
            }
            else
            {
              throw std::runtime_error("Error creating temporary directory");
            }
          }
          else
          {
            throw std::runtime_error("Error deleting temporary file");
          }
        }
        else
        {
          throw
            std::runtime_error(
              "Error choosing unique name for the temporary directory"
            );
        }
#else
        char tmp[] = "/tmp/just-XXXXXX";
        if (char* const p = mkdtemp(tmp))
        {
          _path = p;
        }
        else
        {
          throw std::runtime_error("Error creating temporary directory");
        }
#endif
      }

      ~directory()
      {
        delete_dir(_path);
      }

      const std::string& path() const
      {
        return _path;
      }
    private:
      std::string _path;

      // Noncopyable
      directory(const directory&);
      directory& operator=(const directory&);

      class dir_t
      {
      public:
        explicit dir_t(const std::string& path_) :
#ifdef _WIN32
          _next_eod(false),
#endif
          _d(start_search(path_, _data)),
          _eod(!valid(_d))
        {}

        ~dir_t()
        {
          if (valid(_d))
          {
            close_search(_d);
          }
        }

        bool eod() const
        {
          return _eod;
        }

        std::string next_file()
        {
          std::string f;
          if (!_eod)
          {
#ifdef _WIN32
            if (_next_eod)
            {
              _eod = true;
            }
            else
            {
              f = _data.cFileName;
              _next_eod = !FindNextFile(_d, &_data);
            }
#else
            if (dirent* d = readdir(_d))
            {
              f = d->d_name;
            }
            else
            {
              _eod = true;
            }
#endif
          }

          return (!_eod && (f == "." || f == "..")) ? next_file() : f;
        }
      private:
#ifdef _WIN32
        bool _next_eod;

        typedef HANDLE handle_t;
        typedef WIN32_FIND_DATA data_t;

        static handle_t start_search(
          const std::string& path_,
          data_t& data_
        )
        {
          const std::string p = path_ + "\\*";
          return FindFirstFile(p.c_str(), &data_);
        }

        static void close_search(handle_t h_)
        {
          FindClose(h_);
        }

        static bool valid(handle_t h_)
        {
          return h_ != INVALID_HANDLE_VALUE;
        }
#else
        typedef DIR* handle_t;
        typedef char data_t;

        static handle_t start_search(
          const std::string& path_,
          data_t&
        )
        {
          return opendir(path_.c_str());
        }

        static void close_search(handle_t h_)
        {
          closedir(h_);
        }

        static bool valid(handle_t h_)
        {
          return h_;
        }
#endif
        data_t _data;
        handle_t _d;
        bool _eod;

        // Non-copyable
        dir_t(const dir_t&);
        dir_t& operator=(const dir_t&);
      };

      static bool is_directory(const std::string& path_)
      {
#ifdef _WIN32
        const DWORD attrs = GetFileAttributes(path_.c_str());
        return
          attrs != INVALID_FILE_ATTRIBUTES
            && (attrs & FILE_ATTRIBUTE_DIRECTORY);
#else
        struct stat buf;
        if (stat(path_.c_str(), &buf) == 0)
        {
          return S_ISDIR(buf.st_mode);
        }
        else
        {
          return false;
        }
#endif
      }

#ifdef _WIN32
      static std::string get_temp_path()
      {
        const DWORD len = GetTempPath(0, 0) + 1;
        std::vector<char> buff(len);
        if (GetTempPath(len, &buff[0]) > len)
        {
          throw
            std::runtime_error("Error getting the path of the temp directory.");
        }
        return &buff[0];
      }
#endif

      static std::string path_separator()
      {
#ifdef _WIN32
        return "\\";
#else
        return "/";
#endif
      }

      static void delete_file(const std::string& path_)
      {
#ifdef _WIN32
        if (!DeleteFile(path_.c_str()))
#else
        if (unlink(path_.c_str()))
#endif
        {
          throw std::runtime_error("Error deleting file " + path_);
        }
      }

      static void delete_empty_dir(const std::string& path_)
      {
#ifdef _WIN32
        if (!RemoveDirectory(path_.c_str()))
#else
        if (rmdir(path_.c_str()))
#endif
        {
          throw std::runtime_error("Error deleting empty directory " + path_);
        }
      }

      static void delete_dir(const std::string& path_)
      {
        {
          dir_t d(path_);
          for (std::string e = d.next_file(); !d.eod(); e = d.next_file())
          {
            const std::string fp = path_ + path_separator() + e;
            if (is_directory(fp))
            {
              delete_dir(fp);
            }
            else
            {
              delete_file(fp);
            }
          }
        }
        delete_empty_dir(path_);
      }
    };
  }
}

#endif

