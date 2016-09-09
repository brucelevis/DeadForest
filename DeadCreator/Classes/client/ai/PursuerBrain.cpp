#include "PursuerBrain.hpp"
#include "HumanBase.hpp"
#include "GoalAttackTarget.hpp"

using namespace realtrick::client;

PursuerBrain::PursuerBrain(HumanBase* owner) 
	:
BrainBase(owner)
{
	_thinker = new GoalThink(owner);

	GoalReadyToFight* readyToFight = new GoalReadyToFight(_owner);
	readyToFight->setEvaluator(
		[this](HumanBase* owner) 
	{
		if (_owner->getEquipedWeapon() == nullptr)
			return 10;
		else
			return 0;
	});

	_thinker->addGoalEntry(readyToFight);

	GoalAttackTarget* attack = new GoalAttackTarget(owner);
	attack->setEvaluator([this](HumanBase* owner) { return 1; });
	_thinker->addGoalEntry(attack);
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
