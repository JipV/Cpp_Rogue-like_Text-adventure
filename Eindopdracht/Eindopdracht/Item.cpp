#include "stdafx.h"
#include "Item.h"

Item::Item()
{
}

Item::~Item()
{
}

std::string Item::getDescription() const
{
	return "een onbekend voorwerp";
}

std::ostream& operator<<(std::ostream& os, const Item& obj)
{
	os << obj.getDescription();
	return os;
}