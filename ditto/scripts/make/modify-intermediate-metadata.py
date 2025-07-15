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

import os
import re
import sys

def main():
    """
    Main entry point of the script. Checks whether LLVM IL files (.ll or .mir) contain metadata that would end-up in 
    the compiled binary (and which cannot be stripped due to `strip`'s incompatibility with exotic file formats and
    architectures. If it finds any, it replaces it with commonly observed metadata values.
    """

    if len(sys.argv) != 2:
        print("Usage: python modify-intermediate-metadata.py <intermediate_file>")
        sys.exit(1)

    intermediate_file = sys.argv[1]

    patterns = [
        (re.compile(r'( *; ModuleID = \')(.*?)\'', re.IGNORECASE), lambda m: m.group(1) + 'main.c\''),
        (re.compile(r'( *source_filename = ")(.*?)"', re.IGNORECASE), lambda m: m.group(1) + 'main.c"')
    ]

    with open(intermediate_file, 'r', encoding='utf-8') as input_handle:
        content  = input_handle.read()

    for pattern, repl in patterns:
        content = pattern.sub(repl, content)

    with open(intermediate_file, 'w', encoding='utf-8') as output_handle:
        output_handle.write(content )

if __name__ == "__main__":
    main()

