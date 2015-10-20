#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_QUEUE_DETAIL_H_
#define _CORE_RANDOM_QUEUE_DETAIL_H_

#include <cstring>
#include <algorithm>

#include "random.h"

namespace lime {

    template <class T>
    random_queue<T>::random_queue()
        : ptr(0)
        , len(init_len)
        , data(new T[len]) {
    }

    template <class T>
    random_queue<T>::~random_queue() {
        delete[] data;
    }

    template <class T>
    void random_queue<T>::push(const T& value) {
        if (ptr == len) {
            T* tmp = new T[len * 2];
            memcpy(tmp, data, sizeof(T) * len);
            delete[] data;
            data = tmp;
            len *= 2;
        }
        data[ptr++] = value;
    }

    template <class T>
    T random_queue<T>::pop() {
        Random rand = Random::getRNG();
        int i = rand.randInt(static_cast<int>(ptr));
        ptr--;
        std::swap(data[i], data[ptr]);
        return data[ptr];
    }

    template <class T>
    bool random_queue<T>::empty() const {
        return ptr == 0;
    }

    template <class T>
    int random_queue<T>::size() const {
        return ptr;
    }

}  // namespace lime

#endif  // _CORE_RANDOM_QUEUE_DETAIL_H_
