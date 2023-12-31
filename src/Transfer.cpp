#include "OOBCheckerPass.h"
#include "Utils.h"

namespace dataflow
{
  /**
   * @brief Is the given instruction a user input?
   *
   * @param ins The instruction to check.
   * @return true If it is a user input, false otherwise.
   */
  bool isInput(const llvm::Instruction *ins)
  {
    if (auto call = llvm::dyn_cast<llvm::CallInst>(ins))
    {
      if (auto func = call->getCalledFunction())
      {
        return (func->getName().equals("getchar") ||
                func->getName().equals("fgetc"));
      }
    }
    return false;
  }

  /**
   * Evaluate a PHINode to get its Domain.
   *
   * @param phi PHINode to evaluate
   * @param inMap input facts
   * @return Domain of Phi
   */
  IntervalDomain eval(const llvm::PHINode *phi, const FactMap &inMap)
  {
    if (auto ConstantVal = phi->hasConstantValue())
    {
      return IntervalDomain{ConstantVal};
    }

    IntervalDomain ret;
    for (unsigned int i = 0; i < phi->getNumIncomingValues(); ++i)
    {
      ret |= inMap.getOrExtract(phi->getIncomingValue(i));
    }
    return ret;
  }

  /**
   * @brief Evaluate the +, -, * and / BinaryOperator instructions
   * using the Domain of its operands and return the Domain of the result.
   *
   * @param binOp the binary operator to evaluate
   * @param inMap input facts
   * @return Domain of binary operator
   */
  IntervalDomain eval(const llvm::BinaryOperator *binOp, const FactMap &inMap)
  {
    const auto left = inMap.getOrExtract(binOp->getOperand(0));
    const auto right = inMap.getOrExtract(binOp->getOperand(1));
    switch (binOp->getOpcode())
    {
    case llvm::Instruction::Add:
      return left + right;
    case llvm::Instruction::Sub:
      return left - right;
    case llvm::Instruction::Mul:
      return left * right;
    case llvm::Instruction::SDiv:
    case llvm::Instruction::UDiv:
      return left / right;
    default:
      return IntervalDomain::UNINIT();
    }
  }

  /**
   * @brief Evaluate Cast instructions.
   *
   * @param cast Cast instruction to evaluate
   * @param inMap InMemory of Instruction
   * @return Domain of Cast
   */
  IntervalDomain eval(const llvm::CastInst *cast, const FactMap &inMap)
  {
    return inMap.getOrExtract(cast->getOperand(0));
  }

  /**
   * @brief Evaluate the ==, !=, <, <=, >=, and > Comparision operators using
   * the Domain of its operands to compute the Domain of the result.
   *
   * @param cmp Comparision instruction to evaluate
   * @param inMap InMemory of Cmp
   * @return Domain of Cmp
   */
  IntervalDomain eval(const llvm::CmpInst *cmp, const FactMap &inMap)
  {
    auto left = inMap.getOrExtract(cmp->getOperand(0));
    auto right = inMap.getOrExtract(cmp->getOperand(1));
    if (left.isUnknown() || right.isUnknown())
    {
      return IntervalDomain::UNINIT();
    }
    switch (cmp->getPredicate())
    {
    case llvm::CmpInst::FCMP_OEQ:
    case llvm::CmpInst::ICMP_EQ:
      return (left & right).isEmpty() ? IntervalDomain(0) : IntervalDomain(0, 1);
    case llvm::CmpInst::FCMP_ONE:
    case llvm::CmpInst::ICMP_NE:
      return (left & right).isEmpty() ? IntervalDomain(1) : IntervalDomain(0, 1);
    case llvm::CmpInst::ICMP_SLT:
    case llvm::CmpInst::ICMP_ULT:
      if (left.upper() < right.lower())
        return IntervalDomain(1);
      if (left.lower() >= right.upper())
        return IntervalDomain(0);
      return IntervalDomain(0, 1);
    case llvm::CmpInst::ICMP_SLE:
    case llvm::CmpInst::ICMP_ULE:
      if (left.upper() <= right.lower())
        return IntervalDomain(1);
      if (left.lower() > right.upper())
        return IntervalDomain(0);
      return IntervalDomain(0, 1);
    case llvm::CmpInst::ICMP_SGT:
    case llvm::CmpInst::ICMP_UGT:
      if (left.lower() > right.upper())
        return IntervalDomain(1);
      if (left.upper() <= right.lower())
        return IntervalDomain(0);
      return IntervalDomain(0, 1);
    case llvm::CmpInst::ICMP_SGE:
    case llvm::CmpInst::ICMP_UGE:
      if (left.lower() >= right.upper())
        return IntervalDomain(1);
      if (left.upper() < right.lower())
        return IntervalDomain(0);
      return IntervalDomain(0, 1);
    default:
      return IntervalDomain(0, 1);
    }
  }

  FactMap OOBCheckerPass::genSet(const llvm::Instruction *ins, AnalysisContext &context)
  {
    FactMap ret;
    const auto &inFacts = context.in.at(ins);
    if (isInput(ins))
    {
      ret[variable(ins)] = IntervalDomain::INF_DOMAIN();
    }
    else if (auto phi = llvm::dyn_cast<llvm::PHINode>(ins))
    {
      ret[variable(phi)] = eval(phi, inFacts);
    }
    else if (auto binOp = llvm::dyn_cast<llvm::BinaryOperator>(ins))
    {
      ret[variable(binOp)] = eval(binOp, inFacts);
    }
    else if (auto cast = llvm::dyn_cast<llvm::CastInst>(ins))
    {
      llvm::Value *sourceOperand = cast->getOperand(0);
      uint64_t arraySize = context.arraySizeMap[sourceOperand];
      context.arraySizeMap[cast] = arraySize;
      ret[variable(cast)] = eval(cast, inFacts);
    }
    else if (auto cmp = llvm::dyn_cast<llvm::CmpInst>(ins))
    {
      ret[variable(cmp)] = eval(cmp, inFacts);
    }
    else if (auto alloca = llvm::dyn_cast<llvm::AllocaInst>(ins))
    {
      llvm::Type *allocatedType = alloca->getAllocatedType();
      if (allocatedType->isArrayTy())
      {
        llvm::ArrayType *arrayType = llvm::dyn_cast<llvm::ArrayType>(allocatedType);
        uint64_t arraySize = arrayType->getNumElements();
        context.arraySizeMap[alloca] = arraySize;
      }
      else if (allocatedType->isIntegerTy())
      {
        ret[variable(alloca)] = IntervalDomain::INF_DOMAIN();
      }
    }
    else if (auto GEPInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ins))
    {
      llvm::Value *arrayBase = GEPInst->getOperand(0);
      uint64_t arraySize = context.arraySizeMap[arrayBase];
      if (llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(GEPInst->getOperand(1)))
      {
        int64_t offset = CI->getSExtValue();
        context.arraySizeMap[GEPInst] = arraySize - offset;
      }
      else
      {
        context.arraySizeMap[GEPInst] = arraySize;
      }
    }
    else if (auto store = llvm::dyn_cast<llvm::StoreInst>(ins))
    {
      // *pointer_op = value_op
      const auto toStore = store->getPointerOperand();
      const auto val = store->getValueOperand();

      if (val->getType()->isPointerTy())
      {
        int arraySize = context.arraySizeMap[val];
        context.arraySizeMap[toStore] = arraySize;
      }
      if (val->getType()->isPointerTy())
        return ret;
      const auto valDomain = inFacts.getOrExtract(val);
      std::string toStoreStr = variable(toStore);
      for (auto ptr : context.pointerSet)
      {
        std::string ptrStr = variable(ptr);
        if (context.pa.alias(toStoreStr, ptrStr))
        {
          if (inFacts.contains(ptrStr))
          {
            ret[ptrStr] = inFacts.getOrExtract(ptr) | valDomain;
          }
          else
          {
            ret[ptrStr] = valDomain;
          }
        }
      }
      ret[toStoreStr] = valDomain;
    }
    else if (auto load = llvm::dyn_cast<llvm::LoadInst>(ins))
    {
      auto pointer = load->getPointerOperand();
      if (load->getType()->isIntegerTy())
      {
        ret[variable(load)] = inFacts.getOrExtract(pointer);
      }
      if (pointer->getType()->isPointerTy())
      {
        llvm::Type *elementType = pointer->getType()->getPointerElementType();
        if (elementType->isPointerTy())
        {
          if (context.arraySizeMap.find(pointer) != context.arraySizeMap.end())
          {
            int arraySize = context.arraySizeMap[pointer];
            context.arraySizeMap[load] = arraySize;
          }
        }
      }
    }
    else if (auto branch = llvm::dyn_cast<llvm::BranchInst>(ins))
    {
      // Analysis is flow-insensitive, so do nothing here.
    }
    else if (auto call = llvm::dyn_cast<llvm::CallInst>(ins))
    {
      if (call->getCalledFunction() && call->getCalledFunction()->getName() == "malloc")
      {
        if (llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(0)))
        {
          uint64_t mallocSize = CI->getZExtValue();
          context.arraySizeMap[call] = mallocSize / sizeof(int);
        }
      }
      else if (call->getType()->isIntegerTy())
      {
        ret[variable(call)] = inFacts.getOrExtract(call);
      }
    }
    else if (auto retIns = llvm::dyn_cast<llvm::ReturnInst>(ins))
    {
      // Analysis is intra-procedural, so do nothing here.
    }
    else
    {
      llvm::errs() << "Unhandled instruction: " << *ins << "\n";
    }

    return ret;
  }

  std::unordered_set<std::string> OOBCheckerPass::killSet(const llvm::Instruction *ins, AnalysisContext &context)
  {
    std::unordered_set<std::string> ret;
    const auto &inFacts = context.in.at(ins);

    if (auto store = llvm::dyn_cast<llvm::StoreInst>(ins))
    {
      // *pointer_op = value_op
      const auto toStore = store->getPointerOperand();
      const auto val = store->getValueOperand();
      if (val->getType()->isPointerTy())
      {
        int arraySize = context.arraySizeMap[val];
        context.arraySizeMap[toStore] = arraySize;
      }
      if (val->getType()->isPointerTy())
        return ret;
      const auto valDomain = inFacts.getOrExtract(val);
      std::string toStoreStr = variable(toStore);
      for (auto ptr : context.pointerSet)
      {
        std::string ptrStr = variable(ptr);
        if (context.pa.alias(toStoreStr, ptrStr))
        {
          ret.insert(ptrStr);
        }
      }
      ret.insert(toStoreStr);
    }

    return ret;
  }

} // namespace dataflow