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

#ifndef SRC_CORE_GRID_HPP_
#define SRC_CORE_GRID_HPP_

#include <vector>
#include <algorithm>

namespace lime {

template <class T>
class Grid {
 private:
    int nrows;
    int ncols;
    std::vector<std::vector<T> > data;

 public:
    // * default constructor
     Grid()
         : nrows(0)
         , ncols(0)
         , data() {
     }

    // * constructor
    Grid(int rows, int cols)
        : nrows(rows)
        , ncols(cols)
        , data(rows * cols, std::vector<T>()) {
        msg_assert(rows > 0 && cols > 0, "Size must be positive");
    }

    // * destructor
    virtual ~Grid() {}

    // * copy constructor
    Grid(const Grid& grid)
        : nrows(grid.nrows)
        , ncols(grid.ncols)
        , data(grid.nrows * grid.ncols, std::vector<T>()) {
        dataCopy(grid);
    }

    // * operator =
    Grid& operator=(const Grid& grid) {
        this->nrows = grid.nrows;
        this->ncols = grid.ncols;
        this->data = std::vector<std::vector<T> >(this->nrows * this->ncols, std::vector<T>());
        dataCopy(grid);
        return *this;
    }

    // Resize grid
    void resize(int rows, int cols) {
        msg_assert(rows > 0 && cols > 0, "Size must be positive");
        this->nrows = rows;
        this->ncols = cols;
        data.resize(rows * cols);
    }

    // * get number of rows
    int rows() const {
        return this->nrows;
    }

    // * get number of cols
    int cols() const {
        return this->ncols;
    }

    // * check (i, j) is in the grid range
    bool hasCell(int i, int j) const {
        return i >= 0 && j >= 0 && i < nrows && j < ncols;
    }

    // * push at (i, j)
    void pushAt(int i, int j, T t) {
        data[i * ncols + j].push_back(t);
    }

    // * access vector of (i, j)
    const std::vector<T>& operator()(int i, int j) const {
        msg_assert(i >= 0 && j >= 0 && i < nrows && j < ncols,
                   "Index out of bounds");
        return data[i * ncols + j];
    }

    // * size of vector at (i, j)
    size_t sizeAt(int i, int j) {
        return data[i*ncols + j].size();
    }

 private:
    // * private method to copy grid data
    void dataCopy(const Grid& grid) {
        for (int y = 0; y < this->nrows; y++) {
            for (int x = 0; x < this->ncols; x++) {
                int index = y*this->ncols + x;
                const std::vector<T>& p_src = grid.data[index];
                std::vector<T>& p_dst = data[index];
                p_dst.resize(p_src.size());
                std::copy(p_src.begin(), p_src.end(), p_dst.begin());
            }
        }
    }
};  // class Grid

}  // namespace lime

#endif  // SRC_CORE_GRID_HPP_
