#ifndef JUST_TEST_HPP
#define JUST_TEST_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace just
{
  namespace test
  {
    /*
     * Customisation point: this template function can be specialised to display
     * type that don't support streams without overloading the streaming
     * operator.
     */
    template <class T>
    void display(std::ostream& out_, const T& t_)
    {
      out_ << t_;
    }

    /*
     * assertion_failed
     */
    class assertion_failed
    {
    public:
      assertion_failed(
        const std::string& msg_,
        const std::string& filename_,
        int line_
      ) :
        _msg(msg_),
        _filename(filename_),
        _line(line_)
      {}

      const std::string& msg() const
      {
        return _msg;
      }

      const std::string& filename() const
      {
        return _filename;
      }

      int line() const
      {
        return _line;
      }
    private:
      std::string _msg;
      std::string _filename;
      int _line;
    };

    /*
     * test_case
     */
    class test_case
    {
    public:
      test_case(const std::string& name_, void (*f_)()) :
        _f(f_),
        _name(name_)
      {}

      bool run(std::ostream& report_) const
      {
        report_ << "Running " << _name << std::endl;
        try
        {
          _f();
          return true;
        }
        catch (const assertion_failed& e_)
        {
          report_
            << "  Assertion failed" << std::endl
            << "    " << e_.msg() << std::endl
            << "    Test case: " << _name << std::endl
            << "    Location: " << e_.filename() << ":" << e_.line() <<std::endl
            ;
        }
        catch (const std::exception& e_)
        {
          report_
            << "  std::exception thrown" << std::endl
            << "    what: " << e_.what() << std::endl
            << "    Test case: " << _name << std::endl;
        }
        catch (...)
        {
          report_
            << "  Exception thrown" << std::endl
            << "    Test case: " << _name << std::endl;
        }
        return false;
      }
    private:
      void (*_f)();
      std::string _name;
    };

    /*
     * test_manager
     */
    class test_manager
    {
    public:
      void add(const std::string& name_, void (*f_)())
      {
        _cases.push_back(test_case(name_, f_));
      }

      bool run(std::ostream& report_) const
      {
        int failures = 0;
        for (
          std::vector<test_case>::const_iterator
            i = _cases.begin(),
            e = _cases.end();
          i != e;
          ++i
        )
        {
          if (!i->run(report_))
          {
            ++failures;
          }
        }
        report_ << _cases.size() << " test cases finished." << std::endl;
        if (failures > 0)
        {
          report_ << "  " << failures << " failed." << std::endl;
        }
        else
        {
          report_ << "  All succeeded." << std::endl;
        }
        return failures == 0;
      }
    private:
      std::vector<test_case> _cases;
    };

    /*
     * singleton
     */
    // This assumes, that pointers are 0 initialised at startup
    // It never destroys the object
    template <class T>
    class singleton
    {
    public:
      static T& get()
      {
        if (!_t)
        {
          _t = new T();
        }
        return *_t;
      }
    private:
      static T* _t;
    };

    template <class T>
    T* singleton<T>::_t = 0;

    /*
     * run
     */
    inline int run(int, char *[])
    {
      return singleton<test_manager>::get().run(std::cerr) ? 0 : 1;
    }

    /*
     * assert_msg
     */
    class assert_msg
    {
    public:
      assert_msg(const std::string& filename_, int line_) :
        _filename(filename_),
        _line(line_)
      {}

      void operator()(const std::string& msg_, bool v_) const
      {
        if (!v_)
        {
          throw assertion_failed(msg_, _filename, _line);
        }
      }

      void operator()(bool v_) const
      {
        operator()("Assertion failed", v_);
      }
    private:
      std::string _filename;
      int _line;
    };

    /*
     * assert operations
     */
  #ifdef JUST_TEST_ASSERT_OP
  #  error JUST_TEST_ASSERT_OP already defined
  #endif
  #define JUST_TEST_ASSERT_OP(name, pred) \
    class name \
    { \
    public: \
      name(const std::string& filename_, int line_) : \
        _assert_msg(filename_, line_) \
      {} \
      \
      template <class A, class B> \
      void operator()(A a_, B b_) const \
      { \
        std::ostringstream s; \
        s << "Expected: "; \
        ::just::test::display(s, a_); \
        s << " " #pred " "; \
        ::just::test::display(s, b_); \
        _assert_msg(s.str(), a_ pred b_); \
      } \
    private: \
      assert_msg _assert_msg; \
    }

    JUST_TEST_ASSERT_OP(assert_equal, ==);
    JUST_TEST_ASSERT_OP(assert_not_equal, !=);
    JUST_TEST_ASSERT_OP(assert_less, <);
    JUST_TEST_ASSERT_OP(assert_less_equal, <=);
    JUST_TEST_ASSERT_OP(assert_greater, >);
    JUST_TEST_ASSERT_OP(assert_greater_equal, >=);

  #undef JUST_TEST_ASSERT_OP

    template <class It>
    static void show(It begin_, It end_, std::ostream& o_)
    {
      o_ << "[";
      bool first = true;
      for (It i = begin_; i != end_; ++i)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          o_ << ", ";
        }
        ::just::test::display(o_, *i);
      }
      o_ << "]";
    }

    template <class It1, class It2>
    static bool equal(It1 b1_, It1 e1_, It2 b2_, It2 e2_)
    {
      while (b1_ != e1_ && b2_ != e2_ && *b1_ == *b2_)
      {
        ++b1_; ++b2_;
      }
      return b1_ == e1_ && b2_ == e2_;
    }

    template <class T>
    struct differentiate_from_other_vectors : std::allocator<T> {};

    class assert_equal_container
    {
    public:
      assert_equal_container(const std::string& filename_, int line_) :
        _assert_msg(filename_, line_)
      {}
      
      template <class A, class B>
      void operator()(const A& a_, const B& b_) const
      {
        run(a_, b_);
      }

      // This handles the case when the right operand is a brace initialised
      // container in C++11. For example:
      //   JUST_ASSERT_EQUAL_CONTAINER(some_container, {1, 2, 3})
      // I need to convert it into a vector, because using
      // std::initializer_list<T> would break in legacy C++98 mode.
      // The special handling is needed because the compiler is not allowed to
      // deduce std::initializer_list types.
      template <class A>
      void operator()(
        const A& a_,
        const std::vector<
          typename A::value_type,
          differentiate_from_other_vectors<typename A::value_type>
        >& b_
      ) const
      {
        run(a_, b_);
      }

      // This handles the case when the left operand is a brace initialised
      // container in C++11. For example:
      //   JUST_ASSERT_EQUAL_CONTAINER({1, 2, 3}, some_container)
      template <class B>
      void operator()(
        const std::vector<
          typename B::value_type,
          differentiate_from_other_vectors<typename B::value_type>
        >& a_,
        const B& b_
      ) const
      {
        run(a_, b_);
      }
    private:
      assert_msg _assert_msg;

      // When this is called the initializer_list arguments are converted into
      // vectors.
      template <class A, class B>
      void run(const A& a_, const B& b_) const
      {
        std::ostringstream s;
        s << "Expected: ";
        show(a_.begin(), a_.end(), s);
        s << " == ";
        show(b_.begin(), b_.end(), s);

        _assert_msg(
          s.str(),
          ::just::test::equal(a_.begin(), a_.end(), b_.begin(), b_.end())
        );
      }
    };

    class assert_empty_container
    {
    public:
      assert_empty_container(const std::string& filename_, int line_) :
        _assert_msg(filename_, line_)
      {}
      
      template <class A>
      void operator()(const A& a_) const
      {
        std::ostringstream s;
        s << "Expected to be empty: ";
        show(a_.begin(), a_.end(), s);

        _assert_msg(s.str(), a_.empty());
      }
    private:
      assert_msg _assert_msg;
    };

    namespace impl
    {
      class no_checks
      {
      public:
        template <class E>
        void operator()(const E&) {}
      };

      template <class C1, class C2>
      class check_sequence
      {
      public:
        check_sequence(const C1& c1_, const C2& c2_) :
          _c1(c1_),
          _c2(c2_)
        {}

        template <class E>
        void operator()(const E& e_)
        {
          _c1(e_);
          _c2(e_);
        }
      private:
        C1 _c1;
        C2 _c2;
      };

      class UNTYPED_EXCEPTION_OBJECT_CAN_NOT_BE_ACCESSED;

      class nothing_to_run
      {
      public:
        template <class NextCheckExceptionF>
        // name is expected to be shown in error reports
        UNTYPED_EXCEPTION_OBJECT_CAN_NOT_BE_ACCESSED************************
        check_exception(const NextCheckExceptionF&);
      };

      template <class ExpectedException, class F, class CheckExceptionF>
      class assert_throw
      {
      public:
        friend class assert_throw_runner;

        assert_throw(
          const assert_msg& assert_msg_,
          const F& f_,
          const CheckExceptionF& check_exception_
        ) :
          _assert_msg(assert_msg_),
          _f(f_),
          _check_exception(check_exception_)
        {}

        template <class NextCheckExceptionF>
        assert_throw<
          ExpectedException,
          F,
          check_sequence<CheckExceptionF, NextCheckExceptionF>
        >
        check_exception(const NextCheckExceptionF& f_)
        {
          return
            assert_throw<
              ExpectedException,
              F,
              check_sequence<CheckExceptionF, NextCheckExceptionF>
            >(
              _assert_msg,
              _f,
              check_sequence<CheckExceptionF, NextCheckExceptionF>(
                _check_exception,
                f_
              )
            );
        }
      private:
        assert_msg _assert_msg;
        F _f;
        CheckExceptionF _check_exception;

        void run()
        {
          std::string reason;
          try
          {
            _f();
            reason = "Expression did not throw.";
          }
          catch (const ExpectedException& e_)
          {
            _check_exception(e_);
            return;
          }
          catch (...)
          {
            reason =
              "Expression threw a different exception than what was expected.";
          }

          _assert_msg(reason, false);
        }
      };

      class assert_throw_runner
      {
      public:
        template <class ExpectedException, class F, class CheckExceptionF>
        void operator=(assert_throw<ExpectedException, F, CheckExceptionF> at_)
        {
          at_.run();
        }

        void operator=(const nothing_to_run&) {}
      };
    }

    class assert_throw
    {
    public:
      assert_throw(const std::string& filename_, int line_) :
        _assert_msg(filename_, line_)
      {}
      
      template <class ExpectedException, class F>
      impl::assert_throw<ExpectedException, F, impl::no_checks> run(F f_) const
      {
        return
          impl::assert_throw<ExpectedException, F, impl::no_checks>(
            _assert_msg,
            f_,
            impl::no_checks()
          );
      }

      template <class F>
      impl::nothing_to_run run(F f_) const
      {
        try
        {
          f_();
        }
        catch (...)
        {
          return impl::nothing_to_run();
        }

        _assert_msg("Expression did not throw.", false);

        return impl::nothing_to_run();
      }
    private:
      assert_msg _assert_msg;
    };

    /*
     * what_returns (Functor for checking exception)
     */
    class what_returns
    {
    public:
      what_returns(
        const std::string& filename_,
        int line_,
        const std::string& expected_what_
      ) :
        _assert_msg(filename_, line_),
        _expected_what(expected_what_)
      {}

      template <class E>
      void operator()(const E& e_)
      {
        const std::string w = e_.what();

        std::ostringstream s;
        s
          << ".what() of the exception returned \"" << w << "\" but \""
          << _expected_what << "\" was expected.";

        _assert_msg(s.str(), w == _expected_what);
      }

      class builder
      {
      public:
        builder(const std::string& filename_, int line_) :
          _filename(filename_),
          _line(line_)
        {}

        what_returns operator()(const std::string& expected_what_)
        {
          return what_returns(_filename, _line, expected_what_);
        }
      private:
        std::string _filename;
        int _line;
      };
    private:
      assert_msg _assert_msg;
      std::string _expected_what;
    };
  }
}

/*
 * Test case definition
 */
#ifdef JUST_TEST_CASE
#  error JUST_TEST_CASE already defined
#endif
#define JUST_TEST_CASE(name) \
  void just_test_test_case_f_ ## name(); \
  \
  struct just_test_test_case_ ## name \
  { \
    just_test_test_case_ ## name() \
    { \
      ::just::test::singleton< ::just::test::test_manager >::get().add( \
        #name, &just_test_test_case_f_ ## name \
      ); \
    } \
  }; \
  just_test_test_case_ ## name just_test_test_case_instance_ ## name; \
  \
  void just_test_test_case_f_ ## name()

/*
 * Assertion macros
 */
#ifdef JUST_ASSERT
#  error JUST_ASSERT already defined
#endif
#define JUST_ASSERT ::just::test::assert_msg(__FILE__, __LINE__)

#ifdef JUST_ASSERT_EQUAL
#  error JUST_ASSERT_EQUAL already defined
#endif
#define JUST_ASSERT_EQUAL ::just::test::assert_equal(__FILE__, __LINE__)

#ifdef JUST_ASSERT_EQUAL_CONTAINER
#  error JUST_ASSERT_EQUAL_CONTAINER already defined
#endif
#define JUST_ASSERT_EQUAL_CONTAINER \
  ::just::test::assert_equal_container(__FILE__, __LINE__)

#ifdef JUST_ASSERT_NOT_EQUAL
#  error JUST_ASSERT_NOT_EQUAL already defined
#endif
#define JUST_ASSERT_NOT_EQUAL ::just::test::assert_not_equal(__FILE__, __LINE__)

#ifdef JUST_ASSERT_LESS
#  error JUST_ASSERT_LESS already defined
#endif
#define JUST_ASSERT_LESS ::just::test::assert_less(__FILE__, __LINE__)

#ifdef JUST_ASSERT_LESS_EQUAL
#  error JUST_ASSERT_LESS_EQUAL already defined
#endif
#define JUST_ASSERT_LESS_EQUAL \
  ::just::test::assert_less_equal(__FILE__, __LINE__)

#ifdef JUST_ASSERT_GREATER
#  error JUST_ASSERT_GREATER already defined
#endif
#define JUST_ASSERT_GREATER ::just::test::assert_greater(__FILE__, __LINE__)

#ifdef JUST_ASSERT_GREATER_EQUAL
#  error JUST_ASSERT_GREATER_EQUAL already defined
#endif
#define JUST_ASSERT_GREATER_EQUAL \
  ::just::test::assert_greater_equal(__FILE__, __LINE__)

#ifdef JUST_ASSERT_EMPTY_CONTAINER
#  error JUST_ASSERT_EMPTY_CONTAINER already defined
#endif
#define JUST_ASSERT_EMPTY_CONTAINER \
  ::just::test::assert_empty_container(__FILE__, __LINE__)

#ifdef JUST_ASSERT_THROWS
#  error JUST_ASSERT_THROWS
#endif
#define JUST_ASSERT_THROWS \
  just::test::impl::assert_throw_runner() = \
    ::just::test::assert_throw(__FILE__, __LINE__).run

#    ifdef JUST_WHAT_RETURNS
#      error JUST_WHAT_RETURNS already defined
#    endif
#    define JUST_WHAT_RETURNS \
      ::just::test::what_returns::builder(__FILE__, __LINE__)

/*
 * Macro for defining a main function
 */
#ifdef JUST_TEST_DEFINE_MAIN
#  error JUST_TEST_DEFINE_MAIN already defined
#endif
#define JUST_TEST_DEFINE_MAIN \
  int main(int argc_, char* argv_[]) \
  { \
    return ::just::test::run(argc_, argv_); \
  }

#endif

