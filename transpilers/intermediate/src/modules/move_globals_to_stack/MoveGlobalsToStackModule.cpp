/**
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###################                                         ###################
 * ################### THIS IS AN EXTERNAL LIBRARY FROM GITHUB ###################
 * ###################                                         ###################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * ###############################################################################
 * 
 * The author and owner of this specific function pass is the SheLLVM organisation: 
 * https://github.com/SheLLVM/SheLLVM. Copyright may apply. Please refer to the 
 * original source.
 * 
 * Original source: https://github.com/SheLLVM/SheLLVM/tree/master
 */

/**
 * Copyright (c) 2018 SheLLVM Development Team. All rights reserved.
 * 
 * Developed by: SheLLVM Development Team
 * https://github.com/SheLLVM
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal with the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimers.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimers in the documentation and/or other materials provided with the distribution.
 * Neither the names of SheLLVM Development Team, SheLLVM, nor the names of its contributors may be used to endorse or promote products derived from this Software without specific prior written permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
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
 * A class to move globals to the stack.
 */
class MoveGlobalsToStackModule {

private:

    /**
     * Whether the module is enabled (default) or disabled.
     * 
     * @returns bool Positive if enabled.
     */
    bool moduleIsEnabled() {
        const char* MOVE_GLOBALS_TO_STACK = std::getenv("MOVE_GLOBALS_TO_STACK");
        return (MOVE_GLOBALS_TO_STACK && std::string(MOVE_GLOBALS_TO_STACK) == "true");
    }

public:

    /**
     * Main execution method for the MoveGlobalsToStackModule class.
     *
     * @param Module& M The intermediate module to run on.
     * @param ModuleAnalysisManager& The LLVM module analysis manager.
     * @return bool Indicates if the intermediate module was modified.
     */
    bool run(Module &M, ModuleAnalysisManager &) {
        // Ensure module is enabled
        if (!moduleIsEnabled()) return false;
        
        // Inform user that we are running this module
        dbgs() << "        ↳ Running MoveGlobalsToStackModule module.\n";

        SmallMapVector<Function *, SmallSetVector<GlobalVariable *, 4>, 4> usage;

        for (GlobalVariable &G : M.globals()) {
            if (shouldInline(G)) {
                usage[getUsingFunction(G)].insert(&G);
            }
        }

        for (auto &KV : usage) {
            inlineGlobals(KV.first, KV.second);
        }

        return !usage.empty();
    }

private:

    Function * getUsingFunction(Value & V) {
        Function * F = nullptr;

        SmallVector < User * , 4 > Worklist;
        SmallSet < User * , 4 > Visited;

        for (auto * U: V.users())
            Worklist.push_back(U);
        while (!Worklist.empty()) {
            auto * U = Worklist.pop_back_val();

            if (Visited.count(U))
                continue;
            else
                Visited.insert(U);

            if (isa < ConstantExpr > (U) || isa < ConstantAggregate > (U) ||
                isa < GlobalVariable > (U)) {
                if (isa < GlobalVariable > (U) &&
                    !cast < GlobalVariable > (U) -> isDiscardableIfUnused())
                    return nullptr;
                for (auto * UU: U -> users()) {
                    Worklist.push_back(UU);
                }
                continue;
            }

            auto * I = dyn_cast < Instruction > (U);
            if (!I)
                return nullptr;
            if (!F)
                F = I -> getParent() -> getParent();
            if (I -> getParent() -> getParent() != F)
                return nullptr;
        }

        return F;
    }

    // Copied from GlobalOpt.cpp
    void makeAllConstantUsesInstructions(Constant * C) {
        SmallVector < ConstantExpr * , 4 > Users;
        for (auto * U: C -> users()) {
            if (auto * CE = dyn_cast < ConstantExpr > (U))
                Users.push_back(CE);
            else
                // We should never get here; allNonInstructionUsersCanBeMadeInstructions
                // should not have returned true for C.
                assert(
                    isa < Instruction > (U) &&
                    "Can't transform non-constantexpr non-instruction to instruction!");
        }

        SmallVector < Instruction * , 4 > CEUsers;
        for (auto * U: Users) {
            // DFS DAG traversal of U to eliminate ConstantExprs recursively
            ConstantExpr * CE = nullptr;

            do {
                CE = U; // Start by trying to destroy the root

                CEUsers.clear();
                auto it = CE -> user_begin();
                while (it != CE -> user_end()) {
                    if (isa < ConstantExpr > ( * it)) {
                        // Recursive ConstantExpr found; switch to it
                        CEUsers.clear();
                        CE = cast < ConstantExpr > ( * it);
                        it = CE -> user_begin();
                    } else {
                        // Function; add to UUsers
                        CEUsers.push_back(cast < Instruction > ( * it));
                        it++;
                    }
                }

                // All users of CE are instructions; replace CE with an instruction for
                // each
                for (auto * CEU: CEUsers) {
                    Instruction * NewU = CE -> getAsInstruction();
                    NewU -> insertBefore(CEU);
                    CEU -> replaceUsesOfWith(CE, NewU);
                }

                // We've replaced all the uses, so destroy the constant. (destroyConstant
                // will update value handles and metadata.)
                CE -> destroyConstant();
            } while (CE != U); // Continue until U is destroyed
        }
    }

    bool shouldInline(GlobalVariable & G) {
        if (!G.isDiscardableIfUnused())
            return false; // Goal is to discard these; ignore if that's not possible
        if (!getUsingFunction(G))
            return false; // This isn't safe. We can only be on one function's stack.

        return true;
    }

    void disaggregateVars(
        Instruction * After, Value * Ptr, SmallVectorImpl < Value * > & Idx,
        ConstantAggregate & C, SmallSetVector < GlobalVariable * , 4 > & Vars) {
        SmallSetVector < Value * , 4 > ToUndefine;

        Constant * C2;
        for (unsigned i = 0;
            (C2 = C.getAggregateElement(i)); i++) {
            Idx.push_back(ConstantInt::get(
                Type::getInt32Ty(After -> getParent() -> getContext()), i));

            if (isa < ConstantAggregate > (C2)) {
                disaggregateVars(After, Ptr, Idx, cast < ConstantAggregate > ( * C2), Vars);

            } else if (isa < ConstantExpr > (C2) ||
                (isa < GlobalVariable > (C2) &&
                    Vars.count(cast < GlobalVariable > (C2)))) {
                GetElementPtrInst * GEP =
                    GetElementPtrInst::CreateInBounds(C.getType(), Ptr, Idx);
                GEP -> insertAfter(After);

                ToUndefine.insert(C2);

                new StoreInst(C2, GEP, GEP -> getNextNode());
            }

            Idx.pop_back();
        }

        for (auto * V: ToUndefine)
            C.handleOperandChange(V, UndefValue::get(V -> getType()));
    }

    void extractValuesFromStore(
        StoreInst * inst, SmallSetVector < GlobalVariable * , 4 > & Vars) {
        Value * V = inst -> getValueOperand();
        if (!isa < ConstantAggregate > (V))
            return;

        SmallVector < Value * , 4 > Idx;
        Idx.push_back(
            ConstantInt::get(Type::getInt32Ty(inst -> getParent() -> getContext()), 0));

        disaggregateVars(inst, inst -> getPointerOperand(), Idx,
            cast < ConstantAggregate > ( * V), Vars);
    }

    void inlineGlobals(Function * F,
        SmallSetVector < GlobalVariable * , 4 > & Vars) {
        BasicBlock & BB = F -> getEntryBlock();
        Instruction * insertionPoint = & * BB.getFirstInsertionPt();

        // Step one: Bring all vars into F
        SmallMapVector < GlobalVariable * , Instruction * , 4 > Replacements;
        StoreInst * firstStore = nullptr;
        for (auto * G: Vars) {
            Instruction * inst =
                new AllocaInst(G -> getValueType(), G -> getType() -> getAddressSpace(),
                    nullptr, G -> getAlign().valueOrOne(), "",
                    firstStore ? firstStore : insertionPoint);

            inst -> takeName(G);

            Replacements[G] = inst;

            if (G -> hasInitializer()) {
                Constant * initializer = G -> getInitializer();
                StoreInst * store = new StoreInst(initializer, inst, insertionPoint);
                G -> setInitializer(nullptr);

                extractValuesFromStore(store, Vars);

                if (!firstStore)
                    firstStore = store;
            }
        }

        // Step two: Replace all uses
        for (auto & KV: Replacements) {
            // Some users of G might be ConstantExprs. These can't refer
            // to Instructions, so we need to turn them into explicit Instructions.
            makeAllConstantUsesInstructions(KV.first);

            KV.first -> replaceAllUsesWith(KV.second);
            KV.first -> eraseFromParent();
        }

    }

};