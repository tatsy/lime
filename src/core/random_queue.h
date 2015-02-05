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

#ifndef SRC_CORE_RANDOM_QUEUE_H_
#define SRC_CORE_RANDOM_QUEUE_H_

namespace lime {

template <class Ty>
class random_queue {
 private:
    static const int init_len = 2048;
    size_t ptr;
    size_t len;
    Ty* data;

 public:
    random_queue();
    ~random_queue();

    void push(Ty value);

    /* Return a random value form the queue and remove it 
     */
    Ty pop();

    bool empty() const;

    size_t size() const;
};  // class random_queue

}  // namespace lime

#include "../core/random_queue_detail.h"

#endif  // SRC_CORE_RANDOM_QUEUE_H_
