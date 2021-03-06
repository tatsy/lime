#include "pylime.h"

py::array_t<float> py_randomNoise(const std::tuple<int, int> &size) {
    const int w = std::get<0>(size);
    const int h = std::get<1>(size);
    cv::Mat ret;
    lime::randomNoise(ret, cv::Size(w, h));

    return mat2np(ret);
}

py::array_t<float> py_perlinNoise(const std::tuple<int, int> &size, int level) {
    const int w = std::get<0>(size);
    const int h = std::get<1>(size);
    cv::Mat ret;
    lime::perlinNoise(ret, cv::Size(w, h), level);
    return mat2np(ret);
}

py::array_t<float> py_kuwaharaFilter(const py::array_t<float> &img, int type, int ksize, int nDivide) {
    cv::Mat ret;
    lime::kuwaharaFilter(np2mat(img), ret, type, ksize, nDivide);
    return mat2np(ret);
}

py::array_t<float> py_morphFilter(const py::array_t<float> &img, int type, int ksize) {
    cv::Mat ret;
    lime::morphFilter(np2mat(img), ret, type, ksize);
    return mat2np(ret);
}

py::array_t<float> py_pdeFilter(const py::array_t<float> &img, int type, double lambda, int maxiter) {
    cv::Mat ret;
    lime::pdeFilter(np2mat(img), ret, type, lambda, maxiter);
    return mat2np(ret);
}

py::array_t<float> py_calcVectorField(const py::array_t<float> &img, int ksize, int vfieldType, int edgeDetector) {
    cv::Mat ret;
    lime::calcVectorField(np2mat(img), ret, ksize, vfieldType, edgeDetector);
    return mat2np(ret);
}

py::array_t<float> py_edgeDoG(const py::array_t<float> &img, const py::dict &pdict) {
    lime::DoGParams params;
    if (pdict.contains("kappa")) params.kappa = pdict["kappa"].cast<float>();
    if (pdict.contains("sigma")) params.sigma = pdict["sigma"].cast<float>();
    if (pdict.contains("tau")) params.tau = pdict["tau"].cast<float>();
    if (pdict.contains("phi")) params.phi = pdict["phi"].cast<float>();
    if (pdict.contains("edgeType")) {
        params.edgeType = (lime::NPREdgeType)pdict["edgeType"].cast<int>();
    }

    cv::Mat ret;
    lime::edgeDoG(np2mat(img), ret, params);

    return mat2np(ret);
}

py::array_t<float> py_LIC(const py::array_t<float> &img, const py::array_t<float> &tangent, int L, int type) {
    cv::Mat ret;
    lime::LIC(np2mat(img), ret, np2mat(tangent), L, type);
    return mat2np(ret);
}

py::list py_poissonDisk(const py::array_t<float> &gray, const std::vector<std::tuple<double, double>> &points,
                        int pdsMethod, double minRadius, double maxRadius) {
    std::vector<cv::Point2f> samples;
    const int nPoints = points.size();
    for (int i = 0; i < nPoints; i++) {
        const double a = std::get<0>(points[i]);
        const double b = std::get<1>(points[i]);
        samples.push_back(cv::Point2f((float)a, (float)b));
    }

    lime::poissonDisk(np2mat(gray), &samples, (lime::PDSMethod)pdsMethod, minRadius, maxRadius);

    py::list output;
    for (int i = 0; i < samples.size(); i++) {
        py::tuple t = py::make_tuple((double)samples[i].x, (double)samples[i].y);
        output.append(t);
    }

    return output;
}

void init_npr(py::module m) {
    m.def("randomNoise", &py_randomNoise, "Generate random noise", py::arg("size"));
    m.def("perlinNoise", &py_perlinNoise, "Generate perlin noise", py::arg("size"), py::arg("level"));

    m.attr("KUWAHARA_CLASSICAL") = (int)lime::KUWAHARA_CLASSICAL;
    m.attr("KUWAHARA_GENERALIZED") = (int)lime::KUWAHARA_GENERALIZED;
    m.attr("KUWAHARA_ANISOTROPIC") = (int)lime::KUWAHARA_ANISOTROPIC;

    m.def("kuwaharaFilter", &py_kuwaharaFilter, "Kuwahara filter",
          py::arg("src"), py::arg("type"), py::arg("ksize"), py::arg("ndivide") = 0);

    m.attr("MORPH_ERODE") = (int)lime::MORPH_ERODE;
    m.attr("MORPH_DILATE") = (int)lime::MORPH_DILATE;
    m.attr("MORPH_OPEN") = (int)lime::MORPH_OPEN;
    m.attr("MORPH_CLOSE") = (int)lime::MORPH_CLOSE;
    m.attr("MORPH_GRADIENT") = (int)lime::MORPH_GRADIENT;
    m.attr("MORPH_TOPHAT") = (int)lime::MORPH_TOPHAT;
    m.attr("MORPH_BLACKHAT") = (int)lime::MORPH_BLACKHAT;

    m.def("morphFilter", &py_morphFilter, "Filtering with mathematical morphology");

    m.attr("PDE_ANISO_DIFFUSION") = (int)lime::PDE_ANISO_DIFFUSION;
    m.attr("PDE_SHOCK_FILTER") = (int)lime::PDE_SHOCK_FILTER;
    m.attr("PDE_MEAN_CURVATURE") = (int)lime::PDE_MEAN_CURVATURE;

    m.def("pdeFilter", &py_pdeFilter, "Filtering based on partial differential equations");

    m.attr("VEC_FIELD_SST") = (int)lime::VEC_FIELD_SST;
    m.attr("VEC_FIELD_ETF") = (int)lime::VEC_FIELD_ETF;
    m.attr("EDGE_DETECT_SOBEL") = (int)lime::EDGE_DETECT_SOBEL;
    m.attr("EDGE_DETECT_ROTATIONAL") = (int)lime::EDGE_DETECT_ROTATIONAL;

    m.def("calcVectorField", &py_calcVectorField, "Compute vector fields based on image edges",
          py::arg("img"), py::arg("ksize"), py::arg("method") = (int)lime::VEC_FIELD_SST, py::arg("edge_type") = (int)lime::EDGE_DETECT_SOBEL);

    m.attr("NPR_EDGE_XDOG") = (int)lime::NPR_EDGE_XDOG;
    m.attr("NPR_EDGE_FDOG") = (int)lime::NPR_EDGE_FDOG;

    m.def("edgeDoG", &py_edgeDoG, "Detect edges using difference of Gaussians",
          py::arg("img"), py::arg("params"));

    m.attr("LIC_CLASSICAL") = (int)lime::LIC_CLASSICAL;
    m.attr("LIC_EULERIAN") = (int)lime::LIC_EULERIAN;
    m.attr("LIC_RUNGE_KUTTA") = (int)lime::LIC_RUNGE_KUTTA;

    m.def("LIC", &py_LIC, "Line integral convolution",
          py::arg("img"), py::arg("tangent"), py::arg("L"), py::arg("type") = (int)lime::LIC_EULERIAN);

    m.attr("PDS_RAND_QUEUE") = (int)lime::PDS_RAND_QUEUE;
    m.attr("PDS_FAST_PARALLEL") = (int)lime::PDS_FAST_PARALLEL;

    m.def("poissonDisk", &py_poissonDisk, "Poisson disk sampling",
          py::arg("gray"), py::arg("samples"), py::arg("method") = (int)lime::PDS_FAST_PARALLEL,
          py::arg("min_radius") = 2.0, py::arg("max_radius") = 5.0);
}