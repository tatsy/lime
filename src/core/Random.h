#ifndef _LIME_RANDOM_H_
#define _LIME_RANDOM_H_

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

	}; /* end of class Random */

} /* namespace lime */

#include "Random_detail.h"

#endif /* end of _LIME_RANDOM_H_ */
