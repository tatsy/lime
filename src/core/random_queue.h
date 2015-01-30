#ifndef _RANDOM_QUEUE_H_
#define _RANDOM_QUEUE_H_

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
		

	}; /* class random_queue */

} /* namespace lime */

#include "random_queue_detail.h"

#endif /* _RANDOM_QUEUE_H_ */
