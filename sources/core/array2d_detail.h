#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_ARRAY2D_DETAIL_H_
#define _CORE_ARRAY2D_DETAIL_H_

#include <cstring>
#include <algorithm>

#include "common.h"

namespace lime {

template <class T>
Array2D<T>::Array2D()
    : rows_{0}
    , cols_{0}
    , data_{} {
}

template <class T>
Array2D<T>::Array2D(int rows, int cols)
    : rows_{rows}
    , cols_{cols}
    , data_{} {
    Assertion(rows > 0 && cols > 0, "The array size must be positive.");
    data_ = std::make_unique<T[]>(rows * cols);
    memset(data_.get(), 0, sizeof(T)* rows * cols);
}

template <class T>
Array2D<T>::Array2D(int rows, int cols, const T& value)
    : rows_{rows}
    , cols_{cols}
    , data_{} {
    Assertion(rows > 0 && cols > 0, "The array size must be positive.");
    data_ = std::make_unique<T[]>(rows * cols);
    std::fill(data_, data_ + (rows * cols), value);
}

template <class T>
Array2D<T>::Array2D(const Array2D<T>& ary)
    : Array2D{} {
    this->operator=(ary);
}

template <class T>
Array2D<T>::Array2D(Array2D<T> &&ary)
    : Array2D{} {
    this->operator=(std::move(ary));
}

template <class T>
Array2D<T> & Array2D<T>::operator=(const Array2D<T> &ary) {
    this->rows_ = ary.rows_;
    this->cols_ = ary.cols_;

    data_.reset();
    data_ = std::make_unique<T[]>(ary.rows_ * ary.cols_);
    std::copy(ary.data_.get(), ary.data_.get() + (rows_ * cols_), data_.get());

    return *this;
}

template <class T>
Array2D<T> & Array2D<T>::operator=(Array2D<T> &&ary) {
    this->rows_ = ary.rows_;
    this->cols_ = ary.cols_;
    this->data_ = std::move(ary.data_);
    return *this;
}

template <class T>
T& Array2D<T>::operator()(int i, int j) const {
    Assertion(i >= 0 && j >= 0 && i < rows_ && j < cols_,
             "Array index out of bounds!!");
    return data_[i * cols_ + j];
}

template <class T>
int Array2D<T>::rows() const {
    return rows_;
}

template <class T>
int Array2D<T>::cols() const {
    return cols_;
}

}  // namespace lime

#endif  // _CORE_ARRAY2D_DETAIL_H_
