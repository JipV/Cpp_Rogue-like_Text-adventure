#ifndef __WEAPON_H__
#define __WEAPON_H__

class Weapon
{
	public:

		enum WEAPON_TYPE {
			Sword,
			Poleaxe,
			Dagger,
			Club,
			Hammer
		};

		enum WEAPON_MATERIAL {
			Wood,
			Iron,
			Steel,
			Diamond
		}; 

		Weapon();
		~Weapon();

	private:

		WEAPON_TYPE type_;
		WEAPON_MATERIAL material_;
		int damage_;
		int attackBonus_;
		int minimumLevel_;
};

#endif
