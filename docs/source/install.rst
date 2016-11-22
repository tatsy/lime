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

For Python users, you can build the library with CMake. After building it,
you can install it normally with ``setup.py``. Following is the simple
installation steps.

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
