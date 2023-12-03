#include "Domain.h"
#include <utility>
#include <algorithm>

#ifndef UNIT_TEST
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>

#endif

//===----------------------------------------------------------------------===//
// Abstract Domain Implementation
//===----------------------------------------------------------------------===//

namespace dataflow {

IntervalDomain::IntervalDomain(const llvm::Value *val) {
#ifdef UNIT_TEST
  (void) val;
  _unknown = true;
#else
  if (auto ci = llvm::dyn_cast<llvm::ConstantInt>(val)) {
    auto sval = ci->getSExtValue();
    _intervals.emplace_back(sval, sval);
    _unknown = false;
  } else if (auto gv = llvm::dyn_cast<llvm::GlobalVariable>(val)) {
    // global variable
    if (gv->hasInitializer()) {
      auto init = gv->getInitializer();
      if (auto ci = llvm::dyn_cast<llvm::ConstantInt>(init)) {
        auto sval = ci->getSExtValue();
        _intervals.emplace_back(sval, sval);
        _unknown = false;
      } else {
        _unknown = true;
      }
    } else {
      _unknown = true;
    }
  } else if (auto arg = llvm::dyn_cast<llvm::Argument>(val)) {
    // function argument
    if (arg->getType()->isIntegerTy()) {
      _intervals.emplace_back(Interval::INT_NEG_INF, Interval::INT_INF);
      _unknown = false;
    } else {
      _unknown = true;
    }
  } else if (auto inst = llvm::dyn_cast<llvm::Instruction>(val)) {
    // local variable
    if (auto alloca = llvm::dyn_cast<llvm::AllocaInst>(inst)) {
      if (alloca->getAllocatedType()->isIntegerTy()) {
        _intervals.emplace_back(Interval::INT_NEG_INF, Interval::INT_INF);
        _unknown = false;
      } else {
        _unknown = true;
      }
    } else if (auto call = llvm::dyn_cast<llvm::CallInst>(inst)) {
      if (call->getType()->isIntegerTy()) {
        _intervals.emplace_back(Interval::INT_NEG_INF, Interval::INT_INF);
        _unknown = false;
      } else {
        _unknown = true;
      }
    } else {
      _unknown = true;
    }
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
    if (interval.isEmpty()) continue;
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
  _intervals = std::move(newInterval);
}

IntervalDomain& IntervalDomain::genImpl(const IntervalDomain &other, 
  Interval& (Interval::*op)(const Interval&))
{
  if (_unknown || other._unknown)
    return *this = UNINIT();
  for (auto &interval : _intervals) {
    for (auto &otherInterval : other._intervals) {
      (interval.*op)(otherInterval);
    }
  }
  maintain();
  return *this;
}

IntervalDomain IntervalDomain::operator~() const {
  if (_unknown) return UNINIT();
  IntervalDomain ret;
  if (_intervals.empty()) {
    ret._intervals.emplace_back(Interval::INT_NEG_INF, Interval::INT_INF);
  } else {
    if (_intervals.front().lower() > Interval::INT_NEG_INF) {
      ret._intervals.emplace_back(Interval::INT_NEG_INF, _intervals.front().lower() - 1);
    }
    for (size_t i = 1; i < _intervals.size(); ++i) {
      ret._intervals.emplace_back(_intervals[i - 1].upper() + 1, _intervals[i].lower() - 1);
    }
    if (_intervals.back().upper() < Interval::INT_INF) {
      ret._intervals.emplace_back(_intervals.back().upper() + 1, Interval::INT_INF);
    }
  }
  return ret;
}

void IntervalDomain::clamp(int lo, int hi) {
  if (_unknown) return;
  for (auto &interval : _intervals) {
    interval &= Interval(lo, hi);
  }
  maintain();
}

bool IntervalDomain::operator==(const IntervalDomain &other) const {
  if (_unknown ^ other._unknown) return false;
  return (_unknown && other._unknown) || _intervals == other._intervals;
}

} // namespace dataflow
