'''
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
'''
import sys, os, shutil, subprocess

def run(project, arguments, buildType=''):
    scriptPath = os.path.dirname(os.path.realpath(__file__))
    basePath = scriptPath + '/../out/' + buildType

    cmake = ['cmake']
    for arg in arguments:
        if arg == 'debug':
            cmake.append('-DCMAKE_BUILD_TYPE=Debug')
        elif arg != 'force':
            cmake.append(arg)

    if sys.platform == 'win32':
        cmake.append('-AWin32')
    elif sys.platform == 'darwin':
        cmake.append('-GXcode')
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
    command = ' '.join(cmake)

    if not os.path.exists(basePath):
        os.makedirs(basePath)
    elif 'force' in arguments:
        paths = os.listdir(basePath)
        for path in paths:
            if path.lower().startswith('cmake'):
                full = basePath + '/' + path
                if os.path.isdir(full):
                    shutil.rmtree(full, ignore_errors=False)
                else:
                    os.remove(full)
        print('Cleared previous.')
    os.chdir(basePath)
    subprocess.call(command, shell=True)

    return 0
