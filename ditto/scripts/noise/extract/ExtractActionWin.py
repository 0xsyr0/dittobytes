#!/usr/bin/env python3

# -*- coding: utf-8 -*-

# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

import os
import sys
import pefile

from capstone import *
from helpers.StatusHelper import StatusHelper
from helpers.FileHelper import FileHelper

class ExtractActionWin:
    """The ExtractActionWin class extracts the commonly found patterns or basic-blocks of instructions from a given directory of executables."""

    def __init__(self, platform, arch, directory):
        """Initiate the ExtractActionWin class.

        Args:
            platform (str): The platform to extract noise for.
            arch (str): The arch to extract noise for.
            directory (str): The directory containing binaries for the given platform & arch.

        """

        self.platform = platform
        self.arch = arch
        self.directory = directory

    def extract(self, file_path):
        """Start the extraction.

        Args:
            file_path (str): Potential executable file to extract instructions from.

        Returns:
            dict: A dict with the observed patterns (if any)

        """

        result = {}

        if file_path[-4:] not in ['.exe', '.dll', '.mui']:
            return result

        if not FileHelper.file_is_pe(file_path):
            return result

        StatusHelper.info('Identified PE-file {}'.format(file_path))

        file_pe_data = self.__get_pe_data(file_path)
        if not file_pe_data:
            return result

        md = Cs(file_pe_data['file_architecture']['cs_arch'], file_pe_data['file_architecture']['cs_mode'])
        md.detail = True

        instructions = md.disasm(file_pe_data['text_segment'], file_pe_data['file_image_base'] + file_pe_data['text_segment_virtual_address'])
        result = self.__group_basic_blocks(instructions)

        return result

    def __get_pe_data(self, file_path):
        """Get PE-file information for the given file path.

        Args:
            file_path (str): Potential executable file to extract data from.

        Returns:
            bool or dict: A dict with PE-file data (if any), False otherwise

        """

        try:
            file_pe = pefile.PE(file_path)

            text_section = None
            for section in file_pe.sections:
                if section.Name.rstrip(b'\x00') == b'.text':
                    text_section = section
                    break

            if not text_section:
                return False

            text_segment = text_section.get_data()
            file_image_base = file_pe.OPTIONAL_HEADER.ImageBase
            file_machine = file_pe.FILE_HEADER.Machine
            text_segment_virtual_address = text_section.VirtualAddress

            if file_machine == 0x8664:
                file_architecture = {'arch': 'amd64', 'cs_arch': CS_ARCH_X86, 'cs_mode': CS_MODE_64, 'cs_str': 'x86_64'}
            elif file_machine == 0xAA64:
                file_architecture = {'arch': 'arm64', 'cs_arch': CS_ARCH_ARM64, 'cs_mode': CS_MODE_ARM, 'cs_str': 'arm64'}
            else:
                return False

            return {
                'file_path': file_path,
                'file_image_base': file_image_base,
                'file_architecture': file_architecture,
                'text_segment_virtual_address': text_segment_virtual_address,
                'text_segment': text_segment
            }

        except Exception as error:
            StatusHelper.error(StatusHelper.ERROR_NONFATAL_ERROR)
            ExceptionHelper.print_exception(exception)
            return False

    def __group_basic_blocks(self, instructions):
        """Group instructions per basic block.

        Args:
            instructions (list): A list of instructions (linear)

        Returns:
            list: A list of basic blocks with instructions.

        """

        blocks = []
        current_block = []

        for instruction in instructions:
            current_block.append(self.__instruction_to_json(instruction))
            
            if not any(instruction.group(g) for g in (CS_GRP_JUMP, CS_GRP_CALL, CS_GRP_RET, CS_GRP_IRET, CS_GRP_INT)):
                continue

            if len(current_block) <= 2 or len(current_block) >= 100:
                current_block = []
                continue

            blocks.append(current_block[:])
            current_block = []

        return blocks

    def __instruction_to_json(self, instruction):
        """Store every required thing of an instruction to an object.

        Args:
            instructions (CsInsn): A instruction to parse.

        Returns:
            dict: An object with the instructions information.

        """

        result = {
            "mnemonic": instruction.mnemonic,
            "op_str": instruction.op_str,
            "id": instruction.id,
            "operands": [],
            "size": instruction.size
        }

        for op in instruction.operands:
            op_entry = {"type": op.type}

            if op.type == CS_OP_REG:
                op_entry["reg"] = op.reg
            elif op.type == CS_OP_IMM:
                op_entry["imm"] = op.imm
            elif op.type == CS_OP_MEM:
                op_entry["mem"] = {
                    "base": op.mem.base,
                    "index": op.mem.index,
                    "scale": op.mem.scale,
                    "disp": op.mem.disp,
                }

            result["operands"].append(op_entry)

        return result
