#ifndef _LIME_ARRAY_2D_H_
#define _LIME_ARRAY_2D_H_

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

		Ty& operator()(int i, int j);

		int cols() const;

		int rows() const;

	}; /* class Array2d */

} /* namespace npr */

#include "Array2d_detail.h"

#endif /* _LIME_ARRAY_2D_H_ */
