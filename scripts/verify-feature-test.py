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

def verify_system_and_architecture(source_path, shellcode_path):
    shellcode_name = os.path.basename(os.path.abspath(shellcode_path))

    current_system = __get_system()
    current_architecture = __get_architecture()

    return shellcode_name.startswith('beacon-{}-{}'.format(current_system, current_architecture))

def verify_transpilation(source_path, shellcode_path):
    verify_pattern = r'@verify (\w+) (\w+) \('
    verify_matches = re.findall(verify_pattern, __read_file(source_path))

    available_verifications = {
        'hex_not_present': verify_hex_not_present
    }

    for verification in verify_matches:
        if verification[0] not in available_verifications:
            return (False, 'Invalid verification method {}.'.format(verification[0]))

        if not available_verifications[verification[0]](verification[1], shellcode_path):
            return (False, 'Verification method {}({}) failed.'.format(verification[0], verification[1]))

    return (True, '{} validator(s) executed successfully.'.format(len(verify_matches)))

def verify_result(source_path, shellcode_path):
    try:
        return_value_pattern = r'@return (\w+) The return value to verify: `(.+?)`'
        return_value_matches = re.findall(return_value_pattern, __read_file(source_path))

        expected_return_type = return_value_matches[0][0]
        expected_return_value = return_value_matches[0][1]

        executable_result = subprocess.run([__get_loader(shellcode_path), shellcode_path], capture_output=True, text=True, timeout=5)

        result_return_value_pattern = r'Got result from shellcode \(' + expected_return_type + r'\): +0x\w+ \((signed|unsigned) (-?\d+)\)'
        result_return_value_matches = re.findall(result_return_value_pattern, executable_result.stdout)

        return (
            result_return_value_matches[0][1] == expected_return_value,
            'Got {}. Expected {}.'.format(result_return_value_matches[0][1], expected_return_value)
        )
    except Exception as exception:
        __print_exception(exception)
        return (False, None)

def main():
    """
    Main entry point of the script. Handles command-line arguments and
    initiates the verification of the executable based on the code file.
    """

    if len(sys.argv) != 4:
        print('[!] Invalid arguments: {}.'.format(str(sys.argv)))
        print('[!] Usage: python verify-feature-test.py <source_path> <test_name> <shellcode_path>')
        sys.exit(1)

    source_path = sys.argv[1]
    test_name = sys.argv[2]
    shellcode_path = sys.argv[3]

    print('    - VerifyFeatureTest: {}'.format(test_name))

    print('    - VerifyFeatureTest: Checking if we can run the shellcode on this system and architecture.')
    if not verify_system_and_architecture(source_path, shellcode_path):
        print('    - VerifyFeatureTest: Cannot run shellcode for this system and architecture.')
        sys.exit(0x0000001)

    print('    - VerifyFeatureTest: Checking if return value is still correct for `{}`.'.format(shellcode_path))
    result = verify_result(source_path, shellcode_path)
    if result[0]:
        print('    - VerifyFeatureTest: Return value is still correct. {}'.format(result[1]))
    else:
        print('    - VerifyFeatureTest: Return value is invalid. {}'.format(result[1]))
        sys.exit(0x00000003)

    print('    - VerifyFeatureTest: Checking if transpiler succeeded for `{}`.'.format(shellcode_path))
    result = verify_transpilation(source_path, shellcode_path)
    if result[0]:
        print('    - VerifyFeatureTest: Transpilation succesful. {}'.format(result[1]))
    else:
        print('    - VerifyFeatureTest: Transpilation failed. {}'.format(result[1]))
        sys.exit(0x00000002)


    print('    - VerifyFeatureTest: Finished successfully!')
    sys.exit(0x00000000)

if __name__ == "__main__":
    main()