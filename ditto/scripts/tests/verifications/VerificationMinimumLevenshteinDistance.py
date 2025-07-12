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
from Levenshtein import distance

class VerificationMinimumLevenshteinDistance:
    """Checks whether the given shellcode has a minimum Levenshtein distance with another given shellcode."""

    def requires_supported_os_and_arch(self):
        """Whether this verification requires the os & arch of the shellcode to be executable in this runtime.

        Returns:
            bool: Positive if required.

        """

        return False

    def verify(self, feature_test_specification):
        """Checks whether the given shellcode has a minimum Levenshtein distance with another given shellcode.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        Returns:
            bool: Positive if the verification was successful.

        """

        transpiled_1_path = feature_test_specification['shellcode']
        transpiled_1 = FileHelper.read_file(transpiled_1_path, binary=True)

        transpiled_2_path = transpiled_1_path.replace(f"{feature_test_specification['metamorphication']}.raw", f"{feature_test_specification['test_arguments'][0]}.raw")
        transpiled_2 = FileHelper.read_file(transpiled_2_path, binary=True)

        transpiled_length = max(len(transpiled_1), len(transpiled_2))

        result_absolute = distance(transpiled_1, transpiled_2)
        result_percentage = round((result_absolute / transpiled_length) * 100)
        result_percentage_required = int(feature_test_specification['test_arguments'][1])
        result_enough = result_percentage >= result_percentage_required

        if result_enough:
            pass
            # print('      Succesfull. Levenshtein distance measurement: {} ({}% change)'.format(
            #     result_absolute,
            #     result_percentage
            # ))
        else:
            print('      Levenshtein insufficient. Minimum change of {}% required for length {}, but got {}%.'.format(
                result_percentage_required,
                transpiled_length,
                result_percentage
            ))

        return result_enough
