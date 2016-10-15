#include "GoalNetwork.h"
#include "HumanBase.hpp"
#include "SensoryMemory.h"
#include "Game.hpp"
#include "AbstTargetingSystem.h"

USING_NS_CC;
using namespace realtrick::client;

const float GoalNetwork::kFormationDistance = 125.0f;
cocos2d::Vec2 GoalNetwork::kFormations[GoalNetwork::kNumOfMaxFormations];

void GoalNetwork::staticInitConstants()
{
	cocos2d::Vec2 positions[kNumOfMaxFormations] =
	{
		Vec2(-1, 1),	Vec2(0, 1),		Vec2(1, 1),
		Vec2(-1, 0),					Vec2(1, 0),
		Vec2(-1, -1),	Vec2(0, -1),	Vec2(1, -1),

		Vec2(-2, 2),	Vec2(-1, 2),	Vec2(0, 2),		Vec2(1, 2),		Vec2(2, 2),
		Vec2(-2, 1),													Vec2(2, 1),
		Vec2(-2, 0),													Vec2(2, 0),
		Vec2(-2, -1),													Vec2(2, -1),
		Vec2(-2, -2),	Vec2(-1, -2),	Vec2(0, -2),	Vec2(1, -2),	Vec2(2, -2),
	};

	for (int i = 0; i < kNumOfMaxFormations; i++)
	{
		kFormations[i] = positions[i];
	}
	
}

cocos2d::Vec2 GoalNetwork::queryFormationPos(HumanBase* const owner, HumanBase* const leader, int ownerIdx)
{
	Vec2 formationPos = 
		realtrick::Mat3::pointToWorldSpace(
		kFormations[ownerIdx] * kFormationDistance,
		-leader->getHeading().getPerp(),
		-leader->getHeading().getPerp().getPerp(),
		leader->getWorldPosition());

	return formationPos;
}