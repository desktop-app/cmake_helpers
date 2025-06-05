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

try:
    from win32api import GetFileVersionInfo
except ImportError:
    error('Python module "pywin32" is not installed.\n\nTry "' + sys.executable + ' -m pip install pywin32".')

if len(sys.argv) < 2:
    error('Expected input path to "d3dcompiler_47.dll".')

inputPath = sys.argv[1]
if not os.path.exists(inputPath):
    error('File "' + inputPath + '" doesn\'t exist.')

info = GetFileVersionInfo(inputPath, '\\')
version = [ info['FileVersionMS'] // 65536, info['FileVersionMS'] % 65536, info['FileVersionLS'] // 65536, info['FileVersionLS'] % 65536 ]
if (version != [10, 0, 22621, 3233]):
    error('Bad "d3dcompiler_47.dll" version: ' + '.'.join(str(x) for x in version))

bufferSize = 1024 * 1024
sha256 = hashlib.sha256()

with open(inputPath, 'rb') as f:
    print(hashlib.sha256(f.read()).hexdigest())
