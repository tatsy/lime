#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_GRID_HPP_
#define _CORE_GRID_HPP_

#include <vector>
#include <algorithm>

namespace lime {

    template <class T>
    class LIME_EXPORTS Grid {
    private:
        int _rows;
        int _cols;
        std::vector<std::vector<T> > _data;

    public:
        /** The Grid constructor.
         */
         Grid()
             : _rows{0}
             , _cols{0}
             , _data{} {
         }

        /** The Grid constructor.
         */
        Grid(int rows, int cols)
            : _rows{rows}
            , _cols{cols}
            , _data{rows * cols, std::vector<T>{}} {
            Assertion(rows > 0 && cols > 0, "Size must be positive!!");
        }

        /** The Grid destructor.
         */
        virtual ~Grid() {
        }

        /** The Grid constructor (copy).
         */
        Grid(const Grid& grid)
            : _rows{grid._rows}
            , _cols{grid._cols}
            , _data{grid._rows * grid._cols, std::vector<T>{}} {
            dataCopy(grid);
        }

        /** Assignment operator.
         */
        Grid& operator=(const Grid& grid) {
            this->_rows = grid._rows;
            this->_cols = grid._cols;
            this->_data = std::vector<std::vector<T> >(this->_rows * this->_cols, std::vector<T>{});
            dataCopy(grid);
            return *this;
        }

        /** Resize grid.
         */
        void resize(int rows, int cols) {
            Assertion(rows > 0 && cols > 0, "Size must be positive");
            this->_rows = rows;
            this->_cols = cols;
            this->_data.resize(rows * cols);
        }

        /** Get number of rows.
         */
        int rows() const {
            return this->_rows;
        }

        /** Get number of cols.
         */
        int cols() const {
            return this->_cols;
        }

        /** Check (i, j) is in the grid range
         */
        bool hasCell(int i, int j) const {
            return i >= 0 && j >= 0 && i < _rows && j < _cols;
        }

        /** Push at (i, j).
         */
        void pushAt(int i, int j, const T& t) {
            Assertion(i >= 0 && j >= 0 && i < _rows && j < _cols, 
                      "Cell index out of bounds!!");
            _data[i * _cols + j].push_back(t);
        }

        /** Access vector at (i, j)
         */
        const std::vector<T>& operator()(int i, int j) const {
            Assertion(i >= 0 && j >= 0 && i < _rows && j < _cols,
                      "Cell index out of bounds!!");
            return _data[i * _cols + j];
        }

        /** Size of vector at (i, j)
         */
        decltype(auto) sizeAt(int i, int j) {
            return _data[i * _cols + j].size();
        }

    private:
        /** Copy grid data.
         */
        void dataCopy(const Grid& grid) {
            for (int y = 0; y < _rows; y++) {
                for (int x = 0; x < _cols; x++) {
                    const int index = y * _cols + x;
                    const std::vector<T>& p_src = grid._data[index];
                    std::vector<T>& p_dst = _data[index];
                    p_dst.resize(p_src.size());
                    std::copy(p_src.begin(), p_src.end(), p_dst.begin());
                }
            }
        }

    };  // class Grid

}  // namespace lime

#endif  // _CORE_GRID_HPP_
