#ifndef _LIME_ARRAY_2D_DETAIL_H_
#define _LIME_ARRAY_2D_DETAIL_H_

#include <cstring>

#include "common.hpp"

namespace lime {

	template <class Ty>
	Array2d<Ty>::Array2d()
		: nrows(0)
		, ncols(0)
		, data(0)
	{
	}

	template <class Ty>
	Array2d<Ty>::Array2d(int rows, int cols)
		: nrows(rows)
		, ncols(cols)
		, data(0)
	{
		msg_assert(rows > 0 && cols > 0, "The array size must be positive.");
		data = new Ty[rows * cols];
		memset(data, 0, sizeof(Ty)* rows * cols);
	}

	template <class Ty>
	Array2d<Ty>::Array2d(int rows, int cols, const Ty& value)
		: nrows(rows)
		, ncols(cols)
		, data(0)
	{
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
		, data(0)
	{
		data = new Ty[ary.nrows * ary.ncols];
		memcpy(data, ary.data, sizeof(Ty)* ary.nrows * ary.ncols);
	}

	template <class Ty>
	Array2d<Ty>& Array2d<Ty>::operator=(const Array2d<Ty>& ary)
	{
		this->nrows = ary.nrows;
		this->ncols = ary.ncols;

		delete[] data;
		data = new Ty[ary.nrows * ary.ncols];
		memcpy(data, ary.data, sizeof(Ty)* ary.nrows * ary.ncols);

		return *this;
	}

	template <class Ty>
	Ty& Array2d<Ty>::operator()(int i, int j)
	{
		return data[i * ncols + j];
	}

	template <class Ty>
	int Array2d<Ty>::rows() const
	{
		return nrows;
	}

	template <class Ty>
	int Array2d<Ty>::cols() const
	{
		return ncols;
	}

} /* namespace lime */

#endif /* _LIME_ARRAY_2D_DETAIL_H_ */
