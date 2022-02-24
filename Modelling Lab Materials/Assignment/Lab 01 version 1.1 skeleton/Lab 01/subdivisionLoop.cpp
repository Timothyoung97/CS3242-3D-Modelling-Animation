#ifndef subdivisionLoop_h
#define subdivisionLoop_h

#include <stdio.h>
#include "subdivisionLoop.h"
#endif /* subdivisionLoop_h */

namespace subdivisionLoop
{

	template <class T>
	typename std::enable_if<!std::numeric_limits<T>::isInteger, bool>::type approxEqual(T x, T y, int ulp) 
	{
		
	}

}