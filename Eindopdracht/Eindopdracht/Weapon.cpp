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

std::string Weapon::getDescription() const
{
	return "een " + type_;
}
