#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "Item.h"

class Shield : public Item
{
	public:

		Shield(std::string size, std::string material, int level, int defence);
		~Shield();

		std::string getSize();
		std::string getMaterial();
		int getDefence();

	private:

		std::string size_;
		std::string material_;
		int level_;
		int defence_;
};

#endif
