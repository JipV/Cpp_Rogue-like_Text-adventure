#include "stdafx.h"
#include "Shield.h"

Shield::Shield(std::string size, std::string material, int level, int defence)
{
	size_ = size;
	material_ = material;
	level_ = level;
	defence_ = defence;
}

Shield::~Shield()
{
}

std::string Shield::getSize() {
	return size_;
}

std::string Shield::getMaterial() {
	return material_;
}

int Shield::getDefence() {
	return defence_;
}