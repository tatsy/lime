/******************************************************************************
Copyright 2015 Tatsuya Yatagawa (tatsy)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef SRC_NPR_TENSOR_HPP_
#define SRC_NPR_TENSOR_HPP_

// disable C4351 warning
// class member array is initialized with prescripted values
#pragma warning(disable: 4351)

#include <iostream>
#include <string>
#include <utility>
#include <cmath>

namespace lime {

namespace npr {

class Tensor {
 private:
    static const int N = 4;
    double data[4];

 public:
    Tensor() : data() {
        for (int i = 0; i < N; i++) data[i] = 0.0;
    }

    Tensor(double v11, double v12, double v21, double v22)
        : data() {
        data[0] = v11;
        data[1] = v12;
        data[2] = v21;
        data[3] = v22;
    }

    Tensor(const Tensor& t)
        : data() {
        for (int i = 0; i < N; i++) data[i] = t.data[i];
    }

    Tensor& operator=(const Tensor& t) {
        for (int i = 0; i < N; i++) data[i] = t.data[i];
        return *this;
    }

    Tensor operator+(Tensor t) const {
        Tensor s;
        for (int i = 0; i < N; i++) s.data[i] = data[i] + t.data[i];
        return s;
    }

    Tensor operator-(Tensor t) const {
        Tensor s;
        for (int i = 0; i < N; i++) s.data[i] = data[i] - t.data[i];
        return s;
    }

    Tensor operator*(double d) const {
        Tensor s;
        for (int i = 0; i < N; i++) s.data[i] = data[i] * d;
        return s;
    }

    Tensor operator*(Tensor t) const {
        Tensor s;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                s.data[(i << 1) + j] = 0.0;
                for (int k = 0; k < 2; k++) {
                    s.data[(i << 1) + j] += data[(i << 1) + k] * t.data[(k << 1) + k];
                }
            }
        }
        return s;
    }

    Tensor operator/(double d) const {
        Tensor s;
        for (int i = 0; i < N; i++) s.data[i] = data[i] / d;
        return s;
    }

    static Tensor rotate(Tensor t, double theta) {
        double cs = cos(theta);
        double sn = sin(theta);
        Tensor r(cs, -sn, sn, cs);
        return r * t;
    }

    static Tensor flip(Tensor t, double theta) {
        double cs = cos(theta);
        double sn = sin(theta);
        Tensor f(cs, -sn, -sn, -cs);
        return f * t;
    }

    double& elem(int i, int j) {
        try {
            if (i < 0 || j < 0 || i >= 2 || j >= 2) {
                throw "Exception: invalid element indices";
            }
        }
        catch (std::string msg) {
            std::cout << msg << std::endl;
            return data[0];
        }
        return data[(i << 1) + j];
    }

    double det() const {
        return data[0] * data[3] - data[1] * data[2];
    }

    double norm() const {
        double ret = 0.0;
        for (int i = 0; i < N; i++) {
            ret += data[i] * data[i];
        }
        return sqrt(ret);
    }

    std::pair<double, double> eigen() const {
        double p = -(data[0] + data[3]);
        double q = det();
        double D = sqrt(p * p - 4.0 * q);
        return std::make_pair((-p + D) / 2.0, (-p - D) / 2.0);
    }

    std::string to_string() {
        std::stringstream ss;
        ss << "[ " << data[0] << ", " << data[1] << " ]," << std::endl;
        ss << "[ " << data[2] << ", " << data[3] << " ]";
        return ss.str();
    }
};  // class Tensor

}  // namespace npr

}  // namespace lime

#endif  // SRC_NPR_TESNOR_HPP_
