#ifndef __TRAP_H__
#define __TRAP_H__

class Trap
{
	public:

		enum TRAP_TYPE {
			Trapdoor,
			Snare,
			BearTrap,
			ArrowTrap,
			SpikeTrap
		};

		Trap();
		~Trap();

private:

	TRAP_TYPE type_;
	int automaticDiscovery_;
	int discoverWithSearch_;
	int riskOfFire_;
	int damage_;
	int fix_;

	std::string descriptionLook_;
	std::string	descriptionTigger_;

};

#endif
