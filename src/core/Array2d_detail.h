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

#ifndef SRC_CORE_ARRAY2D_DETAIL_H_
#define SRC_CORE_ARRAY2D_DETAIL_H_

#include <cstring>

#include "common.hpp"

namespace lime {

    template <class Ty>
    Array2d<Ty>::Array2d()
        : nrows(0)
        , ncols(0)
        , data(0) {
    }

    template <class Ty>
    Array2d<Ty>::Array2d(int rows, int cols)
        : nrows(rows)
        , ncols(cols)
        , data(0) {
        msg_assert(rows > 0 && cols > 0, "The array size must be positive.");
        data = new Ty[rows * cols];
        memset(data, 0, sizeof(Ty)* rows * cols);
    }

    template <class Ty>
    Array2d<Ty>::Array2d(int rows, int cols, const Ty& value)
        : nrows(rows)
        , ncols(cols)
        , data(0) {
        msg_assert(rows > 0 && cols > 0, "The array size must be positive.");
        data = new Ty[rows * cols];

        Ty *s = &data[0];
        Ty *e = &data[rows * cols - 1];
        for (Ty* p = s; p != e; ++p) *p = value;
    }

    template <class Ty>
    Array2d<Ty>::Array2d(const Array2d<Ty>& ary)
        : nrows(ary.nrows)
        , ncols(ary.ncols)
        , data(0) {
        data = new Ty[ary.nrows * ary.ncols];
        memcpy(data, ary.data, sizeof(Ty)* ary.nrows * ary.ncols);
    }

    template <class Ty>
    Array2d<Ty>& Array2d<Ty>::operator=(const Array2d<Ty>& ary) {
        this->nrows = ary.nrows;
        this->ncols = ary.ncols;

        delete[] data;
        data = new Ty[ary.nrows * ary.ncols];
        memcpy(data, ary.data, sizeof(Ty)* ary.nrows * ary.ncols);

        return *this;
    }

    template <class Ty>
    Ty& Array2d<Ty>::operator()(int i, int j) {
        return data[i * ncols + j];
    }

    template <class Ty>
    int Array2d<Ty>::rows() const {
        return nrows;
    }

    template <class Ty>
    int Array2d<Ty>::cols() const {
        return ncols;
    }
}  // namespace lime

#endif  // SRC_CORE_ARRAY2D_DETAIL_H_
