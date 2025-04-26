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

#pragma once

/**
 * LLVM includes
 */
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachinePassManager.h"
#include "llvm/CodeGen/MachinePassRegistry.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/Module.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/AArch64/AArch64.h"
#include "llvm/Target/AArch64/AArch64InstrInfo.h"
#include "llvm/TargetParser/Triple.h"

/**
 * Regular includes
 */
#include "../../helpers/RandomHelper.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate `mov` immediate values.
 */
class ARM64_ModifyImmediateModule {

public:

    /**
     * Main execution method for the ARM64_ModifyImmediateModule class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF) {
        const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
        MachineRegisterInfo &MRI = MF.getRegInfo();
        bool Modified = false;

        errs() << "        ↳ Running ARM64 architecture specific implementation.\n";

        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;
 
                // Only modify `mov` instructions with immediate values
                if (!isMovImmediate(Instruction)) {
                    continue;
                }

                // Print debug information
                errs() << "          ↳ Found ARM64 `mov` instruction: ";
                Instruction.print(errs());
            }
        }

        return Modified;
    }

private:

    bool isMovImmediate(const MachineInstr &MI)  {
        unsigned Opcode = MI.getOpcode();

        return 
            Opcode == AArch64::MOVi32imm    ||
            Opcode == AArch64::MOVi64imm    || 
            Opcode == AArch64::ORRXri       || 
            Opcode == AArch64::ORRWri;
    }

};