#include "stdafx.h"
#include "Map.h"

#include "iostream"

int main()
{
	Map map = Map(10, 3, 3);
	map.showMap();

	std::cin.get(); // Console blijft open

	return 0;
}