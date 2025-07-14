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

import sys

class StatusHelper:
    """The StatusHelper class contains error/exit codes."""

    # Success
    SUCCESS = (0x00000000, '+', None)

    # Errors
    ERROR_FATAL_ERROR = (0x00000001, '!', 'A unknown error occurred')
    ERROR_INVALID_ARGUMENTS = (0x00000002, '!', 'Invalid command-line arguments')
    ERROR_CANNOT_TEST_OS_OR_ARCH = (0x00000003, '!', 'This os/arch is not supported in the current runtime')
    ERROR_TEST_FUNCTION_NOT_SUPPORTED = (0x00000004, '!', 'The test function {} is not supported')
    ERROR_TEST_FUNCTION_FAILED = (0x00000005, '!', 'The test function {} failed')
    ERROR_EXE_FILE_NOT_FOUND = (0x00000006, '!', 'EXE file not found {}')
    ERROR_RAW_FILE_NOT_FOUND = (0x00000007, '!', 'RAW file not found {}')
    ERROR_BOF_FILE_NOT_FOUND = (0x00000008, '!', 'BOF file not found {}')
    ERROR_RETURN_VERIFICATION_PROCESS_FAILED = (0x00000009, '!', 'Returns verification process did not exit successfully')

    @staticmethod
    def exit(status, variables=[]):
        """Exit the current Python runtime with the given status (exit code).

        Args:
            status (tuple): The process exit code to use.
            variables (list): Variables to format into the message.

        """

        if isinstance(status, str):
            status = (StatusHelper.ERROR_FATAL_ERROR[0], StatusHelper.ERROR_FATAL_ERROR[1], status)

        if status[0] != 0:
            if len(variables):
                print(f'[{status[1]}] Error: {status[2].format(*variables)}.')
            else:
                print(f'[{status[1]}] Error: {status[2]}.')

        sys.exit(status[0])

    @staticmethod
    def fatal(status, variables=[]):
        """Exit the current Python runtime with the given status (exit code).

        Args:
            status (tuple): The process exit code to use.
            variables (list): Variables to format into the message.

        """

        StatusHelper.exit(status, variables)

    @staticmethod
    def error(status, variables=[]):
        """Log the given status but continue the current Python runtime.

        Args:
            status (tuple): The status to log to the console.
            variables (list): Variables to format into the message.

        """

        if isinstance(status, str):
            status = (0x0, '!', status)

        if len(variables):
            print(f'[{status[1]}] {status[2].format(*variables)}.')
        else:
            print(f'[{status[1]}] {status[2]}.')

    @staticmethod
    def info(status, variables=[]):
        """Log the given status but continue the current Python runtime.

        Args:
            status (tuple): The status to log to the console.
            variables (list): Variables to format into the message.

        """

        if isinstance(status, str):
            status = (0x0, '+', status)

        if len(variables):
            print(f'[{status[1]}] {status[2].format(*variables)}.')
        else:
            print(f'[{status[1]}] {status[2]}.')

    @staticmethod
    def verbose(status, variables=[]):
        """Log the given status but continue the current Python runtime.

        Args:
            status (tuple): The status to log to the console.
            variables (list): Variables to format into the message.

        """

        pass