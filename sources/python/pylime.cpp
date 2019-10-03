#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <tuple>

#include "pylime.h"

// -----------------------------------------------------------------------------
// External module registration methods
// -----------------------------------------------------------------------------

void init_metrics(py::module m);
void init_misc(py::module m);
void init_npr(py::module m);

// -----------------------------------------------------------------------------
// Utility methods
// -----------------------------------------------------------------------------

cv::Mat np2mat(const py::array_t<float> &arr) {
    py::buffer_info buf_info = arr.request();
    const int rows = buf_info.shape[0];
    const int cols = buf_info.shape[1];
    const int channels = buf_info.ndim == 2 ? 1 : buf_info.shape[2];

    cv::Mat mat(cv::Size(cols, rows), CV_MAKE_TYPE(CV_32F, channels), buf_info.ptr);
    
    return std::move(mat);
}

py::array_t<float> mat2np(const cv::Mat &mat) {
    const int channels = mat.channels();
    const int dims = channels == 1 ? 2 : 3;
    const int rows = mat.rows;
    const int cols = mat.cols;
    if (mat.depth() != CV_32F) {
        throw std::runtime_error("Internal error: cv::Mat must be CV_32F type.");
    }

    const auto shape = (dims == 2) ? py::detail::any_container<py::ssize_t>{rows, cols}
                                   : py::detail::any_container<py::ssize_t>{rows, cols, channels};
    const auto stride = (dims == 2) ? py::detail::any_container<py::ssize_t>{cols * sizeof(float), sizeof(float)}
                                    : py::detail::any_container<py::ssize_t>{cols * channels * sizeof(float), channels * sizeof(float), sizeof(float)};
    py::buffer_info buf_info(mat.data, sizeof(float), py::format_descriptor<float>::format(), dims, shape, stride);

    return py::array_t<float>(buf_info);
}

inline void py_print_verson() {
    lime::print_version();
}

// -----------------------------------------------------------------------------
// Module registration
// -----------------------------------------------------------------------------

PYBIND11_MODULE(pylime, m) {
    m.def("print_version", &py_print_verson, "Print version of \"lime\".");
    init_metrics(m);
    init_misc(m);
    init_npr(m);
}
