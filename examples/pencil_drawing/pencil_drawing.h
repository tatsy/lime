#pragma once
#ifndef _NPR_PENCIL_DRAWING_H_
#define _NPR_PENCIL_DRAWING_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include <opencv2/opencv.hpp>
#include "../../include/lime.hpp"

/* Line integral convolution */
void LIConv(cv::Mat& lic, cv::Mat& img, cv::Mat& etf, cv::Mat& noise, double ratio); 

/* Generate pencil drawing */
void pencilDrawing(cv::Mat& img, cv::Mat& out, std:: vector<cv::Point2f>& points);

/* Generate pencil drawing in LOD basis */
void pencilDrawingLOD(cv::Mat& img, cv::Mat& out, std::vector<cv::Point2f>& points, int level);

#endif
