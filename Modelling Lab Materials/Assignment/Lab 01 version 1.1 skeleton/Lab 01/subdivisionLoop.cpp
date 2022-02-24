#ifndef subdivisionLoop_h
#define subdivisionLoop_h

#include <stdio.h>
#include "subdivisionLoop.h"
#endif /* subdivisionLoop_h */

namespace subdivisionLoop
{
	/// <summary>
	/// A function to check if two numbers are almost equal using the machine epsilon
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="x">First number</param>
	/// <param name="y">Second number</param>
	/// <param name="limit"></param>
	/// <returns></returns>
	template <class T>
	typename std::enable_if<!std::numeric_limits<T>::isInteger, bool>::type approxEqual(T x, T y, int ulp)
	{	
		// the machine epsilon is scaled to the magnitude of the values used
		// and multipled with the desired precision in the ulps (units in the last place)
		return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
			// unless the result is subnormal
			|| std::abs(x - y) < std::numeric_limits<T>::min();
	}

}