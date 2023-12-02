#pragma once

#include <unordered_map>
#include "Domain.h"

namespace dataflow {
class FactMap {
    using DomainType = IntervalDomain;
    using Iterator = std::unordered_map<std::string, DomainType>::iterator;
    using ConstIterator = std::unordered_map<std::string, DomainType>::const_iterator;

    std::unordered_map<std::string, DomainType> _data;
public:
    Iterator begin() {
        return _data.begin();
    }
    Iterator end() {
        return _data.end();
    }
    ConstIterator begin() const {
        return _data.begin();
    }
    ConstIterator end() const {
        return _data.end();
    }
    ConstIterator cbegin() const {
        return _data.cbegin();
    }
    ConstIterator cend() const {
        return _data.cend();
    }

    DomainType& operator[](const std::string& key) {
        return _data[key];
    }
    const DomainType& operator[](const std::string& key) const {
        return _data.at(key);
    }

    DomainType getOrExtract(const llvm::Value *val) const;
    void erase(const std::string& key) {
        _data.erase(key);
    }

    /**
     * @brief Joins with another fact maps, accounting for Domain values
     * @param other The other fact map to join with.
    */
    FactMap& operator+=(const FactMap& other);
    FactMap operator+(const FactMap& other) const {
        return FactMap(*this) += other;
    }
    
    /**
     * @brief This function returns true if the two fact maps are equal.
     * @param other The other fact map to compare with.
     * @return true if the two fact maps are equal.
    */
    bool operator==(const FactMap& other) const;
    bool operator!=(const FactMap& other) const {
        return !(*this == other);
    }
    bool contains(const std::string& key) const {
        return _data.find(key) != _data.end();
    }
    bool contains(const std::string& key, const DomainType& value) const {
        return contains(key) && _data.at(key) == value;
    }
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const FactMap &factMap) {
  for (auto &fact : factMap) {
    os << fact.first << " |-> " << fact.second << "\n";
  }
  return os;
}

} // namespace dataflow