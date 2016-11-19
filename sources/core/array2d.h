#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_ARRAY2D_H_
#define _CORE_ARRAY2D_H_

#include <memory>

namespace lime {

/**
 * 2D array class.
 * @ingroup core_module
 **/

template <class T>
class LIME_EXPORTS Array2D {
public:
    //! The Array2D constructor.
    Array2D();

    //! The Array2D constructor.
    Array2D(int rows, int cols);

    /** Allocate memory space and fill it with the specified value
     *  @param[in] rows: Number of rows
     *  @param[in] cols: Number of columns
     *  @param[in] value: A value with which the array is filled
     **/
    Array2D(int rows, int cols, const T &val);

    //! The Array2D constructor (copy).
    Array2D(const Array2D &ary);
    
    //! The Array2D constructor (move).
    Array2D(Array2D &&ary);

    //! Assignment operator (copy).
    Array2D<T> & operator=(const Array2D &ary);
    
    //! Assignment operator (move).
    Array2D<T> & operator=(Array2D &&ary);

    T & operator()(int i, int j) const;

    int cols() const;
    int rows() const;

private:
    int rows_, cols_;
    std::unique_ptr<T[]> data_;

};  // class Array2D

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "array2d_detail.h"
#endif

#endif  // _CORE_ARRAY2D_H_
