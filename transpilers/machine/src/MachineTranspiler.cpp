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
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/CodeGen/MachinePassRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CodeGen/MachinePassManager.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * LLVM function pass that logs the name of each function it visits.
 *
 * MachineTranspiler is a custom LLVM pass derived from MachineFunctionPass.
 * It operates at the machine function level. This pass currently performs 
 * no transformation but serves as a base for future instrumentation or analysis.
 */
class MachineTranspiler : public MachineFunctionPass {

public:

    /**
     * Pass ID
     */
    static char ID;  // Pass ID

    /**
     * Constructor for the MachineTranspiler pass.
     * 
     * Initializes the pass with the unique ID.
     */
    MachineTranspiler() : MachineFunctionPass(ID) {

    }

    /**
     * Retrieves the name of the pass.
     * 
     * This function returns the pass name, used for identification when running
     * the pass through LLVM's pass manager.
     * 
     * @return StringRef The name of the pass.
     */
    StringRef getPassName() const override {
        return "MachineTranspiler";
    }

    /**
     * Main execution method for the MachineTranspiler pass.
     *
     * This function is called by LLVM when the pass is run on a machine function.
     * It iterates through the machine basic blocks and checks each machine instruction.
     * If a `mov` instruction is found, it logs the instruction and marks the function as modified.
     * 
     * @param MachineFunction& MF The machine function to run the pass on.
     * @return bool Indicates if the machine function was modified (i.e., any `mov` instruction found).
     */
    bool runOnMachineFunction(MachineFunction &MF) override {
        bool Modified = false;

        errs() << "    - Running MachineTranspiler on function...\n";

        return Modified;
    }

};

/**
 * Define the Pass ID and register the MachineTranspiler pass with LLVM.
 *
 * This ensures that `llc -run-pass` can recognize and run the pass by name.
 */
char MachineTranspiler::ID = 0;
static llvm::RegisterPass<MachineTranspiler> X("MachineTranspiler", "The Dittobytes polymorphic transpiler!");
