from __future__ import print_function

import os
import sys
import site
import shutil
from six import add_metaclass
from distutils.sysconfig import get_python_lib
from setuptools import setup, find_packages, Extension
from setuptools.command.install import install as install_default

lime_shared_lib = ''

def prebuild():
    global lime_shared_lib

    prefix = ''
    suffix = ''
    py_suffix = ''
    if sys.platform == 'linux' or sys.platform == 'linux2':
        prefix = 'lib'
        suffix = '.so'
        py_suffix = '.so'
    elif sys.platform == 'darwin':
        prefix = 'lib'
        suffix = '.dylib'
        py_suffix = '.so'
    elif sys.platform == 'win32':
        prefix = ''
        suffix = '.dll'
        py_suffix = '.pyd'
    else:
        print('Sorry, unsupported OS: {}'.format(sys.platform))
        return

    out_dir = 'build/lib'
    outfile = '%spylime%s' % (prefix, suffix)
    if not os.path.exists(os.path.join(out_dir, outfile)):
        print('Please build the library with CMake first.')
        print('If you did it, please make sure the path of the shared library.')
        print('The path should be: {}'.format(
            os.path.join(out_dir, outfile)))
        raise Exception('Installation failed!!')

    lime_shared_lib = os.path.join(out_dir, 'lime%s' % py_suffix)
    shutil.copyfile(os.path.join(out_dir, outfile),
              os.path.join(lime_shared_lib))

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
    version='0.3.0',
    author='tatsy',
    author_email='tatsy.mail@gmail.com',
    url='https://github.com/tatsy/lime.git',
    description='Library for IMage Editing.',
    license='MIT',
    classifiers={
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python 3',
        'Programming Language :: C++'
    },
    data_files=[ (get_python_lib(), [ lime_shared_lib ]) ]
)
