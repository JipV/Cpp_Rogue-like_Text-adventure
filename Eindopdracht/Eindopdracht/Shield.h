#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "Item.h"

class Shield : public Item
{
	public:

		Shield(std::string size, int level, int defence);
		~Shield();

		virtual std::string getDescription() const override;

	private:

		std::string size_;
		int level_;
		int defence_;
};

#endif
