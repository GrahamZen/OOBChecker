#include "DivZeroAnalysis.h"
#include "Utils.h"

namespace dataflow
{

  /**
   * @brief Is the given instruction a user input?
   *
   * @param Inst The instruction to check.
   * @return true If it is a user input, false otherwise.
   */
  bool isInput(Instruction *Inst)
  {
    if (auto Call = dyn_cast<CallInst>(Inst))
    {
      if (auto Fun = Call->getCalledFunction())
      {
        return (Fun->getName().equals("getchar") ||
                Fun->getName().equals("fgetc"));
      }
    }
    return false;
  }

  /**
   * Evaluate a PHINode to get its Domain.
   *
   * @param Phi PHINode to evaluate
   * @param InMem InMemory of Phi
   * @return Domain of Phi
   */
  Domain *eval(PHINode *Phi, const Memory *InMem)
  {
    if (auto ConstantVal = Phi->hasConstantValue())
    {
      return new Domain(extractFromValue(ConstantVal));
    }

    Domain *Joined = new Domain(Domain::Uninit);

    for (unsigned int i = 0; i < Phi->getNumIncomingValues(); i++)
    {
      auto Dom = getOrExtract(InMem, Phi->getIncomingValue(i));
      Joined = Domain::join(Joined, Dom);
    }
    return Joined;
  }

  /**
   * @brief Evaluate the +, -, * and / BinaryOperator instructions
   * using the Domain of its operands and return the Domain of the result.
   *
   * @param BinOp BinaryOperator to evaluate
   * @param InMem InMemory of BinOp
   * @return Domain of BinOp
   */
  Domain *eval(BinaryOperator *BinOp, const Memory *InMem)
  {
    /**
     * DONE: Write your code here that evaluates +, -, * and /
     * based on the Domains of the operands.
     */
    const auto left = BinOp->getOperand(0);
    const auto right = BinOp->getOperand(1);
    switch (BinOp->getOpcode())
    {
    case Instruction::Add:
      return Domain::add(getOrExtract(InMem, left), getOrExtract(InMem, right));
    case Instruction::Sub:
      return Domain::sub(getOrExtract(InMem, left), getOrExtract(InMem, right));
    case Instruction::Mul:
      return Domain::mul(getOrExtract(InMem, left), getOrExtract(InMem, right));
    case Instruction::SDiv:
    case Instruction::UDiv:
      return Domain::div(getOrExtract(InMem, left), getOrExtract(InMem, right));
    default:
      return new Domain(Domain::Uninit);
    }
  }

  /**
   * @brief Evaluate Cast instructions.
   *
   * @param Cast Cast instruction to evaluate
   * @param InMem InMemory of Instruction
   * @return Domain of Cast
   */
  Domain *eval(CastInst *Cast, const Memory *InMem)
  {
    /**
     * DONE: Write your code here to evaluate Cast instruction.
     */
    return getOrExtract(InMem, Cast->getOperand(0));
  }

  /**
   * @brief Evaluate the ==, !=, <, <=, >=, and > Comparision operators using
   * the Domain of its operands to compute the Domain of the result.
   *
   * @param Cmp Comparision instruction to evaluate
   * @param InMem InMemory of Cmp
   * @return Domain of Cmp
   */
  Domain *eval(CmpInst *Cmp, const Memory *InMem)
  {
    /**
     * DONE: Write your code here that evaluates:
     * ==, !=, <, <=, >=, and > based on the Domains of the operands.
     *
     * NOTE: There is a lot of scope for refining this, but you can just return
     * MaybeZero for comparisons other than equality.
     */
    auto left = getOrExtract(InMem, Cmp->getOperand(0));
    auto right = getOrExtract(InMem, Cmp->getOperand(1));
    if (left->Value == Domain::Uninit || right->Value == Domain::Uninit)
      return new Domain(Domain::Uninit);
    switch (Cmp->getPredicate())
    {
    case CmpInst::FCMP_OEQ:
    case CmpInst::ICMP_EQ:
      if (Domain::equal(*left, *right) && left->Value == Domain::Zero)
        return new Domain(Domain::NonZero);
      else if (Domain::sub(left, right)->Value == Domain::NonZero || Domain::sub(right, left)->Value == Domain::NonZero)
        return new Domain(Domain::Zero);
      else
        return new Domain(Domain::MaybeZero);
    case CmpInst::FCMP_ONE:
    case CmpInst::ICMP_NE:
      if (Domain::equal(*left, *right) && (left->Value == Domain::Zero))
        return new Domain(Domain::Zero);
      else if (Domain::sub(left, right)->Value == Domain::NonZero || Domain::sub(right, left)->Value == Domain::NonZero)
        return new Domain(Domain::NonZero);
      else
        return new Domain(Domain::MaybeZero);
    case CmpInst::ICMP_SLT:
    case CmpInst::ICMP_SLE:
    case CmpInst::ICMP_SGE:
    case CmpInst::ICMP_SGT:
      return new Domain{Domain::MaybeZero};
    default:
      return new Domain(Domain::Uninit);
    }
  }

  void DivZeroAnalysis::transfer(Instruction *Inst, const Memory *In,
                                 Memory &NOut, PointerAnalysis *PA,
                                 SetVector<Value *> PointerSet)
  {
    if (isInput(Inst))
    {
      // The instruction is a user controlled input, it can have any value.
      NOut[variable(Inst)] = new Domain(Domain::MaybeZero);
    }
    else if (auto Phi = dyn_cast<PHINode>(Inst))
    {
      // Evaluate PHI node
      NOut[variable(Phi)] = eval(Phi, In);
    }
    else if (auto BinOp = dyn_cast<BinaryOperator>(Inst))
    {
      // Evaluate BinaryOperator
      NOut[variable(BinOp)] = eval(BinOp, In);
    }
    else if (auto Cast = dyn_cast<CastInst>(Inst))
    {
      // Evaluate Cast instruction
      NOut[variable(Cast)] = eval(Cast, In);
    }
    else if (auto Cmp = dyn_cast<CmpInst>(Inst))
    {
      // Evaluate Comparision instruction
      NOut[variable(Cmp)] = eval(Cmp, In);
    }
    else if (auto Alloca = dyn_cast<AllocaInst>(Inst))
    {
      // Do nothing here.
    }
    else if (auto Store = dyn_cast<StoreInst>(Inst))
    {
      /**
       * TODO: Store instruction can either add new variables or overwrite existing variables into memory maps.
       * To update the memory map, we rely on the points-to graph constructed in PointerAnalysis.
       *
       * To build the abstract memory map, you need to ensure all pointer references are in-sync, and
       * will converge upon a precise abstract value. To achieve this, implement the following workflow:
       *
       * Iterate through the provided PointerSet:
       *   - If there is a may-alias (i.e., `alias()` returns true) between two variables:
       *     + Get the abstract values of each variable.
       *     + Join the abstract values using Domain::join().
       *     + Update the memory map for the current assignment with the joined abstract value.
       *     + Update the memory map for all may-alias assignments with the joined abstract value.
       *
       * Hint: You may find getOperand(), getValueOperand(), and getPointerOperand() useful.
       */
      const auto curr_pointer = Store->getPointerOperand();
      const auto curr_value = Store->getValueOperand();
      if (curr_value->getType()->isPointerTy())
        return;
      const auto curr_valueDomain = getOrExtract(In, curr_value);
      std::string curr_pointer_str = variable(curr_pointer);
      for (auto pointer : PointerSet)
      {
        std::string pointer_str = variable(pointer);
        if (PA->alias(curr_pointer_str, pointer_str))
        {
          if (In->count(pointer_str) != 0)
            NOut[pointer_str] = Domain::join(getOrExtract(In, pointer), curr_valueDomain);
          else
            NOut[pointer_str] = curr_valueDomain;
        }
      }
      NOut[curr_pointer_str] = curr_valueDomain;
    }
    else if (auto Load = dyn_cast<LoadInst>(Inst))
    {
      /**
       * TODO: Rely on the existing variables defined within the `In` memory to
       * know what abstract domain should be for the new variable
       * introduced by a load instruction.
       *
       * If the memory map already contains the variable, propagate the existing
       * abstract value to NOut.
       * Otherwise, initialize the memory map for it.
       *
       * Hint: You may use getPointerOperand().
       */
      auto pointer = Load->getPointerOperand();
      if (Load->getType()->isIntegerTy())
      {
        NOut[variable(Load)] = getOrExtract(In, pointer);
      }
    }
    else if (auto Branch = dyn_cast<BranchInst>(Inst))
    {
      // Analysis is flow-insensitive, so do nothing here.
    }
    else if (auto Call = dyn_cast<CallInst>(Inst))
    {
      /**
       * TODO: Populate the NOut with an appropriate abstract domain.
       *
       * You only need to consider calls with int return type.
       */
      if (Call->getType()->isIntegerTy())
      {
        NOut[variable(Call)] = getOrExtract(In, Call);
      }
    }
    else if (auto Return = dyn_cast<ReturnInst>(Inst))
    {
      // Analysis is intra-procedural, so do nothing here.
    }
    else
    {
      errs() << "Unhandled instruction: " << *Inst << "\n";
    }
  }

} // namespace dataflow