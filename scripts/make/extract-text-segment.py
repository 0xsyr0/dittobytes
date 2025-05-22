#!/usr/bin/env python3

# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

import lief
import sys
import platform
from enum import Enum

class Architecture(Enum):
    """Enumeration representing the target architecture."""
    X86_64 = "x86_64"
    ARM64 = "arm64"

class OperatingSystem(Enum):
    """Enumeration representing the target operating system."""
    WINDOWS = "Windows"
    LINUX = "Linux"
    MACOS = "Darwin"

class BinaryFormat(Enum):
    """Enumeration representing the binary format."""
    PE = "PE"
    ELF = "ELF"
    MACHO = "Mach-O"

class TextSegmentExtractor:
    """
    A class responsible for extracting the .text segment from binary files
    across different architectures and operating systems.
    """

    def __init__(self):
        """Initializes the TextSegmentExtractor."""
        pass

    def _get_binary_format(self, binary: lief.Binary) -> BinaryFormat:
        """
        Determines the binary format of the given LIEF binary object.

        Args:
            binary: A lief.Binary object.

        Returns:
            A BinaryFormat enum member representing the binary format,
            or None if the format is not recognized.
        """
        if isinstance(binary, lief.PE.Binary):
            return BinaryFormat.PE
        elif isinstance(binary, lief.ELF.Binary):
            return BinaryFormat.ELF
        elif isinstance(binary, lief.MachO.Binary):
            return BinaryFormat.MACHO
        return None

    def _get_text_section_name(self, binary_format: BinaryFormat) -> str:
        """
        Returns the standard name of the text segment for a given binary format.

        Args:
            binary_format: The BinaryFormat enum member.

        Returns:
            The standard name of the text segment.
        """
        if binary_format == BinaryFormat.PE:
            return ".text"
        elif binary_format == BinaryFormat.ELF:
            return ".text"
        elif binary_format == BinaryFormat.MACHO:
            # Mach-O can have different segment/section names.
            # We'll look for a section within the "__TEXT" segment.
            return "__text"
        return ".text" # Default if not recognized

    def extract(self, input_file: str, output_file: str) -> None:
        """
        Parses the input binary file and extracts the .text segment bytes
        to the specified output file.

        Args:
            input_file: Path to the input binary file.
            output_file: Path to the output file where the .text segment will be written.
        """
        try:
            # Parse the binary using LIEF
            binary = lief.parse(input_file)
            if not binary:
                print(f"[!] Error: Failed to parse {input_file}")
                return

            binary_format = self._get_binary_format(binary)
            if not binary_format:
                print(f"[!] Error: Unrecognized binary format for {input_file}")
                return

            text_section_name = self._get_text_section_name(binary_format)
            text_section = None

            if binary_format == BinaryFormat.MACHO:
                for segment in binary.segments:
                    if segment.name == "__TEXT":
                        for section in segment.sections:
                            if section.name == "__text":
                                text_section = section
                                break
                        if text_section:
                            break
            else:
                for section in binary.sections:
                    if section.name == text_section_name:
                        text_section = section
                        break

            if not text_section:
                print(f"Error: {text_section_name} section not found in {input_file} (format: {binary_format.value})")
                return

            # Extract the raw bytes of the .text section
            text_data = text_section.content

            # Write the extracted text section to a raw binary file
            with open(output_file, 'wb') as out_file:
                out_file.write(text_data)

            #print(f"[+] Successfully extracted {text_section_name} segment to {output_file} (format: {binary_format.value})")

        except lief.bad_file as e:
            print(f"[!] Error parsing {input_file}: {e}")
        except Exception as e:
            print(f"[!] An unexpected error occurred: {e}")

def main():
    """
    Main entry point of the script. Handles command-line arguments and
    uses the TextSegmentExtractor to extract the .text segment.
    """

    if len(sys.argv) != 3:
        print("Usage: python extract-text-segment.py <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    extractor = TextSegmentExtractor()
    extractor.extract(input_file, output_file)

if __name__ == "__main__":
    main()