#ifndef _LIME_POINT_H_
#define _LIME_POINT_H_

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
		Point_& operator=(const Point_& p) {
			this->x = p.x;
			this->y = p.y;
			return *this;
		}

		// * operator +
		Point_ operator+(Point_ p) const {
			Point_ q;
			q.x = this->x + p.x;
			q.y = this->y + p.y;
			return q;
		}

		// * operator -
		Point_ operator-(Point_ p) const {
			Point_ q;
			q.x = this->x - p.x;
			q.y = this->y - p.y;
			return q;
		}

		Point_ operator-() const {
			return Point(-x, -y);
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
		Point_ operator*(T t) const {
			Point_ q;
			q.x = this->x * t;
			q.y = this->y * t;
			return q;
		}

		// * operator /
		Point_ operator/(T t) const {
			Point_ q;
			q.x = this->x / t;
			q.y = this->y / t;
			return q;
		}

		// * operator <
		bool operator<(const Point_& p) const {
			if (x != p.x) return x < p.x;
			return y < p.y;
		}

		// * operator >
		bool operator>(const Point_& p) const {
			if (x != p.x) return x > p.x;
			return y > p.y;
		}

		// * norm
		double norm() const {
			return hypot(this->x, this->y);
		}

		// * normalize
		Point_<T> normalize() const {
			double l = this->norm();
			return Point_(x / l, y / l);
		}

		// * scaleing
		Point_<T> scale(double d) {
			return Point_(x*d, y*d);
		}

	}; /* class Point_ */

	typedef Point_<int> Point;
	typedef Point_<int> Point2i;
	typedef Point_<float> Point2f;
	typedef Point_<double> Point2d;

} /* namespace lime */

#endif /* _LIME_POINT_H_ */
