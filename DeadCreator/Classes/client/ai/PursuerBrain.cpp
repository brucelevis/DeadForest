#include "PursuerBrain.hpp"
#include "HumanBase.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalFindWeapon.hpp"
#include "GoalEquipWeapon.h"
#include "GoalReload.h"
#include "GoalRunAway.h"
#include "GoalFollowPlayer.hpp"
#include "InventoryData.hpp"
#include "Game.hpp"

using namespace realtrick::client;

PursuerBrain* PursuerBrain::createDefault(HumanBase* owner)
{
	PursuerBrain* brain = new PursuerBrain(owner);
	
	GoalAttackTarget* attack = new GoalAttackTarget(owner);
	attack->setEvaluator(std::bind(&GoalAttackTarget::evaluate, attack, owner));
	brain->getGoalThink()->addGoalEntry(attack);

	GoalEquipWeapon* equip = new GoalEquipWeapon(owner);
	equip->setEvaluator(std::bind(&GoalEquipWeapon::evaluate, equip, owner));
	brain->getGoalThink()->addGoalEntry(equip);

	GoalFindWeapon* find = new GoalFindWeapon(owner);
	find->setEvaluator(std::bind(&GoalFindWeapon::evaluate, find, owner));
	brain->getGoalThink()->addGoalEntry(find);

	GoalReload* reload = new GoalReload(owner);
	reload->setEvaluator(std::bind(&GoalReload::evaluate, reload, owner));
	brain->getGoalThink()->addGoalEntry(reload);

	GoalRunAway* runaway = new GoalRunAway(owner);
	runaway->setEvaluator(std::bind(&GoalRunAway::evaluate, runaway, owner));
	brain->getGoalThink()->addGoalEntry(runaway);

	GoalFollowPlayer* follow = new GoalFollowPlayer(owner);
	follow->setEvaluator(std::bind(&GoalFollowPlayer::evaluate, follow, owner));
	brain->getGoalThink()->addGoalEntry(follow);

	return brain;
}

void PursuerBrain::think()
{
	if (_regulator.isReady())
		_thinker->process();
}

