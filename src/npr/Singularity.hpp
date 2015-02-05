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

#ifndef SRC_NPR_SINGULARITY_HPP_
#define SRC_NPR_SINGULARITY_HPP_

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

namespace npr {

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

}  // namespace npr

}  // namespace lime

#endif  // SRC_NPR_SINGULARITY_HPP_
