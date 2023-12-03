#include "Interval.h"
#include <utility>
#include <algorithm>

#ifndef UNIT_TEST
#include <llvm/IR/Constants.h>
#endif

namespace dataflow {
void Interval::cut(const Interval &other) {
  if (!overlaps(other)) return;
  if (lo <= other.lo) {
    hi = std::min(hi, other.lo - 1);
  } else {
    lo = std::max(lo, other.hi + 1);
  }
}

Interval& Interval::operator&=(const Interval& other) {
  if (!overlaps(other)) {
    return *this = EMPTY();
  }
  lo = std::max(lo, other.lo);
  hi = std::min(hi, other.hi);
  return *this;
}
Interval& Interval::operator|=(const Interval& other) {
  lo = std::min(lo, other.lo);
  hi = std::max(hi, other.hi);
  return *this;
}

Interval& Interval::operator+=(const Interval &other) {
  lo += other.lo;
  hi += other.hi;
  return *this;
}
Interval& Interval::operator-=(const Interval &other) {
  lo -= other.hi;
  hi -= other.lo;
  return *this;
}
Interval& Interval::operator*=(const Interval &other) {
  auto it = std::minmax({lo * other.lo, lo * other.hi, hi * other.lo, hi * other.hi});
  lo = it.first;
  hi = it.second;
  return *this;
}
Interval& Interval::operator/=(const Interval &other) {
  if (other.contains(0)) {
    lo = INT_NEG_INF;
    hi = INT_INF;
  } else {
    auto it = std::minmax({lo / other.lo, lo / other.hi, hi / other.lo, hi / other.hi});
    lo = it.first;
    hi = it.second;
  }
  return *this;
}
bool Interval::operator==(const Interval &other) const {
  return lo == other.lo && hi == other.hi;
}
} // namespace dataflow
