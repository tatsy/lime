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

While building the source codes with CMake, you may ought to specify your
python include and library directories.

If the installation is failed, you should check whether `lime` binary file appears in 'build/lib' directory
(the name of the binary would be `lime.pyd` (Windows), 'liblime.dylib' (MacOS) or 'lime.so' (Linux)).
