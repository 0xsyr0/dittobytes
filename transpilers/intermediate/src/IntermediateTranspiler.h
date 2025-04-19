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
 * The author and owner of this specific function pass is the Shellvm organisation: 
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

#ifndef INTERMEDIATE_TRANSPILER_H
#define INTERMEDIATE_TRANSPILER_H

#include "llvm/IR/PassManager.h"

using namespace llvm;

struct IntermediateTranspiler : PassInfoMixin<IntermediateTranspiler> {

    bool shouldInline(GlobalVariable &G);

    void disaggregateVars(Instruction *After, Value *Ptr, SmallVectorImpl<Value *> &Idx, ConstantAggregate &C, SmallSetVector<GlobalVariable *, 4> &Vars);

    void extractValuesFromStore(StoreInst *inst, SmallSetVector<GlobalVariable *, 4> &Vars);

    void inlineGlobals(Function *F, SmallSetVector<GlobalVariable *, 4> &Vars);

    llvm::PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);

};

#endif