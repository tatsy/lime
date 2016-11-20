#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_SINGULARITY_HPP_
#define _NPR_SINGULARITY_HPP_

// type of singularity
enum {
    SINGULAR_WEDGE = 0x01,
    SINGULAR_TRISECTOR,
    SINGULAR_SADDLE,
    SINGULAR_NODE,
    SINGULAR_CENTER,
    SINGULAR_HIGHER_ORDER
};

namespace lime {

/**
 * Singularity point of the vector field.
 * @ingroup npr
 **/
class SingularPoint {
 public:
    int x, y;        // position
    double theta;    // rotating angle
    int type;        // singularity types

    SingularPoint()
        : x(0), y(0), theta(0.0), type(0) {}

    SingularPoint(int x_, int y_, double theta_, int type_)
        : x(x_), y(y_), theta(theta_), type(type_) {}

    SingularPoint(const SingularPoint& sp)
        : x(sp.x), y(sp.y), theta(sp.theta), type(sp.type) {}

    SingularPoint& operator=(const SingularPoint& sp) {
        x = sp.x;
        y = sp.y;
        theta = sp.theta;
        type = sp.type;
        return *this;
    }
};  // class SingularPoint

}  // namespace lime

#endif  // _NPR_SINGULARITY_HPP_
