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

from helpers.StatusHelper import StatusHelper
from helpers.FileHelper import FileHelper

class ExtractActionLin:
    """The ExtractActionLin class extracts the commonly found patterns or basic-blocks of instructions from a given directory of executables."""

    def __init__(self, platform, arch, directory):
        """Initiate the ExtractActionLin class.

        Args:
            platform (str): The platform to extract noise for.
            arch (str): The arch to extract noise for.
            directory (str): The directory containing binaries for the given platform & arch.

        """

        self.platform = platform
        self.arch = arch
        self.directory = directory

    def extract(self, file_path):
        """Start the extraction.

		Args:
			file_path (str): Potential executable file to extract instructions from.

        """

        pass
