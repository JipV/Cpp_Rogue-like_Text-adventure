#include "stdafx.h"
#include "Enemy.h"
#include "Trap.h"
#include "Random.h"

// ReSharper disable once CppPossiblyUninitializedMember
Trap::Trap() : triggered_(false),
level_(-1),
chanceToFind_(100),
directDamage_(0),
damageOverTime_(0),
damageOverTimeTurns_(0),
roomDescription_(""),
damageOverTimeMessage_(""),
damageOverTimeDone_(""),
triggerCommands_({})
{

}

// copy constructor
Trap::Trap(const Trap& rhs) :
	triggered_(false),
	level_(rhs.level_),
	chanceToFind_(rhs.chanceToFind_),
	chanceToTrigger_(rhs.chanceToTrigger_),
	directDamage_(rhs.directDamage_),
	damageOverTime_(rhs.damageOverTime_),
	damageOverTimeTurns_(rhs.damageOverTimeTurns_),
	roomDescription_(rhs.roomDescription_),
	findDescription_(rhs.findDescription_),
	triggerDescription_(rhs.triggerDescription_),
	damageOverTimeMessage_(rhs.damageOverTimeMessage_),
	damageOverTimeDone_(rhs.damageOverTimeDone_),
	triggerCommands_(rhs.triggerCommands_)
{
	// TODO: copy enemies to summon
}

Trap::~Trap()
{
	std::for_each(enemiesToSummon_.begin(), enemiesToSummon_.end(), [](Enemy* e)
	{
		delete e;
	});

	enemiesToSummon_.clear();
}

bool Trap::handleAction(std::string fullCommand, std::vector<std::string> action)
{
	if (triggered_)
		return false;

	if (triggerCommands_.size() == 0)
		return tryTrigger();
	
	//TODO check if fullCommand is in list

	return false;
}

bool Trap::tryTrigger()
{
	if (!triggered_ && Random::getRandomNumber(1,100) <= chanceToTrigger_)
	{
		triggered_ = true;
		std::cout << triggerDescription_ << std::endl;
		std::cout << "(traps doen nog niet echt iets, maar dit is een begin)\n";

		return true;
	}

	return false;
}
