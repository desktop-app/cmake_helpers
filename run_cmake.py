import sys
import os
import shutil
import subprocess


def run(project, arguments, buildType=''):
    scriptPath = os.path.dirname(os.path.realpath(__file__))
    basePath = os.path.join(scriptPath, '..', 'out', buildType)

    cmake = ['cmake']
    cmake_args = []

    for arg in arguments:
        if arg == 'debug':
            cmake_args.append('-DCMAKE_BUILD_TYPE=Debug')
        elif arg != 'force':
            cmake_args.append(arg)

    if sys.platform == 'win32':
        cmake_args.append('-AWin32')
    elif sys.platform == 'darwin':
        cmake_args.append('-GXcode')
    elif buildType:
        cmake_args.append('-DCMAKE_BUILD_TYPE=' + buildType)
    elif '-DCMAKE_BUILD_TYPE=Debug' not in cmake_args:
        cmake_args.append('-DCMAKE_BUILD_TYPE=Release')

    specialTarget = ''
    specialTargetFile = os.path.join(scriptPath, '..', project, 'build', 'target')
    if os.path.isfile(specialTargetFile):
        with open(specialTargetFile, 'r') as f:
            for line in f:
                target = line.strip()
                if len(target) > 0:
                    cmake_args.append('-DDESKTOP_APP_SPECIAL_TARGET=' + target)

    cmake_args.extend(['-Werror=dev', '-Werror=deprecated', '--warn-uninitialized', '..' if not buildType else '../..'])
    command = ' '.join(cmake + cmake_args)

    os.makedirs(basePath, exist_ok=True)

    if 'force' in arguments:
        paths = os.listdir(basePath)
        for path in paths:
            if path.lower().startswith('cmake'):
                full = os.path.join(basePath, path)
                if os.path.isdir(full):
                    shutil.rmtree(full, ignore_errors=False)
                else:
                    os.remove(full)
        print('Cleared previous.')

    os.chdir(basePath)
    subprocess.run(command, shell=True, check=True)

    return 0
