Installation Guide
===================

----------
Download
----------

Please download the source files from |github_link|.

.. code-block:: shell

  git clone https://github.com/tatsy/lime.git

.. |github_link| raw:: html

   <a href="https://github.com/tatsy/lime.git" target="_blank">GitHub</a>

------
C/C++
------

Since this library includes only headers, you do not have to any compilation for using the library in C/C++. Please include `$LIME_ROOT/sources/lime.hpp` in your program source codes.

Also, you can build the static library for accelerating the compilation of your program with `LIME`. To do so, what you need to do is only to specify `LIME_BUILD_STATIC_LIB` to the cmake option, and the library can be built with the following commands.

.. code-block:: shell

  git clone https://github.com/tatsy/lime.git
  mkdir build && cd build
  cmake -D LIME_BUILD_EXAMPLES=OFF \
        -D LIME_BUILD_TESTS=OFF \
        -D LIME_BUILD_STATIC_LIB=ON ..
  make -j4
  sudo make install

--------
Python
--------

..  raw:: html
  
  <p style="color: red;">We are now free from Boost! You can install Python module just our source code!</p>


For Python users, you can build the library with CMake. While `lime` depends on `OpenCV` (3.0 or higher), you need to install these libraries in advance. Please make sure, the versions of these libraries meet the following requirements.

You can build and install the python library with following shell script. If you build the module with Anaconda/Miniconda, make sure the path to the python executable is specified to `PYTHON_EXECUTABLE`.

.. code-block:: shell

  git clone https://github.com/tatsy/lime.git
  git submodule update --init --recursive  # Download "pybind11" needed to build python module
  mkdir build && cd build
  cmake -D LIME_BUILD_EXAMPLES=OFF \
        -D LIME_BUILD_TESTS=OFF \
        -D PYTHON_EXECUTABLE=/home/user/miniconda3/envs/lime/bin/python \
        -D LIME_BUILD_PYTHON_MODULE=ON ..
  cmake --build . -- -j4
  cd ..
  python setup.py install  # You may need to use "sudo" for system-wide python.

If the installation is failed, you should check whether `pylime` binary file appears in 'build/lib' directory (the name of the binary would be like `pylime.cp36-win_amd64.pyd` (Windows), 'pylime.cython-36m-darwin.so' (MacOS) or 'pylime.cython-36m-linux.so' (Linux)).
