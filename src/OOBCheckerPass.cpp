#include "OOBCheckerPass.h"
#include "Utils.h"

namespace dataflow {

bool OOBCheckerPass::check(llvm::Instruction *ins) {
    // TODO
    return false;
}


bool OOBCheckerPass::runOnFunction(llvm::Function &func) {
  llvm::outs() << "Running " << getAnalysisName() << " on " << func.getName() << "\n";

  // Initializing InMap and OutMap.
  AnalysisContext context { PointerAnalysis { func } };
  for (auto iter = inst_begin(func), end = inst_end(func); iter != end; ++iter) {
    auto ins = &(*iter);
    context.in[ins] = {};
    context.out[ins] = {};
  }

  // The chaotic iteration algorithm is implemented inside doAnalysis().
  auto pa = PointerAnalysis(func);
  doAnalysis(func, context);

  // Check each instruction in function F for potential divide-by-zero error.
  llvm::outs() << context.in << "\n";
  llvm::outs() << context.out << "\n";

  for (auto iter = inst_begin(func), end = inst_end(func); iter != end; ++iter) {
    auto ins = &(*iter);
    if (check(ins)) {
      llvm::outs() << "Potential array out of bounds error: " << *ins << "\n";
    }
  }

  for (auto iter = inst_begin(func), end = inst_end(func); iter != end; ++iter) {
    auto ins = &*iter;
    printInstructionTransfer(ins, context.in.at(ins), context.out.at(ins));
  }
  return false;
}

char OOBCheckerPass::ID = 1;
static llvm::RegisterPass<OOBCheckerPass> X("OOBChecker", "Array Out of Bounds Checker",
                                       false, false);
} // namespace dataflow