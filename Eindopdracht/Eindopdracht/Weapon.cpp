#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(std::string type, std::string material, int level, int attack)
{
	type_ = type;
	material_ = material;
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

std::string Weapon::getDescription() const
{
	return "een " + type_ + " van " + material_;
}
