#define BOOST_PYTHON_STATIC_LIB
#include <set>

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

inline void py_print_verson() {
    lime::print_version();
}

// -----------------------------------------------------------------------------
// NPR module
// -----------------------------------------------------------------------------

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

npy::ndarray py_kuwaharaFilter(const npy::ndarray &img, int type, int ksize, int nDivide = 0) {
    cv::Mat input;
    np2mat(img, input);

    cv::Mat ret;
    lime::kuwaharaFilter(input, ret, type, ksize, nDivide);

    return mat2np(ret);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(ol_py_kuwaharaFilter, py_kuwaharaFilter, 3, 4);

npy::ndarray py_morphFilter(const npy::ndarray &img, int type, int ksize) {
    cv::Mat input;
    np2mat(img, input);

    cv::Mat ret;
    lime::morphFilter(input, ret, type, ksize);

    return mat2np(ret);
}

npy::ndarray py_pdeFilter(const npy::ndarray &img, int type, double lambda, int maxiter) {
    cv::Mat input;
    np2mat(img, input);

    cv::Mat ret;
    lime::pdeFilter(input, ret, type, lambda, maxiter);

    return mat2np(ret);
}

npy::ndarray py_calcVectorField(const npy::ndarray &img, int ksize,
                                int vfieldType = lime::VEC_FIELD_SST,
                                int edgeDetector = lime::EDGE_DETECT_SOBEL) {
    cv::Mat input;
    np2mat(img, input);

    cv::Mat ret;
    lime::calcVectorField(input, ret, ksize, vfieldType, edgeDetector);

    return mat2np(ret);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(ol_py_calcVectorField, py_calcVectorField, 2, 4);


npy::ndarray py_edgeDoG(const npy::ndarray &img, const py::dict &pdict) {
    cv::Mat input;
    np2mat(img, input);

    std::set<std::string> keys;
    int entries = py::len(pdict);
    for (int i = 0; i < entries; i++) {
        keys.insert(py::extract<std::string>(pdict.keys()[i]));
    }

    lime::DoGParams params;
    if (keys.find("kappa") != keys.end()) params.kappa = py::extract<double>(pdict["kappa"]);
    if (keys.find("sigma") != keys.end()) params.sigma = py::extract<double>(pdict["sigma"]);
    if (keys.find("tau")  != keys.end()) params.tau = py::extract<double>(pdict["tau"]);
    if (keys.find("phi")  != keys.end()) params.phi = py::extract<double>(pdict["phi"]);
    if (keys.find("edgeType") != keys.end()) {
        int value = py::extract<int>(pdict["edgeType"]);
        params.edgeType = (lime::NPREdgeType)value;
    }

    cv::Mat ret;
    lime::edgeDoG(input, ret, params);

    return mat2np(ret);
}

npy::ndarray py_LIC(const npy::ndarray &img, const npy::ndarray &tangent,
                    int L, int type = lime::LIC_EULERIAN) {
    cv::Mat input, tangmat;
    np2mat(img, input);
    np2mat(tangent, tangmat);

    cv::Mat ret;
    lime::LIC(input, ret, tangmat, L, type);

    return mat2np(ret);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(ol_py_LIC, py_LIC, 3, 4);


py::list py_poissonDisk(const npy::ndarray &gray, const py::list &points,
                        int pdsMethod = (int)lime::PDS_FAST_PARALLEL,
                        double minRadius = 2.0, double maxRadius = 5.0) {
    cv::Mat input;
    np2mat(gray, input);

    std::vector<cv::Point2f> samples;
    int nPoints = py::len(points);
    for (int i = 0; i < nPoints; i++) {
        double a = py::extract<double>(points[i][0]);
        double b = py::extract<double>(points[i][1]);
        samples.push_back(cv::Point2f((float)a, (float)b));
    }

    lime::poissonDisk(input, &samples, (lime::PDSMethod)pdsMethod, minRadius, maxRadius);

    py::list output;
    for (int i = 0; i < samples.size(); i++) {
        py::tuple t = py::make_tuple((double)samples[i].x, (double)samples[i].y);
        output.append(t);
    }

    return output;
}

BOOST_PYTHON_FUNCTION_OVERLOADS(ol_py_poissonDisk, py_poissonDisk, 2, 5);

// -----------------------------------------------------------------------------
// Misc module
// -----------------------------------------------------------------------------

npy::ndarray py_colorConstancy(const npy::ndarray &img, int type) {
    cv::Mat input;
    np2mat(img, input);

    cv::Mat ret;
    lime::colorConstancy(input, ret, type);

    return mat2np(ret);
}

// -----------------------------------------------------------------------------
// Module registration.
// -----------------------------------------------------------------------------

BOOST_PYTHON_MODULE(lime) {
    npy::initialize();

    // Exception translator.
    py::register_exception_translator<PyLimeException>(&translate);

    // NPR methods.
    py::def("randomNoise", py_randomNoise);
    py::def("perlinNoise", py_perlinNoise);

    py::scope().attr("KUWAHARA_CLASSICAL") = (int)lime::KUWAHARA_CLASSICAL;
    py::scope().attr("KUWAHARA_GENERALIZED") = (int)lime::KUWAHARA_GENERALIZED;
    py::scope().attr("KUWAHARA_ANISOTROPIC") = (int)lime::KUWAHARA_ANISOTROPIC;

    py::def("kuwaharaFilter", py_kuwaharaFilter, ol_py_kuwaharaFilter(
        py::args("src", "ftype", "ksize", "ndivide")));

    py::scope().attr("MORPH_ERODE") = (int)lime::MORPH_ERODE;
    py::scope().attr("MORPH_DILATE") = (int)lime::MORPH_DILATE;
    py::scope().attr("MORPH_OPEN") = (int)lime::MORPH_OPEN;
    py::scope().attr("MORPH_CLOSE") = (int)lime::MORPH_CLOSE;
    py::scope().attr("MORPH_GRADIENT") = (int)lime::MORPH_GRADIENT;
    py::scope().attr("MORPH_TOPHAT") = (int)lime::MORPH_TOPHAT;
    py::scope().attr("MORPH_BLACKHAT") = (int)lime::MORPH_BLACKHAT;

    py::def("morphFilter", py_morphFilter);

    py::scope().attr("PDE_ANISO_DIFFUSION") = (int)lime::PDE_ANISO_DIFFUSION;
    py::scope().attr("PDE_SHOCK_FILTER") = (int)lime::PDE_SHOCK_FILTER;
    py::scope().attr("PDE_MEAN_CURVATURE") = (int)lime::PDE_MEAN_CURVATURE;

    py::def("pdeFilter", py_pdeFilter);

    py::scope().attr("VEC_FIELD_SST") = (int)lime::VEC_FIELD_SST;
    py::scope().attr("VEC_FIELD_ETF") = (int)lime::VEC_FIELD_ETF;
    py::scope().attr("EDGE_DETECT_SOBEL") = (int)lime::EDGE_DETECT_SOBEL;
    py::scope().attr("EDGE_DETECT_ROTATIONAL") = (int)lime::EDGE_DETECT_ROTATIONAL;

    py::def("calcVectorField", py_calcVectorField, ol_py_calcVectorField(
        py::args("img", "ksize", "vftype", "edtype")));

    py::scope().attr("NPR_EDGE_XDOG") = (int)lime::NPR_EDGE_XDOG;
    py::scope().attr("NPR_EDGE_FDOG") = (int)lime::NPR_EDGE_FDOG;

    py::def("edgeDoG", py_edgeDoG);

    py::scope().attr("LIC_CLASSICAL") = (int)lime::LIC_CLASSICAL;
    py::scope().attr("LIC_EULERIAN") = (int)lime::LIC_EULERIAN;
    py::scope().attr("LIC_RUNGE_KUTTA") = (int)lime::LIC_RUNGE_KUTTA;

    py::def("LIC", py_LIC, ol_py_LIC(
        py::args("img", "tangent", "L", "ftype")));

    py::scope().attr("PDS_RAND_QUEUE") = (int)lime::PDS_RAND_QUEUE;
    py::scope().attr("PDS_FAST_PARALLEL") = (int)lime::PDS_FAST_PARALLEL;

    py::def("poissonDisk", py_poissonDisk, ol_py_poissonDisk(
        py::args("gray", "samples", "method", "min_radius", "max_radius")));

    // Misc methods.
    py::scope().attr("CONSTANCY_HORN") = (int)lime::CONSTANCY_HORN;
    py::scope().attr("CONSTANCY_RAHMAN") = (int)lime::CONSTANCY_RAHMAN;
    py::scope().attr("CONSTANCY_FAUGERAS") = (int)lime::CONSTANCY_FAUGERAS;

    py::def("colorConstancy", py_colorConstancy);
}
