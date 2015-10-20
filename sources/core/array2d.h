#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_ARRAY2D_H_
#define _CORE_ARRAY2D_H_

#include <memory>

namespace lime {

    template <class T>
    class Array2d {
    private:
        int _rows, _cols;
        std::unique_ptr<T[]> _data;

    public:
        /** The Array2d constructor.
         */
        Array2d();

        /** The Array2d constructor.
         */
        Array2d(int rows, int cols);

        /** Allocate memory space and fill it with the specified value
         *  @param[in] rows: Number of rows
         *  @param[in] cols: Number of columns
         *  @param[in] value: A value with which the array is filled
         */
        Array2d(int rows, int cols, const T& val);

        /** The Array2d constructor (copy).
         */
        Array2d(const Array2d& ary);

        /** Assignment operator.
         */
        Array2d& operator=(const Array2d& ary);

        T& operator()(int i, int j) const;

        int cols() const;
        int rows() const;

    };  // class Array2d

}  // namespace lime

#include "Array2d_detail.h"

#endif  // _CORE_ARRAY2D_H_
