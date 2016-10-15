#include "AbstTargetingSystem.h"
#include "HumanBase.hpp"
#include "SensoryMemory.h"
#include "Game.hpp"

USING_NS_CC;
using namespace realtrick::client;

bool AbstTargetingSystem::isAimAccurate(
	const cocos2d::Vec2& targetPos,
	float targetRadius,
	const cocos2d::Vec2& ownerPos,
	const cocos2d::Vec2& ownerHeading,
	float error)
{
	float targetRadiusNoised = targetRadius * error;
	cocos2d::Vec2 toTarget = targetPos - ownerPos;
	cocos2d::Vec2 correctAim = toTarget.getNormalized();
	cocos2d::Vec2 aimPerp = (correctAim.getPerp() * targetRadiusNoised + toTarget).getNormalized();

	float minAimAccuracy = correctAim.dot(aimPerp);
	float ownerAimAccuracy = correctAim.dot(ownerHeading);

	return ownerAimAccuracy > minAimAccuracy;
}

//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
AbstTargetingSystem::AbstTargetingSystem(HumanBase* const owner)
	:
	_owner(owner),
	_target(nullptr),
	_leader(nullptr)
{}

void AbstTargetingSystem::update()
{
	float closestDist = std::numeric_limits<float>::max();
	_target = nullptr;

	//grab a list of all the opponents the owner can sense
	const auto& enemies = _owner->getSensoryMemory()->getListOfRecentlySensedEntities(false);

	for (auto e : enemies)
	{
		float dist = e->getWorldPosition().distance(_owner->getWorldPosition());

		if (dist < closestDist)
		{
			closestDist = dist;
			_target = e;
		}
	}

	_leader = nullptr;

	// If ally with player, then player is the leader
	if (!_owner->getGame()->isAllyState(_owner->getPlayerType(),
		(_owner->getGame()->getPlayerPtr())->getPlayerType()) &&
		(_owner->getGame()->getPlayerPtr())->isAlive())
	{
		_leader = _owner->getGame()->getPlayerPtr();
	}

	const auto& ally = _owner->getSensoryMemory()->getListOfRecentlySensedEntities(false);
	for (auto e : ally)
	{
		if (_leader == nullptr)
		{
			_leader = e;
		}
	}
}


bool AbstTargetingSystem::isTargetWithinFOV() const
{
  return _owner->getSensoryMemory()->isOpponentWithinFOV(_target);
}

//returns true if there is a currently assigned target
bool AbstTargetingSystem::isTargetPresent() const 
{
	return _target != nullptr;
}

bool AbstTargetingSystem::isTargetAttackable() const
{
  return _owner->getSensoryMemory()->isOpponentAttackable(_target);
}

cocos2d::Vec2 AbstTargetingSystem::getLastRecordedPosition() const
{
  return _owner->getSensoryMemory()->getLastRecordedPositionOfOpponent(_target);
}

std::chrono::duration<double> AbstTargetingSystem::getTimeTargetHasBeenVisible() const
{
  return _owner->getSensoryMemory()->getTimeOpponentHasBeenVisible(_target);
}

std::chrono::duration<double> AbstTargetingSystem::getTimeTargetHasBeenOutOfView() const
{
  return _owner->getSensoryMemory()->getTimeOpponentHasBeenOutOfView(_target);
}
