#include "ZombieBrain.hpp"
#include "HumanBase.hpp"

using namespace realtrick::client;

ZombieBrain* ZombieBrain::createDefault(HumanBase* owner)
{
	ZombieBrain* brain = new ZombieBrain(owner);
	GoalAttackTarget* goal = new GoalAttackTarget(owner, 1.5f);
	goal->setEvaluator(std::bind(&GoalAttackTarget::evaluate, goal, owner));
	brain->getGoalThink()->addGoalEntry(goal);

	return brain;
}

ZombieBrain* ZombieBrain::createWithDestination(
	HumanBase* owner,
	const cocos2d::Vec2& desti,
	float arriveRange)
{
	ZombieBrain* brain = new ZombieBrain(owner);
	GoalAttackToDestination* goal =
		new GoalAttackToDestination(owner, desti, arriveRange);
	goal->setEvaluator([brain](HumanBase* owner) { return 1; });
	brain->getGoalThink()->addGoalEntry(goal);
	return brain;
}
