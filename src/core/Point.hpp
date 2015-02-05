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

#ifndef SRC_CORE_POINT_H_
#define SRC_CORE_POINT_H_

#include <opencv2/opencv.hpp>

namespace lime {

inline int sign(double d) {
    static const double eps = 1.0e-8;
    if (d < -eps) return -1;
    if (d >  eps) return 1;
    return 0;
}

template <class T>
class Point_ : public cv::Point_<T> {
 public:
    // * default constructor
    Point_() : cv::Point_<T>() {}

    // * constructor
    Point_(T x, T y)
        : cv::Point_<T>(x, y) {}

    // * destructor
    ~Point_() {}

    // * copy constructor
    Point_(const Point_& p)
        : cv::Point_<T>(p.x, p.y) {}

    // * operator =
    Point_<T>& operator=(const Point_& p) {
        this->x = p.x;
        this->y = p.y;
        return *this;
    }

    // * operator +
    Point_<T> operator+(Point_ p) const {
        Point_ q;
        q.x = this->x + p.x;
        q.y = this->y + p.y;
        return q;
    }

    // * operator -
    Point_<T> operator-(Point_ p) const {
        Point_ q;
        q.x = this->x - p.x;
        q.y = this->y - p.y;
        return q;
    }

    Point_<T> operator-() const {
        return Point(-this->x, -this->y);
    }

    // * dot product
    T dot(const Point_& p) const {
        return this->x * p.x + this->y * p.y;
    }

    // * determinant
    T det(const Point_& p) const {
        return this->x * p.y - this->y * p.x;
    }

    // * operator *
    Point_<T> operator*(T t) const {
        Point_ q;
        q.x = this->x * t;
        q.y = this->y * t;
        return q;
    }

    // * operator /
    Point_<T> operator/(T t) const {
        Point_ q;
        q.x = this->x / t;
        q.y = this->y / t;
        return q;
    }

    // * operator <
    bool operator<(const Point_& p) const {
        if (this->x != p.x) return this->x < p.x;
        return this->y < p.y;
    }

    // * operator >
    bool operator>(const Point_& p) const {
        if (this->x != p.x) return this->x > p.x;
        return this->y > p.y;
    }

    // * norm
    double norm() const {
        return hypot(this->x, this->y);
    }

    // * normalize
    Point_<T> normalize() const {
        double l = this->norm();
        return Point_(this->x / l, this->y / l);
    }

    // * scaleing
    Point_<T> scale(double d) {
        return Point_(this->x*d, this->y*d);
    }
}; /* class Point_ */

typedef Point_<int> Point;
typedef Point_<int> Point2i;
typedef Point_<float> Point2f;
typedef Point_<double> Point2d;

} /* namespace lime */

#endif /* _LIME_POINT_H_ */
