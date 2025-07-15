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

import string
import binary2strings as b2s
from helpers.FileHelper import FileHelper

class VerifyForensicallyClean:
    """Checks whether the given file does not contain obvious traces leading back to Dittobytes."""

    def requires_supported_os_and_arch(self):
        """Whether this verification requires the os & arch of the shellcode to be executable in this runtime.

        Returns:
            bool: Positive if required.

        """

        return False

    def __verify_spoiler_keywords(self, feature_test_specification):
        """Checks whether the given file does not contain obvious traces leading back to Dittobytes string wise.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.
        
        """
        
        for file_type in ['exe', 'raw', 'obj']:
            haystack = FileHelper.read_file(feature_test_specification['{}_file_path'.format(file_type)], 'rb')
            results = b2s.extract_all_strings(haystack, min_chars=4)

            for needle in ['beacon', 'ditto', 'shellcode']:

                if any(needle in item[0].lower() for item in results):
                    print('      Dittobytes traceable string `{}` found in EXE `{}`.'.format(
                        needle,
                        feature_test_specification['{}_file_path'.format(file_type)]
                    )) 
                    return False

        return True

    def verify(self, feature_test_specification):
        """Checks whether the given file does not contain obvious traces leading back to Dittobytes.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.

        """

        if not self.__verify_spoiler_keywords(feature_test_specification):
            return False

        return True