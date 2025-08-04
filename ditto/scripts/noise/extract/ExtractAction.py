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

import os
import sys
import json

from helpers.StatusHelper import StatusHelper
from helpers.FileHelper import FileHelper
from extract.ExtractActionWin import ExtractActionWin
from extract.ExtractActionLin import ExtractActionLin
from extract.ExtractActionMac import ExtractActionMac

class ExtractAction:
    """The ExtractAction class extracts the commonly found patterns or basic-blocks of instructions from a given directory of executables."""

    def __init__(self, platform, arch, directory):
        """Initiate the ExtractAction class.

        Args:
            platform (str): The platform to extract noise for.
            arch (str): The arch to extract noise for.
            directory (str): The directory containing binaries for the given platform & arch.

        """

        self.platform = platform
        self.arch = arch
        self.directory = directory

        if not FileHelper.directory_exists(self.directory):
            StatusHelper.fatal(StatusHelper.ERROR_INVALID_DIRECTORY)

        if not self.platform in ['win', 'lin', 'mac']:
            StatusHelper.fatal(StatusHelper.ERROR_INVALID_PLATFORM)

        if not self.arch in ['amd64', 'arm64']:
            StatusHelper.fatal(StatusHelper.ERROR_INVALID_ARCH)

        match self.platform:
            case 'win':
                self.extractor = ExtractActionWin(platform, arch, directory)
            case 'lin':
                self.extractor = ExtractActionLin(platform, arch, directory)
            case 'mac':
                self.extractor = ExtractActionMac(platform, arch, directory)

    def run(self):
        """Start the extraction."""

        basic_blocks = []

        StatusHelper.info('Iterating over PE-files in directory {}'.format(self.directory))

        for file_path, _, file_names in os.walk(self.directory):
            for file_name in file_names:
                file_path = os.path.join(file_path, file_name)

                new_basic_blocks = self.extractor.extract(file_path)

                if len(new_basic_blocks):
                    basic_blocks.extend(new_basic_blocks)
                
                if len(basic_blocks) != 0:
                    break

            if len(basic_blocks) != 0:
                break

        output_path = '{}/../data/{}-{}.json'.format(
            os.path.dirname(os.path.abspath(__file__)),
            self.platform,
            self.arch
        )

        StatusHelper.info('Saving to {}-{}.json'.format(self.platform, self.arch))

        with open(output_path, 'w') as output_handle:
            json.dump(basic_blocks, output_handle, indent=4)

        StatusHelper.info('Finished')