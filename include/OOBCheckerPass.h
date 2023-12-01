#pragma once

#include <llvm/ADT/SetVector.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Domain.h"
#include "PointerAnalysis.h"
#include "Utils.h"

namespace dataflow {
struct AnalysisContext {
  PointerAnalysis pa;
  std::unordered_set<const llvm::Value*> pointerSet;
  InsFactMap in, out;
  // TODO: add other context info here
};

struct OOBCheckerPass : public llvm::FunctionPass {
  static char ID;
  OOBCheckerPass() : llvm::FunctionPass(ID) {}

  /**
   * This function is called for each function F in the input C program
   * that the compiler encounters during a pass.
   */
  bool runOnFunction(llvm::Function &func) override;

protected:
  /**
   * Returns the newly generated facts based on the instruction type/parameters.
   * @param ins The instruction to be analyzed.
   * @param context Context information at this point of the analysis.
   * @return The updated facts.
   */
  FactMap genSet(const llvm::Instruction *ins, const AnalysisContext& context);
  /**
   * Returns the newly generated facts based on the instruction type/parameters.
   * @param ins The instruction to be analyzed.
   * @param context Context information at this point of the analysis.
   * @return The keys that need to be removed
   */
  std::unordered_set<std::string> killSet(const llvm::Instruction *ins, const AnalysisContext& context);

  /**
   * @brief This function implements the chaotic iteration algorithm using
   * flowIn(), transfer(), and flowOut().
   *
   * @param func The function to be analyzed.
   * @param context Context information at this point of the analysis.
   */
  void doAnalysis(const llvm::Function& func, AnalysisContext& context);

  /**
   * Can the Instruction Inst incurr an array out of bounds error?
   *
   * @param ins Instruction to check.
   * @return true if the instruction can cause an array out of bounds error.
   */
  bool check(llvm::Instruction *ins);

  const char* getAnalysisName() const { return "OOBCheckerPass"; }
};
} // namespace dataflow
