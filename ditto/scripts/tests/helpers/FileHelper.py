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

from helpers.ExceptionHelper import ExceptionHelper

class FileHelper:
    """The FileHelper class contains functions related to file interactions (like reading a file)."""

    @staticmethod
    def read_file(file_path, binary = False):
        """Read the contents of a file in either text or binary mode.

        Args:
            file_path (str): The path to the file to be read.
            binary (bool): Whether to read the file in binary mode. Defaults to False.

        Returns:
            str or bytes or None: The file content as a string or bytes, or None if an error occurred.

        """

        content = None

        try:
            if binary:
                with open(file_path, 'rb') as file_handle:
                    content = file_handle.read()
            else:
                with open(file_path, 'r', encoding='utf-8') as file_handle:
                    content = file_handle.read()
        except Exception as exception:
            ExceptionHelper.print_exception(exception)

        return content

    @staticmethod
    def file_exists(file_path):
        """Check if a file exists at the given path.

        Args:
            file_path (str): The path to the file to check.

        Returns:
            bool: True if the file exists, False otherwise.
            
        """

        return os.path.isfile(file_path)
