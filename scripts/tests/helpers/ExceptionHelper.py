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
import traceback

class ExceptionHelper:
    """The ExceptionHelper class contains functions to help in handling exceptions."""

    @staticmethod
    def print_exception(exception):
        """Print the exception type, file, and line number where it occurred.

        Args:
            exception (Exception): The caught exception instance.

        """

        exc_type, exc_value, exc_tb = sys.exc_info()
        filename, lineno, _, _ = traceback.extract_tb(exc_tb)[-1]
        print(f"    - {type(exception).__name__} exception at {filename}:{lineno}: {exception}")