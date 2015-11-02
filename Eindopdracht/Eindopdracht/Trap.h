#ifndef __TRAP_H__
#define __TRAP_H__

class Room;
class Enemy;
class Hero;

class Trap
{
	friend class TrapGenerator;
public:
	Trap();
	Trap(const Trap& rhs);
	~Trap();

	bool handleAction(std::string fullCommand, std::vector<std::string> action, Hero* hero, Room* room);
	bool searchRoom(int perception);
	void showRoomDescription();
private:
	bool triggered_;

	int level_,
		chanceToFind_,
		chanceToTrigger_,
		directDamage_,
		damageOverTime_,
		damageOverTimeTurns_;

	std::string roomDescription_,
		findDescription_,
		triggerDescription_,
		damageOverTimeMessage_,
		damageOverTimeDone_;

	std::vector<std::string> triggerCommands_;
	std::vector<Enemy*> enemiesToSummon_;

	bool tryTrigger(Hero* hero, Room* room);
};

#endif
