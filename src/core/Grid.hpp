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
        : rows(grid.rows)
        , cols(grid.cols)
        , data(grid.rows * grid.cols, std::vector<T>()) {
        dataCopy(grid);
    }

    // * operator =
    Grid& operator=(const Grid& grid) {
        this->rows = grid.rows;
        this->cols = grid.cols;
        this->data = std::vector<std::vector<T> >(this->rows * this->cols, std::vector<T>());
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
    bool isin(int i, int j) const {
        return i >= 0 && j >= 0 && i < rows && j < cols;
    }

    // * push at (i, j)
    void pushAt(int i, int j, T t) {
        data[i*cols + j].push_back(t);
    }

    // * access pointer of (i, j)
    const std::vector<T>* ptrAt(int i, int j) const {
        return &data[i*cols + j];
    }

    // * size of vector at (i, j)
    size_t sizeAt(int i, int j) {
        return data[i*cols + j].size();
    }

 private:
    // * private method to copy grid data
    void dataCopy(const Grid& grid) {
        for (int y = 0; y < this->rows; y++) {
            for (int x = 0; x < this->cols; x++) {
                int index = y*this->cols + x;
                std::vector<T>* p_src = &grid.data[index];
                std::vector<T>* p_dst = &data[index];
                *p_dst = std::vector<T>(p_src->size());
                std::copy(p_src->begin(), p_src->end(), p_dst->begin());
            }
        }
    }
};  // class Grid

}  // namespace lime

#endif  // SRC_CORE_GRID_HPP_
