#include "stdafx.h"
#include "Trap.h"

// ReSharper disable once CppPossiblyUninitializedMember
Trap::Trap() : triggered_(false), 
chanceToFind_(100), 
directDamage_(0), 
damageOverTime_(0), 
damageOverTimeTurns_(0), 
roomDescription_(""), 
damageOverTimeMessage_(""), 
damageOverTimeDone_(""),
triggerCommands_({"any"})
{
	
}

Trap::~Trap()
{
}
