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
#include "../../../../shared/helpers/RandomHelper.cpp"

/**
 * Modify `mov` immediate substitution options
 */
#include "options_amd64/TransformMovImmediatesOptionAMD64_XOR.cpp"
#include "options_arm64/TransformMovImmediatesOptionARM64_XOR.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate mov immediate values.
 */
class TransformMovImmediatesModule {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

    /**
     * List of lambdas that invoke the `run` methods of AMD64 option classes.
     */
    std::vector<std::function<bool(MachineFunction&, bool)>> options_amd64;

    /**
     * List of lambdas that invoke the `run` methods of ARM64 option classes.
     */
    std::vector<std::function<bool(MachineFunction&, bool)>> options_arm64;

    /**
     * Whether the module is enabled (default) or disabled.
     * 
     * @returns bool Positive if enabled.
     */
    bool moduleIsEnabled() {
        const char* MM_TEST_TRANSFORM_MOV_IMMEDIATES = std::getenv("MM_TEST_TRANSFORM_MOV_IMMEDIATES");
        bool result = (MM_TEST_TRANSFORM_MOV_IMMEDIATES && std::string(MM_TEST_TRANSFORM_MOV_IMMEDIATES) == "true");

        return result || moduleIsBeingTested();
    }

    /**
     * Whether the module is being feature tested or not (default).
     * 
     * @returns bool Positive if enabled.
     */
    bool moduleIsBeingTested() {
        const char* MM_TRANSFORM_MOV_IMMEDIATES = std::getenv("MM_TRANSFORM_MOV_IMMEDIATES");
        bool result = (MM_TRANSFORM_MOV_IMMEDIATES && std::string(MM_TRANSFORM_MOV_IMMEDIATES) == "true");

        return result;
    }

public:

    /**
     * Constructor that initializes the list of substitution option classes.
     */
    TransformMovImmediatesModule() {
        options_amd64 = {
            [&](MachineFunction &MF, bool modifyAll) { return TransformMovImmediatesOptionAMD64_XOR().runOnMachineFunction(MF, modifyAll); }
        };

        options_arm64 = {
            [&](MachineFunction &MF, bool modifyAll) { return TransformMovImmediatesOptionARM64_XOR().runOnMachineFunction(MF, modifyAll); }
        };
    }

    /**
     * Main execution method for the TransformMovImmediatesModule class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF) {
        // Ensure module is enabled
        if (!moduleIsEnabled()) return false;
        bool modifyAll = moduleIsBeingTested();

        // Run random compatible options
        auto architecture = MF.getTarget().getTargetTriple().getArch();
        int index = 0;

        switch (architecture) {
            // AMD64
            case Triple::x86_64:
                index = std::rand() % options_amd64.size();
                modified = options_amd64[index](MF, modifyAll) || modified;
                break;
            // ARM64
            case Triple::aarch64:
                index = std::rand() % options_arm64.size();
                modified = options_arm64[index](MF, modifyAll) || modified;
                break;
            // Unknown architecture
            default:
                report_fatal_error(formatv("TransformMovImmediatesModule failed due to unknown architecture: {0}.", architecture));
                break;
        }

        return modified;
    }

};