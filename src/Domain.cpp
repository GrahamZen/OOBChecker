#include "Domain.h"
#include <utility>
#include <algorithm>

#ifndef UNIT_TEST
#include <llvm/IR/Constants.h>
#endif

//===----------------------------------------------------------------------===//
// Abstract Domain Implementation
//===----------------------------------------------------------------------===//

namespace dataflow {

IntervalDomain::IntervalDomain(const llvm::Value *val) {
#ifdef UNIT_TEST
  (void) val;
  unknown = true;
#else
  if (auto ci = llvm::dyn_cast<llvm::ConstantInt>(val)) {
    lo = hi = ci->getSExtValue();
  } else {
    unknown = true;
  }
#endif
}

void IntervalDomain::cut(const IntervalDomain &other) {
  if (unknown || other.unknown) return;
  if (!overlaps(other)) return;
  if (lo <= other.lo) {
    hi = std::min(hi, other.lo - 1);
  } else {
    lo = std::max(lo, other.hi + 1);
  }
}

IntervalDomain& IntervalDomain::operator&=(const IntervalDomain& other) {
  if (unknown || other.unknown) {
    return *this = UNINIT();
  }
  if (!overlaps(other)) {
    return *this = EMPTY();
  }
  lo = std::max(lo, other.lo);
  hi = std::min(hi, other.hi);
  return *this;
}
IntervalDomain& IntervalDomain::operator|=(const IntervalDomain& other) {
  if (unknown || other.unknown) {
    return *this = UNINIT();
  }
  lo = std::min(lo, other.lo);
  hi = std::max(hi, other.hi);
  return *this;
}

IntervalDomain& IntervalDomain::operator+=(const IntervalDomain &other) {
  if (unknown || other.unknown) {
    return *this = UNINIT();
  }
  lo += other.lo;
  hi += other.hi;
  return *this;
}
IntervalDomain& IntervalDomain::operator-=(const IntervalDomain &other) {
  if (unknown || other.unknown) {
    return *this = UNINIT();
  }
  lo -= other.hi;
  hi -= other.lo;
  return *this;
}
IntervalDomain& IntervalDomain::operator*=(const IntervalDomain &other) {
  if (unknown || other.unknown) {
    return *this = UNINIT();
  }
  auto it = std::minmax({lo * other.lo, lo * other.hi, hi * other.lo, hi * other.hi});
  lo = it.first;
  hi = it.second;
  return *this;
}
IntervalDomain& IntervalDomain::operator/=(const IntervalDomain &other) {
  if (unknown || other.unknown) {
    return *this = UNINIT();
  }

  if (other.contains(0)) {
    auto div = [](int x, int y) {
      if (y == 0) return INF;
      return x / y;
    };
    lo = std::min({div(lo, other.lo), div(lo, other.hi), div(hi, other.lo), div(hi, other.hi)});
    hi = INF;
  } else {
    auto it = std::minmax({lo / other.lo, lo / other.hi, hi / other.lo, hi / other.hi});
    lo = it.first;
    hi = it.second;
  }
  return *this;
}
bool IntervalDomain::operator==(const IntervalDomain &other) const {
  if (unknown ^ other.unknown) return false;
  if (lo > hi && other.lo > other.hi) return true;
  return (unknown && other.unknown) || (lo == other.lo && hi == other.hi);
}

void IntervalDomain::print(llvm::raw_ostream &os) const {
  os << "[" << lo << ", " << hi << "]";
}

} // namespace dataflow
