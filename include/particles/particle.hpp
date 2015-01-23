/**
 * @file particle.hpp
 *
 * @brief particle: pack of position, velocity and mass
 */

#pragma once

#include "vec.hpp"

#include <initializer_list>
#include <functional>

namespace {
extern void* enabler;
}

namespace particles {
namespace internal {

template <class T>
struct Info {
  T val;
};

template <>
struct Info<void> {};

}  // namespace internal

/**
 * @brief particle moving in N dimension
 * @tparam T floating point
 * @tparam N dimension
 */
template <class T, std::size_t N, class I=void>
class Particle {
 public:
  typedef T value_type;
  static constexpr auto DIM = N;

  Particle() : position_(), velocity_(), mass_(1) {}

  Particle(const Vec<T, N>& x, const Vec<T, N>& v, T m=1)
      : position_(x), velocity_(v), mass_(m) {}

  /** @brief specify position using initializer_list 
   *  @todo unable to construct using emplace_back (see searcher_test.cpp:21)
   */
  Particle(std::initializer_list<T> init_list, T m = 1)
      : position_(init_list), velocity_(), mass_(m) {}

  /** @brief specify position and velocity using initializer_list */
  Particle(std::initializer_list<T> pos_init, std::initializer_list<T> vel_init,
           T m = 1)
      : position_(pos_init), velocity_(vel_init), mass_(m) {}

  Vec<T, N>& position() { return position_; }
  Vec<T, N>& velocity() { return velocity_; }
  T& mass() { return mass_; }
  const Vec<T, N>& position() const { return position_; }
  const Vec<T, N>& velocity() const { return velocity_; }
  const T& mass() const { return mass_; }

  /** @brief returns i-th element of position */
  T& position(std::size_t i) { return position_[i]; }
  /** @brief returns i-th element of velocity */
  T& velocity(std::size_t i) { return velocity_[i]; }
  /** @brief returns i-th element of position (const) */
  const T& position(std::size_t i) const { return position_[i]; }
  /** @brief returns i-th element of velocity (const) */
  const T& velocity(std::size_t i) const { return velocity_[i]; }

  T squared_distance(const Particle<T, N>& p) const {
    return position().squared_distance(p.position());
  }
  T squared_distance(const Particle<T, N>* p) const {
    return position().squared_distance(p->position());
  }

  /** @brief dimension of vector */
  static constexpr std::size_t dim() { return N; }

  template <class U=I, typename std::enable_if< std::is_void<U>::value>::type *& = enabler>
  void info() const {}

  template <class U=I, typename std::enable_if<!std::is_void<U>::value>::type *& = enabler>
  auto& info() { return info_.val; }

  template <class U=I, typename std::enable_if<!std::is_void<U>::value>::type *& = enabler>
  const auto& info() const { return info_.val; }

 private:
  Vec<T, N> position_;
  Vec<T, N> velocity_;
  internal::Info<I> info_;
  T mass_;
};

template <class T, std::size_t N>
T squared_distance(const Particle<T, N>& p, const Particle<T, N>& q) {
  return p.position().squared_distance(q.position());
}
template <class T, std::size_t N>
T squared_distance(const Particle<T, N>* p, const Particle<T, N>* q) {
  return p->position().squared_distance(q->position);
}

}  // namespace particles
