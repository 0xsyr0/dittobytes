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
class TransformStackMovImmediatesOptionAMD64_XOR {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

public:

    /**
     * Main execution method for the TransformStackMovImmediatesOptionAMD64_XOR class.
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
        dbgs() << "        ↳ Running AMD64 module: TransformStackMovImmediates(option=XOR,modifyAll=" << modifyAll << ").\n";

        // For each line in each basic block, perform our substitution
        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                // Only modify `mov` instructions with immediate values
                if (!isStackMovImmediate(Instruction)) {
                    continue;
                }

                // Inform user that we encountered a `mov` instruction with immediate value
                dbgs() << "          ↳ Found AMD64 mov instruction with immediate: ";
                Instruction.print(dbgs());

                // Obtain information about the instruction
                const DebugLoc& debugLocation = Instruction.getDebugLoc();
                int destinationFrameIndex = Instruction.getOperand(0).getIndex();
                size_t immediateValue = (size_t) Instruction.getOperand((Instruction.getNumOperands() - 1)).getImm();
                size_t immediateSize = getMovImmediateSize(Instruction);
                size_t originalOpcode = Instruction.getOpcode();
                size_t movRegImmediateOpcode = getMovRegImmediateReplacement(Instruction);
                unsigned xorOpcode = getMovSizeXorReplacement(Instruction);

                // Generate XOR key on compile time
                size_t xorKey = RandomHelper::getSimilarIntegerForDestination(immediateSize, immediateValue, false);

                // `xor` the immediate value and mask it to the correct size
                size_t immediateValueEncoded = immediateValue ^ xorKey;
                int64_t immediateMask = (immediateSize == 64) ? -1 : ((1ULL << immediateSize) - 1);
                immediateValueEncoded = immediateValueEncoded & immediateMask;

                // Register to use in the substition
                Register virtualXorKeyRegister;

                // Allocate a virtual register for the `xor` key
                switch (immediateSize) {
                    case 64: virtualXorKeyRegister = MRI.createVirtualRegister(&X86::GR64RegClass); break;
                    case 32: virtualXorKeyRegister = MRI.createVirtualRegister(&X86::GR32RegClass); break;
                    case 16: virtualXorKeyRegister = MRI.createVirtualRegister(&X86::GR16RegClass); break;
                    default: virtualXorKeyRegister = MRI.createVirtualRegister(&X86::GR8RegClass); break;
                }

                // 1. mov [xor key register], [xor key immediate value]
                // 2. mov [rbp+offset], [encoded immediate value]
                // 3. xor [rbp+offset], [xor key register]
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(movRegImmediateOpcode), virtualXorKeyRegister).addImm(xorKey);
        
                // 2. mov [destinationFrameIndex], immediateValueEncoded
                // MachineInstr* StackMovImmediateEncoded = MF.CloneMachineInstr(&Instruction);
                // StackMovImmediateEncoded->getOperand(StackMovImmediateEncoded->getNumOperands() - 1).setImm(immediateValueEncoded);
                // MachineBasicBlock.insert(MachineInstruction, StackMovImmediateEncoded);
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(originalOpcode))
                    .addFrameIndex(destinationFrameIndex).addImm(1).addReg(0).addImm(0).addReg(0)
                    .addImm(immediateValueEncoded);

                // // 3. xor [destinationFrameIndex], virtualXorKeyRegister
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(xorOpcode))
                    .addFrameIndex(destinationFrameIndex).addImm(1).addReg(0).addImm(0).addReg(0)
                    .addReg(virtualXorKeyRegister);

                // Erase the original instruction after inserting the new ones
                Instruction.eraseFromParent();

                // Inform module and user that we've successfully substituted the immediate value.
                modified = true;
                dbgs() << "          ✓ Modified immediate value using random option `XOR`.\n";
            }
        }

        return modified;
    }

private:

    /**
     * Checks if the instruction is a MOV to stack memory with immediate.
     *
     * @param MachineInstr& instruction The instruction to check.
     * @return bool True if it's a stack MOV immediate.
     */
    bool isStackMovImmediate(const MachineInstr &instruction) {
        // Match opcode mov instructions that store an immediate into memory.
        switch (instruction.getOpcode()) {
            case X86::MOV8mi:
            case X86::MOV16mi:
            case X86::MOV32mi:
            case X86::MOV64mi32:
                break;
            default:
                return false;
        }

        // First operand must be a frame index (stack variable)
        if (!instruction.getOperand(0).isFI()) return false;

        // Last operand must be an immediate
        if (!instruction.getOperand(instruction.getNumOperands() - 1).isImm()) return false;

        return true;
    }

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
     * @return size_t The size of the immediate value in bits (8, 16, 32, or 64).
     */
    size_t getMovImmediateSize(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case X86::MOV8mi:
                return 8;
                break;
            case X86::MOV16mi:
                return 16;
                break;
            case X86::MOV32mi:
                return 32;
                break;
            case X86::MOV64mi32:
                return 64;
                break;
            default:
                report_fatal_error(formatv("TransformStackMovImmediatesOptionAMD64_XOR - Unknown immediate size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

    /**
     * Determines the XOR replacement opcode for a given MOV instruction opcode.
     * 
     * This function maps certain MOV instruction opcodes to corresponding XOR opcodes
     * for AMD64 instructions. The provided `MachineInstr`'s opcode is checked and
     * replaced with an appropriate XOR opcode based on the MOV instruction's immediate size.
     * 
     * If the opcode does not match any known MOV instruction types, a fatal error is reported.
     *
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked and replaced with the corresponding XOR opcode.
     * @return unsigned The corresponding XOR opcode for the MOV instruction's immediate size.
     */
    unsigned getMovSizeXorReplacement(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case X86::MOV8mi:    return X86::XOR8mr;
            case X86::MOV16mi:   return X86::XOR16mr;
            case X86::MOV32mi:   return X86::XOR32mr;
            case X86::MOV64mi32: return X86::XOR64mr;
            default:
                report_fatal_error(formatv("TransformStackMovImmediatesOptionAMD64_XOR - Unknown XOR replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

    /**
     * Determines the MOVri replacement opcode for a given MOVmi instruction opcode.
     * 
     * This function maps certain MOVmi instruction opcodes to corresponding MOVri opcodes
     * for AMD64 instructions. The provided `MachineInstr`'s opcode is checked and
     * replaced with an appropriate MOVri opcode based on the MOVmi instruction's immediate size.
     * 
     * If the opcode does not match any known MOV instruction types, a fatal error is reported.
     *
     * @param MachineFunction& MF instruction The `MachineInstr` whose opcode will be checked and replaced with the corresponding MOVri opcode.
     * @return unsigned The corresponding MOVmi opcode for the MOVri instruction's immediate size.
     */
    unsigned getMovRegImmediateReplacement(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

        switch (opcode) {
            case X86::MOV8mi:    return X86::MOV8ri;
            case X86::MOV16mi:   return X86::MOV16ri;
            case X86::MOV32mi:   return X86::MOV32ri;
            case X86::MOV64mi32: return X86::MOV64ri32;
            default:
                report_fatal_error(formatv("TransformStackMovImmediatesOptionAMD64_XOR - Unknown MOVri replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

};