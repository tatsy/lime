#ifndef _RANDOM_QUEUE_DETAIL_H_
#define _RANDOM_QUEUE_DETAIL_H_

#include <cstring>

#include "Random.h"

namespace lime {

	template <class Ty>
	random_queue<Ty>::random_queue()
		: ptr(0)
		, len(init_len)
		, data(new Ty[len])
	{
	}

	template <class Ty>
	random_queue<Ty>::~random_queue()
	{
		delete[] data;
	}

	template <class Ty>
	void random_queue<Ty>::push(Ty value)
	{
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
		int i = rand.randInt((int)ptr);
		ptr--;
		swap(data[i], data[ptr]);
		return data[ptr];
	}

	template <class Ty>
	bool random_queue<Ty>::empty() const
	{
		return ptr == 0;
	}

	template <class Ty>
	size_t random_queue<Ty>::size() const
	{
		return ptr;
	}

} /* namespace lime */

#endif /* _RANDOM_QUEUE_DETAIL_H_ */
