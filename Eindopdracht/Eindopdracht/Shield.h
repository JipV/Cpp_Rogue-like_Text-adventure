#ifndef __SHIELD_H__
#define __SHIELD_H__

class Shield
{
	public:

		enum SHIELD_SIZE {
			Large,
			Medium,
			Small
		};

		enum SHIELD_MATERIAL {
			Wood,
			Iron
		};

		Shield();
		~Shield();

	private:

		SHIELD_SIZE size_;
		SHIELD_MATERIAL material_;
		int defenseBonus_;
		int minimumLevel_;
};

#endif
