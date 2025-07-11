/**
 * GNU General Public License, version 2.0.
 *
 * Copyright (c) 2025 Tijme Gommers (@tijme).
 *
 * This source code file is part of Dittobytes. Dittobytes is 
 * licensed under GNU General Public License, version 2.0, and 
 * you are free to use, modify, and distribute this file under 
 * its terms. However, any modified versions of this file must 
 * include this same license and copyright notice.
 */

/**
 * LLVM includes
 */
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/IPO/GlobalOpt.h"

/**
 * Namespace(s) to use
 */
using namespace std;
using namespace llvm;

/**
 * A class to expand `memset` calls to manual sets.
 */
class ExpandMemsetCallsModule  {

private:

    /**
     * Whether this class modified the intermediate function.
     */
    bool modified = false;

    /**
     * Whether the module is enabled (default) or disabled.
     * 
     * @returns bool Positive if enabled.
     */
    bool moduleIsEnabled() {
        const char* EXPAND_MEMSET_CALLS = std::getenv("EXPAND_MEMSET_CALLS");
        return (EXPAND_MEMSET_CALLS && std::string(EXPAND_MEMSET_CALLS) == "true");
    }

public:

    /**
     * Main execution method for the ExpandMemsetCallsModule class.
     *
     * @param Function& F The intermediate function to expand memset calls in.
     * @return bool Indicates if the intermediate function was modified.
     */
    bool run(Function &F) {
        // Ensure module is enabled
        if (!moduleIsEnabled()) return false;
        
        SmallVector<CallInst *, 8> MemSetCalls;

        // Inform user that we are running this module
        dbgs() << "        ↳ Running ExpandMemsetCalls module.\n";

        for (auto &BB : F) {
            for (auto &I : BB) {
                if (auto *CI = dyn_cast<CallInst>(&I)) {
                    if (auto *F = CI->getCalledFunction()) {
                        if (F->getName().starts_with("llvm.memset")) {
                            MemSetCalls.push_back(CI);
                        }
                    }
                }
            }
        }

        for (auto *CI : MemSetCalls) {
            // Inform user that we encountered a `memset` call
            dbgs() << "          ↳ Expanding a `memset` call.\n";

            auto *Dst = CI->getArgOperand(0);
            auto *Val = CI->getArgOperand(1);
            auto *Len = CI->getArgOperand(2);
            auto *IsVolatile = CI->getArgOperand(3);

            if (auto *ConstLen = dyn_cast<ConstantInt>(Len)) {
                uint64_t Size = ConstLen->getZExtValue();
                IRBuilder<> IRB(CI);

                // Value to be set (need to truncate/extend to i8 if not already)
                Value *ValueToSet = IRB.CreateTruncOrBitCast(Val, IRB.getInt8Ty());
                for (uint64_t i = 0; i < Size; ++i) {
                    Value *Offset = IRB.getInt64(i);
                    Value *DstGEP = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), Dst, Offset);
                    StoreInst *Store = IRB.CreateStore(ValueToSet, DstGEP);
                    Store->setVolatile(cast<ConstantInt>(IsVolatile)->isOne());
                }

                CI->eraseFromParent();
                modified = true;
            }
        }

        return modified;
    }

};
