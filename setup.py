from __future__ import print_function

import os
import re
import sys
import site
import shutil
from six import add_metaclass
from distutils.sysconfig import get_python_lib
from setuptools import setup, find_packages, Extension
from setuptools.command.install import install as install_default

lime_shared_lib = ''

def get_version():
    v = ''
    with open('VERSION', 'r') as lines:
        v = list(lines)[0]
    return v

def prebuild():
    global lime_shared_lib

    suffix = ''
    if sys.platform == 'linux' or sys.platform == 'linux2':
        suffix = '.so'
    elif sys.platform == 'darwin':
        suffix = '.so'
    elif sys.platform == 'win32':
        suffix = '.pyd'
    else:
        print('Sorry, unsupported OS: {}'.format(sys.platform))
        return

    out_dir = 'build/lib'
    out_format = re.compile('pylime\.([a-zA-Z0-9]+)?(-[a-zA-Z0-9\_]+)?(-[a-zA-Z0-9\_]+)?' + suffix)

    out_file = None
    for f in os.listdir(out_dir):
        m = out_format.search(f)
        if m is not None:
            out_file = m.group(0)
            print('Install module "%s"' % out_file)
            break

    if out_file is None:
        print('Please build the library with CMake first.')
        print('If you did it, please make sure the path of the shared library.')
        raise Exception('Installation failed!!')

    shutil.copyfile(os.path.join(out_dir, out_file), os.path.join(site.getsitepackages()[-1], out_file))

# Install
class install(install_default):
    def run(self):
        install_default.run(self)

# Check build.
prebuild()
print(lime_shared_lib)

# Setup
setup(
    cmdclass={ 'install' : install },
    name='lime',
    version=get_version(),
    author='tatsy',
    author_email='tatsy.mail@gmail.com',
    url='https://github.com/tatsy/lime.git',
    description='Library for IMage Editing.',
    license='MIT',
    classifiers={
        'Development Status :: 4 - Beta',
        'Programming Language :: C++',
        'Programming Language :: Python 2',
        'Programming Language :: Python 3'
    },
    data_files=[ (get_python_lib(), [ lime_shared_lib ]) ]
)
