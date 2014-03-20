#ifndef JUST_CONSOLE_HPP
#define JUST_CONSOLE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef _WIN32
#  include <windows.h>
#endif

#include <iostream>
#include <cassert>

namespace just
{
  namespace console
  {
    namespace impl
    {
      template <class>
      class color
      {
      public:
        bool operator==(color c_) const { return _id == c_._id; }
        bool operator!=(color c_) const { return !(*this == c_); }
  
        static const color black;
        static const color red;
        static const color bright_red;
        static const color green;
        static const color bright_green;
        static const color yellow;
        static const color bright_yellow;
        static const color blue;
        static const color bright_blue;
        static const color magenta;
        static const color bright_magenta;
        static const color cyan;
        static const color bright_cyan;
        static const color gray;
        static const color white;
      private:
        color(int id_) : _id(id_) {}
  
        int _id;
      };
  
      template <class T> const color<T> color<T>::black(0);
      template <class T> const color<T> color<T>::red(1);
      template <class T> const color<T> color<T>::bright_red(2);
      template <class T> const color<T> color<T>::green(3);
      template <class T> const color<T> color<T>::bright_green(4);
      template <class T> const color<T> color<T>::yellow(5);
      template <class T> const color<T> color<T>::bright_yellow(6);
      template <class T> const color<T> color<T>::blue(7);
      template <class T> const color<T> color<T>::bright_blue(8);
      template <class T> const color<T> color<T>::magenta(9);
      template <class T> const color<T> color<T>::bright_magenta(10);
      template <class T> const color<T> color<T>::cyan(11);
      template <class T> const color<T> color<T>::bright_cyan(12);
      template <class T> const color<T> color<T>::gray(13);
      template <class T> const color<T> color<T>::white(14);
  
      template <class T>
      std::ostream& operator<<(std::ostream& o_, color<T> c_)
      {
             if (c_ == color<T>::black)          { o_ << "black"; }
        else if (c_ == color<T>::red)            { o_ << "red"; }
        else if (c_ == color<T>::green)          { o_ << "green"; }
        else if (c_ == color<T>::yellow)         { o_ << "yellow"; }
        else if (c_ == color<T>::blue)           { o_ << "blue"; }
        else if (c_ == color<T>::magenta)        { o_ << "magenta"; }
        else if (c_ == color<T>::cyan)           { o_ << "cyan"; }
        else if (c_ == color<T>::gray)           { o_ << "gray"; }
        else if (c_ == color<T>::bright_red)     { o_ << "bright_red"; }
        else if (c_ == color<T>::bright_green)   { o_ << "bright_green"; }
        else if (c_ == color<T>::bright_yellow)  { o_ << "bright_yellow"; }
        else if (c_ == color<T>::bright_blue)    { o_ << "bright_blue"; }
        else if (c_ == color<T>::bright_magenta) { o_ << "bright_magenta"; }
        else if (c_ == color<T>::bright_cyan)    { o_ << "bright_cyan"; }
        else if (c_ == color<T>::white)          { o_ << "white"; }
        else { assert(!"Invalid color value"); }
  
        return o_;
      }
    }
  
    typedef impl::color<void> color;
  
#ifdef _WIN32
    inline void text_color(color c_)
    {
      WORD c = 0;
      if (c_ == color::black)
      {
        c = 0;
      }
      else if (c_ == color::red)
      {
        c = FOREGROUND_RED;
      }
      else if (c_ == color::green)
      {
        c = FOREGROUND_GREEN;
      }
      else if (c_ == color::yellow)
      {
        c = FOREGROUND_RED | FOREGROUND_GREEN;
      }
      else if (c_ == color::blue)
      {
        c = FOREGROUND_BLUE;
      }
      else if (c_ == color::magenta)
      {
        c = FOREGROUND_RED | FOREGROUND_BLUE;
      }
      else if (c_ == color::cyan)
      {
        c = FOREGROUND_GREEN | FOREGROUND_BLUE;
      }
      else if (c_ == color::gray)
      {
        c = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
      }
      else if (c_ == color::bright_red)
      {
        c = FOREGROUND_INTENSITY | FOREGROUND_RED;
      }
      else if (c_ == color::bright_green)
      {
        c = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
      }
      else if (c_ == color::bright_yellow)
      {
        c = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
      }
      else if (c_ == color::bright_blue)
      {
        c = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
      }
      else if (c_ == color::bright_magenta)
      {
        c = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
      }
      else if (c_ == color::bright_cyan)
      {
        c = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
      }
      else if (c_ == color::white)
      {
        c =
          FOREGROUND_INTENSITY
          | FOREGROUND_RED
          | FOREGROUND_GREEN
          | FOREGROUND_BLUE;
      }
      else
      {
        assert(!"Invalid color value");
      }
  
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }
  
    inline void reset()
    {
      text_color(color::gray);
    }
  
#else
    inline void text_color(color c_)
    {
           if (c_ == color::black)          { std::cout << "\033[30m";   }
      else if (c_ == color::red)            { std::cout << "\033[31m";   }
      else if (c_ == color::green)          { std::cout << "\033[32m";   }
      else if (c_ == color::yellow)         { std::cout << "\033[33m";   }
      else if (c_ == color::blue)           { std::cout << "\033[34m";   }
      else if (c_ == color::magenta)        { std::cout << "\033[35m";   }
      else if (c_ == color::cyan)           { std::cout << "\033[36m";   }
      else if (c_ == color::gray)           { std::cout << "\033[37m";   }
      else if (c_ == color::bright_red)     { std::cout << "\033[1;31m"; }
      else if (c_ == color::bright_green)   { std::cout << "\033[1;32m"; }
      else if (c_ == color::bright_yellow)  { std::cout << "\033[1;33m"; }
      else if (c_ == color::bright_blue)    { std::cout << "\033[1;34m"; }
      else if (c_ == color::bright_magenta) { std::cout << "\033[1;35m"; }
      else if (c_ == color::bright_cyan)    { std::cout << "\033[1;36m"; }
      else if (c_ == color::white)          { std::cout << "\033[1;37m"; }
      else { assert(!"Invalid color value"); }
    }
  
    inline void reset()
    {
      std::cout << "\033[0m";
    }
#endif
  }
}

#endif

