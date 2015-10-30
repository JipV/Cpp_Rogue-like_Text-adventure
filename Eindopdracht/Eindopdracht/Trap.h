#ifndef __TRAP_H__
#define __TRAP_H__

class Enemy;

class Trap
{
friend class TrapGenerator;
	public:
		Trap();
		~Trap();

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

	Enemy* enemyToSummon_;
};

#endif
