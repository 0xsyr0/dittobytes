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
 * Namespace(s) to use
 */
using namespace llvm;

/**
 * A class to obfuscate mov immediate values.
 */
class AMD64_ModifyImmediateModule {

public:

    /**
     * Main execution method for the AMD64_ModifyImmediateModule class.
     *
     * @param MachineFunction& MF The machine function to run the substitution on.
     * @return bool Indicates if the machine function was modified.
     */
    bool runOnMachineFunction(MachineFunction &MF) {
        const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
        MachineRegisterInfo &MRI = MF.getRegInfo();
        bool Modified = false;

        errs() << "        ↳ Running AMD64 architecture specific implementation.\n";

        for (auto &MachineBasicBlock : MF) {
            for (auto MachineInstruction = MachineBasicBlock.begin(); MachineInstruction != MachineBasicBlock.end(); ) {
                MachineInstr &Instruction = *MachineInstruction++;

                // Only modify mov instructions with immediate values
                if (!isMovImmediate(Instruction)) {
                    continue;
                }

                // Print debug information
                errs() << "          ↳ Found AMD64 mov instruction: ";
                Instruction.print(errs());

                // Get original variables
                const DebugLoc& DebugLocation = Instruction.getDebugLoc();
                size_t opcode = Instruction.getOpcode();
                Register destination = Instruction.getOperand(0).getReg();
                size_t immediate = (size_t) Instruction.getOperand(1).getImm();
                size_t movSize = getMovImmediateSize(Instruction);

                // Generate XOR key
                size_t xorKey = RandomHelper::getSimilarIntegerForDestination(
                    movSize,
                    immediate
                );

                size_t immediateEncoded = immediate ^ xorKey;
                int64_t mask = (movSize == 64) ? -1 : ((1ULL << movSize) - 1);
                immediateEncoded = immediateEncoded & mask;

                Register KeyReg;
                Register TargetReg = destination;

                  // Allocate virtual registers
                switch (movSize) {
                    case 64:
                        KeyReg = MRI.createVirtualRegister(&X86::GR64RegClass);
                        // TargetReg = MRI.createVirtualRegister(&X86::GR64RegClass);
                        break;
                    case 32:
                        KeyReg = MRI.createVirtualRegister(&X86::GR32RegClass);
                        // TargetReg = MRI.createVirtualRegister(&X86::GR32RegClass);
                        break;
                    case 16:
                        KeyReg = MRI.createVirtualRegister(&X86::GR16RegClass);
                        // TargetReg = MRI.createVirtualRegister(&X86::GR16RegClass);
                        break;
                    default:
                        KeyReg = MRI.createVirtualRegister(&X86::GR8RegClass);
                        // TargetReg = MRI.createVirtualRegister(&X86::GR8RegClass);
                        break;
                }

                MachineBasicBlock::iterator InsertPoint = MachineInstruction; // Get iterator to the current instruction

                // 1. mov keyReg, imm
                BuildMI(MachineBasicBlock, InsertPoint, DebugLocation, TII->get(opcode), KeyReg).addImm(xorKey);

                // 2. mov targetReg, encoded_imm
                BuildMI(MachineBasicBlock, InsertPoint, DebugLocation, TII->get(opcode), TargetReg).addImm(immediateEncoded);

                // 3. xor targetReg, keyReg
                unsigned XorOpcode = getMovSizeXorReplacement(Instruction);

                BuildMI(MachineBasicBlock, InsertPoint, DebugLocation, TII->get(XorOpcode), TargetReg)
                    .addReg(TargetReg)
                    .addReg(KeyReg);

                // Erase the original instruction *after* inserting the new ones
                // MachineInstruction = MachineBasicBlock.erase(std::prev(InsertPoint));
                Instruction.eraseFromParent();

                Modified = true;
                errs() << "          ↳ Modified immediate value using technique: XOR.\n";
            }
        }
        
        return Modified;
    }

private:

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
                report_fatal_error(formatv("AMD64_ModifyImmediateModule - Unknown immediate size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

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
                report_fatal_error(formatv("AMD64_ModifyImmediateModule - Unknown XOR replacement size for opcode {0:X}: {1}.", opcode, instruction));
                return 0;
        }
    }

    bool isMovImmediate(const MachineInstr &instruction) {
        unsigned opcode = instruction.getOpcode();

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