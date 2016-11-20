#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_QUEUE_H_
#define _CORE_RANDOM_QUEUE_H_

namespace lime {

/**
 * Random queue.
 * @ingroup core
 **/
template <class T>
class random_queue {
public:
    //! Constructor.
    random_queue();

    //! Destructor.
    virtual ~random_queue();

    //! Push value to the queue.
    void push(const T &value);

    //! Return a random value form the queue and remove it.
    T pop();

    //! Check if the queue is empty or not.
    bool empty() const;

    //! Returns # of elements in the queue.
    int size() const;

private:
    static const int init_len = 2048;
    int ptr;
    int len;
    T *data;

};  // class random_queue

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "random_queue_detail.h"
#endif

#endif  // _CORE_RANDOM_QUEUE_H_
