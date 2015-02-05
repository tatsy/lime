/******************************************************************************
Copyright 2015 Tatsuya Yatagawa (tatsy)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef SRC_CORE_ARRAY2D_H_
#define SRC_CORE_ARRAY2D_H_

namespace lime {

template <class Ty>
class Array2d {
 private:
    int nrows, ncols;
    Ty* data;

 public:
    Array2d();

    Array2d(int rows, int cols);

    /* Allocate memory space and fill it with the specified value
     * @param[in] rows: number of rows
     * @param[in] cols: number of columns
     * @param[in] value: a value with which the array is filled
     */
    Array2d(int rows, int cols, const Ty& val);

    Array2d(const Array2d& ary);

    Array2d& operator=(const Array2d& ary);

    Ty& operator()(int i, int j) const;

    int cols() const;

    int rows() const;
};  // class Array2d

}  // namespace lime

#include "Array2d_detail.h"

#endif  // SRC_CORE_ARRAY2D_H_
