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
#include "../../../../../shared/helpers/RandomHelper.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate mov immediate values.
 */
class TransformNullificationsOptionAMD64 {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

public:

    /**
     * Main execution method for the TransformNullificationsOptionAMD64 class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @param bool modifyAll Whether all the occurrences should be modified (for testing purposes).
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF, bool modifyAll) {
        // Local variables
        const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
        MachineRegisterInfo &MRI = MF.getRegInfo();

        // Inform user that we are running this option of the module
        dbgs() << "        ↳ Running AMD64 module: TransformNullifications(modifyAll=" << modifyAll << ").\n";

        // For each line in each basic block, perform our substitution
        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                // Only modify `mov` instructions with immediate values
                if (!isXorToNullifyRegister(Instruction)) {
                    continue;
                }

                // Inform user that we encountered a `mov` instruction with immediate value
                dbgs() << "          ↳ Found AMD64 xor instruction to nullify a register: ";
                Instruction.print(dbgs());

                if (!modifyAll && !RandomHelper::getChanceOneIn(2)) {
                    dbgs() << "          ↳ Skipping modification because change of 1 in X was not hit.";
                }

                // Build the new instruction: mov reg, 0
                DebugLoc DL = Instruction.getDebugLoc();
                BuildMI(MachineBasicBlock, Instruction, DL, TII->get(getXorSizeMovReplacement(Instruction)), Instruction.getOperand(0).getReg()).addImm(0);

                Instruction.eraseFromParent();
                modified = true;
            }
        }

        return modified;
    }

private:

    /**
     * Checks if the given instruction is a XOR with two times the same registry (to nullify a register).
     * 
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked to determine if it's a XOR to nullify a register.
     * @return bool Returns `true` if the instruction is a XOR to nullify a register, otherwise `false`.
     */
    bool isXorToNullifyRegister(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        if (instruction.getNumOperands() < 3) return false;
        if (!instruction.getOperand(0).isReg()) return false;
        if (!instruction.getOperand(1).isReg()) return false;
        if (!instruction.getOperand(2).isReg()) return false;

        if (opcode != X86::XOR8rr && opcode != X86::XOR16rr && opcode != X86::XOR32rr && opcode != X86::XOR64rr) {
            return false;
        }

        // XOR dst, src => dst = dst ^ src; check dst == src
        unsigned dst = instruction.getOperand(0).getReg();
        unsigned src1 = instruction.getOperand(1).getReg();
        unsigned src2 = instruction.getOperand(2).getReg();

        return (dst == src1) && (src1 == src2);
    }

    /**
     * Determines the MOV replacement opcode for a given XOR instruction opcode.
     * 
     * This function maps certain XOR instruction opcodes to corresponding MOV opcodes
     * for AMD64 instructions. The provided `MachineInstr`'s opcode is checked and
     * replaced with an appropriate MOV opcode based on the XOR instruction's size.
     * 
     * If the opcode does not match any known XOR instruction types, a fatal error is reported.
     *
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked and replaced with the corresponding XOR opcode.
     * @return unsigned The corresponding MOV opcode for the XOR instruction's immediate size.
     */
    unsigned getXorSizeMovReplacement(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case X86::XOR8rr: return X86::MOV8ri;
            case X86::XOR16rr: return X86::MOV16ri;
            case X86::XOR32rr: return X86::MOV32ri;
            case X86::XOR64rr: return X86::MOV64ri;
            default:
                report_fatal_error(formatv("TransformNullificationsOptionAMD64 - Unknown MOV replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

};