#pragma once

#include "Manipulators/MinionManipulator.h"
#include "EntitiesManager/EntitiesManager.h"

namespace Manipulators
{
	MinionManipulator::MinionManipulator(EntitiesManager &mgr, Entity::Card &card) :
		mgr_(mgr), card_(card)
	{
	}
}