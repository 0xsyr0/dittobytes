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

import re
import os
import sys
import platform
import traceback
import subprocess

def __print_exception(exception):
    exc_type, exc_value, exc_tb = sys.exc_info()
    filename, lineno, _, _ = traceback.extract_tb(exc_tb)[-1]
    print(f"    - {type(exception).__name__} exception at {filename}:{lineno}: {exception}")

def __get_system():
    available_systems = {'Windows': 'win', 'Linux': 'lin', 'Darwin': 'mac'}

    try:
        return available_systems[platform.system()]
    except Exception as exception:
        __print_exception(exception)
        return False       

def __get_architecture():
    available_architectures = {'AMD64': 'amd64', 'x86_64': 'amd64', 'arm64': 'arm64', 'aarch64': 'arm64'}

    try:
        return available_architectures[platform.machine()]
    except Exception as exception:
        __print_exception(exception)
        return False       

def __read_file(file_path, binary = False):
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

def __get_loader(shellcode_path):
    directory = os.path.dirname(os.path.abspath(shellcode_path))

    current_system = __get_system()
    current_architecture = __get_architecture()

    return '{}/{}{}'.format(
        directory,
        'loader-{}-{}'.format(current_system, current_architecture),
        '.exe' if current_system == 'win' else ''
    )

def verify_hex_not_present(hex_needle, haystack_path):
    haystack = __read_file(haystack_path, 'rb')
    needle = bytes.fromhex(hex_needle)

    return needle not in haystack

def verify_system_and_architecture(code_path, shellcode_path):
    shellcode_name = os.path.basename(os.path.abspath(shellcode_path))

    current_system = __get_system()
    current_architecture = __get_architecture()

    return shellcode_name == 'beacon-{}-{}.bin'.format(current_system, current_architecture)

def verify_transpilation(code_path, shellcode_path):
    verify_pattern = r'@verify (\w+) (\w+) \('
    verify_matches = re.findall(verify_pattern, __read_file(code_path))

    available_verifications = {
        'hex_not_present': verify_hex_not_present
    }

    for verification in verify_matches:
        if verification[0] not in available_verifications:
            print('    - VerifyFeatureTest: Invalid verification method {}.'.format(verification[0]))
            return False

        if not available_verifications[verification[0]](verification[1], shellcode_path):
            print('    - VerifyFeatureTest: Verification method {}({}) failed.'.format(verification[0], verification[1]))
            return False

    return True

def verify_result(code_path, shellcode_path):
    return_value_pattern = r'@return (\w+) The return value to verify: `(.+?)`'
    return_value_matches = re.findall(return_value_pattern, __read_file(code_path))

    expected_return_type = return_value_matches[0][0]
    expected_return_value = return_value_matches[0][1]

    try:
        executable_result = subprocess.run([__get_loader(shellcode_path), shellcode_path], capture_output=True, text=True, timeout=5)

        result_return_value_pattern = r'Got result from shellcode \(' + expected_return_type + r'\): +0x\w+ \((signed|unsigned) (-?\d+)\)'
        result_return_value_matches = re.findall(result_return_value_pattern, executable_result.stdout)

        return result_return_value_matches[0][1] == expected_return_value
    except Exception as exception:
        __print_exception(exception)
        return False

def main():
    """
    Main entry point of the script. Handles command-line arguments and
    initiates the verification of the executable based on the code file.
    """

    if len(sys.argv) != 3:
        print('[!] Usage: python verify-feature-test.py <code_path> <shellcode_path>')
        sys.exit(1)

    code_path = sys.argv[1]
    shellcode_path = sys.argv[2]

    print('    - VerifyFeatureTest: Checking if we can run the shellcode on this system and architecture.')
    if not verify_system_and_architecture(code_path, shellcode_path):
        print('    - VerifyFeatureTest: Cannot run shellcode for this system and architecture.')
        sys.exit(0x0000000)

    print('    - VerifyFeatureTest: Checking if transpiler succeeded.')
    if not verify_transpilation(code_path, shellcode_path):
        print('    - VerifyFeatureTest: Transpilation failed.')
        sys.exit(0x00000001)

    print('    - VerifyFeatureTest: Checking if return value is still correct.')
    if not verify_result(code_path, shellcode_path):
        print('    - VerifyFeatureTest: Return value is invalid.')
        sys.exit(0x00000002)

    print('    - VerifyFeatureTest: Finished successfully!')
    sys.exit(0x00000000)

if __name__ == "__main__":
    main()