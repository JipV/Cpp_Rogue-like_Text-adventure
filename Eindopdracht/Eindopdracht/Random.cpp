#include "stdafx.h"
#include "Random.h"

std::random_device dev;
std::default_random_engine dre{ dev() };

Random::Random()
{
}

Random::~Random()
{
}

int Random::getRandomNumber(int min, int max)
{
	std::uniform_int_distribution<int> randomNumber{ min, max };
	return randomNumber(dre);
}