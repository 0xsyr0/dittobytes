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
    Main entry point of the script. Checks wether certain `@Beacon` DLL imports are done. If so,
    notify the user (via the console), that the BOF argument might needs to be passed.
    """

    if len(sys.argv) != 2:
        print("Usage: python notify-user-about-bof.py <input_obj_file>")
        sys.exit(1)   

   # Skip warning if BOF is in environment
    if 'BOF' in os.environ:
        sys.exit(0)

    input_file = sys.argv[1]

    try:
        with open(input_file, 'r') as input_handle:
            content = input_handle.read()
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

    beacon_regex = re.compile(r"declare\s+dllimport\s+.*@Beacon\w+\s*\(")

    if beacon_regex.search(content):
        print("    - Found a DLL import reference with `@Beacon...`. ⚠️")
        print("      ↳ Please consider pass `BOF=true` in `make` command.")

if __name__ == "__main__":
    main()