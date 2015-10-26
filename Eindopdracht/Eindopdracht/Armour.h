#ifndef __ARMOUR_H__
#define __ARMOUR_H__

class Armour
{
	public:

		enum ARMOUR_TYPE {
			Light,
			Medium,
			Heavy
		};

		enum ARMOUR_MATERIAL {
		};

		Armour();
		~Armour();

	private:

		ARMOUR_TYPE type_;
		ARMOUR_MATERIAL material_;
		int defenseBonus_;
		int attackBonus_;
		int minimumLevel_;
};

#endif
