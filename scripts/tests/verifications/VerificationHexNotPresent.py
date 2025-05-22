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

class VerificationHexNotPresent:
    """Checks whether the given hex byte sequence is not present in the given shellcode."""

    def requires_supported_os_and_arch(self):
        """Whether this verification requires the os & arch of the shellcode to be executable in this runtime.

        Returns:
            bool: Positive if required.

        """

        return False

    def verify(self, feature_test_specification):
        """Checks whether the given hex byte sequence is not present in the given shellcode.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.

        """

        haystack = FileHelper.read_file(feature_test_specification['shellcode'], 'rb')
        needle = bytes.fromhex(feature_test_specification['test_arguments'][0])
        hex_is_not_present_in_shellcode = needle not in haystack

        original_path = feature_test_specification['shellcode'].replace(f"{feature_test_specification['metamorphication']}.bin", 'original.bin')
        haystack = FileHelper.read_file(original_path, 'rb')
        hex_is_present_in_original = needle in haystack

        if hex_is_not_present_in_shellcode:
            pass
            # print('      Succesfull. Did not find `{}` in shellcode `{}`.'.format(
            #     feature_test_specification['test_function'],
            #     feature_test_specification['test_arguments'][0],
            #     feature_test_specification['shellcode']
            # ))
        else:
            print('      Found `{}` in `{}-{}` shellcode with `{}` metamorphication: `{}`.'.format(
                feature_test_specification['test_arguments'][0],
                feature_test_specification['compiled_for_os'],
                feature_test_specification['compiled_for_arch'],
                feature_test_specification['metamorphication'],
                feature_test_specification['shellcode']
            ))

        if not hex_is_present_in_original:
            print('      Found `{}` in `{}-{}` shellcode without metamorphication: `{}`.'.format(
                feature_test_specification['test_arguments'][0],
                feature_test_specification['compiled_for_os'],
                feature_test_specification['compiled_for_arch'],
                original_path
            ))

        return hex_is_not_present_in_shellcode and hex_is_present_in_original