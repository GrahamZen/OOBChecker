#include "OOBCheckerPass.h"
#include "Utils.h"
#include <queue>
namespace dataflow {
    /**
     * @brief Get the Predecessors of a given instruction in the control-flow graph.
     *
     * @param ins The instruction to get the predecessors of.
     * @return Vector of all predecessors of Inst.
     */
    std::vector<const llvm::Instruction *> getPredecessors(const llvm::Instruction *ins) {
        std::vector<const llvm::Instruction *> ret;
        auto blk = ins->getParent();
        for (auto iter = blk->rbegin(), end = blk->rend(); iter != end; ++iter) {
            if (&(*iter) == ins) {
                ++iter;
                if (iter != end) {
                    ret.push_back(&(*iter));
                    return ret;
                }
                for (auto pre = pred_begin(blk), be = pred_end(blk); pre != be; ++pre) {
                    ret.push_back(&(*((*pre)->rbegin())));
                }
                return ret;
            }
        }
        return ret;
    }

    /**
     * @brief Get the successors of a given instruction in the control-flow graph.
     *
     * @param ins The instruction to get the successors of.
     * @return Vector of all successors of Inst.
     */
    std::vector<const llvm::Instruction *> getSuccessors(const llvm::Instruction *ins)
    {
        std::vector<const llvm::Instruction *> ret;
        auto blk = ins->getParent();
        for (auto iter = blk->begin(), end = blk->end(); iter != end; ++iter) {
            if (&(*iter) == ins) {
                ++iter;
                if (iter != end) {
                    ret.push_back(&(*iter));
                    return ret;
                }
                for (auto succ = succ_begin(blk), bs = succ_end(blk); succ != bs; ++succ) {
                    ret.push_back(&(*((*succ)->begin())));
                }
                return ret;
            }
        }
        return ret;
    }

    void OOBCheckerPass::doAnalysis(const llvm::Function& func, AnalysisContext& context) {
        std::queue<const llvm::Instruction*> insQueue;
        auto firstIns = &(*inst_begin(func));
        for (auto iter = func.arg_begin(); iter != func.arg_end(); ++iter) {
            auto arg = &(*iter);
            context.in[firstIns][variable(arg)] = IntervalDomain { arg };
            context.pointerSet.insert(arg);
        }

        for (auto iter = inst_begin(func); iter != inst_end(func); ++iter) {
            auto ins = &(*iter);
            insQueue.push(ins);
            context.pointerSet.insert(ins);
        }
        while (!insQueue.empty()) {
            auto ins = insQueue.front();
            insQueue.pop();

            for (auto predIns : getPredecessors(ins)) {
                context.in.at(ins) += context.out.at(predIns);
            }
            // gen set
            auto gen = genSet(ins, context);
            auto kill = killSet(ins, context);
            auto newOut = context.in.at(ins);
            for (auto key : kill) {
                if(newOut.contains(key)) {
                    newOut.erase(key);
                }
            }
            newOut += gen;
            if (newOut != context.out.at(ins)) {
                for(auto succIns : getSuccessors(ins)) {
                    insQueue.emplace(succIns);
                }
                context.out.at(ins) = newOut;
            }
        }
    }

} // namespace dataflow