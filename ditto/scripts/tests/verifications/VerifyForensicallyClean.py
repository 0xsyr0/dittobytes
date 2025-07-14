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

class VerifyForensicallyClean:
    """Checks whether the given file does not contain obvious traces leading back to Dittobytes."""

    def requires_supported_os_and_arch(self):
        """Whether this verification requires the os & arch of the shellcode to be executable in this runtime.

        Returns:
            bool: Positive if required.

        """

        return False

    def verify(self, feature_test_specification):
        """Checks whether the given file does not contain obvious traces leading back to Dittobytes.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.

        """

        haystack = FileHelper.read_file(feature_test_specification['raw_file_path'], 'rb')
        
        # TODO: Implement this check (verify whether e.g. the string 'beacon' is present)
        return True