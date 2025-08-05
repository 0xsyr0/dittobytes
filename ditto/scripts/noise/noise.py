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
from extract.ExtractAction import ExtractAction
from provide.ProvideAction import ProvideAction

def main():
    """A script to extract code from legitimate binaries, and then serve it to Dittobytes as semantic noise (meaningful dead code)."""

    if len(sys.argv) < 2:
        StatusHelper.error('Usage: python noise.py <action (extract|provide)> <args ...>')
        StatusHelper.fatal(StatusHelper.ERROR_INVALID_ARGUMENTS)

    arg_action = sys.argv[1]

    if arg_action == 'extract' and len(sys.argv) != 5:
        StatusHelper.error('Usage: python noise.py extract <platform> <arch> <directory>')
        StatusHelper.fatal(StatusHelper.ERROR_INVALID_ARGUMENTS)
    elif arg_action == 'extract':
        ExtractAction(sys.argv[2], sys.argv[3], sys.argv[4]).run()
        StatusHelper.exit(StatusHelper.SUCCESS)

    if arg_action == 'provide' and len(sys.argv) != 4:
        StatusHelper.error('Usage: python noise.py extract <platform> <arch>')
        StatusHelper.fatal(StatusHelper.ERROR_INVALID_ARGUMENTS)
    elif arg_action == 'provide':
        ProvideAction(sys.argv[2], sys.argv[3]).run()
        StatusHelper.exit(StatusHelper.SUCCESS)

    StatusHelper.exit(StatusHelper.ERROR_INVALID_ARGUMENTS)

if __name__ == "__main__":
    main()