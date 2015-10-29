#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "random"

class Random
{
public:

	Random();
	~Random();

	static int getRandomNumber(int min, int max);
};

#endif

