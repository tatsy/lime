/*******************************************************************************
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
*******************************************************************************/

#ifndef EXAMPLES_PENCIL_DRAWING_MATH_UTILS_H_
#define EXAMPLES_PENCIL_DRAWING_MATH_UTILS_H_

inline double fast_exp(double y) {
    double d;
    *(reinterpret_cast<int*>(&d) + 0) = 0;
    *(reinterpret_cast<int*>(&d) + 1) = static_cast<int>(1512775 * y + 1072632447);
    return d;
}

inline double gauss(double x, double sigma) {
    double ratio = -(x * x) / sigma;
    return fast_exp(ratio);
}

#endif /* EXAMPLES_PENCIL_DRAWING_MATH_UTILS_H_ */
