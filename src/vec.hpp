/**
 * \file vec.hpp
 *
 *
 */

#pragma once

#include "expression.hpp"
#include <array>
#include <initializer_list>

namespace particles {

template <class T, std::size_t N>
class Vec {
 public:
  typedef T value_type;
  typedef std::array<T, N> array_t;

  Vec() : value_() {}
  Vec(std::initializer_list<T> init_list);
  Vec(const Vec& v) : value_(v.value_) {}

  T& operator[](std::size_t i) { return value_[i]; }
  T& operator()(std::size_t i) { return (*this)[i]; }
  const T& operator[](std::size_t i) const { return value_[i]; }
  const T& operator()(std::size_t i) const { return (*this)[i]; }
  template <std::size_t I>
  T& get() { return std::get<I>(value_); }

  template <class E>
  Vec& operator=(const E& r);
  Vec& operator+=(const Vec& v);
  Vec& operator-=(const Vec& v);
  Vec& operator*=(T x);
  Vec& operator/=(T x);

  // Iterators
  auto begin() { value_.begin(); }
  auto end() { value_.end(); }
  auto cbegin() const { return value_.cbegin(); }
  auto cend() const { return value_.cend(); }
  auto rbegin() { value_.rbegin(); }
  auto rend() { value_.rend(); }
  auto crbegin() const { return value_.crbegin(); }
  auto crend() const { return value_.crend(); }

 private:
  array_t value_;
};

template <class T, std::size_t N>
Vec<T, N>::Vec(std::initializer_list<T> init_list) {
  // TODO: size check
  auto it = value_.begin();
  for (auto x : init_list) *(it++) = x;
}

template <class T, std::size_t N>
template <class E>
inline Vec<T, N>& Vec<T, N>::operator=(const E& r) {
  // expression::Assign<N, array_t, E>
  // TODO: Use template to expand this loop
  for (std::size_t i = 0; i < N; i++) (*this)[i] = r[i];
  return *this;
}

template <class T, std::size_t N>
inline Vec<T, N>& Vec<T, N>::operator+=(const Vec<T, N>& v) {
  for (std::size_t i = 0; i < N; ++i) (*this)[i] += v[i];
  return *this;
}

template <class T, std::size_t N>
inline Vec<T, N>& Vec<T, N>::operator-=(const Vec<T, N>& v) {
  for (std::size_t i = 0; i < N; ++i) (*this)[i] -= v[i];
  return *this;
}

template <class T, std::size_t N>
inline Vec<T, N>& Vec<T, N>::operator*=(T x) {
  for (std::size_t i = 0; i < N; ++i) (*this)[i] *= x;
  return *this;
}

template <class T, std::size_t N>
inline Vec<T, N>& Vec<T, N>::operator/=(T x) {
  for (std::size_t i = 0; i < N; ++i) (*this)[i] /= x;
  return *this;
}

}  // namespace particles

namespace std {

template <size_t I, class T, size_t N>
inline T& get(particles::Vec<T, N>& v) noexcept {
  return v.get<I>();
}

}  // namespace std
