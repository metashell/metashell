#ifndef JUST_ENVIRONMENT_HPP
#define JUST_ENVIRONMENT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#ifdef _WIN32
#  include <windows.h>
#  include <vector>
#else
#  include <stdlib.h>
#endif

#include <stdexcept>
#include <algorithm>

namespace just
{
  namespace environment
  {
#ifdef _WIN32
    inline std::string path_separator() { return ";"; }
#endif

#ifndef _WIN32
    inline std::string path_separator() { return ":"; }
#endif
    namespace impl
    {
#ifdef _WIN32
      inline DWORD value_length(const std::string& name_)
      {
        // In case of an error, the returned value is 0
        return GetEnvironmentVariable(name_.c_str(), NULL, 0);
      }
#endif
      
      inline std::string prepend_to_path(
        const std::string& item_,
        const std::string& path_
      )
      {
        return path_.empty() ? item_ : item_ + path_separator() + path_;
      }

      inline bool ends_with(const std::string& s_, const std::string& suffix_)
      {
        if (s_.size() < suffix_.size())
        {
          return false;
        }
        else
        {
          return
            std::equal(s_.end() - suffix_.size(), s_.end(), suffix_.begin());
        }
      }

      inline std::string append_to_path(
        const std::string& item_,
        const std::string& path_
      )
      {
        const std::string sep = path_separator();

        if (path_.empty() || path_ == sep)
        {
          return item_ + path_;
        }
        else
        {
          return path_ + (ends_with(path_, sep) ? item_ + sep : sep + item_);
        }
      }
    }

#ifdef _WIN32
    inline bool exists(const std::string& name_)
    {
      if (GetEnvironmentVariable(name_.c_str(), NULL, 0) == 0)
      {
        return GetLastError() != ERROR_ENVVAR_NOT_FOUND;
      }
      else
      {
        return true;
      }
    }
#endif

#ifndef _WIN32
    inline bool exists(const std::string& name_)
    {
      return getenv(name_.c_str()) != 0;
    }
#endif

#ifdef _WIN32
    inline std::string get(const std::string& name_)
    {
      const DWORD len = impl::value_length(name_);
      if (len > 0)
      {
        std::vector<char> buff(len);
        if (GetEnvironmentVariable(name_.c_str(), &buff[0], len) == len - 1)
        {
          std::vector<char>::iterator i = buff.end();
          --i;
          return std::string(buff.begin(), i);
        }
      }
      return std::string();
    }
#endif

#ifndef _WIN32
    inline std::string get(const std::string& name_)
    {
      const char* s = getenv(name_.c_str());
      return s ? std::string(s) : std::string();
    }
#endif

#ifdef _WIN32
    inline void set(const std::string& name_, const std::string& value_)
    {
      if (!SetEnvironmentVariable(name_.c_str(), value_.c_str()))
      {
        throw std::runtime_error("Error setting " + name_ + "=" + value_);
      }
    }
#endif

#ifndef _WIN32
    inline void set(const std::string& name_, const std::string& value_)
    {
      if (setenv(name_.c_str(), value_.c_str(), 1))
      {
        throw std::runtime_error("Error setting " + name_ + "=" + value_);
      }
    }
#endif

#ifdef _WIN32
    inline void remove(const std::string& name_)
    {
      if (!SetEnvironmentVariable(name_.c_str(), NULL))
      {
        throw std::runtime_error("Error removing environment variable " + name_);
      }
    }
#endif

#ifndef _WIN32
    inline void remove(const std::string& name_)
    {
      ::unsetenv(name_.c_str());
    }
#endif

    inline void prepend_to_path(const std::string& item_)
    {
      set("PATH", impl::prepend_to_path(item_, get("PATH")));
    }

    inline void append_to_path(const std::string& item_)
    {
      set("PATH", impl::append_to_path(item_, get("PATH")));
    }

    class override_guard
    {
    public:
      override_guard(const std::string& name_, const std::string& value_) :
        _name(name_),
        _old_value(get(name_)),
        _existed(exists(name_))
      {
        set(name_, value_);
      }

      ~override_guard()
      {
        if (_existed)
        {
          set(_name, _old_value);
        }
        else
        {
          remove(_name);
        }
      }

      const std::string& old_value() const
      {
        return _old_value;
      }

      bool existed() const
      {
        return _existed;
      }
    private:
      std::string _name;
      std::string _old_value;
      bool _existed;

      override_guard(const override_guard&); // = delete
      override_guard& operator=(const override_guard&); // delete
    };
  }
}

#endif

