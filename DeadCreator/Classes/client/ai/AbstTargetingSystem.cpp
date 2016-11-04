#include "AbstTargetingSystem.hpp"
#include "HumanBase.hpp"
#include "SensoryMemory.hpp"
#include "Game.hpp"
#include "GoalNetwork.hpp"

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
	Vec2 toTarget = targetPos - ownerPos;
	Vec2 correctAim = toTarget.getNormalized();
	Vec2 aimPerp = (correctAim.getPerp() * targetRadiusNoised + toTarget).getNormalized();

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
{
	_followers.resize(GoalNetwork::kNumOfMaxFollowers, nullptr);
}

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

	for (auto e : _followers)
	{
		if (e)
		{
			if (!e->isAlive())
				e = nullptr;
		}
	}

	if(_leader)
		_leader->getTargetSys()->removeFollower(_owner);
	_leader = nullptr;

	// If ally with player, then player is the leader
	if (_owner->getGame()->isAllyState(_owner->getPlayerType(),
		_owner->getGame()->getPlayerPtr()->getPlayerType()) &&
		_owner->getGame()->getPlayerPtr()->isAlive() &&
		_owner != _owner->getGame()->getPlayerPtr())
	{
		_leader = _owner->getGame()->getPlayerPtr();
		_leader->getTargetSys()->addFollower(_owner);
	}

	if (_leader == nullptr)
	{
		// #additional  actually here needs some code for finding the best leader
		const auto& ally = _owner->getSensoryMemory()->getListOfRecentlySensedEntities(true);
		for (auto e : ally)
		{
			if (e == _owner)
				continue;

			_leader = e;
			_leader->getTargetSys()->addFollower(_owner);
			break;
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


bool AbstTargetingSystem::addFollower(HumanBase* const follower)
{
	//bool removed = removeFollower(follower);

	for (auto& e : _followers)
	{
		if (e == nullptr)
		{
			e = follower;
			return true;
		}
	}
	return false;
}

bool AbstTargetingSystem::removeFollower(HumanBase* const follower)
{
	for (auto& e : _followers)
	{
		if (e == follower)
		{
			e = nullptr;
			return true;
		}
	}
	return false;
}

int AbstTargetingSystem::queryFollowerIndex(HumanBase* const follower)
{
	for (int i = 0; i < _followers.size(); i++)
	{
		if (_followers[i] == follower)
			return i;
	}
	return -1;
}
