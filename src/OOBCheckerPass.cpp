#include "OOBCheckerPass.h"
#include "Utils.h"

namespace dataflow
{

  bool OOBCheckerPass::check(llvm::Instruction *ins, const AnalysisContext &context)
  {
    if (auto *gep = llvm::dyn_cast<llvm::GetElementPtrInst>(ins))
    {
      const auto arrayPtr = gep->getPointerOperand();
      auto *index = gep->idx_begin() + 1;
      int arraySize = context.arraySizeMap.at(arrayPtr);
      if (auto *constIndex = llvm::dyn_cast<llvm::ConstantInt>(index))
      {
        int val = constIndex->getSExtValue();
        if (constIndex->getSExtValue() < 0 || constIndex->getSExtValue() >= arraySize)
        {
          return true;
        }
      }
      else
      {
        llvm::Type *elementType = gep->getResultElementType();
        int numOp = gep->getNumOperands();
        if (gep->getNumOperands() == 3)
        {
          llvm::Value *idxProm = *(gep->idx_begin() + 1);
          auto accessIndex = context.in.at(ins).getOrExtract(idxProm);
          if (accessIndex.lower() < 0 || accessIndex.upper() >= arraySize)
          {
            return true;
          }
        }
        else if (gep->getNumOperands() == 2)
        {
          llvm::Value *idxProm = *(gep->idx_begin());
          auto accessIndex = context.in.at(ins).getOrExtract(idxProm);
          if (accessIndex.lower() < 0 || accessIndex.upper() >= arraySize)
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  bool OOBCheckerPass::runOnFunction(llvm::Function &func)
  {
    llvm::outs() << "Running " << getAnalysisName() << " on " << func.getName() << "\n";

    // Initializing InMap and OutMap.
    AnalysisContext context{PointerAnalysis{func}};
    for (auto iter = inst_begin(func), end = inst_end(func); iter != end; ++iter)
    {
      auto ins = &(*iter);
      context.in[ins] = {};
      context.out[ins] = {};
    }

    // The chaotic iteration algorithm is implemented inside doAnalysis().
    auto pa = PointerAnalysis(func);
    doAnalysis(func, context);

    // Check each instruction in function F for potential divide-by-zero error.
    for (auto iter = inst_begin(func), end = inst_end(func); iter != end; ++iter)
    {
      auto ins = &(*iter);
      if (check(ins, context))
      {
        llvm::errs() << "Potential array out of bounds error: " << *ins << "\n";
      }
    }

    for (auto iter = inst_begin(func), end = inst_end(func); iter != end; ++iter)
    {
      auto ins = &*iter;
      printInstructionTransfer(ins, context.in.at(ins), context.out.at(ins));
    }
    return false;
  }

  char OOBCheckerPass::ID = 1;
  static llvm::RegisterPass<OOBCheckerPass> X("OOBChecker", "Array Out of Bounds Checker",
                                              false, false);
} // namespace dataflow
