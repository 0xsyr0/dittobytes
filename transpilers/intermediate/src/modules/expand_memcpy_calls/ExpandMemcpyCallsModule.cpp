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
 * A class to expand `memcpy` calls to manual copies.
 */
class ExpandMemcpyCallsModule {

private:

    /**
     * Whether this class modified the intermediate function.
     */
    bool modified = false;

public:

    /**
     * Main execution method for the ExpandMemcpyCallsModule class.
     *
     * @param Function& F The intermediate function to expand memcpy calls in.
     * @return bool Indicates if the intermediate function was modified.
     */
    bool run(Function &F) {
        SmallVector<CallInst *, 8> MemCpyCalls;

        // Inform user that we are running this module
        dbgs() << "        ↳ Running ExpandMemcpyCalls module.\n";

        for (auto &BB : F) {
            for (auto &I : BB) {
                if (auto *CI = dyn_cast<CallInst>(&I)) {
                    if (auto *F = CI->getCalledFunction()) {
                        if (F->getName().starts_with("llvm.memcpy")) {
                            MemCpyCalls.push_back(CI);
                        }
                    }
                }
            }
        }

        for (auto *CI : MemCpyCalls) {
            // Inform user that we encountered a `memcpy` call
            dbgs() << "          ↳ Expanding a `memcpy` call.";

            auto *Dst = CI->getArgOperand(0);
            auto *Src = CI->getArgOperand(1);
            auto *Len = CI->getArgOperand(2);
            auto *IsVolatile = CI->getArgOperand(3);

            if (auto *ConstLen = dyn_cast<ConstantInt>(Len)) {
                uint64_t Size = ConstLen->getZExtValue();
                IRBuilder<> IRB(CI);

                for (uint64_t i = 0; i < Size; ++i) {
                    Value *Offset = IRB.getInt64(i);
                    Value *SrcGEP = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), Src, Offset);
                    Value *DstGEP = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), Dst, Offset);
                    LoadInst *Load = IRB.CreateLoad(IRB.getInt8Ty(), SrcGEP);
                    Load->setVolatile(cast<ConstantInt>(IsVolatile)->isOne());
                    StoreInst *Store = IRB.CreateStore(Load, DstGEP);
                    Store->setVolatile(cast<ConstantInt>(IsVolatile)->isOne());
                }

                CI->eraseFromParent();
                modified = true;
            }
        }

        return modified;
    }

};
