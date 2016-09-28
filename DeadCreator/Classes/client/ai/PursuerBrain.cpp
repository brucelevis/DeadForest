#include "PursuerBrain.hpp"
#include "HumanBase.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalFindWeapon.hpp"
#include "GoalReadyToFight.hpp"
#include "InventoryData.hpp"

using namespace realtrick::client;

PursuerBrain::PursuerBrain(HumanBase* owner) 
	:
BrainBase(owner)
{
	_thinker = new GoalThink(owner);
	GoalAttackTarget* attack = new GoalAttackTarget(owner);
	attack->setEvaluator([this](HumanBase* owner) { return 1; });
	_thinker->addGoalEntry(attack);

	GoalReadyToFight* ready = new GoalReadyToFight(owner);

	// #instance loss problem
	ready->setEvaluator(std::bind(&GoalReadyToFight::evaluate, owner));
	_thinker->addGoalEntry(ready);
}

PursuerBrain::~PursuerBrain()
{
	delete _thinker;
	_thinker = nullptr;
}

void PursuerBrain::think()
{
	if (_regulator.isReady())
		_thinker->process();
}

