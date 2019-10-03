#ifndef _MSC_VER
#pragma once
#endif

#ifndef _PYTHON_PYLIME_H_
#define _PYTHON_PYLIME_H_

#include <opencv2/opencv.hpp>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "lime.hpp"

namespace py = pybind11;

//! Convert numpy.ndarray to OpenCV Mat.
cv::Mat np2mat(const py::array_t<float> &arr);

//! Convert OpenCV Mat to numpy.ndarray.
py::array_t<float> mat2np(const cv::Mat &mat);

#endif  // _PYTHON_PYLIME_H_
