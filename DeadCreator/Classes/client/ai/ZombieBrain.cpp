#include "ZombieBrain.hpp"
#include "HumanBase.hpp"
#include "GoalRaid.hpp"
#include "GoalMainAttack.hpp"
#include "GoalWander.hpp"
#include "GoalHide.hpp"
#include "GoalProvoke.hpp"
#include "GoalAmbush.hpp"
#include "GoalRunAway.hpp"

using namespace realtrick::client;


ZombieBrain* ZombieBrain::createDefault(HumanBase* owner)
{
    ZombieBrain* brain = new ZombieBrain(owner);
    
    GoalAttackTarget* attack = new GoalAttackTarget(owner);
    attack->setEvaluator(std::bind(&GoalAttackTarget::evaluate, attack, owner));
    brain->getGoalThink()->addGoalEntry(attack);
    
    GoalWander* wander = new GoalWander(owner);
    wander->setEvaluator(std::bind(&GoalWander::evaluate, wander, owner));
    brain->getGoalThink()->addGoalEntry(wander);
    
    GoalHide* hide = new GoalHide(owner);
    hide->setEvaluator(std::bind(&GoalHide::evaluate, hide, owner));
    brain->getGoalThink()->addGoalEntry(hide);
    
    GoalProvoke* provoke = new GoalProvoke(owner);
    provoke->setEvaluator(std::bind(&GoalProvoke::evaluate, provoke, owner));
    brain->getGoalThink()->addGoalEntry(provoke);
    
    return brain;
}


ZombieBrain* ZombieBrain::createWithDestination( HumanBase* owner, const cocos2d::Vec2& desti, float arriveRange)
{
    ZombieBrain* brain = new ZombieBrain(owner);
    GoalAttackToDestination* goal = new GoalAttackToDestination(owner, desti, arriveRange);
    
    goal->setEvaluator([brain](HumanBase* owner) { return 1; });
    brain->getGoalThink()->addGoalEntry(goal);
    return brain;
}
