#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_MORPHOLOGY_DETAIL_H_
#define _NPR_MORPHOLOGY_DETAIL_H_

#include <algorithm>

namespace lime {

namespace {

void morphErode(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width = img.cols;
    const int height = img.rows;
    const int dim = img.channels();

    out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
    for (int c = 0; c < dim; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float val = 1.0f;
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        if (dx * dx + dy * dy > ksize * ksize) continue;
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double dist = dx * dx + dy * dy;
                            float u = img.at<float>(yy, xx*dim + c);
                            val = std::min(val, u);
                        }
                    }
                }
                out.at<float>(y, x*dim + c) = val;
            }
        }
    }
}

void morphDilate(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    int width = img.cols;
    int height = img.rows;
    int dim = img.channels();
    cv::Mat temp = img;

    out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
    for (int c = 0; c < dim; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float val = 0.0f;
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        if (dx * dx + dy * dy > ksize * ksize) continue;
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double dist = dx * dx + dy * dy;
                            float u = temp.at<float>(yy, xx*dim + c);
                            val = std::max(val, u);
                        }
                    }
                }
                out.at<float>(y, x*dim + c) = val;
            }
        }
    }
}

void morphOpen(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat temp = img;
    morphErode(temp, out, ksize);
    temp = out;
    morphDilate(temp, out, ksize);
}

void morphClose(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat temp = img;
    morphDilate(temp, out, ksize);
    temp = out;
    morphErode(temp, out, ksize);
}

void morphGradient(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat erode, dilate;
    morphErode(img, erode, ksize);
    morphDilate(img, dilate, ksize);
    cv::subtract(dilate, erode, out);
}

void morphTophat(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat opening;
    morphOpen(img, opening, ksize);
    cv::subtract(img, opening, out);
}

void morphBlackhat(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat closing;
    morphClose(img, closing, ksize);
    cv::subtract(closing, img, out);
}

}  // unnamed namespace

void morphFilter(cv::InputArray input, cv::OutputArray output, int type, int ksize) {
    switch (type) {
    case MORPH_ERODE:
        morphErode(input, output, ksize);
        break;

    case MORPH_DILATE:
        morphDilate(input, output, ksize);
        break;

    case MORPH_OPEN:
        morphOpen(input, output, ksize);
        break;

    case MORPH_CLOSE:
        morphClose(input, output, ksize);
        break;

    case MORPH_GRADIENT:
        morphGradient(input, output, ksize);
        break;

    case MORPH_TOPHAT:
        morphTophat(input, output, ksize);
        break;

    case MORPH_BLACKHAT:
        morphBlackhat(input, output, ksize);
        break;

    default:
        ErrorMsg("Unknown morphological filter type!!");
    }
}

}  // namespace lime

#endif  // _NPR_MORPHOLOGY_DETAIL_H_
