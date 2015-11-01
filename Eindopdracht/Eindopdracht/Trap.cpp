#include "stdafx.h"
#include "Enemy.h"
#include "Trap.h"
#include "Random.h"
#include "Hero.h"
#include "Room.h"

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
triggerCommands_({}),
enemiesToSummon_({})
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
	enemiesToSummon_ = {};

	std::for_each(rhs.enemiesToSummon_.begin(), rhs.enemiesToSummon_.end(), [this](Enemy* e)
	{
		enemiesToSummon_.push_back(new Enemy(*e));
	});
}

Trap::~Trap()
{
	std::for_each(enemiesToSummon_.begin(), enemiesToSummon_.end(), [](Enemy* e)
	{
		delete e;
	});

	enemiesToSummon_.clear();
}

bool Trap::handleAction(std::string fullCommand, std::vector<std::string> action, Hero* hero, Room* room)
{
	if (triggered_)
		return false;

	if (triggerCommands_.empty() ||
		std::find(triggerCommands_.begin(), triggerCommands_.end(), "fullCommand") != triggerCommands_.end())
		return tryTrigger(hero, room);

	return false;
}

bool Trap::searchRoom()
{
	if (!triggered_ && Random::getRandomNumber(1,100) <= chanceToFind_)
	{
		triggered_ = true; // Zorg ervoor dat de val niet meer af gaat
		std::cout << findDescription_ << std::endl;
		return true;
	}

	return false;
}

void Trap::showRoomDescription()
{
	if (!triggered_)
		std::cout << roomDescription_;
}

bool Trap::tryTrigger(Hero* hero, Room* room)
{
	if (!triggered_ && Random::getRandomNumber(1,100) <= chanceToTrigger_)
	{
		triggered_ = true;
		std::cout << triggerDescription_ << std::endl;

		if (directDamage_)
		{
			std::cout << "De val doet " << directDamage_ << " schade." << std::endl;
			hero->takeDirectDamage(directDamage_);
		}

		if (damageOverTime_ && damageOverTimeTurns_)
		{
			std::cout << "damage over time werkt nog niet..." << std::endl;
			// TODO: damage over time
		}

		if (!enemiesToSummon_.empty())
		{
			std::for_each(enemiesToSummon_.begin(), enemiesToSummon_.end(), [room](Enemy* e)
			{
				room->addEnemy(e);
			});
		}

		return true;
	}

	return false;
}
