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
import glob

from helpers.FileHelper import FileHelper
from helpers.ExceptionHelper import ExceptionHelper
from helpers.StatusHelper import StatusHelper
from helpers.EnvironmentHelper import EnvironmentHelper

from verifications.VerificationReturns import VerificationReturns
from verifications.VerificationMetamorphicationsNotOriginal import VerificationMetamorphicationsNotOriginal
from verifications.VerificationHexNotPresent import VerificationHexNotPresent

class VerificationHelper:
    """The VerificationHelper class contains functions to parse feature test files."""

    @staticmethod
    def get_available_metamorphications(shellcode_compiled_for_os, shellcode_compiled_for_arch, shellcode_is_compatible_with_os, shellcode_is_compatible_with_arch, shellcode_feature_test_file_name, shellcode_feature_test_file_path):
        """Return a dict of all verifications to check, and for which shellcodes.

        Args:
            shellcode_compiled_for_os (str): The OS the shellcode is compiled for.
            shellcode_compiled_for_arch (str): The architecture the shellcode is compiled for.
            shellcode_is_compatible_with_os (str): The OS the shellcode source could potentially compile to.
            shellcode_is_compatible_with_arch (str): The architecture the shellcode source could potentially compile to.
            shellcode_feature_test_file_name (str): The file name of the feature test.
            shellcode_feature_test_file_path (str): The file path of the feature test.

        Returns:
            dict: The dict of verifications to check, and for which shellcodes.

        """

        available_metamorphications = []

        # First list all metamorphications in the test file
        feature_test_contents = FileHelper.read_file(shellcode_feature_test_file_path)
        verify_pattern = r'@verify +(\w+?) +(\w+?) +(\w+?) +(\w+?) +((-|,|.\d|\w)+?) +(.*)'
        verify_matches = re.findall(verify_pattern, feature_test_contents)

        for verify_match in verify_matches:
            verify_match_oses = EnvironmentHelper.get_all_oses() if verify_match[0] == 'all' else [verify_match[0]]
            verify_match_arches = EnvironmentHelper.get_all_arches() if verify_match[1] == 'all' else [verify_match[1]]
            verify_match_metamorphication = verify_match[2]

            # If 'all', then read from compiled shellcodes via .glob()
            if verify_match_metamorphication == 'all':
                continue

            # If OS doesn't match, we can skip this
            if shellcode_compiled_for_os not in verify_match_oses:
                continue

            # If architecture doesn't match, we can skip this
            if shellcode_compiled_for_arch not in verify_match_arches:
                continue

            if verify_match_metamorphication not in available_metamorphications:
                available_metamorphications.append(verify_match_metamorphication)

        # Then list all metamorphications in the builds directory
        wildcard = './builds/beacon-{}-{}-tests_*_*_{}_*.bin'.format(
            shellcode_compiled_for_os,
            shellcode_compiled_for_arch,
            shellcode_feature_test_file_name
        )

        potential_metamorphications = glob.glob(wildcard)
        for potential_metamorphication in potential_metamorphications:
            metamorphication = potential_metamorphication.split(f'{shellcode_feature_test_file_name}_')[1][0:-4]

            if metamorphication in available_metamorphications:
                continue

            available_metamorphications.append(metamorphication)

        return available_metamorphications

    @staticmethod
    def get_feature_tests(shellcode_is_compatible_with_os, shellcode_is_compatible_with_arch, shellcode_feature_test_file_name, shellcode_feature_test_file_path):
        """Return a dict of all verifications to check, and for which shellcodes.

        Args:
            shellcode_is_compatible_with_os (str): The OS the shellcode could potentially run on.
            shellcode_is_compatible_with_arch (str): The architecture the shellcode could potentially run on.
            shellcode_feature_test_file_name (str): The file name of the feature test.
            shellcode_feature_test_file_path (str): The file path of the feature test.

        Returns:
            dict: The dict of verifications to check, and for which shellcodes.

        """

        results = {}

        feature_test_contents = FileHelper.read_file(shellcode_feature_test_file_path)
        verify_pattern = r'@verify +(\w+?) +(\w+?) +(\w+?) +(\w+?) +((-|,|.\d|\w)+?) +(.*)'
        verify_matches = re.findall(verify_pattern, feature_test_contents)

        # Then iterate over all `@verify` specifications in the feature test source code
        for verify_match in verify_matches:
            verify_match_oses = EnvironmentHelper.get_all_oses() if verify_match[0] == 'all' else [verify_match[0]]
            verify_match_arches = EnvironmentHelper.get_all_arches() if verify_match[1] == 'all' else [verify_match[1]]
            verify_match_metamorphication = verify_match[2]
            verify_match_test_function = verify_match[3]
            verify_match_test_arguments = verify_match[4].split(',')

            for verify_match_os in verify_match_oses:
                for verify_match_arch in verify_match_arches:

                    available_metamorphications = VerificationHelper.get_available_metamorphications(verify_match_os, verify_match_arch, shellcode_is_compatible_with_os, shellcode_is_compatible_with_arch, shellcode_feature_test_file_name, shellcode_feature_test_file_path)

                    metamorphications = available_metamorphications if verify_match_metamorphication == 'all' else [verify_match_metamorphication]
                    for metamorphication in metamorphications:
               
                        # Define shellcode test file index key
                        shellcode_file_path = './builds/beacon-{}-{}-tests_{}_{}_{}_{}.bin'.format(
                            verify_match_os,
                            verify_match_arch,
                            shellcode_is_compatible_with_os,
                            shellcode_is_compatible_with_arch,
                            shellcode_feature_test_file_name,
                            metamorphication
                        )
                        if not FileHelper.file_exists(shellcode_file_path):
                            StatusHelper.fatal(StatusHelper.ERROR_SHELLCODE_NOT_FOUND, [shellcode_file_path])

                        if shellcode_file_path not in results.keys():
                            results[shellcode_file_path] = {}

                        # Define verify specification index key
                        verify_specification_index_key = '{}_{}_{}_{}_{}'.format(
                            verify_match_os,
                            verify_match_arch,
                            metamorphication,
                            verify_match_test_function,
                            verify_match_test_arguments
                        )

                        results[shellcode_file_path][verify_specification_index_key] = {
                            'metamorphication': metamorphication,
                            'test_file_name': shellcode_feature_test_file_name,
                            'test_file_path': shellcode_feature_test_file_path,
                            'test_function': verify_match_test_function,
                            'test_arguments': verify_match_test_arguments,
                            'shellcode': shellcode_file_path,
                            'compiled_for_os': verify_match_os,
                            'compiled_for_arch': verify_match_arch,
                            'source_can_compile_to_os': verify_match[0],
                            'source_can_compile_to_arch': verify_match[1]
                        }

        return results

    @staticmethod
    def get_verification_class(test_function):
        """Get the class which contains the given test function.
        
        Args:
            test_function (str): The name of the (supported) verification function.

        Returns:
            class: An initiated class which contains the test function.
        """

        available_verifications = {
            'hex_not_present': VerificationHexNotPresent(),
            'metamorphications_not_original': VerificationMetamorphicationsNotOriginal(),
            'returns': VerificationReturns()
        }


        if test_function not in available_verifications.keys():
            StatusHelper.fatal(StatusHelper.ERROR_TEST_FUNCTION_NOT_SUPPORTED, [test_function])

        return available_verifications[test_function]

    @staticmethod
    def verify_or_fail(feature_test_specification):
        """Run the given feature test specification and exit on failure.

        Args:
            feature_test_specification (dict): A dict containing all test details.

        """

        # If the verification is os/arch specific, and we don't support that os/arch in this runtime; skip the test.
        loader = EnvironmentHelper.get_loader(feature_test_specification['compiled_for_os'], feature_test_specification['compiled_for_arch'])
        verification_class = VerificationHelper.get_verification_class(feature_test_specification['test_function'])
        if verification_class.requires_supported_os_and_arch() and not loader:
            # print('      Verification `{}` skipped. Compatible os/arch required to run {}/{}.'.format(
            #     feature_test_specification['test_function'],
            #     feature_test_specification['compiled_for_os'],
            #     feature_test_specification['compiled_for_arch']
            # ))
            return

        print('    - For, `{}-{}` checking `{}({})` on shellcode with `{}` metamorphication.'.format(
            feature_test_specification['compiled_for_os'],
            feature_test_specification['compiled_for_arch'],
            feature_test_specification['test_function'],
            ', '.join(feature_test_specification['test_arguments']),
            feature_test_specification['metamorphication']
        ))

        if not verification_class.verify(feature_test_specification):
            StatusHelper.fatal(StatusHelper.ERROR_TEST_FUNCTION_FAILED, [
                feature_test_specification['test_function']
            ])
