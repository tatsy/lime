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

Since this library includes only headers, you do not have to any compilation for
using the library in C/C++. Please include `$LIME_ROOT/sources/lime.hpp` in
your program source codes.

--------
Python
--------

For Python users, you can build the library with CMake. While `lime` depends
on `OpenCV` and `boost python/numpy`, you need to install these libraries beforehands.
Please make sure, the versions of these libraries meet the following requirements.

* OpenCV 3.0 or higher
* Boost 1.63 or higher and build boost-python and boost-numpy

When you intall Boost libraries, please carefully check that Boost build uses correct Python headers and libraries.
Generally, the Boost installation from source codes would be like following.

.. code-block:: shell

  wget -q https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz
  tar -zxf boost_1_64_0.tar.gz
  cd boost_1_64_0
  ./bootstrap.sh
  ./b2 address-model=64 \
    include=$YOUR_PYTHON_INCLUDE_DIR \
    -d0 --with-python -j2 install .

After install these libraries, you can build and install the python library with following shell script.

.. code-block:: shell

  git clone https://github.com/tatsy/lime.git
  mkdir build && cd build
  cmake -D LIME_BUILD_EXAMPLES=OFF \
        -D LIME_BUILD_TESTS=OFF \
        -D LIME_BUILD_PYTHON_MODULE=ON ..
  make
  cd ..
  sudo python setup.py

If you use the Anaconda, you may have to specify Python include directory and library paths like following.

.. code-block:: shell

  git clone https://github.com/tatsy/lime.git
  mkdir build && cd build
  cmake -D LIME_BUILD_EXAMPLES=OFF \
        -D LIME_BUILD_TESTS=OFF \
        -D LIME_BUILD_PYTHON_MODULE=ON \
        -D PYTHON_INCLUDE_DIR=$YOUR_PYTHON_INCLUDE_DIR \
        -D PYTHON_LIBRARY=$YOUR_PYTHON_LIBRARY ..
  make
  cd ..
  sudo python setup.py

If the installation is failed, you should check whether `lime` binary file appears in 'build/lib' directory
(the name of the binary would be `lime.pyd` (Windows), 'liblime.dylib' (MacOS) or 'lime.so' (Linux)).
