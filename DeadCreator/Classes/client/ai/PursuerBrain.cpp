#include "PursuerBrain.hpp"
#include "HumanBase.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalFindWeapon.hpp"
#include "GoalEquipWeapon.h"
#include "GoalReload.h"
#include "GoalRunAway.h"
#include "GoalFollowPlayer.hpp"
#include "GoalMakeFormation.h"
#include "InventoryData.hpp"
#include "Game.hpp"

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

	GoalRunAway* runaway = new GoalRunAway(owner);
	runaway->setEvaluator(std::bind(&GoalRunAway::evaluate, runaway, owner));
	_thinker->addGoalEntry(runaway);

	GoalFollowPlayer* follow = new GoalFollowPlayer(owner);
	follow->setEvaluator(std::bind(&GoalFollowPlayer::evaluate, follow, owner));
	_thinker->addGoalEntry(follow);	

	//GoalMakeFormation* formation = new GoalMakeFormation(owner);
	//formation->setEvaluator([](HumanBase* owner) { return 1000; });
	//_thinker->addGoalEntry(formation);
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

