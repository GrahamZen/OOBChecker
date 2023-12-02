#pragma once

#include "Domain.h"
#include "FactMap.h"
#include <unordered_map>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
extern const char *WHITESPACES;

namespace dataflow {

using InsFactMap = std::unordered_map<const llvm::Instruction*, FactMap>;

/**
 * @brief Get a human-readable string name for an llvm Value
 *
 * @param val The llvm Value to get the string representation of
 * @return std::string The string representation of Val.
 */
std::string variable(const llvm::Value *val);

/**
 * @brief Encode the memory address of an llvm Value
 *
 * @param val The llvm Value to get the encoding of
 * @return std::string The encoded memory address of Val
 */
std::string address(const llvm::Value *val);

/**
 * @brief Print the Before and After domains of an instruction
 * wrt. In and Out memory.
 *
 * Format:
 *   <instruction>:    [ <before> --> <after> ]
 *
 * @param ins The instruction to print the domains for.
 * @param inMap The incoming domains.
 * @param outMap The outgoing domains.
 */
void printInstructionTransfer(const llvm::Instruction *ins, const FactMap& inMap,
                              const FactMap& outMap);

/**
 * @brief Print the In and Out memory of every instruction in function F to
 * stderr.
 *
 * This gives the human-readable representaion of the results of dataflow
 * analysis.
 *
 * @param func Function whose dataflow analysis result to print.
 * @param inMap Map of In memory of every instruction in function func.
 * @param outMap Map of Out memory of every instruction in function func.
 */
void printMap(const llvm::Function &func, const InsFactMap &inMap, const InsFactMap &outMap);

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const InsFactMap &inMap) {
  for (auto &entry : inMap) {
    os << *entry.first << "\n" << entry.second << "\n";
  }
  return os;
}

} // namespace dataflow