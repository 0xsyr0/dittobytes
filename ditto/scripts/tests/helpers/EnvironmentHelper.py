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

import platform

from helpers.ExceptionHelper import ExceptionHelper

class EnvironmentHelper:
    """The EnvironmentHelper class contains functions related to the current runtime's platform & architecture."""

    available_systems = {'Windows': 'win', 'Linux': 'lin', 'Darwin': 'mac'}

    available_architectures = {'AMD64': 'amd64', 'x86_64': 'amd64', 'arm64': 'arm64', 'aarch64': 'arm64'}

    @staticmethod
    def get_all_oses():
        """Get a list of all the supported operating systems.

        Returns:
            list: THe supported operating systems.

        """

        return EnvironmentHelper.available_systems.values()

    @staticmethod
    def get_current_os():
        """Detect the current operating system.

        Returns:
            str or bool: A shorthand identifier ('win', 'lin', or 'mac'), or False on failure.

        """

        try:
            return EnvironmentHelper.available_systems[platform.system()]
        except Exception as exception:
            ExceptionHelper.print_exception(exception)
            return None

    @staticmethod
    def get_all_arches():
        """Get a list of all the supported architectures.

        Returns:
            list: THe supported architectures.

        """

        return list(set(EnvironmentHelper.available_architectures.values()))

    @staticmethod
    def get_current_arch():
        """Detect the system's CPU architecture.

        Returns:
            str or bool: A shorthand architecture identifier ('amd64' or 'arm64'), or False on failure.

        """

        try:
            return EnvironmentHelper.available_architectures[platform.machine()]
        except Exception as exception:
            ExceptionHelper.print_exception(exception)
            return None

    @staticmethod
    def get_loader(target_os, target_arch):
        """Get a test command prefix (with a loader) for the target os/arch (if supported).

        Args:
            target_os (str): The OS to get a loader for.
            target_arch (str): The arch to get a loader for.

        Returns:
            str: The loader command (if available).

        """

        build_dir = './build'

        available_loaders = {
            'win-amd64-to-win-amd64': f'{build_dir}/loader-win-amd64.exe',
            'win-arm64-to-win-amd64': f'{build_dir}/loader-win-arm64.exe',
            'lin-amd64-to-lin-amd64': f'{build_dir}/loader-lin-amd64',
            'lin-amd64-to-lin-arm64': f'qemu-aarch64 -L /usr/aarch64-linux-gnu {build_dir}/loader-lin-arm64',
            'lin-arm64-to-lin-arm64': f'{build_dir}/loader-lin-arm64',
            'mac-amd64-to-mac-amd64': f'{build_dir}/loader-mac-amd64',
            'mac-arm64-to-mac-arm64': f'{build_dir}/loader-mac-arm64'
        }

        current_os = EnvironmentHelper.get_current_os()
        current_arch = EnvironmentHelper.get_current_arch()

        try:
            return available_loaders[f'{current_os}-{current_arch}-to-{target_os}-{target_arch}']
        except Exception as exception:
            return None

    @staticmethod
    def can_run_target(target_os, target_arch):
        """Check whether the current runtime can run tests for the given runtime.

        Args:
            target_os (str): The OS to test for.
            target_arch (str): The arch to test for.

        Returns:
            bool: Positive if this runtime can test the given feature test.

        """

        return EnvironmentHelper.get_loader(target_os, target_arch) != None;

    @staticmethod
    def get_desired_test_runtimes(target_os, target_arch):
        """Get a a dict of os/arch combinations to preferably test the feature test on.

        Args:
            target_os (str): The OS to test for.
            target_arch (str): The arch to test for.

        Returns:
            dict: A dictionary with target os/arch combinations (in which 'all' is expended to all supported os/arch's).

        """

        result = {}

        available_os = ['win', 'lin', 'mac']
        available_arch = ['amd64', 'arm64']

        all_target_os = available_os if target_os == 'all' else [target_os]
        all_target_arch = available_arch if target_arch == 'all' else [target_arch]

        for target_os in all_target_os:

            if target_os not in result.keys():
                result[target_os] = []

            for target_arch in all_target_arch:
                result[target_os].append(target_arch)

        return result

    @staticmethod
    def filter_desired_test_runtimes(desired_test_runtimes):
        """Get a a dict of os/arch combinations to test the current feature test on.

        Args:
            desired_test_runtimes (dict): A dictionary with target os/arch combinations (in which 'all' is expended to all supported os/arch's).

        Returns:
            dict: A dictionary with target os/arch combinations that should currently be tested.

        """

        result = {}

        for target_os, target_os_arches in desired_test_runtimes.items():
            for target_arch in target_os_arches:
                if EnvironmentHelper.can_run_target(target_os, target_arch):
                    if target_os not in result.keys():
                        result[target_os] = []

                    result[target_os].append(target_arch)

        return result