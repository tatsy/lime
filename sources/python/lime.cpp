#define BOOST_PYTHON_STATIC_LIB
#include <opencv2/opencv.hpp>

#include "pyutils.h"
#include "lime.hpp"


void translate(const PyLimeException &ex) {
    PyErr_SetString(PyExc_RuntimeError, ex.what());
}

inline void np2mat(const npy::ndarray &arr, cv::Mat &mat) {
    const int dims = arr.get_nd();
    Assertion(dims == 2 || dims == 3,
              "Input numpy::ndarray is invalid!!");
    
    const int rows = (int)arr.shape(0);
    const int cols = (int)arr.shape(1);
    const int chls = dims == 2 ? 1 : (int)arr.shape(2);
    
    mat = cv::Mat(rows, cols, CV_MAKETYPE(CV_32F, chls));
    if (arr.get_dtype() == npy::dtype::get_builtin<uint8_t>()) {
        uint8_t *data = (uint8_t *)arr.get_data();
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                for (int c = 0; c < chls; c++) {
                    mat.at<float>(y, x * chls + c) =
                    (float)data[(y * cols + x) * chls + c] / 255.0;
                }
            }
        }
    } else if (arr.get_dtype() == npy::dtype::get_builtin<double>()) {
        double *data = (double *)arr.get_data();
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                for (int c = 0; c < chls; c++) {
                    mat.at<float>(y, x * chls + c) = (float)data[(y * cols + x) * chls + c];
                }
            }
        }
    } else if (arr.get_dtype() == npy::dtype::get_builtin<float>()) {
        float *data = (float *)arr.get_data();
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                for (int c = 0; c < chls; c++) {
                    mat.at<float>(y, x * chls + c) = data[(y * cols + x) * chls + c];
                }
            }
        }
    } else {
        ErrorMsg("Input ndarray must be either of uint8, float32 or float64 type.");
    }
}

inline npy::ndarray mat2np(const cv::Mat &mat) {
    const int chls = mat.channels();
    const int dims = chls == 1 ? 2 : 3;
    const int rows = mat.rows;
    const int cols = mat.cols;
    Assertion(mat.depth() == CV_32F, "Internal error: cv::Mat must be CV_32F type.");
    
    py::tuple shape = dims == 2 ? py::make_tuple(rows, cols)
    : py::make_tuple(rows, cols, chls);
    npy::ndarray arr = npy::zeros(shape, npy::dtype::get_builtin<float>());
    float *data = (float *)arr.get_data();
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int c = 0; c < chls; c++) {
                data[(y * cols + x) * chls + c] = mat.at<float>(y, x * chls + c);
            }
        }
    }
    
    return arr;
}

// -----------------------------------------------------------------------------
// NPR module
// -----------------------------------------------------------------------------

npy::ndarray py_colorConstancy(const npy::ndarray &img, int type) {
    cv::Mat input;
    np2mat(img, input);

    cv::Mat ret;
    lime::colorConstancy(input, ret, type);
    
    return mat2np(ret);
}

npy::ndarray py_randomNoise(const py::tuple &size) {
    const int w = py::extract<int>(size[0]);
    const int h = py::extract<int>(size[1]);
    cv::Mat ret;
    lime::randomNoise(ret, cv::Size(w, h));
    
    return mat2np(ret);
}

npy::ndarray py_perlinNoise(const py::tuple &size, int level) {
    const int w = py::extract<int>(size[0]);
    const int h = py::extract<int>(size[1]);
    cv::Mat ret;
    lime::perlinNoise(ret, cv::Size(w, h), level);

    return mat2np(ret);
}

// -----------------------------------------------------------------------------
// Module registration.
// -----------------------------------------------------------------------------

BOOST_PYTHON_MODULE(lime) {
    npy::initialize();
    
    // Exception translator
    py::register_exception_translator<PyLimeException>(&translate);
    
    // Methods
    py::scope().attr("CONSTANCY_HORN") = (int)lime::CONSTANCY_HORN;
    py::scope().attr("CONSTANCY_RAHMAN") = (int)lime::CONSTANCY_RAHMAN;
    py::scope().attr("CONSTANCY_FAUREGAS") = (int)lime::CONSTANCY_FAUGERAS;
    
    py::def("colorConstancy", py_colorConstancy);
    py::def("randomNoise", py_randomNoise);
    py::def("perlinNoise", py_perlinNoise);
}
