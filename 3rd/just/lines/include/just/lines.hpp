#ifndef JUST_LINES_HPP
#define JUST_LINES_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <iosfwd>
#include <fstream>

namespace just
{
  namespace lines
  {
    namespace impl
    {
      template <class T>
      struct remove_const { typedef T type; };

      template <class T>
      struct remove_const<const T> { typedef T type; };

      template <class Char>
      Char* end_of_c_string(Char* s_)
      {
        if (s_)
        {
          while (*s_)
          {
            ++s_;
          }
        }
        return s_;
      }

      template <class T, class>
      struct ignore_second
      {
        typedef T type;
      };
    }

    template <class InputIt, bool KeepNewlines = false>
    class iterator :
      public std::iterator<
        std::forward_iterator_tag,
        const std::basic_string<
          typename std::iterator_traits<InputIt>::value_type
        >
      >
    {
    public:
      iterator() : _at_end(true) {}
      iterator(InputIt begin_, InputIt end_) :
        _at(begin_),
        _end(end_),
        _at_end(false),
        _current(),
        _last_char(0)
      {
        if (_at != _end)
        {
          if (*_at != '\n')
          {
            ++(*this);
          }
          else if (KeepNewlines)
          {
            _current = "\n";
          }
        }
      }

      bool operator==(const iterator& it_) const
      {
        return
          (_at_end && it_._at_end)
          || (!_at_end && !it_._at_end && _at == it_._at);
      }
      bool operator<(const iterator& it_) const
      {
        return !_at_end && (it_.at_end || _at < it_._at);
      }

      bool operator!=(const iterator& it_) const { return !(*this == it_); }
      bool operator<=(const iterator& it_) const { return !(it_ < *this); }
      bool operator>(const iterator& it_) const { return it_ < *this; }
      bool operator>=(const iterator& it_) const { return !(*this < it_); }

      typename iterator::pointer operator->() const { return &_current; }
      typename iterator::reference operator*() const { return _current; }

      iterator& operator++()
      {
        assert(!_at_end);
        _current.clear();
        if (_at == _end)
        {
          if (_last_char == '\r')
          {
            _last_char = 0;
          }
          else
          {
            _at_end = true;
          }
        }
        else
        {
          do
          {
            if ((KeepNewlines || *_at != '\r') && *_at != '\n')
            {
              _current += *_at;
            }
            next_char();
          }
          while (_at != _end && *_at != '\n' && _last_char != '\r');
          if (KeepNewlines && _at != _end && *_at == '\n')
          {
            _current += '\n';
          }
        }

        return *this;
      }

      iterator operator++(int)
      {
        iterator i(*this);
        ++(*this);
        return i;
      }
    private:
      InputIt _at;
      InputIt _end;
      bool _at_end;
      std::basic_string<typename std::iterator_traits<InputIt>::value_type>
        _current;
      typename std::iterator_traits<InputIt>::value_type _last_char;

      void next_char()
      {
        assert(!_at_end);
        assert(_at != _end);

        _last_char = *_at;
        ++_at;
      }
    };

    template <bool KeepNewlines, class String>
    iterator<typename String::const_iterator, KeepNewlines>
    begin_lines(const String& s_)
    {
      return iterator<typename String::const_iterator, KeepNewlines>(
        s_.begin(),
        s_.end()
      );
    }
    
    template <class String>
    iterator<typename String::const_iterator, false>
    begin_lines(const String& s_)
    {
      return begin_lines<false>(s_);
    }
    
    template <bool KeepNewlines>
    iterator<std::istream_iterator<char>, KeepNewlines>
    begin_lines(std::istream& s_)
    {
      s_ >> std::noskipws;
      return
        iterator<std::istream_iterator<char>, KeepNewlines>(
          std::istream_iterator<char>(s_),
          std::istream_iterator<char>()
        );
    }
    
    inline
    iterator<std::istream_iterator<char>, false>
    begin_lines(std::istream& s_)
    {
      return begin_lines<false>(s_);
    }
    
    template <bool KeepNewlines, class String>
    iterator<typename String::const_iterator, KeepNewlines>
    end_lines(const String&)
    {
      return iterator<typename String::const_iterator, KeepNewlines>();
    }
    
    template <class String>
    iterator<typename String::const_iterator, false> end_lines(const String&)
    {
      return iterator<typename String::const_iterator, false>();
    }
    
    template <bool KeepNewlines>
    iterator<std::istream_iterator<char>, KeepNewlines>
    end_lines(std::istream& s_)
    {
      return iterator<std::istream_iterator<char>, KeepNewlines>();
    }

    inline
    iterator<std::istream_iterator<char>, false>
    end_lines(std::istream& s_)
    {
      return iterator<std::istream_iterator<char>, false>();
    }
    
    template <class String, bool KeepNewlines = false>
    class basic_view
    {
    public:
      typedef
        lines::iterator<typename String::const_iterator, KeepNewlines> iterator;
      typedef iterator const_iterator;

      explicit basic_view(const String& string_) : _string(&string_) {}
      // in c++11: explicit basic_view(String&&) = delete;

      iterator begin() const
      {
        return begin_lines<KeepNewlines>(*_string);
      }

      iterator end() const
      {
        return end_lines<KeepNewlines>(*_string);
      }

      bool empty() const
      {
        // There is always at least one line
        return false;
      }
    private:
      const String* _string;

      template <class OtherThanString>
      explicit basic_view(const OtherThanString&); // = delete;
    };

    template <bool KeepNewlines>
    class basic_view<std::istream, KeepNewlines>
    {
    public:
      typedef
        lines::iterator<std::istream_iterator<char>, KeepNewlines> iterator;
      typedef iterator const_iterator;

      explicit basic_view(std::istream& stream_) : _stream(&stream_) {}

      iterator begin()
      {
        return begin_lines<KeepNewlines>(*_stream);
      }

      iterator end()
      {
        return end_lines<KeepNewlines>(*_stream);
      }

      bool empty() const
      {
        // There is always at least one line
        return false;
      }
    private:
      std::istream* _stream;
    };

    typedef basic_view<std::string> view;

    template <bool KeepNewlines>
    class basic_file_view
    {
    public:
      typedef
        lines::iterator<std::istream_iterator<char>, KeepNewlines> iterator;
      typedef iterator const_iterator;

      explicit basic_file_view(const std::string& path_) :
        _file(path_.c_str(), std::ios_base::binary | std::ios_base::in)
      {}

      iterator begin()
      {
        return begin_lines<KeepNewlines>(_file);
      }

      iterator end()
      {
        return end_lines<KeepNewlines>(_file);
      }

      bool empty() const
      {
        // There is always at least one line
        return false;
      }

      operator basic_view<std::istream, KeepNewlines>()
      {
        return basic_view<std::istream, KeepNewlines>(_file);
      }
    private:
      std::ifstream _file;
    };

    typedef basic_file_view<false> file_view;

    template <bool KeepNewlines, class String>
    typename impl::ignore_second<
      basic_view<String, KeepNewlines>,
      typename String::const_iterator
    >::type
    view_of(const String& s_)
    {
      return basic_view<String, KeepNewlines>(s_);
    }

    template <class String>
    typename impl::ignore_second<
      basic_view<String, false>,
      typename String::const_iterator
    >::type
    view_of(const String& s_)
    {
      return basic_view<String, false>(s_);
    }

    template <bool KeepNewlines>
    basic_view<std::istream, KeepNewlines> view_of(std::istream& in_)
    {
      return basic_view<std::istream, KeepNewlines>(in_);
    }

    inline basic_view<std::istream, false> view_of(std::istream& in_)
    {
      return basic_view<std::istream, false>(in_);
    }

    template <bool KeepNewlines, class String, class Container>
    typename impl::ignore_second<void, typename String::const_iterator>::type
    split(const String& s_, Container& out_)
    {
      std::copy(
        begin_lines<KeepNewlines>(s_),
        end_lines<KeepNewlines>(s_),
        std::back_inserter(out_)
      );
    }

    template <class String, class Container>
    typename impl::ignore_second<void, typename String::const_iterator>::type
    split(const String& s_, Container& out_)
    {
      split<false>(s_, out_);
    }

    template <bool KeepNewlines, class Char, class Container>
    void split(Char* s_, Container& out_)
    {
      std::copy(
        iterator<Char*, KeepNewlines>(s_, impl::end_of_c_string(s_)),
        iterator<Char*, KeepNewlines>(),
        std::back_inserter(out_)
      );
    }

    template <class Char, class Container>
    void split(Char* s_, Container& out_)
    {
      split<false>(s_, out_);
    }

    template <bool KeepNewlines, class Container>
    void split(std::istream& in_, Container& out_)
    {
      std::copy(
        begin_lines<KeepNewlines>(in_),
        end_lines<KeepNewlines>(in_),
        std::back_inserter(out_)
      );
    }

    template <class Container>
    void split(std::istream& in_, Container& out_)
    {
      split<false>(in_, out_);
    }

    template <bool KeepNewlines, class Container>
    void split_lines_of_file(const std::string& path_, Container& out_)
    {
      std::ifstream f(path_.c_str(), std::ios_base::binary | std::ios_base::in);
      split<KeepNewlines>(f, out_);
    }

    template <class Container>
    void split_lines_of_file(const std::string& path_, Container& out_)
    {
      split_lines_of_file<false>(path_, out_);
    }

    template <bool KeepNewlines, class String>
    typename impl::ignore_second<
      std::vector<String>,
      typename String::const_iterator
    >::type
    split(const String& s_)
    {
      return
        std::vector<String>(
          begin_lines<KeepNewlines>(s_),
          end_lines<KeepNewlines>(s_)
        );
    }

    template <class String>
    typename impl::ignore_second<
      std::vector<String>,
      typename String::const_iterator
    >::type
    split(const String& s_)
    {
      return std::vector<String>(begin_lines<false>(s_), end_lines<false>(s_));
    }

    template <bool KeepNewlines, class Char>
    std::vector<std::basic_string<typename impl::remove_const<Char>::type> >
    split(Char* s_)
    {
      return
        std::vector<
          std::basic_string<typename impl::remove_const<Char>::type>
        >(
          iterator<Char*, KeepNewlines>(s_, impl::end_of_c_string(s_)),
          iterator<Char*, KeepNewlines>()
        );
    }

    template <class Char>
    std::vector<std::basic_string<typename impl::remove_const<Char>::type> >
    split(Char* s_)
    {
      return
        std::vector<
          std::basic_string<typename impl::remove_const<Char>::type>
        >(
          iterator<Char*, false>(s_, impl::end_of_c_string(s_)),
          iterator<Char*, false>()
        );
    }

    template <bool KeepNewlines>
    std::vector<std::string> split(std::istream& in_)
    {
      std::vector<std::string> result;
      split<KeepNewlines>(in_, result);
      return result;
    }

    inline std::vector<std::string> split(std::istream& in_)
    {
      std::vector<std::string> result;
      split<false>(in_, result);
      return result;
    }

    template <bool KeepNewlines>
    std::vector<std::string> split_lines_of_file(const std::string& path_)
    {
      std::vector<std::string> result;
      split_lines_of_file<KeepNewlines>(path_, result);
      return result;
    }

    inline
    std::vector<std::string>
    split_lines_of_file(const std::string& path_)
    {
      std::vector<std::string> result;
      split_lines_of_file<false>(path_, result);
      return result;
    }
  }
}

#endif

