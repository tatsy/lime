#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_POINT_HPP_
#define _CORE_POINT_HPP_

#include <opencv2/opencv.hpp>

#include "common.h"

namespace lime {

/**
 * Take sign of the value.
 * @ingroup core
 **/
inline int sign(double d) {
    if (d < -EPS) return -1;
    if (d >  EPS) return  1;
    return 0;
}

/**
 * The 2D point class.
 * @ingroup core_module
 **/
template <class T>
class Point_ {
public:
    //! The Point constructor.
    explicit Point_(T x_ = T{0}, T y_ = T{0})
        : x{x_}
        , y{y_} {
    }

    //! The Point destructor.
    ~Point_() {
    }

    //! The Point constructor (copy).
    Point_(const Point_ &p)
        : x{p.x}
        , y{p.y} {
    }

    //! Assignment operator.
    Point_<T> & operator=(const Point_ &p) {
        this->x = p.x;
        this->y = p.y;
        return *this;
    }

    Point_<T> & operator+=(const Point_ &p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }

    Point_<T> & operator-=(const Point_ &p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }

    Point_<T> operator-() const {
        return { -x, -y };
    }

    Point_<T> & operator*=(T t) {
        this->x *= t;
        this->y *= t;
        return *this;
    }

    Point_<T> & operator/=(T t) {
        Assertion(t != T{0}, "Zero division");
        this->x /= t;
        this->y /= t;
        return *this;
    }

    //! Dot product.
    T dot(const Point_ &p) const {
        return x * p.x + y * p.y;
    }

    //! Determinant.
    T det(const Point_& p) const {
        return x * p.y - y * p.x;
    }

    bool operator==(const Point_ &p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point_ &p) const {
        return !this->operator==(p);
    }

    bool operator<(const Point_ &p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }

    bool operator>(const Point_ &p) const {
        if (x != p.x) return x > p.x;
        return y > p.y;
    }

    //! Norm.
    double norm() const {
        return std::hypot(x, y);
    }

    //! Normalization.
    Point_<T> normalize() const {
        const double l = this->norm();
        Assertion(l != 0.0, "Zero vector cannot be normalized!!");
        return Point_<T>(x / l, y / l);
    }

    //! Implicit convertion to cv::Point_<T>.
    operator cv::Point_<T>() const {
        return { x, y };
    }

public:
    T x{0};
    T y{0};

    static_assert(std::is_arithmetic<T>::value,
                "Template type must be arithmetic!!");

};  // class Point_

using Point   = Point_<int>;
using Point2i = Point_<int>;
using Point2f = Point_<float>;
using Point2d = Point_<double>;

}  // namespace lime

template <class T>
lime::Point_<T>
operator+(const lime::Point_<T>& p1,
          const lime::Point_<T>& p2) {
    auto retval = p1;
    retval += p2;
    return retval;
}

template <class T>
lime::Point_<T>
operator-(const lime::Point_<T>& p1,
          const lime::Point_<T>& p2) {
    auto retval = p1;
    retval -= p2;
    return retval;
}

template <class T>
lime::Point_<T>
operator*(const lime::Point_<T>& p, T t) {
    auto retval = p;
    retval *= t;
    return retval;
}

template <class T>
lime::Point_<T>
operator/(const lime::Point_<T>& p, T t) {
    auto retval = p;
    retval /= t;
    return retval;
}

#endif  // _CORE_POINT_HPP_
