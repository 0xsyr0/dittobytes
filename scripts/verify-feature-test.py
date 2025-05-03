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
    """Print the exception type, file, and line number where it occurred.

    Args:
        exception (Exception): The caught exception instance.

    """

    exc_type, exc_value, exc_tb = sys.exc_info()
    filename, lineno, _, _ = traceback.extract_tb(exc_tb)[-1]
    print(f"    - {type(exception).__name__} exception at {filename}:{lineno}: {exception}")

def __get_system():
    """Detect the current operating system.

    Returns:
        str or bool: A shorthand identifier ('win', 'lin', or 'mac'), or False on failure.

    """

    available_systems = {'Windows': 'win', 'Linux': 'lin', 'Darwin': 'mac'}

    try:
        return available_systems[platform.system()]
    except Exception as exception:
        __print_exception(exception)
        return False       

def __get_architecture():
    """Detect the system's CPU architecture.

    Returns:
        str or bool: A shorthand architecture identifier ('amd64' or 'arm64'), or False on failure.

    """

    available_architectures = {'AMD64': 'amd64', 'x86_64': 'amd64', 'arm64': 'arm64', 'aarch64': 'arm64'}

    try:
        return available_architectures[platform.machine()]
    except Exception as exception:
        __print_exception(exception)
        return False       

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

def __get_loader(shellcode_path):
    """Construct the loader path based on the shellcode path, system, and architecture.

    Args:
        shellcode_path (str): Path to the shellcode file.

    Returns:
        str: Absolute path to the corresponding loader executable.

    """

    directory = os.path.dirname(os.path.abspath(shellcode_path))

    current_system = __get_system()
    current_architecture = __get_architecture()

    return '{}/{}{}'.format(
        directory,
        'loader-{}-{}'.format(current_system, current_architecture),
        '.exe' if current_system == 'win' else ''
    )

def verify_hex_not_present(hex_needle, haystack_path):
    """Check if a given hex byte sequence is absent from a binary file.

    Args:
        hex_needle (str): The byte pattern to search for, in hex string format.
        haystack_path (str): Path to the binary file to search in.

    Returns:
        bool: True if the byte sequence is not present, False otherwise.

    """

    haystack = __read_file(haystack_path, 'rb')
    needle = bytes.fromhex(hex_needle)

    return needle not in haystack

def verify_system_and_architecture(source_path, shellcode_path):
    """Verify if the shellcode filename matches the current system and architecture.

    Args:
        source_path (str): Path to the source directory (unused but included for context).
        shellcode_path (str): Path to the shellcode file.

    Returns:
        bool: True if filename prefix matches the current system and architecture.

    """

    shellcode_name = os.path.basename(os.path.abspath(shellcode_path))

    current_system = __get_system()
    current_architecture = __get_architecture()

    return shellcode_name.startswith('beacon-{}-{}'.format(current_system, current_architecture))

def verify_original(source_path, shellcode_path):
    """Run validation checks for the original shellcode to ensure it contains forbidden patterns.

    Parses the source file for `@verify` directives and runs checks specific to the original, 
    unmodified shellcode. A match indicates a validation failure (e.g., forbidden bytes are present).

    Args:
        source_path (str): Path to the source file containing `@verify` directives.
        shellcode_path (str): Path to the original shellcode file.

    Returns:
        tuple: (bool, str) indicating whether the original passed validation and an explanation message.

    """

    verifications = {}
    verify_pattern = r'@verify (\w+) (\w+) (\w+) (\w+) \('
    verify_matches = re.findall(verify_pattern, __read_file(source_path))

    available_verifications = {
        'hex_not_present': verify_hex_not_present
    }

    # Build verification dict
    for verification in verify_matches:
        if verification[1] not in verifications.keys():
            verifications[verification[1]] = []

        verifications[verification[1]].append({
            'architecture': verification[0],
            'function': verification[2],
            'value': verification[3]
        })

    for metamorphication, group in verifications.items():
        group_checks = 0
        group_succeeded = False

        for verification in group:
            if verification['architecture'].lower() not in ['any', __get_architecture()]:
                continue

            if verification['function'] not in available_verifications:
                return (False, 'Invalid verification method {}.'.format(verification['function']))

            group_checks += 1

            if not available_verifications[verification['function']](verification['value'], shellcode_path):
                group_succeeded = True

        if group_checks == 0:
            return (False, 'No available group checks for {}.'.format(metamorphication))

        if not group_succeeded:
            return (False, 'No group checks passed for {}.'.format(metamorphication))

    return (True, 'Verification methods for {} metamorphications executed successfully.'.format(len(verifications.keys())))

def verify_transpilation(metamorphication, source_path, shellcode_path):
    """Run verification checks embedded in the source file against the transpiled shellcode.

    Parses the source file for `@verify` directives and executes matching validation methods
    based on architecture and method name.

    Args:
        metamorphication (str): The transpiler that we need to verify.
        source_path (str): Path to the source file containing `@verify` directives.
        shellcode_path (str): Path to the transpiled shellcode file.

    Returns:
        tuple: (bool, str) indicating whether all verifications passed and an explanation message.

    """

    verify_pattern = r'@verify (\w+) (\w+) (\w+) (\w+) \('
    verify_matches = re.findall(verify_pattern, __read_file(source_path))

    available_verifications = {
        'hex_not_present': verify_hex_not_present
    }

    for verification in verify_matches:
        if verification[0].lower() not in ['any', __get_architecture()]:
            continue

        if verification[1].lower() != metamorphication:
            continue

        if verification[2] not in available_verifications:
            return (False, 'Invalid verification method {}.'.format(verification[2]))

        if not available_verifications[verification[2]](verification[3], shellcode_path):
            return (False, 'Verification method {}({}) failed.'.format(verification[2], verification[3]))

    return (True, '{} validator(s) executed successfully.'.format(len(verify_matches)))

def verify_result(metamorphication, source_path, shellcode_path):
    """Verify the return value of the executed shellcode against an expected value defined in the source.

    Parses the source file for a `@return` directive specifying the expected return type and value. 
    Executes the shellcode and compares the returned result with the expected value.

    Args:
        metamorphication (str): The transpiler that we need to verify.
        source_path (str): Path to the source file containing the expected return value.
        shellcode_path (str): Path to the shellcode to execute.

    Returns:
        tuple: (bool, str) indicating if the shellcode returned the expected value, with a message.

    """

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
    """Main function to execute the feature verification process on the shellcode.

    This function coordinates the verification of a shellcode's system compatibility, 
    transpilation integrity, original signature, and return value correctness. It exits 
    with specific error codes depending on which verification step fails.

    Exits:
        0x00000000: All verifications passed.
        0x00000001: System and architecture mismatch, shellcode cannot run.
        0x00000002: Original shellcode signature verification failed.
        0x00000003: Transpilation verification failed.
        0x00000004: Return value verification failed.
    
    """

    if len(sys.argv) != 5:
        print('[!] Invalid arguments: {}.'.format(str(sys.argv)))
        print('[!] Usage: python verify-feature-test.py <metamorphication> <source_path> <test_name> <shellcode_path>')
        sys.exit(1)

    metamorphication = sys.argv[1]
    source_path = sys.argv[2]
    test_name = sys.argv[3]
    shellcode_path = sys.argv[4]

    # We must be able to run on this system and architecture
    if not verify_system_and_architecture(source_path, shellcode_path):
        print('    - Verify({}): Cannot run shellcode for this system and architecture.'.format(test_name))
        sys.exit(0x00000001)

    # Run negation checks for original shellcodes
    if metamorphication == 'original':

        # Check if the signatures are identified in the original binary
        # print('    - Verify({}): Checking if signature is present in original binary.'.format(test_name))
        result = verify_original(source_path, shellcode_path)
        if result[0]:
            print('    - Verify({}/{}): Binary is original. {}'.format(test_name, metamorphication, result[1]))
        else:
            print('    - Verify({}/{}): Binary is not original. {}'.format(test_name, metamorphication, result[1]))
            sys.exit(0x00000002)

    # Run actual checks for transpiled shellcodes
    if metamorphication != 'original':

        # Check every verification for a specific metamorphication
        # print('    - Verify({}/{}): Checking if transpiler succeeded.'.format(test_name, metamorphication))
        result = verify_transpilation(metamorphication, source_path, shellcode_path)
        if result[0]:
            print('    - Verify({}/{}): Transpilation succesful. {}'.format(test_name, metamorphication, result[1]))
        else:
            print('    - Verify({}/{}): Transpilation failed. {}'.format(test_name, metamorphication, result[1]))
            sys.exit(0x00000003)

        # Check if the return value of the shellcode is still as expected
        # print('    - Verify({}/{}): Checking if return value is still correct.'.format(test_name, metamorphication))
        result = verify_result(metamorphication, source_path, shellcode_path)
        if result[0]:
            print('    - Verify({}/{}): Return value is still correct. {}'.format(test_name, metamorphication, result[1]))
        else:
            print('    - Verify({}/{}): Return value is invalid. {}'.format(test_name, metamorphication, result[1]))
            sys.exit(0x00000004)

    print('    - Verify({}/{}): Finished successfully'.format(test_name, metamorphication))
    sys.exit(0x00000000)

if __name__ == "__main__":
    main()