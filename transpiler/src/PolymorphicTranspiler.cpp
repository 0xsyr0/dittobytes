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

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * LLVM function pass that logs the name of each function it visits.
 *
 * PolymorphicTranspiler is a custom LLVM pass derived from PassInfoMixin.
 * It operates at the function level and is intended for use with LLVM's new
 * pass manager. This pass currently performs no transformation but serves
 * as a base for future instrumentation or analysis.
 */
struct PolymorphicTranspiler : PassInfoMixin<PolymorphicTranspiler> {

    /**
     * Main execution method for the PolymorphicTranspiler pass.
     *
     * This function is called by LLVM when the pass is run on a function. It logs
     * the function name and indicates that all analyses are preserved (i.e., the IR is unchanged).
     *
     * @param Function& F The function to run the pass on.
     * @param FunctionAnalysisManager& Unused, but required by the pass interface.
     * @return PreservedAnalyses Indicates that the function was not modified.
     */
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        static bool RandSeeded = false;
        if (!RandSeeded) {
            srand(time(nullptr));
            RandSeeded = true;
        }

        errs() << "    - Found function: " << F.getName() << "\n";

        bool Modified = false;

        for (auto &BB : F) {
            for (auto It = BB.begin(), End = BB.end(); It != End; ) {
                Instruction *Inst = &*It++;
                if (auto *AddInst = dyn_cast<BinaryOperator>(Inst)) {
                    if (AddInst->getOpcode() == Instruction::Add && AddInst->getType()->isIntegerTy()) {
                        IRBuilder<> Builder(AddInst);
                        Value *A = AddInst->getOperand(0);
                        Value *B = AddInst->getOperand(1);
                        Value *Result = nullptr;

                        switch (rand() % 5) {
                            case 0: {
                                // Original add
                                Result = Builder.CreateAdd(A, B, "add_orig");
                                break;
                            }
                            case 1: {
                                // A - (-B)
                                Value *NegB = Builder.CreateNeg(B, "neg_b");
                                Result = Builder.CreateSub(A, NegB, "sub_neg_b");
                                break;
                            }
                            case 2: {
                                // (A ^ B) + 2 * (A & B)
                                Value *Xor = Builder.CreateXor(A, B, "xor_ab");
                                Value *And = Builder.CreateAnd(A, B, "and_ab");
                                Value *Shl = Builder.CreateShl(And, 1, "shl2_ab");
                                Result = Builder.CreateAdd(Xor, Shl, "opt_add1");
                                break;
                            }
                            case 3: {
                                // (A | B) + (A & B)
                                Value *Or = Builder.CreateOr(A, B, "or_ab");
                                Value *And = Builder.CreateAnd(A, B, "and_ab2");
                                Result = Builder.CreateAdd(Or, And, "opt_add2");
                                break;
                            }
                            case 4: {
                                // A + (B - 0)
                                Value *Zero = ConstantInt::get(A->getType(), 0);
                                Value *Temp = Builder.CreateSub(B, Zero, "temp_b");
                                Result = Builder.CreateAdd(A, Temp, "opt_add3");
                                break;
                            }
                        }

                        AddInst->replaceAllUsesWith(Result);
                        AddInst->eraseFromParent();
                        Modified = true;
                    }
                }
            }
        }

        return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }


};

/**
 * Provides metadata and registration callbacks for the PolymorphicTranspiler pass plugin.
 *
 * This function defines the plugin's name, version, and the callbacks used to register
 * the custom pass with LLVM's new pass manager. It is called by llvmGetPassPluginInfo().
 *
 * @return PassPluginLibraryInfo Struct containing the plugin's metadata and registration hooks.
 */
PassPluginLibraryInfo getPolymorphicTranspilerPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, 
        "PolymorphicTranspiler", 
        LLVM_VERSION_STRING, 
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM, ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "polymorphic-transpiler") {
                        FPM.addPass(PolymorphicTranspiler());
                        return true;
                    }

                    return false;
                }
            );

            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    FunctionPassManager FPM;
                    FPM.addPass(PolymorphicTranspiler());
                    MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
                }
            );
        }
    };
}

/**
 * Entry point for the LLVM pass plugin.
 *
 * This function is required by LLVM to identify and load the plugin. It returns
 * metadata about the plugin, including its name, version, and callback hooks.
 *
 * @return ::llvm::PassPluginLibraryInfo Struct containing plugin metadata and callbacks.
 */
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getPolymorphicTranspilerPluginInfo();
}