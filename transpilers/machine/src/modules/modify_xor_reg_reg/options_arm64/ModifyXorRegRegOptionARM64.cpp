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
#include "../../../../../shared/helpers/RandomHelper.cpp"

/**
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate `mov` immediate values.
 */
class ModifyXorRegRegOptionARM64 {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

public:

    /**
     * Main execution method for the ModifyXorRegRegOptionARM64 class.
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
        dbgs() << "        ↳ Running ARM64 module: ModifyXorRegReg(modifyAll=" << modifyAll << ").\n";

        // For each line in each basic block, perform our substitution
        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                bool instructionIsXorToNullifyRegister = isXorToNullifyRegister(Instruction);
                bool instructionIsMovToNullifyRegister = isMovToNullifyRegister(Instruction);

                // Only modify `mov` instructions with immediate values
                if (!instructionIsXorToNullifyRegister && !instructionIsMovToNullifyRegister) {
                    continue;
                }

                // Inform user that we encountered a `mov` instruction with immediate value
                dbgs() << "          ↳ Found ARM64 xor instruction to nullify a register: ";
                Instruction.print(dbgs());

                if (!modifyAll && !RandomHelper::getChanceOneIn(2)) {
                    dbgs() << "          ↳ Skipping modification because change of 1 in X was not hit.";
                }

                DebugLoc DL = Instruction.getDebugLoc();

                if (instructionIsXorToNullifyRegister) {
                    // Build the new instruction: mov reg, 0
                    BuildMI(MachineBasicBlock, Instruction, DL, TII->get(getXorSizeMovReplacement(Instruction)), Instruction.getOperand(0).getReg())
                        .addImm(0)
                        .addImm(0);
                }

                if (instructionIsMovToNullifyRegister) {
                    // Build the new instruction: xor reg, reg
                    unsigned xorOpcode = getMovSizeXorReplacement(Instruction);
                    BuildMI(MachineBasicBlock, Instruction, DL, TII->get(xorOpcode), Instruction.getOperand(0).getReg())
                        .addReg((xorOpcode == AArch64::ORRWrs) ? AArch64::WZR : AArch64::XZR)
                        .addReg((xorOpcode == AArch64::ORRWrs) ? AArch64::WZR : AArch64::XZR)
                        .addImm(0);
                }

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
     * @param MachineFunction& instruction instruction The `MachineInstr` whose opcode will be checked to determine if it's a XOR to nullify a register.
     * @return bool Returns `true` if the instruction is a XOR to nullify a register, otherwise `false`.
     */
    bool isXorToNullifyRegister(const MachineInstr& instruction) {
        unsigned opcode = instruction.getOpcode();

        if (instruction.getNumOperands() < 4) return false;
        if (!instruction.getOperand(0).isReg() || !instruction.getOperand(1).isReg() || !instruction.getOperand(2).isReg()) return false;

        Register dst = instruction.getOperand(0).getReg();
        Register src1 = instruction.getOperand(1).getReg();
        Register src2 = instruction.getOperand(2).getReg();

        // Match EOR reg, reg, reg (rr form)
        if ((opcode == AArch64::EORWrr || opcode == AArch64::EORXrr) && dst == src1 && src1 == src2 && dst.isPhysical()) {
            return true;
        }

        // Match EOR reg, reg, reg, shift (rs form) only if shift type is LSL and amount is 0
        if ((opcode == AArch64::EORWrs || opcode == AArch64::EORXrs) && dst == src1 && src1 == src2 && dst.isPhysical()) {
            const MachineOperand &shiftOp = instruction.getOperand(3);
            if (!shiftOp.isImm()) return false;

            unsigned shiftAmt = shiftOp.getImm(); // packed: (shiftType << 6) | amount
            unsigned shiftType = shiftAmt >> 6;
            unsigned shiftVal = shiftAmt & 0x3F;

            // AArch64::LSL == 0
            if (shiftType == AArch64SE::LSL && shiftVal == 0) {
                return true;
            }
        }

        // Match OOR reg, reg, shift (if NULL register is used)
        if (opcode == AArch64::ORRWrs) {
            return (src1 == AArch64::WZR && src2 == AArch64::WZR);
        } else if (opcode == AArch64::ORRXrs) {
            return (src1 == AArch64::XZR && src2 == AArch64::XZR);
        }

        return false;
    }

    /**
     * Checks if the given instruction is a MOV with that moves zero to a register (to nullify a register).
     * 
     * @param MachineFunction& instruction instruction The `MachineInstr` whose opcode will be checked to determine if it's a MOV to nullify a register.
     * @return bool Returns `true` if the instruction is a MOV to nullify a register, otherwise `false`.
     */
    bool isMovToNullifyRegister(const MachineInstr& instruction) {
        unsigned opcode = instruction.getOpcode();

        // Match only MOVZ (zeroing) instructions
        if (opcode != AArch64::MOVZWi && opcode != AArch64::MOVZXi)
            return false;

        // Must have 3 operands: dest reg, imm, shift
        if (instruction.getNumOperands() != 3)
            return false;

        const MachineOperand &immOp = instruction.getOperand(1);
        const MachineOperand &shiftOp = instruction.getOperand(2);

        // Check immediate and shift
        return immOp.isImm() && immOp.getImm() == 0 && shiftOp.isImm() && shiftOp.getImm() == 0;
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
            case AArch64::EORWrr:
            case AArch64::EORWrs:
            case AArch64::ORRWrs:
                return AArch64::MOVZWi;
            case AArch64::EORXrr:
            case AArch64::EORXrs:
            case AArch64::ORRXrs:
                return AArch64::MOVZXi;
            default:
                report_fatal_error(formatv("ModifyXorRegRegOptionARM64 - Unknown MOV replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

    /**
     * Determines the XOR replacement opcode for a given MOV instruction opcode.
     * 
     * This function maps certain MOV instruction opcodes to corresponding XOR opcodes
     * for AMD64 instructions. The provided `MachineInstr`'s opcode is checked and
     * replaced with an appropriate MOV opcode based on the XOR instruction's size.
     * 
     * If the opcode does not match any known MOV instruction types, a fatal error is reported.
     *
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked and replaced with the corresponding MOV opcode.
     * @return unsigned The corresponding XOR opcode for the MOV instruction's immediate size.
     */
    unsigned getMovSizeXorReplacement(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case AArch64::MOVZWi:
                return AArch64::ORRWrs;
            case AArch64::MOVZXi:
                return AArch64::ORRXrs;
            default:
                report_fatal_error(formatv("ModifyXorRegRegOptionARM64 - Unknown XOR replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

};