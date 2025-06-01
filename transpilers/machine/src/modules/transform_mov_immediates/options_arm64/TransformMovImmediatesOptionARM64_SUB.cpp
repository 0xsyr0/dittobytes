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
class TransformMovImmediatesOptionARM64_SUB {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

public:

    /**
     * Main execution method for the TransformMovImmediatesOptionARM64_SUB class.
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
        dbgs() << "        ↳ Running ARM64 module: TransformMovImmediates(option=SUB,modifyAll=" << modifyAll << ").\n";

        // First convert `movn`, `movz` or `movk` instructions to regular movs
        normalizeMovs(MF);

        // For each line in each basic block, perform our substitution
        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                // Only modify `mov` instructions with immediate values
                if (!isMovImmediate(Instruction)) {
                    continue;
                }

                // Inform user that we encountered a `mov` instruction with immediate value
                dbgs() << "          ↳ Found ARM64 mov instruction with immediate: ";
                Instruction.print(dbgs());

                // Obtain information about the instruction
                const DebugLoc& debugLocation = Instruction.getDebugLoc();
                Register destinationRegister = Instruction.getOperand(0).getReg();
                size_t immediateValue = (size_t) Instruction.getOperand(1).getImm();
                size_t immediateSize = getMovImmediateSize(Instruction);
                size_t originalOpcode = Instruction.getOpcode();
                unsigned subOpcode = getMovSizeSubReplacement(Instruction);

                // Generate mask on compile time
                size_t subKey = RandomHelper::getSimilarIntegerForDestination(immediateSize, immediateValue, false);

                // `add` the immediate value and mask it to the correct size
                size_t immediateValueEncoded = (immediateValue + subKey);
                int64_t immediateMask = (immediateSize == 64) ? -1 : ((1ULL << immediateSize) - 1);
                immediateValueEncoded = immediateValueEncoded & immediateMask;

                // Register to use in the substition
                Register virtualSubKeyRegister;

                // Allocate a virtual register for the `sub` key
                switch (immediateSize) {
                    case 64: virtualSubKeyRegister = MRI.createVirtualRegister(&AArch64::GPR64RegClass); break;
                    default: virtualSubKeyRegister = MRI.createVirtualRegister(&AArch64::GPR32RegClass); break;
                }

                // 1. mov [original register], [encoded immediate value]
                // 2. mov [sub key register], [sub key]
                // 3. sub [original register], [original register], [sub key register]
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(originalOpcode), destinationRegister).addImm(immediateValueEncoded);
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(originalOpcode), virtualSubKeyRegister).addImm(subKey);
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(subOpcode), destinationRegister).addReg(destinationRegister).addReg(virtualSubKeyRegister);

                // Erase the original instruction after inserting the new ones
                Instruction.eraseFromParent();

                // Inform module and user that we've successfully substituted the immediate value.
                modified = true;
                dbgs() << "          ✓ Modified immediate value using random option `SUB`.\n";
            }
        }
        
        return modified;
    }

private:

    /**
     * Determines the size of the immediate value for a given machine instruction.
     * 
     * This function checks the opcode of the provided `MachineInstr` and returns the size
     * of the immediate value associated with the instruction. The size is returned in bits
     * (e.g., 8, 16, 32, or 64 bits) based on the instruction type.
     * 
     * If the opcode does not match any known MOV instruction types, a fatal error is reported.
     *
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked to determine the immediate size.
     * @return size_t The size of the immediate value in bits (32 or 64).
     */
    size_t getMovImmediateSize(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case AArch64::MOVi32imm:
                return 32;
                break;
            case AArch64::MOVi64imm:
                return 64;
                break;
            default:
                report_fatal_error(formatv("TransformMovImmediatesOptionARM64_SUB - Unknown immediate size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

    /**
     * Determines the SUB replacement opcode for a given MOV instruction opcode.
     * 
     * This function maps certain MOV instruction opcodes to corresponding SUB opcodes
     * for AMD64 instructions. The provided `MachineInstr`'s opcode is checked and
     * replaced with an appropriate SUB opcode based on the MOV instruction's immediate size.
     * 
     * If the opcode does not match any known MOV instruction types, a fatal error is reported.
     *
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked and replaced with the corresponding SUB opcode.
     * @return unsigned The corresponding SUB opcode for the MOV instruction's immediate size.
     */
    unsigned getMovSizeSubReplacement(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case AArch64::MOVi32imm: return AArch64::SUBWrr;
            case AArch64::MOVi64imm: return AArch64::SUBXrr;
            default:
                report_fatal_error(formatv("TransformMovImmediatesOptionARM64_SUB - Unknown SUB replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

    /**
     * Checks if the given instruction is a MOV instruction with an immediate operand.
     * 
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked to determine if it's a MOV with an immediate operand.
     * @return bool Returns `true` if the instruction is a MOV immediate instruction, otherwise `false`.
     */
    bool isMovImmediate(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        if (instruction.getNumOperands() != 2) return false;
        if (!instruction.getOperand(0).isReg()) return false;
        if (!instruction.getOperand(1).isImm()) return false;

        switch (opcode) {
            case AArch64::MOVi32imm:
            case AArch64::MOVi64imm:
                return true;
                break;
            default:
                return false;
                break;
        }
    }
        
    /**
     * Normalizes `movn`, `movz`, and `movk` instructions into standard `mov` instructions.
     *
     * This function scans through the machine function's instructions and replaces supported
     * immediate-move variants (e.g., MOVN, MOVZ) with equivalent `MOVi32imm` or `MOVi64imm`
     * instructions. This simplifies further transformations, such as SUB-based immediate obfuscation.
     * 
     * `MOVK` instructions are skipped as they require reconstructing full register values from partial updates.
     *
     * @param MachineFunction& MF The machine function to normalize MOV instructions in.
     */
    void normalizeMovs(MachineFunction& MF) {
        // Local variables
        const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();

        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                // Ensure we only substitute compatible instruction definitions
                if (Instruction.getNumOperands() != 3 || !Instruction.getOperand(1).isImm() || !Instruction.getOperand(2).isImm())
                    continue;

                // Inform user that we encountered a `mov(z|k|n)` instruction with immediate value
                dbgs() << "          ↳ Found ARM64 mov(z|k|n) instruction with immediate: ";
                Instruction.print(dbgs());

                // Obtain information about the instruction
                const DebugLoc& debugLocation = Instruction.getDebugLoc();
                Register destinationRegister = Instruction.getOperand(0).getReg();
                size_t immediateValue = (size_t) Instruction.getOperand(1).getImm();
                size_t immediateShift = (size_t) Instruction.getOperand(2).getImm();
                size_t originalOpcode = Instruction.getOpcode();

                // Keep track of new information
                bool replace = false;
                size_t newOpcode = 0;
                size_t newValue = 0;

                switch (originalOpcode) {
                    case AArch64::MOVNWi:
                        newValue = ~(immediateValue << immediateShift) & 0xFFFFFFFF;
                        newOpcode = AArch64::MOVi32imm;
                        replace = true;
                        break;
                    case AArch64::MOVNXi:
                        newValue = ~(immediateValue << immediateShift);
                        newOpcode = AArch64::MOVi64imm;
                        replace = true;
                        break;
                    case AArch64::MOVZWi:
                        newValue = (immediateValue << immediateShift) & 0xFFFFFFFF;
                        newOpcode = AArch64::MOVi32imm;
                        replace = true;
                        break;
                    case AArch64::MOVZXi:
                        newValue = (immediateValue << immediateShift);
                        newOpcode = AArch64::MOVi64imm;
                        replace = true;
                        break;
                    case AArch64::MOVKWi:
                    case AArch64::MOVKXi:
                        // MOVK is not fully reconstructable standalone—skip
                        continue;
                }

                if (replace) {
                    BuildMI(MachineBasicBlock, Instruction, debugLocation, TII->get(newOpcode), destinationRegister).addImm(newValue);
                    Instruction.eraseFromParent();
                    modified = true;
                }
            }
        }
    }

};