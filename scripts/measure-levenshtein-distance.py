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
import Levenshtein

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

    Raises:
        SystemExit: If incorrect arguments are provided or upon successful completion.

    """
    
    if len(sys.argv) != 3:
        print('[!] Invalid arguments: {}.'.format(str(sys.argv)))
        print('[!] Usage: python measure-levenshtein-distance.py <shellcode_left_path> <shellcode_right_path>')
        sys.exit(1)

    shellcode_left_path = sys.argv[1]
    shellcode_right_path = sys.argv[2]

    shellcode_left = __read_file(shellcode_left_path, binary=True)
    shellcode_right = __read_file(shellcode_right_path, binary=True)
    print('    - Measure Levenshtein Distance:  Left size is {}.'.format(len(shellcode_left)))
    print('    - Measure Levenshtein Distance: Right size is {}.'.format(len(shellcode_right)))

    result = Levenshtein.distance(shellcode_left, shellcode_right)

    print('    - Measure Levenshtein Distance:   Resulted in {}.'.format(result))
    print('    - Measure Levenshtein Distance: Finished successfully!')
    sys.exit(0x00000000)

if __name__ == "__main__":
    main()