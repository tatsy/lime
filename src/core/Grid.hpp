#ifndef _LIME_GRID_H_
#define _LIME_GRID_H_

#include <vector>

namespace lime {

	template <class T>
	class Grid {
	private:
		int rows;
		int cols;
		std::vector<std::vector<T> > data;

	public:
		// * default constructor
		Grid() : data() {}

		// * constructor
		Grid(int r, int c)
			: rows(r), cols(c), data(r * c, std::vector<T>()) {}

		// * destructor
		virtual ~Grid() {}

		// * copy constructor
		Grid(const Grid& grid)
			: rows(grid.rows), cols(grid.cols), data(grid.rows * grid.cols, vector<T>())
		{
			dataCopy(grid);
		}

		// * operator =
		Grid& operator=(Grid& grid) {
			this->rows = grid.rows;
			this->cols = grid.cols;
			this->data = vector<vector<T> >(this->rows * this->cols, vector<T>());
			dataCopy(grid);
			return *this;
		}

		// * get number of rows
		int nrows() const {
			return this->rows;
		}

		// * get number of cols
		int ncols() const {
			return this->cols;
		}

		// * check (i, j) is in the grid range
		bool isin(int i, int j) {
			return i >= 0 && j >= 0 && i < rows && j < cols;
		}

		// * push at (i, j)
		void pushAt(int i, int j, T t) {
			data[i*cols + j].push_back(t);
		}

		// * access pointer of (i, j)
		std::vector<T>* ptrAt(int i, int j) {
			return &data[i*cols + j];
		}

		// * size of vector at (i, j)
		size_t sizeAt(int i, int j) {
			return data[i*cols + j].size();
		}


	private:
		// * private method to copy grid data
		void dataCopy(Grid& grid) {
			for (int y = 0; y < this->rows; y++) {
				for (int x = 0; x < this->cols; x++) {
					int index = y*this->cols + x;
					vector<T>* p_src = &grid.data[index];
					vector<T>* p_dst = &data[index];
					*p_dst = vector<T>(p_src->size());
					std::copy(p_src->begin(), p_src->end(), p_dst->begin());
				}
			}
		}

	}; /* class Grid */

} /* namespace lime */

#endif /* _LIME_GRID_H_ */
