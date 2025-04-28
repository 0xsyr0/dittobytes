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
#include "llvm/Target/X86/X86.h"
#include "llvm/Target/X86/X86InstrInfo.h"
#include "llvm/Target/X86/X86TargetMachine.h"
#include "llvm/TargetParser/Triple.h"

/**
 * Regular includes
 */
#include "../../helpers/RandomHelper.cpp"

/**
 * Modify `mov` immediate substitution options
 */
#include "./options_amd64/ModifyMovImmediateOptionAMD64_XOR.cpp"
#include "./options_arm64/ModifyMovImmediateOptionARM64_XOR.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate mov immediate values.
 */
class ModifyMovImmediateModule {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

    /**
     * List of lambdas that invoke the `run` methods of AMD64 option classes.
     */
    std::vector<std::function<bool(MachineFunction&)>> options_amd64;

    /**
     * List of lambdas that invoke the `run` methods of ARM64 option classes.
     */
    std::vector<std::function<bool(MachineFunction&)>> options_arm64;

public:

    /**
     * Constructor that initializes the list of substitution option classes.
     */
    ModifyMovImmediateModule() {
        options_amd64 = {
            [&](MachineFunction &MF) { return ModifyMovImmediateOptionAMD64_XOR().runOnMachineFunction(MF); }
        };

        options_arm64 = {
            [&](MachineFunction &MF) { return ModifyMovImmediateOptionARM64_XOR().runOnMachineFunction(MF); }
        };
    }

    /**
     * Main execution method for the ModifyMovImmediateModule class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF) {
        auto architecture = MF.getTarget().getTargetTriple().getArch();
        int index = 0;

        switch (architecture) {
            // AMD64
            case Triple::x86_64:
                index = std::rand() % options_amd64.size();
                modified = options_amd64[index](MF) || modified;
                break;
            // ARM64
            case Triple::aarch64:
                index = std::rand() % options_arm64.size();
                modified = options_arm64[index](MF) || modified;
                break;
            // Unknown architecture
            default:
                report_fatal_error(formatv("MachineTranspiler failed due to unknown architecture: {0}.", architecture));
                break;
        }

        return modified;
    }

};