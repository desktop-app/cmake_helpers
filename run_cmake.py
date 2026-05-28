# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

import sys, os, shutil, subprocess

def resolveVisualStudioGenerator():
    fallback = 'Visual Studio 17 2022'
    vswhere = os.environ.get(
        'ProgramFiles(x86)',
        'C:\\Program Files (x86)') + '\\Microsoft Visual Studio\\Installer\\vswhere.exe'
    if not os.path.isfile(vswhere):
        return fallback
    try:
        import json
        out = subprocess.check_output([
            vswhere, '-latest', '-prerelease',
            '-products', '*',
            '-requires', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
            '-format', 'json',
        ])
        data = json.loads(out)
        if not data:
            return fallback
        major = int(data[0]['installationVersion'].split('.')[0])
    except Exception:
        return fallback
    if major >= 18:
        return 'Visual Studio 18 2026'
    if major >= 17:
        return 'Visual Studio 17 2022'
    if major >= 16:
        return 'Visual Studio 16 2019'
    return fallback

def run(project, arguments, buildType=''):
    scriptPath = os.path.dirname(os.path.realpath(__file__))
    basePath = scriptPath + '/../out/' + buildType

    cmake = ['cmake']
    vsArch = ''
    explicitGenerator = False
    for arg in arguments:
        if arg == 'debug':
            cmake.append('-DCMAKE_BUILD_TYPE=Debug')
        elif arg == 'x86' or arg == 'x64' or arg == 'arm':
            vsArch = arg
        elif arg != 'force':
            if arg.startswith('-G'):
                explicitGenerator = True
            cmake.append(arg)
    if sys.platform == 'win32' and not explicitGenerator:
        cmake.append('-G' + resolveVisualStudioGenerator())
        if vsArch == 'x64':
            cmake.append('-Ax64')
            cmake.append('-T v143')
        elif vsArch == 'arm':
            cmake.append('-AARM64')
        else:
            cmake.append('-AWin32') # default
            cmake.append('-T v143')
    elif vsArch != '':
        print("[ERROR] x86/x64/arm switch is supported only with Visual Studio.")
        return 1
    elif sys.platform == 'darwin':
        if not explicitGenerator:
            cmake.append('-GXcode')
    else:
        if not explicitGenerator:
            cmake.append('-GNinja Multi-Config')
        elif buildType:
            cmake.append('-DCMAKE_BUILD_TYPE=' + buildType)
        elif not '-DCMAKE_BUILD_TYPE=Debug' in cmake:
            cmake.append('-DCMAKE_BUILD_TYPE=Release')

    specialTarget = ''
    specialTargetFile = scriptPath + '/../' + project + '/build/target'
    if os.path.isfile(specialTargetFile):
        with open(specialTargetFile, 'r') as f:
            for line in f:
                target = line.strip()
                if len(target) > 0:
                    cmake.append('-DDESKTOP_APP_SPECIAL_TARGET=' + target)

    cmake.extend(['-Werror=dev', '-Werror=deprecated', '--warn-uninitialized', '..' if not buildType else '../..'])
    command = '"' + '" "'.join(cmake) + '"'

    if not os.path.exists(basePath):
        os.makedirs(basePath)
    elif 'force' in arguments:
        paths = os.listdir(basePath)
        for path in paths:
            low = path.lower();
            if not low.startswith('debug') and not low.startswith('release'):
                full = basePath + '/' + path
                if os.path.isdir(full):
                    shutil.rmtree(full, ignore_errors=False)
                else:
                    os.remove(full)
        print('Cleared previous.')
    os.chdir(basePath)
    return subprocess.call(command, shell=True)
