#include "stdafx.h"
#include "Shield.h"

Shield::Shield(std::string size, int level, int defence)
{
	size_ = size;
	level_ = level;
	defence_ = defence;
}

Shield::~Shield()
{
}

std::string Shield::getDescription() const
{
	return "een " + size_ + " schild";
}
