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
    auto sval = ci->getSExtValue();
    _intervals.emplace_back(sval, sval);
    _unknown = false;
  } else {
    _unknown = true;
  }
#endif
}
void IntervalDomain::maintain() {
  auto newInterval = std::vector<Interval>();
  std::sort(_intervals.begin(), _intervals.end(), [](const Interval &a, const Interval &b) {
    return a.lower() < b.lower();
  });
  for (const auto &interval : _intervals) {
    if (newInterval.empty()) {
      newInterval.push_back(interval);
    } else {
      auto &last = newInterval.back();
      if (last.overlaps(interval)) {
        last |= interval;
      } else {
        newInterval.push_back(interval);
      }
    }
  }
}

IntervalDomain& IntervalDomain::genImpl(const IntervalDomain &other, 
  Interval& (Interval::*op)(const Interval&))
{
  if (_unknown || other._unknown)
    return *this = UNINIT();
  for (auto &interval : _intervals) {
    for (auto &otherInterval : other._intervals) {
      interval = (interval.*op)(otherInterval);
    }
  }
  maintain();
  return *this;
}

IntervalDomain& IntervalDomain::operator&=(const IntervalDomain& other) {
  if (_unknown || other._unknown) {
    return *this = UNINIT();
  }
  if (!overlaps(other)) {
    return *this = EMPTY();
  }

  return *this;
}
IntervalDomain& IntervalDomain::operator|=(const IntervalDomain& other) {
  if (_unknown || other._unknown) {
    return *this = UNINIT();
  }
  _intervals.insert(_intervals.end(), other._intervals.begin(), other._intervals.end());
  maintain();
  return *this;
}
bool IntervalDomain::operator==(const IntervalDomain &other) const {
  if (_unknown ^ other._unknown) return false;
  return (_unknown && other._unknown) || _intervals == other._intervals;
}

} // namespace dataflow
