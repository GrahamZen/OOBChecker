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

//===----------------------------------------------------------------------===//
// Abstract Domain Implementation
//===----------------------------------------------------------------------===//

/*
 * Implement your abstract domain.
 */
class IntervalDomain {
  static const int INF = std::numeric_limits<int>::max();
  static const int NEG_INF = std::numeric_limits<int>::min();

  int lo, hi;
  bool unknown { false };
public:
  IntervalDomain(int lo, int hi) : lo(lo), hi(hi) {}
  IntervalDomain(int val) : lo(val), hi(val) {}
  IntervalDomain() : unknown(true) {}
  IntervalDomain(const llvm::Value *val);

  static IntervalDomain INF_DOMAIN() { return IntervalDomain(NEG_INF, INF); }
  static IntervalDomain UNINIT() { return IntervalDomain(); }
  static IntervalDomain EMPTY() { return IntervalDomain(2,1); }

  bool contains(int val) const {
    if (unknown) return true;
    return lo <= val && val <= hi;
  }
  bool contains(const IntervalDomain &other) const {
    if (unknown) return true;
    return lo <= other.lo && other.hi <= hi;
  }
  bool overlaps(const IntervalDomain &other) const {
    if (unknown || other.unknown) return true;
    return lo <= other.hi && other.lo <= hi;
  }
  void cut(const IntervalDomain &other);
  
  bool isUnknown() const {
    return unknown;
  }
  bool isEmpty() const {
    return lo > hi;
  }

  int lower() const { return lo; }
  int upper() const { return hi; }

  /**
   * @brief intersects a domain with another one
   * @param other the domain to be intersected with.
   * @return the intersected domain.
   */
  IntervalDomain& operator&=(const IntervalDomain& other);
  /**
   * @brief combines the information contained in two domains.
   * @param other the domain to be joined with.
   * @return the joined domain.
   */
  IntervalDomain& operator|=(const IntervalDomain& other);
  IntervalDomain& operator+=(const IntervalDomain &other);
  IntervalDomain& operator-=(const IntervalDomain &other);
  IntervalDomain& operator*=(const IntervalDomain &other);
  IntervalDomain& operator/=(const IntervalDomain &other);
  bool operator==(const IntervalDomain &other) const;

  IntervalDomain operator&(const IntervalDomain& other) const {
    IntervalDomain res = *this;
    res &= other;
    return res;
  }
  IntervalDomain operator|(const IntervalDomain& other) const {
    IntervalDomain res = *this;
    res |= other;
    return res;
  }
  IntervalDomain operator+(const IntervalDomain &other) const {
    IntervalDomain res = *this;
    res += other;
    return res;
  }
  IntervalDomain operator-(const IntervalDomain &other) const {
    IntervalDomain res = *this;
    res -= other;
    return res;
  }
  IntervalDomain operator*(const IntervalDomain &other) const {
    IntervalDomain res = *this;
    res *= other;
    return res;
  }
  IntervalDomain operator/(const IntervalDomain &other) const {
    IntervalDomain res = *this;
    res /= other;
    return res;
  }
  IntervalDomain operator-() const {
    if (unknown) return *this;
    return IntervalDomain(-hi, -lo);
  }
  bool operator<(const IntervalDomain &other) const {
    return hi < other.lo;
  }
  bool operator>(const IntervalDomain &other) const {
    return lo > other.hi;
  }
  bool operator<=(const IntervalDomain &other) const {
    return hi <= other.lo;
  }
  bool operator>=(const IntervalDomain &other) const {
    return lo >= other.hi;
  }
  bool operator!=(const IntervalDomain &other) const {
    return !(*this == other);
  }
  void print(llvm::raw_ostream &os) const;
};

inline IntervalDomain operator+(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } + rhs;
}
inline IntervalDomain operator-(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } - rhs;
}
inline IntervalDomain operator*(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } * rhs;
}
inline IntervalDomain operator/(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } / rhs;
}
inline bool operator==(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } == rhs;
}
inline bool operator!=(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } != rhs;
}
inline bool operator<(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } < rhs;
}
inline bool operator>(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } > rhs;
}
inline bool operator<=(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } <= rhs;
}
inline bool operator>=(int lhs, const IntervalDomain &rhs) {
  return IntervalDomain { lhs } >= rhs;
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const IntervalDomain& domain ) {
  domain.print(os);
  return os;
}

} // namespace dataflow