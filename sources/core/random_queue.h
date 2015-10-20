#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_QUEUE_H_
#define _CORE_RANDOM_QUEUE_H_

namespace lime {

    template <class T>
    class random_queue {
     private:
        static const int init_len = 2048;
        int ptr;
        int len;
        T* data;

     public:
        random_queue();
        ~random_queue();

        void push(const T& value);

        /** Return a random value form the queue and remove it.
         */
        T pop();

        bool empty() const;

        int size() const;

    };  // class random_queue

}  // namespace lime

#include "../core/random_queue_detail.h"

#endif  // _CORE_RANDOM_QUEUE_H_
