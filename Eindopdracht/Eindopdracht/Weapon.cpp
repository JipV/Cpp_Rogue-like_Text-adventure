#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(std::string type, int level, int attack)
{
	type_ = type;
	level_ = level;
	attack_ = attack;
}

Weapon::~Weapon()
{
}

std::string Weapon::getType() {
	return type_;
}

int Weapon::getAttack() {
	return attack_;
}
