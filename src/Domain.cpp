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

  if (lo == hi && lo == 1) {
    if (!other.contains(0)) {
      lo = 1 / other.hi;
      hi = 1 / other.lo;
    } else if (other.hi == 0) {
      lo = NEG_INF;
      hi = 1 / other.lo;
    } else if (other.lo == 0) {
      lo = 1 / other.hi;
      hi = INF;
    } else {
      lo = NEG_INF;
      hi = INF;
    }
  } else {
    auto tmp = IntervalDomain {1} / other;
    *this *= tmp;
  }
  return *this;
}
bool IntervalDomain::operator==(const IntervalDomain &other) const {
  if (unknown ^ other.unknown) return false;
  return (unknown && other.unknown) || (lo == other.lo && hi == other.hi);
}

void IntervalDomain::print(llvm::raw_ostream &os) const {
  os << "[" << lo << ", " << hi << "]";
}

} // namespace dataflow
