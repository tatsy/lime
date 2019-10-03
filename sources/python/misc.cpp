#include "pylime.h"

py::array_t<float> py_colorConstancy(const py::array_t<float> &img, int type) {
    cv::Mat ret;
    lime::colorConstancy(np2mat(img), ret, type);

    return mat2np(ret);
}

void init_misc(py::module m) {
    m.attr("CONSTANCY_HORN") = (int)lime::CONSTANCY_HORN;
    m.attr("CONSTANCY_RAHMAN") = (int)lime::CONSTANCY_RAHMAN;
    m.attr("CONSTANCY_FAUGERAS") = (int)lime::CONSTANCY_FAUGERAS;

    m.def("colorConstancy", &py_colorConstancy, "Color constancy");
}
