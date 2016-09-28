//
//  GoalFindWeapon.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalFindWeapon.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "EntityManager.hpp"
#include "EntityBase.hpp"
#include "GoalMoveToPosition.hpp"
#include "GoalEquipWeapon.h"
#include "SensoryMemory.h"

using namespace realtrick::client;
using namespace realtrick;


GoalFindWeapon::GoalFindWeapon(HumanBase* owner, EntityType findWeapon) 
	:
	GoalCompositeBase(owner),
	_findWeapon(findWeapon)
{
    setGoalType(GoalType::FIND_WEAPON);
}


GoalFindWeapon::~GoalFindWeapon()
{
}


void GoalFindWeapon::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);

	const float bigf = std::numeric_limits<float>::max();
	float dist = bigf;
	cocos2d::Vec2 desti;

	for (auto e : _owner->getGame()->getEntityManager()->getEntities())
	{
		if (e.second->getEntityType() == _findWeapon)
		{
			cocos2d::Vec2 itemPos = e.second->getWorldPosition();
			float distance = itemPos.distance(_owner->getWorldPosition());

			if (distance > _owner->getSensoryMemory()->getViewRange())
				continue;

			if (dist > distance)
			{
				dist = distance;
				desti = itemPos;
			}
		}
	}

	if (dist < bigf - 1)
	{
		addSubgoal(new GoalEquipWeapon(_owner, _findWeapon));
		addSubgoal(new GoalMoveToPosition(_owner, desti));
	}
	else
	{
		setGoalStatus(GoalStatus::FAILED);
	}
}


GoalStatus GoalFindWeapon::process()
{
    if ( isInactive() ) activate();
    
    auto subGoalStatus = processSubgoals();
    return subGoalStatus;
}


void GoalFindWeapon::terminate()
{
	removeAllSubgoals();
}









