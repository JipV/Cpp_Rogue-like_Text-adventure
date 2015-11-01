#ifndef __ITEM_H__
#define __ITEM_H__

class Item
{
	public:
		Item();
		virtual ~Item();

		virtual std::string getDescription() const;
};

std::ostream& operator<<(std::ostream& os, const Item& obj);

#endif
