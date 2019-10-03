#include "pylime.h"

template <typename T>
double py_MSE(const py::array_t<T>& I, const py::array_t<T>& J) {
    return lime::MSE(np2mat(I), np2mat(J));
}

template <typename T>
double py_RMSE(const py::array_t<T>& I, const py::array_t<T>& J) {
    return lime::RMSE(np2mat(I), np2mat(J));
}

template <typename T>
double py_PSNR(const py::array_t<T>& I, const py::array_t<T>& J) {
    return lime::PSNR(np2mat(I), np2mat(J));
}

template <typename T>
double py_SSIM(const py::array_t<T>& I, const py::array_t<T>& J) {
    return lime::SSIM(np2mat(I), np2mat(J));
}

void init_metrics(py::module m) {
    m.def("MSE", &py_MSE<uint8_t>, "Mean squared error (MSE)");
    m.def("MSE", &py_MSE<float>, "Mean squared error (MSE)");
    m.def("RMSE", &py_MSE<uint8_t>, "Root mean squared error (RMSE)");
    m.def("RMSE", &py_MSE<float>, "Root mean squared error (RMSE)");
    m.def("PSNR", &py_MSE<uint8_t>, "Peak signal-to-noise ratio (PSNR)");
    m.def("PSNR", &py_MSE<float>, "Peak signal-to-noise ratio (PSNR)");
    m.def("SSIM", &py_MSE<uint8_t>, "Structural similarity (SSIM)");
    m.def("SSIM", &py_MSE<float>, "Structural similarity (SSIM)");
}
