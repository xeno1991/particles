#pragma once

/**
 * @file util.hpp
 *
 * @brief utilities: macros, and others
 */

#pragma once

#include <iostream>

/**
 * @brief disallow copy constructor and operator=
 * Use this in private section!
 *
 * @code
 * class Foo {
 *  public:
 *   Foo(int f);
 *   ~Foo();
 *  private:
 *   DISALLOW_COPY_AND_ASSIGN(Foo);
 * };
 * @endcode
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete

/**
 * @brief overloads std::begin and std::end
 */
#define OVERLOAD_STD_BEGIN_AND_END(TEMPLATE_PARAM, CLASS) \
  namespace std { \
  template <TEMPLATE_PARAM> \
  inline auto begin(CLASS& c) { \
    return c.begin(); \
  } \
  template <TEMPLATE_PARAM> \
  inline auto end(CLASS& c) { \
    return c.end(); \
  } \
  }

/**
 * @brief check condition
 *
 * @code
 * CHECK(1 == 1) << "one is one\n";     // output nothing
 * CHECK(1 == 2) << "one is two\n";     // terminated here!!
 * @endcode
 */
#define CHECK(cond)                                                       \
  ((cond) ? (particles::util::internal::CheckImpl(true, std::cerr) << "") \
          : (particles::util::internal::CheckImpl(false, std::cerr)       \
             << __FILE__ << ":" << __LINE__ << " " #cond " "))

namespace particles {
namespace util {

namespace internal {

/** @brief output nothing */
class NullOstream : public std::ostream {
  public:
    template <class T>
    std::ostream& operator<<(const T& t) {
      return *this;
    }
};

class CheckImpl {
 public:
  CheckImpl(bool cond, std::ostream& ost) : cond_(cond), ost_(ost) {}
  ~CheckImpl() {
    if (!cond_) std::exit(EXIT_FAILURE);
  }
  template <class T>
  std::ostream& operator<<(const T& t) {
    static NullOstream null_ostream;
    return cond_ ? null_ostream : (ost_ << t);
  }
 private:
 bool cond_;
  std::ostream& ost_;
};

template <class T>
struct Type {
  typedef T type;
};

}  // namespace internal

/**
 * @brief choose type following condition
 *
 * @code
 * class Hoge {
 *   ...
 * };
 * typedef Hoge value_type;
 *
 * // Hoge
 * typename type_cond<std::is_class<value_type>::value, value_type, int>::type a; 
 * @endcode
 */
template <bool cond, class T, class F>
struct type_cond;

template <class T, class F>
struct type_cond<true, T, F>  { typedef T type; };

template <class T, class F>
struct type_cond<false, T, F> { typedef F type; };


}  // namespace util
}  // namespace particles
