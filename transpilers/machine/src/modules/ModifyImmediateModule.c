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
#include "llvm/TargetParser/Triple.h"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate `mov` immediate values.
 */
class ModifyImmediateModule {

public:

    /**
     * Constructor for the ModifyImmediateModule class.
     */
    ModifyImmediateModule() {

    }

    /**
     * Main execution method for the ModifyImmediateModule class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF)  {
        bool Modified = false;

        errs() << "      ↺ ModifyImmediateModule passed function " << MF.getName() << ".\n";

		const auto *TII = MF.getSubtarget().getInstrInfo();

		for (const auto &MBB : MF) {
			for (const auto &MI : MBB) {
				if (X86::isMOV(MI.getOpcode())) {
					errs() << "         Found X86 MOV Instruction (using X86::isMov()): ";
					MI.print(errs());
					errs() << "\n";
				}
			}
		}

		return false; 
	}

};