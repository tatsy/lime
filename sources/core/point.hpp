#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_POINT_HPP_
#define _CORE_POINT_HPP_

#include <type_traits>

#include <opencv2/opencv.hpp>

#include "common.hpp"

template <class T>
using is_arith_t = typename std::enable_if<std::is_arithmetic<T>::value>::type;

namespace lime {

    inline int sign(double d) {
        if (d < -EPS) return -1;
        if (d >  EPS) return  1;
        return 0;
    }

    template <class T, class Enable = void>
    class Point_;

    template <class T>
    class Point_<T, is_arith_t<T> > {
    public:
        T x{0};
        T y{0};

    public:
        /** Default constructor.
         */
        Point_() {
        }

        /** The Point constructor.
         */
        Point_(T x, T y)
            : x{x}
            , y{y} {
        }

        /** The Point destructor.
         */
        ~Point_() {
        }

        /** The Point constructor (copy).
         */
        Point_(const Point_& p)
            : x{p.x}
            , y{p.y} {
        }

        /** Assignment operator.
         */
        Point_<T>& operator=(const Point_& p) {
            this->x = p.x;
            this->y = p.y;
            return *this;
        }

        /** Plus operator.
         */
        Point_<T> operator+=(const Point_& p) {
            this->x += p.x;
            this->y += p.y;
            return *this;
        }

        /** Minus operator.
         */
        Point_<T> operator-=(const Point_& p) {
            this->x -= p.x;
            this->y -= p.y;
            return *this;
        }

        /** Negation operator.
         */
        Point_<T> operator-() const {
            return { -x, -y };
        }

        /** Dot product.
         */
        T dot(const Point_& p) const {
            return x * p.x + y * p.y;
        }

        /** Determinant.
         */
        T det(const Point_& p) const {
            return x * p.y - y * p.x;
        }

        /** Multiplication operator.
         */
        Point_<T> operator*=(T t) {
            this->x *= t;
            this->y *= t;
            return *this;
        }

        /** Division operator.
         */
        Point_<T> operator/=(T t) {
            Assertion(t != T{0}, "Zero division");
            this->x /= t;
            this->y /= t;
            return *this;
        }

        /** Comparation operator ==.
        */
        bool operator==(const Point_& p) const {
            return x == p.x && y == p.y;
        }

        /** Comparation operator !=.
         */
        bool operator!=(const Point_& p) const {
            return !this->operator==(p);
        }

        /** Comparation operator <.
         */
        bool operator<(const Point_& p) const {
            if (x != p.x) return x < p.x;
            return y < p.y;
        }

        /** Comparation operator >.
         */
        bool operator>(const Point_& p) const {
            if (x != p.x) return x > p.x;
            return y > p.y;
        }

        /** Norm.
         */
        double norm() const {
            return hypot(x, y);
        }

        /** Normalization.
         */
        Point_<T> normalize() const {
            const double l = this->norm();
            Assertion(l != 0.0, "Zero division");
            return { x / l, y / l };
        }

        /** Convert to cv::Point_<T>.
         */
        operator cv::Point_<T>() const {
            return { x, y };
        }

    };  // class Point_

    using Point   = Point_<int>;
    using Point2i = Point_<int>;
    using Point2f = Point_<float>;
    using Point2d = Point_<double>;

}  // namespace lime

template <class T>
lime::Point_<T, is_arith_t<T> >
operator+(const lime::Point_<T, is_arith_t<T> >& p1,
          const lime::Point_<T, is_arith_t<T> >& p2) {
    auto retval = p1;
    retval += p2;
    return retval;
}

template <class T>
lime::Point_<T, is_arith_t<T> >
operator-(const lime::Point_<T, is_arith_t<T> >& p1,
          const lime::Point_<T, is_arith_t<T> >& p2) {
    auto retval = p1;
    retval -= p2;
    return retval;
}

template <class T>
lime::Point_<T, is_arith_t<T> >
operator*(const lime::Point_<T, is_arith_t<T> >& p, T t) {
    auto retval = p;
    retval *= t;
    return retval;
}

template <class T>
lime::Point_<T, is_arith_t<T> >
operator/(const lime::Point_<T, is_arith_t<T> >& p, T t) {
    auto retval = p;
    retval /= t;
    return retval;
}

#endif  // _CORE_POINT_HPP_
