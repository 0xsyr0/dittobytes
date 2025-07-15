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

import re
import ctypes
import subprocess

from helpers.FileHelper import FileHelper
from helpers.EnvironmentHelper import EnvironmentHelper
from helpers.ExceptionHelper import ExceptionHelper
from helpers.StatusHelper import StatusHelper

class VerificationReturns:
    """Checks whether the given shellcode returns the expected result."""

    def requires_supported_os_and_arch(self):
        """Whether this verification requires the os & arch of the shellcode to be executable in this runtime.

        Returns:
            bool: Positive if required.

        """

        return True

    def verify(self, feature_test_specification):
        """Checks whether the given shellcode returns the expected result.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.

        """

        exe_return_value = None
        raw_return_value = None
        exe_return_value_is_valid = False
        raw_return_value_is_valid = False
        executable_result = None

        try:
            expected_return_type = feature_test_specification['test_arguments'][0]
            expected_return_value = feature_test_specification['test_arguments'][1]

            # RAW format
            command = EnvironmentHelper.get_loader(feature_test_specification['compiled_for_os'], feature_test_specification['compiled_for_arch']).split()
            command.append(feature_test_specification['raw_file_path'])

            executable_result = subprocess.run(command, capture_output=True, text=True, timeout=10)

            if executable_result.returncode != 0:
                StatusHelper.error(StatusHelper.ERROR_RETURN_VERIFICATION_PROCESS_FAILED)
                raw_return_value_is_valid = False
            else:
                result_return_value_pattern = r'Got result from shellcode \(' + expected_return_type + r'\): +0x\w+ \((signed|unsigned) (-?\d+)\)'
                result_return_value_matches = re.findall(result_return_value_pattern, executable_result.stdout)
                raw_return_value = result_return_value_matches[0][1]
                raw_return_value_is_valid = str(raw_return_value) == str(expected_return_value)

            # EXE format
            command = EnvironmentHelper.get_emulator(feature_test_specification['compiled_for_os'], feature_test_specification['compiled_for_arch']).split()
            command.append(feature_test_specification['exe_file_path'])

            executable_result = subprocess.run(command, capture_output=True, text=True, timeout=15, check=False)
            exe_return_value = executable_result.returncode

            # Only 8-bit integers are supported in EXE exit codes
            if expected_return_type == 'int8_t' or expected_return_type == 'uint8_t':
                exe_return_value = ctypes.c_int8(exe_return_value).value if expected_return_type == 'int8_t' else exe_return_value
                exe_return_value = ctypes.c_uint8(exe_return_value).value if expected_return_type == 'uint8_t' else exe_return_value
                exe_return_value_is_valid = str(exe_return_value) == str(expected_return_value)
            else:
                exe_return_value_is_valid = True
                # exe_return_value = ctypes.c_int16(exe_return_value).value if expected_return_type == 'int16_t' else exe_return_value
                # exe_return_value = ctypes.c_uint16(exe_return_value).value if expected_return_type == 'uint16_t' else exe_return_value
                # exe_return_value = ctypes.c_int32(exe_return_value).value if expected_return_type == 'int32_t' else exe_return_value
                # exe_return_value = ctypes.c_uint32(exe_return_value).value if expected_return_type == 'uint32_t' else exe_return_value
                # exe_return_value = ctypes.c_int64(exe_return_value).value if expected_return_type == 'int64_t' else exe_return_value
                # exe_return_value = ctypes.c_uint64(exe_return_value).value if expected_return_type == 'uint64_t' else exe_return_value
        except Exception as exception:
            ExceptionHelper.print_exception(exception)
            
        if exe_return_value_is_valid and raw_return_value_is_valid:
            pass
            # print('      Succesfull. Return values `{}`/`{}` is valid.'.format(
            #     feature_test_specification['test_function'],
            #     raw_return_value,
            #     exe_return_value
            # ))
        else:
            if not exe_return_value_is_valid:
                print('      Return value `{}` is invalid (expected `{}`) for EXE `{}`.'.format(
                    exe_return_value,
                    feature_test_specification['test_arguments'][1],
                    feature_test_specification['exe_file_path']
                ))
            if not raw_return_value_is_valid:
                print('      Return value `{}` is invalid (expected `{}`) for RAW `{}`.'.format(
                    raw_return_value,
                    feature_test_specification['test_arguments'][1],
                    feature_test_specification['raw_file_path']
                ))
            
        return exe_return_value_is_valid and raw_return_value_is_valid

