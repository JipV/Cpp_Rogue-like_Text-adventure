#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "string";

class Enemy
{
	public:
		Enemy();
		~Enemy();

	private:
		std::string description_;
		int level_;
		int hp_;
};

#endif

