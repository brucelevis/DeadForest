#include "PursuerBrain.hpp"
#include "HumanBase.hpp"
#include "GoalAttackTarget.hpp"

using namespace realtrick::client;

PursuerBrain::PursuerBrain(HumanBase* owner) 
	:
BrainBase(owner)
{
	_thinker = new GoalThink(owner);

	/*GoalHuntTarget* huntTarget = new GoalHuntTarget(_owner);
	huntTarget->setEvaluator([this](HumanBase* owner) { return 1; });
	_thinker->addGoalEntry(huntTarget);*/

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
