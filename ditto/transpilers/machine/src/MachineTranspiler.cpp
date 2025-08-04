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
 * Modules
 */
#include "modules/insert_semantic_noise/InsertSemanticNoiseModule.cpp"
#include "modules/transform_nullifications/TransformNullificationsModule.cpp"
#include "modules/transform_reg_mov_immediates/TransformRegMovImmediatesModule.cpp"
#include "modules/transform_stack_mov_immediates/TransformStackMovImmediatesModule.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * LLVM function pass that calls specific modules of each function it visits.
 *
 * MachineTranspiler is a custom LLVM pass derived from MachineFunctionPass.
 * It operates at the machine function level. This pass currently performs 
 * no transformation on its own, but calls several modules that do so.
 */
class MachineTranspiler : public MachineFunctionPass {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

    /**
     * Available steps in the pass pipeline where this pass is invoked.
     */
    enum MachineTranspilerStep {
        UnknownStep,
        FirstStep,
        LastStep
    };

    /**
     * Retrieve a state of where we currently are in the pass pipeline.
     * 
     * @returns MachineTranspilerStep Where in the pass pipeline we currently are.
     */
    MachineTranspilerStep getMachineTranspilerStep() {
        const char* machineTranspilerStep = std::getenv("MACHINE_TRANSPILER_STEP");

        if (machineTranspilerStep) {
            if (std::string(machineTranspilerStep) == "first") return FirstStep;
            if (std::string(machineTranspilerStep) == "last") return LastStep;
        }

        return UnknownStep;
    }


public:

    /**
     * Pass ID
     */
    static char ID;

    /**
     * Constructor for the MachineTranspiler pass.
     * 
     * Initializes the pass with the unique ID.
     */
    MachineTranspiler() : MachineFunctionPass(ID) {
        RandomHelper::seed();
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
     * 
     * @param MachineFunction& MF The machine function to run the pass on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF) override {
        MachineTranspilerStep step = getMachineTranspilerStep();

        dbgs() << "      ↳ MachineTranspiler passing function `" << MF.getName() << "(...)` for step `" << step << "`.\n";

        switch (step) {
            case FirstStep:
                // Module: Modify `mov reg, imm` immediate's
                modified = TransformRegMovImmediatesModule().runOnMachineFunction(MF) || modified;
                // Module: Modify `mov [reg+var_a], imm` immediate's
                modified = TransformStackMovImmediatesModule().runOnMachineFunction(MF) || modified;
                break;
            case LastStep:
                // Module: Insert semantic noise (meaningful dead code)
                // modified = InsertSemanticNoiseModule().runOnMachineFunction(MF) || modified;
                // Module: Replace `xor reg, reg` instructions
                modified = TransformNullificationsModule().runOnMachineFunction(MF) || modified;
                break;
            case UnknownStep:        
                dbgs() << "        ↳ Unknown step `" << step << "`.\n";
                break;
        }

        return modified;
    }

};

/**
 * Define the Pass ID and register the MachineTranspiler pass with LLVM.
 *
 * This ensures that `llc -run-pass` can recognize and run the pass by name.
 */
char MachineTranspiler::ID = 0;
static llvm::RegisterPass<MachineTranspiler> MachineTranspilerRegistration(
    "MachineTranspiler", 
    "The Dittobytes MachineFunctionPass Transpiler!"
);
