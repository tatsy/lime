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

#ifndef SRC_CORE_RANDOM_QUEUE_DETAIL_H_
#define SRC_CORE_RANDOM_QUEUE_DETAIL_H_

#include <cstring>
#include <algorithm>

#include "Random.h"

namespace lime {

template <class Ty>
random_queue<Ty>::random_queue()
    : ptr(0)
    , len(init_len)
    , data(new Ty[len]) {
}

template <class Ty>
random_queue<Ty>::~random_queue() {
    delete[] data;
}

template <class Ty>
void random_queue<Ty>::push(Ty value) {
    if (ptr == len) {
        Ty* tmp = new Ty[len * 2];
        memcpy(tmp, data, sizeof(Ty) * len);
        delete[] data;
        data = tmp;
        len *= 2;
    }
    data[ptr++] = value;
}

template <class Ty>
Ty random_queue<Ty>::pop() {
    Random rand = Random::getRNG();
    int i = rand.randInt(static_cast<int>(ptr));
    ptr--;
    std::swap(data[i], data[ptr]);
    return data[ptr];
}

template <class Ty>
bool random_queue<Ty>::empty() const {
    return ptr == 0;
}

template <class Ty>
size_t random_queue<Ty>::size() const {
    return ptr;
}

}  // namespace lime

#endif  // SRC_CORE_RANDOM_QUEUE_DETAIL_H_
