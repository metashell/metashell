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
    inline int run(int argc_, char* argv_[])
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
        _assert(filename_, line_) \
      {} \
      \
      template <class A, class B> \
      void operator()(A a_, B b_) const \
      { \
        std::ostringstream s; \
        s << "Expected: " << a_ << " " #pred " " << b_; \
        _assert(s.str(), a_ pred b_); \
      } \
    private: \
      assert_msg _assert; \
    }
  
    JUST_TEST_ASSERT_OP(assert_equal, ==);
    JUST_TEST_ASSERT_OP(assert_not_equal, !=);
    JUST_TEST_ASSERT_OP(assert_less, <);
    JUST_TEST_ASSERT_OP(assert_less_equal, <=);
    JUST_TEST_ASSERT_OP(assert_greater, >);
    JUST_TEST_ASSERT_OP(assert_greater_equal, >=);
  
  #undef JUST_TEST_ASSERT_OP
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

#ifdef JUST_ASSERT_THROWS
#  error JUST_ASSERT_THROWS
#endif
#define JUST_ASSERT_THROWS(exc, expr) \
  { \
    bool __just_test_expression_threw = false; \
    try \
    { \
      (expr); \
    } \
    catch (const exc&) \
    { \
      __just_test_expression_threw = true; \
    } \
    catch (...) \
    { \
      throw ::just::test::assertion_failed( \
        "Expected to throw " #exc " but threw something else.", \
        __FILE__, \
        __LINE__ \
      ); \
    } \
    if (!__just_test_expression_threw) \
    { \
      throw \
        ::just::test::assertion_failed( \
          "Expected to throw " #exc, \
          __FILE__, \
          __LINE__ \
        ); \
    } \
  }

#ifdef JUST_ASSERT_THROWS_SOMETHING
#  error JUST_ASSERT_THROWS_SOMETHING
#endif
#define JUST_ASSERT_THROWS_SOMETHING(expr) \
  { \
    bool __just_test_expression_threw = false; \
    try \
    { \
      (expr); \
    } \
    catch (...) { \
      __just_test_expression_threw = true; \
    } \
    if (!__just_test_expression_threw) \
    { \
      throw \
        ::just::test::assertion_failed( \
          "Expected to throw ", \
          __FILE__, \
          __LINE__ \
        ); \
    } \
  }

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

