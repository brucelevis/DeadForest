#include "AbstTargetingSystem.h"
#include "HumanBase.hpp"
#include "SensoryMemory.h"

USING_NS_CC;
using namespace realtrick::client;

//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
AbstTargetingSystem::AbstTargetingSystem(HumanBase* const owner)
	:
	_owner(owner),
	_current_target(nullptr)
{}

void AbstTargetingSystem::update()
{
	double closest_dist_so_far = std::numeric_limits<double>::max();
	_current_target = nullptr;

	//grab a list of all the opponents the owner can sense
	std::list<HumanBase*> sensed_bots;
	sensed_bots = _owner->getSensoryMemory()->getListOfRecentlySensedOpponents();

	std::list<HumanBase*>::const_iterator bot = sensed_bots.begin();
	for (bot; bot != sensed_bots.end(); ++bot)
	{
		//make sure the bot is alive and that it is not the owner
		if ((*bot)->isAlive() && (*bot != _owner))
		{
			//if (_owner->getTeam() != (*bot)->getTeam())
			//{
				double dist = ((*bot)->getWorldPosition() - _owner->getWorldPosition()).getLength();

				if (dist < closest_dist_so_far)
				{
					closest_dist_so_far = dist;
					_current_target = *bot;
				}
			//}
		}
	}
}


bool AbstTargetingSystem::isTargetWithinFOV() const
{
  return _owner->getSensoryMemory()->isOpponentWithinFOV(_current_target);
}

//returns true if there is a currently assigned target
bool AbstTargetingSystem::isTargetPresent() const 
{
	return _current_target != nullptr;
}

bool AbstTargetingSystem::isTargetAttackable() const
{
  return _owner->getSensoryMemory()->isOpponentAttackable(_current_target);
}

cocos2d::Vec2 AbstTargetingSystem::getLastRecordedPosition() const
{
  return _owner->getSensoryMemory()->getLastRecordedPositionOfOpponent(_current_target);
}

std::chrono::duration<double> AbstTargetingSystem::getTimeTargetHasBeenVisible() const
{
  return _owner->getSensoryMemory()->getTimeOpponentHasBeenVisible(_current_target);
}

std::chrono::duration<double> AbstTargetingSystem::getTimeTargetHasBeenOutOfView() const
{
  return _owner->getSensoryMemory()->getTimeOpponentHasBeenOutOfView(_current_target);
}

//returns a pointer to the target. null if no target current.
HumanBase* AbstTargetingSystem::getTarget() const 
{
	return _current_target; 
}

void AbstTargetingSystem::setTarget(HumanBase* target)
{
	_current_target = target;
}

//sets the target pointer to null
void AbstTargetingSystem::clearTarget() 
{
	_current_target = nullptr; 
}