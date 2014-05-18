#ifndef JUST_TEMP_HPP
#define JUST_TEMP_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <stdexcept>

#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


namespace just
{
  namespace temp
  {
    class directory
    {
    public:
      directory()
      {
        char tmp[] = "/tmp/just-XXXXXX";
        if (char* const p = mkdtemp(tmp))
        {
          _path = p;
        }
        else
        {
          throw std::runtime_error("Error creating temporary directory");
        }
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
          _d(opendir(path_.c_str())),
          _eod(!_d)
        {}

        ~dir_t()
        {
          if (_d)
          {
            closedir(_d);
          }
        }

        bool eod() const
        {
          return _eod;
        }

        std::string next_file()
        {
          if (!_eod)
          {
            if (dirent* d = readdir(_d))
            {
              const std::string f(d->d_name);
              return (f == "." || f == "..") ? next_file() : f;
            }
            else
            {
              _eod = true;
            }
          }
          return std::string();
        }
      private:
        DIR* _d;
        bool _eod;

        // Non-copyable
        dir_t(const dir_t&);
        dir_t& operator=(const dir_t&);
      };

      static bool is_directory(const std::string& path_)
      {
        struct stat buf;
        if (stat(path_.c_str(), &buf) == 0)
        {
          return S_ISDIR(buf.st_mode);
        }
        else
        {
          return false;
        }
      }

      static void delete_dir(const std::string& path_)
      {
        {
          dir_t d(path_);
          for (std::string e = d.next_file(); !d.eod(); e = d.next_file())
          {
            const std::string fp = path_ + "/" + e;
            if (is_directory(fp))
            {
              delete_dir(fp);
            }
            else
            {
              unlink(fp.c_str());
            }
          }
        }
        rmdir(path_.c_str());
      }
    };
  }
}

#endif

