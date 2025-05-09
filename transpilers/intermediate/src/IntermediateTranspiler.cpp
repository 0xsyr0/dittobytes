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
 * Modules
 */
#include "modules/expand_memcpy_calls/ExpandMemcpyCallsModule.cpp"
#include "modules/move_globals_to_stack/MoveGlobalsToStackModule.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * LLVM function pass that calls specific modules of each function it visits.
 *
 * IntermediateTranspiler is a custom LLVM pass derived from PassInfoMixin.
 * It operates at the intermediate function level. This pass currently performs 
 * no transformation on its own, but calls several modules that do so.
 */
struct IntermediateTranspiler : PassInfoMixin<IntermediateTranspiler> {

    /**
     * Main execution method for the IntermediateTranspiler function pass.
     *
     * This function is called by LLVM when the pass is run on a function. It logs
     * the function name and indicates if all analyses are preserved or not.
     *
     * @param Function& F The function to run the pass on.
     * @param FunctionAnalysisManager& Unused, but required by the pass interface.
     * @return PreservedAnalyses Indicates that the function was not modified.
     */
    PreservedAnalyses run(Function& F, FunctionAnalysisManager&) {
        dbgs() << "      ↺ IntermediateTranspiler passed function " << F.getName() << ".\n";

        bool modified = false;

        // Module 1: Expand `memcpy` calls
        modified = ExpandMemcpyCallsModule().run(F) || modified;

        return modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

    /**
     * Main execution method for the IntermediateTranspiler module pass.
     *
     * This function is called by LLVM when the pass is run on a module. It logs
     * the module name and indicates if all analyses are preserved or not.
     *
     * @param Function& F The function to run the pass on.
     * @param FunctionAnalysisManager& Unused, but required by the pass interface.
     * @return PreservedAnalyses Indicates that the function was not modified.
     */
    PreservedAnalyses run(Module& M, ModuleAnalysisManager& MAM) {
        dbgs() << "      ↺ IntermediateTranspiler passed module " << M.getName() << ".\n";

        bool modified = false;

        // Module 1: Move global variables to the stack
        modified = MoveGlobalsToStackModule().run(M, MAM) || modified;

        return modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

};

/**
 * Provides metadata and registration callbacks for the IntermediateTranspiler pass plugin.
 *
 * This function defines the plugin's name, version, and the callbacks used to register
 * the custom pass with LLVM's new pass manager. It is called by llvmGetPassPluginInfo().
 *
 * @return PassPluginLibraryInfo Struct containing the plugin's metadata and registration hooks.
 */
PassPluginLibraryInfo getPolymorphicTranspilerPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, 
        "IntermediateTranspiler", 
        LLVM_VERSION_STRING, 
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM, ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "intermediate-transpiler") {
                        MPM.addPass(IntermediateTranspiler());
                        return true;
                    }
                    return false;
                }
            );

            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM, ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "intermediate-transpiler") {
                        FPM.addPass(IntermediateTranspiler());
                        return true;
                    }

                    return false;
                }
            );

            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    MPM.addPass(IntermediateTranspiler());

                    FunctionPassManager FPM;
                    FPM.addPass(IntermediateTranspiler());
                    MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
                }
            );
        }
    };
};

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