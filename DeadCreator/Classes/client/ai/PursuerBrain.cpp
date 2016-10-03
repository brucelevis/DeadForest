#include "PursuerBrain.hpp"
#include "HumanBase.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalFindWeapon.hpp"
#include "GoalEquipWeapon.h"
#include "GoalReload.h"
#include "InventoryData.hpp"

using namespace realtrick::client;

PursuerBrain::PursuerBrain(HumanBase* owner) 
	:
BrainBase(owner)
{
	_thinker = new GoalThink(owner);

	GoalAttackTarget* attack = new GoalAttackTarget(owner);
	attack->setEvaluator(std::bind(&GoalAttackTarget::evaluate, attack, owner));
	_thinker->addGoalEntry(attack);

	GoalEquipWeapon* equip = new GoalEquipWeapon(owner);
	equip->setEvaluator(std::bind(&GoalEquipWeapon::evaluate, equip, owner));
	_thinker->addGoalEntry(equip);

	GoalFindWeapon* find = new GoalFindWeapon(owner);
	find->setEvaluator(std::bind(&GoalFindWeapon::evaluate, find, owner));
	_thinker->addGoalEntry(find);

	GoalReload* reload = new GoalReload(owner);
	reload->setEvaluator(std::bind(&GoalReload::evaluate, reload, owner));
	_thinker->addGoalEntry(reload);

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

