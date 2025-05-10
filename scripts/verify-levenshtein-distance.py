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

import sys

from Levenshtein import distance

def __read_file(file_path, binary = False):
    """Read the contents of a file in either text or binary mode.

    Args:
        file_path (str): The path to the file to be read.
        binary (bool): Whether to read the file in binary mode. Defaults to False.

    Returns:
        str or bytes or None: The file content as a string or bytes, or None if an error occurred.

    """

    content = None

    try:
        if binary:
            with open(file_path, 'rb') as file_handle:
                content = file_handle.read()
        else:
            with open(file_path, 'r', encoding='utf-8') as file_handle:
                content = file_handle.read()
    except Exception as exception:
        __print_exception(exception)

    return content

def main():
    """Measure the Levenshtein distance between two shellcode binaries.

    Expects two command-line arguments pointing to binary shellcode files.
    Loads the files, computes their sizes, and prints the Levenshtein distance.

    Exits:
        0x00000000: There is enough change in the given shellcodes.
        0x00000001: There is insufficient change in the given shellcodes.

    Raises:
        SystemExit: If incorrect arguments are provided or upon successful completion.

    """
    
    if len(sys.argv) != 4:
        print('[!] Invalid arguments: {}.'.format(str(sys.argv)))
        print('[!] Usage: python measure-levenshtein-distance.py <test_name> <shellcode_left_path> <shellcode_right_path>')
        sys.exit(1)

    test_name = sys.argv[1]
    shellcode_left_path = sys.argv[2]
    shellcode_right_path = sys.argv[3]

    shellcode_left = __read_file(shellcode_left_path, binary=True)
    shellcode_right = __read_file(shellcode_right_path, binary=True)
    shellcode_length = max(len(shellcode_left), len(shellcode_right))

    print('    - Verify({}/transpiled): Measuring Levenshtein distance of original (size {}) to transpiled (size {}) shellcode.'.format(test_name, len(shellcode_left), len(shellcode_right)))

    result_absolute = distance(shellcode_left, shellcode_right)
    result_percentage = round((result_absolute / shellcode_length) * 100);

    print('    - Verify({}/transpiled): Result of Levenshtein distance measurement: {} ({}% change).'.format(test_name, result_absolute, result_percentage))

    verification_brackets = {
        0:   20,   # There must be a minimal change of 20%
        25:  20,   # For shellcodes of 25 bytes or larger, the minimum change must be 20%
        50:  20,   # For shellcodes of 50 bytes or larger, the minimum change must be 20%
        100: 20    # For shellcodes of 100 bytes or larger, the minimum change must be 20%
    }

    for verification_bracket in verification_brackets.keys():
        minimum_change = verification_brackets[verification_bracket]

        if shellcode_length < verification_bracket:
            continue

        if result_percentage < minimum_change:
            print('    - Verify({}/transpiled): Minimum change of {}% required for length {}, but got {}%.'.format(test_name, minimum_change, shellcode_length, result_percentage))
            sys.exit(0x00000001)

    sys.exit(0x00000000)

if __name__ == "__main__":
    main()