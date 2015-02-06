#!/bin/sh
find include src tests examples -regex ".*\.c\|.*\.cpp\|.*\.h\|.*\.hpp" | xargs python cpplint.py --extensions=h,hpp,c,cpp
