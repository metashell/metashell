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
    }

    template <class InputIt>
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
          ++(*this);
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
            if (*_at != '\r' && *_at != '\n')
            {
              _current += *_at;
            }
            next_char();
          }
          while (_at != _end && *_at != '\n' && _last_char != '\r');
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

    template <class String>
    iterator<typename String::const_iterator> begin_lines(const String& s_)
    {
      return iterator<typename String::const_iterator>(s_.begin(), s_.end());
    }
    
    template <class String>
    iterator<typename String::const_iterator> end_lines(const String&)
    {
      return iterator<typename String::const_iterator>();
    }
    
    template <class String>
    class basic_view
    {
    public:
      typedef lines::iterator<typename String::const_iterator> iterator;
      typedef iterator const_iterator;

      explicit basic_view(const String& string_) : _string(&string_) {}

      iterator begin() const
      {
        return begin_lines(*_string);
      }

      iterator end() const
      {
        return end_lines(*_string);
      }

      bool empty() const
      {
        // There is always at least one line
        return false;
      }
    private:
      const String* _string;
    };

    typedef basic_view<std::string> view;

    template <class String>
    basic_view<String> view_of(const String& s_)
    {
      return basic_view<String>(s_);
    }

    template <class String, class Container>
    void split(const String& s_, Container& out_)
    {
      std::copy(begin_lines(s_), end_lines(s_), std::back_inserter(out_));
    }

    template <class Char, class Container>
    void split(Char* s_, Container& out_)
    {
      std::copy(
        iterator<Char*>(s_, impl::end_of_c_string(s_)),
        iterator<Char*>(),
        std::back_inserter(out_)
      );
    }

    template <class String>
    std::vector<String> split(const String& s_)
    {
      return std::vector<String>(begin_lines(s_), end_lines(s_));
    }

    template <class Char>
    std::vector<std::basic_string<typename impl::remove_const<Char>::type> >
    split(Char* s_)
    {
      return
        std::vector<
          std::basic_string<typename impl::remove_const<Char>::type>
        >(
          iterator<Char*>(s_, impl::end_of_c_string(s_)),
          iterator<Char*>()
        );
    }
  }
}

#endif

