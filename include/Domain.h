#pragma once

#include <limits>
#include <vector>
#include "Interval.h"

namespace dataflow {
class IntervalDomain {
  using ConstIterator = std::vector<Interval>::const_iterator;

  std::vector<Interval> _intervals;
  bool _unknown { true };
  void maintain();
  IntervalDomain& genImpl(const IntervalDomain &other, Interval& (Interval::*op)(const Interval&));

public:
  IntervalDomain(int lo, int hi) {
    if (lo <= hi) {
      _intervals.emplace_back(lo, hi);
      _unknown = false;
    }
  }
  IntervalDomain() = default;
  IntervalDomain(int val) : IntervalDomain(val, val) {}
  explicit IntervalDomain(bool unknown) : _unknown(unknown) {}
  explicit IntervalDomain(const llvm::Value *val);

  static IntervalDomain INF_DOMAIN() { return IntervalDomain(Interval::INT_NEG_INF, Interval::INT_INF); }
  static IntervalDomain UNINIT() { return IntervalDomain(true); }
  static IntervalDomain EMPTY() { return IntervalDomain(false); }
  
  ConstIterator begin() const {
    return _intervals.begin();
  }
  ConstIterator end() const {
    return _intervals.end();
  }
  size_t size() const {
    return _intervals.size();
  }

  bool contains(int val) const {
    if (_unknown) return true;
    for (auto &interval : _intervals) {
      if (interval.contains(val)) return true;
    }
    return false;
  }
  bool overlaps(const IntervalDomain &other) const {
    if (_unknown || other._unknown) return true;
    for (auto &interval : _intervals) {
      for (auto &otherInterval : other._intervals) {
        if (interval.overlaps(otherInterval)) return true;
      }
    }
    return false;
  }
  
  bool isUnknown() const {
    return _unknown;
  }
  bool isEmpty() const {
    return _intervals.empty();
  }

  int lower() const {
    if (_unknown) return Interval::INT_NEG_INF;
    if (_intervals.empty()) return Interval::INT_INF;
    return _intervals.front().lower();
  }
  int upper() const {
    if (_unknown) return Interval::INT_INF;
    if (_intervals.empty()) return Interval::INT_NEG_INF;
    return _intervals.back().upper();
  }

  /**
   * @brief intersects a domain with another one
   * @param other the domain to be intersected with.
   * @return the intersected domain.
   */
  IntervalDomain& operator&=(const IntervalDomain& other) {
    return genImpl(other, &Interval::operator&=);
  }
  /**
   * @brief combines the information contained in two domains.
   * @param other the domain to be joined with.
   * @return the joined domain.
   */
  IntervalDomain& operator|=(const IntervalDomain& other) {
    return genImpl(other, &Interval::operator|=);
  }
  IntervalDomain& operator+=(const IntervalDomain &other) {
    return genImpl(other, &Interval::operator+=);
  }
  IntervalDomain& operator-=(const IntervalDomain &other) {
    return genImpl(other, &Interval::operator-=);
  }
  IntervalDomain& operator*=(const IntervalDomain &other) {
    return genImpl(other, &Interval::operator*=);
  }
  IntervalDomain& operator/=(const IntervalDomain &other) {
    return genImpl(other, &Interval::operator/=);
  }

  /**
   * @brief get the complement of the domain.
   * @return the complement of the domain.
  */
  IntervalDomain operator~() const;

  /**
   * @brief clamp the domain to a given range.
   * @param lo the lower bound of the range.
   * @param hi the upper bound of the range.
  */
  void clamp(int lo, int hi);

  /**
   * @brief check if two domains are equal.
   * @param other the domain to be compared with.
  */
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
  IntervalDomain operator-() const;
  bool operator!=(const IntervalDomain &other) const {
    return !(*this == other);
  }
};

template <typename StreamLike>
inline typename
std::enable_if<
    std::is_base_of<std::ostream, StreamLike>::value ||
    std::is_same<StreamLike, llvm::raw_ostream>::value,
    StreamLike&
>::type
operator<<(StreamLike &os, const IntervalDomain& domain) {
  if (domain.isUnknown()) {
    os << "non-integral";
    return os;
  }
  if (domain.isEmpty()) {
    os << "empty";
    return os;
  }
  int i = 0;
  for (auto& interval : domain) {
    os << interval << ", "[i == domain.size() - 1];
    ++i;
  }
  return os;
}

} // namespace dataflow