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
#include "llvm/Target/X86/X86.h"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to test a substitution of the `mov` instruction.
 */
class TestSubstitution {

public:

    /**
     * Constructor for the TestSubstitution class.
     */
    TestSubstitution() {

    }

    /**
     * Main execution method for the TestSubstitution class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF)  {
        bool Modified = false;

        errs() << "      ↺ TestSubstitution passed function " << MF.getName() << ".\n";

		// Iterate through the basic blocks and instructions
		for (auto &MBB : MF) {
			for (auto &MI : MBB) {
				// Example of substituting MOV with XOR+ADD
				if (MI.getOpcode() == TargetOpcode::MOV) {
					// Substitute with XOR+ADD
					MachineOperand &dest = MI.getOperand(0);
					MachineOperand &src = MI.getOperand(1);

					if (src.isImm()) {
						int64_t immValue = src.getImm();
						int64_t randValue = rand() % 1000;  // Random value for obfuscation
						
						// XOR reg, reg
						MachineInstr *newInst = BuildMI(
							MBB, 
							MI, 
							MI.getDebugLoc(), 
							MF.getSubtarget().getInstrInfo()->get(TargetOpcode::XOR)
						).addOperand(dest).addOperand(dest);

						// ADD reg, (imm - rand)
						BuildMI(
							MBB, 
							MI, 
							MI.getDebugLoc(), 
							MF.getSubtarget().getInstrInfo()->get(TargetOpcode::ADD)
						).addOperand(dest).addImm(immValue - randValue);

						MI.eraseFromParent();
						Modified = true;
					}
				}

			}
		}

	    return Modified;
    }

};