#include "DivZeroAnalysis.h"
#include "Utils.h"

namespace dataflow
{

    /**
     * @brief Get the Predecessors of a given instruction in the control-flow graph.
     *
     * @param Inst The instruction to get the predecessors of.
     * @return Vector of all predecessors of Inst.
     */
    std::vector<Instruction *> getPredecessors(Instruction *Inst)
    {
        std::vector<Instruction *> Ret;
        auto Block = Inst->getParent();
        for (auto Iter = Block->rbegin(), End = Block->rend(); Iter != End; ++Iter)
        {
            if (&(*Iter) == Inst)
            {
                ++Iter;
                if (Iter != End)
                {
                    Ret.push_back(&(*Iter));
                    return Ret;
                }
                for (auto Pre = pred_begin(Block), BE = pred_end(Block); Pre != BE;
                     ++Pre)
                {
                    Ret.push_back(&(*((*Pre)->rbegin())));
                }
                return Ret;
            }
        }
        return Ret;
    }

    /**
     * @brief Get the successors of a given instruction in the control-flow graph.
     *
     * @param Inst The instruction to get the successors of.
     * @return Vector of all successors of Inst.
     */
    std::vector<Instruction *> getSuccessors(Instruction *Inst)
    {
        std::vector<Instruction *> Ret;
        auto Block = Inst->getParent();
        for (auto Iter = Block->begin(), End = Block->end(); Iter != End; ++Iter)
        {
            if (&(*Iter) == Inst)
            {
                ++Iter;
                if (Iter != End)
                {
                    Ret.push_back(&(*Iter));
                    return Ret;
                }
                for (auto Succ = succ_begin(Block), BS = succ_end(Block); Succ != BS;
                     ++Succ)
                {
                    Ret.push_back(&(*((*Succ)->begin())));
                }
                return Ret;
            }
        }
        return Ret;
    }

    /**
     * @brief Joins two Memory objects (Mem1 and Mem2), accounting for Domain
     * values.
     *
     * @param Mem1 First memory.
     * @param Mem2 Second memory.
     * @return The joined memory.
     */
    Memory *join(Memory *Mem1, Memory *Mem2)
    {
        /**
         * DONE: Write your code that joins two memories.
         *
         * If some instruction with domain D is either in Mem1 or Mem2, but not in
         *   both, add it with domain D to the Result.
         * If some instruction is present in Mem1 with domain D1 and in Mem2 with
         *   domain D2, then Domain::join D1 and D2 to find the new domain D,
         *   and add instruction I with domain D to the Result.
         */
        Memory *Result = new Memory(*Mem1);
        for (auto pair : *Mem2)
        {
            if (Result->count(pair.first) == 0)
                Result->insert(pair);
            else
            {
                Result->at(pair.first) = Domain::join(Result->at(pair.first), pair.second);
            }
        }
        return Result;
    }

    void DivZeroAnalysis::flowIn(Instruction *Inst, Memory *InMem)
    {
        /**
         * DONE: Write your code to implement flowIn.
         *
         * For each predecessor Pred of instruction Inst, do the following:
         *   + Get the Out Memory of Pred using OutMap.
         *   + Join the Out Memory with InMem.
         */
        std::vector<Instruction *> Preds = getPredecessors(Inst);
        for (auto it = Preds.begin(); it != Preds.end(); it++)
        {
            Memory *OutMem = OutMap[*it];
            InMem = join(InMem, OutMem);
        }
        InMap[Inst] = InMem;
    }

    /**
     * @brief This function returns true if the two memories Mem1 and Mem2 are
     * equal.
     *
     * @param Mem1 First memory
     * @param Mem2 Second memory
     * @return true if the two memories are equal, false otherwise.
     */
    bool equal(Memory *Mem1, Memory *Mem2)
    {
        /**
         * DONE: Write your code to implement check for equality of two memories.
         *
         * If any instruction I is present in one of Mem1 or Mem2,
         *   but not in both and the Domain of I is not UnInit, the memories are
         *   unequal.
         * If any instruction I is present in Mem1 with domain D1 and in Mem2
         *   with domain D2, if D1 and D2 are unequal, then the memories are unequal.
         */
        for (auto p : *Mem1)
        {
            if (Mem2->count(p.first) != 0)
            {
                if (!Domain::equal(*(p.second), *(Mem2->at(p.first))))
                    return false;
            }
            else
            {
                if (p.second->Value != Domain::Uninit)
                    return false;
            }
        }
        for (auto p : *Mem2)
        {
            if (Mem1->count(p.first) != 0)
            {
                if (!Domain::equal(*(p.second), *(Mem1->at(p.first))))
                    return false;
            }
            else
            {
                if (p.second->Value != Domain::Uninit)
                    return false;
            }
        }
        return true;
    }

    void DivZeroAnalysis::flowOut(Instruction *Inst, Memory *Pre, Memory *Post,
                                  SetVector<Instruction *> &WorkSet)
    {
        /**
         * DONE: Write your code to implement flowOut.
         *
         * For each given instruction, merge abstract domain from pre-transfer memory
         * and post-transfer memory, and update the OutMap.
         * If the OutMap changed then also update the WorkSet.
         */
        Memory PreMinusKill = *InMap[Inst];
        if (Inst->getOpcode() == Instruction::Store)
        {
            PreMinusKill.erase(variable(Inst->getOperand(1)));
        }
        Memory *OutMem = join(&PreMinusKill, Post);
        if (!equal(OutMem, OutMap[Inst]))
        {
            OutMap[Inst] = OutMem;
            for (auto inst : getSuccessors(Inst))
                WorkSet.insert(inst);
        }
    }

    void DivZeroAnalysis::doAnalysis(Function &F, PointerAnalysis *PA)
    {
        SetVector<Instruction *> WorkSet;
        SetVector<Value *> PointerSet;
        /**
         * DONE: Write your code to implement the chaotic iteration algorithm
         * for the analysis.
         *
         * First, find the arguments of function call and instantiate abstract domain values
         * for each argument.
         * Initialize the WorkSet and PointerSet with all the instructions in the function.
         * The rest of the implementation is almost similar to the previous lab.
         *
         * While the WorkSet is not empty:
         * - Pop an instruction from the WorkSet.
         * - Construct it's Incoming Memory using flowIn.
         * - Evaluate the instruction using transfer and create the OutMemory.
         *   Note that the transfer function takes two additional arguments compared to previous lab:
         *   the PointerAnalysis object and the populated PointerSet.
         * - Use flowOut along with the previous Out memory and the current Out
         *   memory, to check if there is a difference between the two to update the
         *   OutMap and add all successors to WorkSet.
         */
        Memory *Mem = new Memory;
        for (auto arg = F.arg_begin(); arg != F.arg_end(); arg++)
        {
            Mem->insert({variable(arg), new Domain(Domain::MaybeZero)});
            PointerSet.insert(arg);
        }
        InMap[&(*inst_begin(F))] = Mem;

        for (inst_iterator Iter = inst_begin(F), End = inst_end(F); Iter != End; ++Iter)
        {
            auto Inst = &(*Iter);
            WorkSet.insert(Inst);
            PointerSet.insert(Inst);
        }
        while (!WorkSet.empty())
        {
            auto Inst = WorkSet.front();
            WorkSet.remove(Inst);
            flowIn(Inst, InMap[Inst]);
            Memory Gen;
            transfer(Inst, InMap[Inst], Gen, PA, PointerSet);
            flowOut(Inst, InMap[Inst], &Gen, WorkSet);
        }
    }

} // namespace dataflow