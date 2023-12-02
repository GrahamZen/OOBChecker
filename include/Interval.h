#pragma once

#include <limits>

#ifdef UNIT_TEST
#include <iostream>
namespace llvm {
using raw_ostream = std::ostream;
using Value = int;
#define outs() std::cout
}
#else
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Value.h>
#endif

namespace dataflow {
class Interval {
  int lo, hi;
public:
  static const int INT_INF = std::numeric_limits<int>::max();
  static const int INT_NEG_INF = std::numeric_limits<int>::min();

  Interval(int lo, int hi) : lo(lo), hi(hi) {}
  Interval(int val) : lo(val), hi(val) {}

  static Interval INF() { return Interval(INT_NEG_INF, INT_INF); }
  static Interval EMPTY() { return Interval(2,1); }

  bool contains(int val) const {
    return lo <= val && val <= hi;
  }
  bool contains(const Interval &other) const {
    return lo <= other.lo && other.hi <= hi;
  }
  bool overlaps(const Interval &other) const {
    return lo <= other.hi && other.lo <= hi;
  }
  void cut(const Interval &other);

  bool isEmpty() const { return lo > hi; }

  int lower() const { return lo; }
  int upper() const { return hi; }

  /**
   * @brief intersects a domain with another one
   * @param other the domain to be intersected with.
   * @return the intersected domain.
   */
  Interval& operator&=(const Interval& other);
  /**
   * @brief combines the information contained in two domains.
   * @param other the domain to be joined with.
   * @return the joined domain.
   */
  Interval& operator|=(const Interval& other);
  Interval& operator+=(const Interval &other);
  Interval& operator-=(const Interval &other);
  Interval& operator*=(const Interval &other);
  Interval& operator/=(const Interval &other);
  bool operator==(const Interval &other) const;

  Interval operator&(const Interval& other) const {
    Interval res = *this;
    res &= other;
    return res;
  }
  Interval operator|(const Interval& other) const {
    Interval res = *this;
    res |= other;
    return res;
  }
  Interval operator+(const Interval &other) const {
    Interval res = *this;
    res += other;
    return res;
  }
  Interval operator-(const Interval &other) const {
    Interval res = *this;
    res -= other;
    return res;
  }
  Interval operator*(const Interval &other) const {
    Interval res = *this;
    res *= other;
    return res;
  }
  Interval operator/(const Interval &other) const {
    Interval res = *this;
    res /= other;
    return res;
  }
  Interval operator-() const {
    return Interval(-hi, -lo);
  }
  bool operator!=(const Interval &other) const {
    return !(*this == other);
  }
  void print(llvm::raw_ostream &os) const;
};

inline Interval operator+(int lhs, const Interval &rhs) {
  return Interval { lhs } + rhs;
}
inline Interval operator-(int lhs, const Interval &rhs) {
  return Interval { lhs } - rhs;
}
inline Interval operator*(int lhs, const Interval &rhs) {
  return Interval { lhs } * rhs;
}
inline Interval operator/(int lhs, const Interval &rhs) {
  return Interval { lhs } / rhs;
}
inline bool operator==(int lhs, const Interval &rhs) {
  return Interval { lhs } == rhs;
}
inline bool operator!=(int lhs, const Interval &rhs) {
  return Interval { lhs } != rhs;
}
inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const Interval& interval) {
  os << "[" << interval.lower() << ", " << interval.upper() << "]";
  return os;
}

} // namespace dataflow