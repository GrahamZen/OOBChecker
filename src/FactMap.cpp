#include "FactMap.h"
#include "Utils.h"

namespace dataflow {

FactMap::DomainType FactMap::getOrExtract(const llvm::Value *val) const {
  auto key = variable(val);
  if (contains(key)) {
    return operator[](key);
  } else {
    return IntervalDomain { val };
  }
}
FactMap& FactMap::operator+=(const FactMap& other) {
    for (auto kvp : other) {
        if (!contains(kvp.first)) {
            operator[](kvp.first) = kvp.second;
        } else {
            operator[](kvp.first) |= kvp.second;
        }
    }
    return *this;
}
bool FactMap::operator==(const FactMap& other) const {
    auto cmpOne = [](const FactMap& a, const FactMap& b) {
        for (auto kvp : a) {
            if (b.contains(kvp.first)) {
                if (kvp.second != b[kvp.first])
                    return false;
            } else {
                if (kvp.second != IntervalDomain::UNINIT())
                    return false;
            }
        }
        return true;
    };
    return cmpOne(*this, other) && cmpOne(other, *this);
}
} // namespace dataflow