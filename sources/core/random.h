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

#ifndef SRC_CORE_RANDOM_H_
#define SRC_CORE_RANDOM_H_

namespace lime {

/*
 * A singleton class for generating random numbers
 */
class Random {
 public:
    static Random& getRNG(int seed = -1);

 private:
    explicit Random(int seed = -1);
    Random& operator=(const Random& rand);

 public:
    /* Generate a random integer from [0, n-1]
        */
    int randInt(const int n) const;

    /* Generate a floating point random number from [0, 1)
        */
    double randReal() const;

    /* Genrate a random number from a normal distribution with mean = 0 and STD = 1 */
    double randNorm() const;

};  // class Random

}  // namespace lime

#include "Random_detail.h"

#endif  // SRC_CORE_RANDOM_H_ */
