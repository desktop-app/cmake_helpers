# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

from __future__ import print_function
import sys, os, re, hashlib

def error(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    sys.exit(1)

if len(sys.argv) < 2:
    error('Expected input path to "crashpad_handler.exe".')

inputPath = sys.argv[1]
if not os.path.exists(inputPath):
    error('File "' + inputPath + '" doesn\'t exist.')

sha256 = hashlib.sha256()

with open(inputPath, 'rb') as f:
    print(hashlib.sha256(f.read()).hexdigest())
