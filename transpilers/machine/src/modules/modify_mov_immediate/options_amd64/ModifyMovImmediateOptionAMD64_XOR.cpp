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
class ModifyMovImmediateOptionAMD64_XOR {

private:

    /**
     * Whether this class modified the machine function.
     */
    bool modified = false;

public:

    /**
     * Main execution method for the ModifyMovImmediateOptionAMD64_XOR class.
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
        dbgs() << "        ↳ Running AMD64 ModifyMovImmediate module with random option `XOR`.\n";

        // For each line in each basic block, perform our substitution
        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                // Only modify `mov` instructions with immediate values
                if (!isMovImmediate(Instruction)) {
                    continue;
                }

                // Inform user that we encountered a `mov` instruction with immediate value
                dbgs() << "          ↳ Found AMD64 mov instruction with immediate: ";
                Instruction.print(dbgs());

                // Obtain information about the instruction
                const DebugLoc& debugLocation = Instruction.getDebugLoc();
                Register destinationRegister = Instruction.getOperand(0).getReg();
                size_t immediateValue = (size_t) Instruction.getOperand(1).getImm();
                size_t immediateSize = getMovImmediateSize(Instruction);
                size_t originalOpcode = Instruction.getOpcode();
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
                // 2. mov [original register], [encoded immediate value]
                // 3. xor [original register], [xor key register]
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(originalOpcode), virtualXorKeyRegister).addImm(xorKey);
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(originalOpcode), destinationRegister).addImm(immediateValueEncoded);
                BuildMI(MachineBasicBlock, MachineInstruction, debugLocation, TII->get(xorOpcode), destinationRegister).addReg(destinationRegister).addReg(virtualXorKeyRegister);

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
            case X86::MOV8ri:
                return 8;
                break;
            case X86::MOV16ri:
                return 16;
                break;
            case X86::MOV32ri:
                return 32;
                break;
            case X86::MOV64ri:            
            case X86::MOV64ri32:
                return 64;
                break;
            // case X86::MOV8ri:
            // case X86::MOV8mi:
            // case X86::MOV16mi:
            // case X86::MOV32mi:
            // case X86::MOV32ri64:
            // case X86::MOV64ri32:
            // case X86::MOV64mi32:
            // case X86::MOV32mi64:
            default:
                report_fatal_error(formatv("ModifyMovImmediateOptionAMD64_XOR - Unknown immediate size for opcode {0:X}: {1}.", opcode, instruction));
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
            case X86::MOV8ri: return X86::XOR8rr;
            case X86::MOV16ri: return X86::XOR16rr;
            case X86::MOV32ri: return X86::XOR32rr;
            case X86::MOV64ri: return X86::XOR64rr;
            case X86::MOV64ri32: return X86::XOR64rr;
            // case X86::MOV8ri: return X86::XOR8rr;
            // case X86::MOV8mi: return X86::XOR8mi;
            // case X86::MOV16mi: return X86::XOR16mi;
            // case X86::MOV32mi: return X86::XOR32mi;
            // case X86::MOV32ri64: return X86::XOR64rr;
            // case X86::MOV32mi64: return X86::XOR32mi;
            // case X86::MOV64ri32: return X86::XOR64rr;
            // case X86::MOV64mi32: return X86::XOR64mi32;
            default:
                report_fatal_error(formatv("ModifyMovImmediateOptionAMD64_XOR - Unknown XOR replacement size for opcode {0:X}: {1}.", opcode, instruction));
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
            case X86::MOV8ri:
            case X86::MOV16ri:
            case X86::MOV32ri:
            case X86::MOV64ri:
            case X86::MOV64ri32:
            // case X86::MOV8mi:
            // case X86::MOV16mi:
            // case X86::MOV32mi:
            // case X86::MOV32ri64:
            // case X86::MOV32mi64:
            // case X86::MOV64mi:
            // case X86::MOV64mi32:
                return true;
                break;
            default:
                return false;
                break;
        }
    }

};