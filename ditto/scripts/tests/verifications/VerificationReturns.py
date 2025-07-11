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

        return_value = None
        return_value_is_valid = False
        executable_result = None

        try:
            expected_return_type = feature_test_specification['test_arguments'][0]
            expected_return_value = feature_test_specification['test_arguments'][1]

            command = EnvironmentHelper.get_loader(feature_test_specification['compiled_for_os'], feature_test_specification['compiled_for_arch']).split(' ')
            command.append(feature_test_specification['shellcode'])

            executable_result = subprocess.run(command, capture_output=True, text=True, timeout=5)

            if executable_result.returncode != 0:
                StatusHelper.error(StatusHelper.ERROR_RETURN_VERIFICATION_PROCESS_FAILED)
                return_value_is_valid = False
            else:
                result_return_value_pattern = r'Got result from shellcode \(' + expected_return_type + r'\): +0x\w+ \((signed|unsigned) (-?\d+)\)'
                result_return_value_matches = re.findall(result_return_value_pattern, executable_result.stdout)
                return_value = result_return_value_matches[0][1]
                return_value_is_valid = return_value == expected_return_value
        except Exception as exception:
            ExceptionHelper.print_exception(exception)
            
        if return_value_is_valid:
            pass
            # print('      Succesfull. Return value `{}` is valid.'.format(
            #     feature_test_specification['test_function'],
            #     return_value
            # ))
        else:
            print('      Return value `{}` is invalid (expected `{}`) for shellcode `{}`.'.format(
                return_value,
                feature_test_specification['test_arguments'][0],
                feature_test_specification['shellcode']
            ))
            
        return return_value_is_valid

