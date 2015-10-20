#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_ARRAY2D_DETAIL_H_
#define _CORE_ARRAY2D_DETAIL_H_

#include <cstring>
#include <algorithm>

#include "common.hpp"

namespace lime {

    template <class T>
    Array2d<T>::Array2d()
        : _rows{0}
        , _cols{0}
        , _data{} {
    }

    template <class T>
    Array2d<T>::Array2d(int rows, int cols)
        : _rows{rows}
        , _cols{cols}
        , _data{} {
        Assertion(rows > 0 && cols > 0, "The array size must be positive.");
        data = std::make_unique<T[]>(rows * cols);
        memset(data, 0, sizeof(T)* rows * cols);
    }

    template <class T>
    Array2d<T>::Array2d(int rows, int cols, const T& value)
        : _rows{rows}
        , _cols{cols}
        , _data{} {
        Assertion(rows > 0 && cols > 0, "The array size must be positive.");
        data = std::make_unique<T[]>(rows * cols);
        std::fill(data, data + (rows * cols), value);
    }

    template <class T>
    Array2d<T>::Array2d(const Array2d<T>& ary)
        : _rows{ary.nrows}
        , _cols{ary.ncols}
        , _data{} {
        _data = std::make_unique<T[]>(_rows * _cols);
        std::copy(ary._data, ary._data + (_rows * _cols), _data);
    }

    template <class T>
    Array2d<T>& Array2d<T>::operator=(const Array2d<T>& ary) {
        this->_rows = ary._rows;
        this->_cols = ary._cols;

        _data.reset();
        _data = std::make_unique<T[]>(ary.nrows * ary.ncols);
        std::copy(ary._data, ary._data + (_rows * _cols), _data);

        return *this;
    }

    template <class T>
    T& Array2d<T>::operator()(int i, int j) const {
        Assertion(i >= 0 && j >= 0 && i < nrows && j < ncols,
                  "Array index out of bounds!!");
        return data[i * ncols + j];
    }

    template <class T>
    int Array2d<T>::rows() const {
        return _rows;
    }

    template <class T>
    int Array2d<T>::cols() const {
        return _cols;
    }

}  // namespace lime

#endif  // _CORE_ARRAY2D_DETAIL_H_
