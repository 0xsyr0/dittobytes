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

from helpers.FileHelper import FileHelper

class VerificationMetamorphicationsNotOriginal:
    """Checks whether the given shellcode is not equal to the original shellcode."""

    def requires_supported_os_and_arch(self):
        """Whether this verification requires the os & arch of the shellcode to be executable in this runtime.

        Returns:
            bool: Positive if required.

        """

        return False

    def verify(self, feature_test_specification):
        """Checks whether the given shellcode is not equal to the original shellcode.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.

        """

        # Skip if this is the original shellcode itself
        if feature_test_specification['metamorphication'] == 'original':
            # print('      Skipped. Shellcode is the original itself.'.format(
            #     feature_test_specification['test_function']
            # ))

            return True

        transpiled_path = feature_test_specification['shellcode']
        transpiled = FileHelper.read_file(transpiled_path, binary=True)

        original_path = transpiled_path.replace(f"{feature_test_specification['metamorphication']}.bin", 'original.bin')
        original = FileHelper.read_file(original_path, binary=True)

        is_not_original = original != transpiled

        if is_not_original:
            pass
            # print('      Succesfull. Shellcode is not equal to the original.'.format(
            #     feature_test_specification['test_function'],
            #     feature_test_specification['shellcode']
            # ))
        else:
            print('      The shellcode is identical to the original shellcode: `{}`.'.format(
                feature_test_specification['shellcode']
            ))

        return is_not_original
