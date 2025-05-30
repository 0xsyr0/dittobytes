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

import sys

from helpers.StatusHelper import StatusHelper
from helpers.ExceptionHelper import ExceptionHelper
from helpers.FileHelper import FileHelper
from helpers.EnvironmentHelper import EnvironmentHelper
from helpers.VerificationHelper import VerificationHelper
from collections import OrderedDict

def main():
    """Run all `@verify` verifications that are supported on this os/arch."""

    if len(sys.argv) != 7 and len(sys.argv) != 8:
        StatusHelper.error('Usage: python test.py <shellcode_is_compatible_with_os> <shellcode_is_compatible_with_arch> <shellcode_feature_test_file_name> <test_file_path> <filter_os> <filter_arch> <filter_metamorphication:optional>')
        StatusHelper.error('Given: {}'.format(' '.join(sys.argv)))
        StatusHelper.fatal(StatusHelper.ERROR_INVALID_ARGUMENTS)

    shellcode_is_compatible_with_os = sys.argv[1]
    shellcode_is_compatible_with_arch = sys.argv[2]
    shellcode_feature_test_file_name = sys.argv[3]
    shellcode_feature_test_file_path = sys.argv[4]
    filter_os = sys.argv[5]
    filter_arch = sys.argv[6]
    filter_metamorphication = sys.argv[7] if len(sys.argv) == 8 else 'all'

    feature_tests = VerificationHelper.get_feature_tests(shellcode_is_compatible_with_os, filter_os, shellcode_is_compatible_with_arch, filter_arch, filter_metamorphication, shellcode_feature_test_file_name, shellcode_feature_test_file_path)
    feature_tests = OrderedDict(sorted(feature_tests.items()))

    for shellcode_file, feature_test_specifications in feature_tests.items():
        for feature_test_specification in feature_test_specifications.values():
            VerificationHelper.verify_or_fail(feature_test_specification)

    StatusHelper.exit(StatusHelper.SUCCESS)

if __name__ == "__main__":
    main()