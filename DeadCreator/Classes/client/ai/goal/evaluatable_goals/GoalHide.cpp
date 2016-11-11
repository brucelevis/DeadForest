//
//  GoalHide.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalHide.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "EntityManager.hpp"
#include "EntityBase.hpp"
#include "GoalMoveToPosition.hpp"
#include "GoalEquipWeapon.hpp"
#include "GoalRunAway.hpp"
#include "SensoryMemory.hpp"
#include "InventoryData.hpp"
#include "InputCommands.hpp"
#include "AbstTargetingSystem.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;


GoalHide::GoalHide(HumanBase* owner, float character_bias) :
GoalEvaluatable(owner, character_bias)
{
    setGoalName("hide");
    setGoalType(GoalType::HIDE);
}


void GoalHide::findHidePositions( const std::vector<cocos2d::Vec2>& vertices, cocos2d::Vec2 entityPos, cocos2d::Vec2 enemyPos, std::list<cocos2d::Vec2>& hidePositions)
{
    const float kEntityRadius = 100.0f;
    
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        size_t j = (i + 1) % vertices.size();
        cocos2d::Vec2 begin = vertices[i];
        cocos2d::Vec2 end = vertices[j];
        cocos2d::Vec2 norm = (begin - end).getNormalized().getPerp();
        cocos2d::Vec2 hidePos = (end + begin) / 2 + norm * kEntityRadius;
        
        // if intersects, it is validate pos to hide
        if ((entityPos - enemyPos).dot(hidePos - entityPos) > 0)
        {
            if (physics::intersect(Segment(begin, end), Segment(hidePos, enemyPos)))
            {
                hidePositions.push_back(hidePos);
            }
        }
    }
}

bool GoalHide::findHidePosition(const std::vector<realtrick::Polygon>& poly,
                                HumanBase* owner,
                                cocos2d::Vec2 targetPos,
                                cocos2d::Vec2& hidePos)
{
    std::list<cocos2d::Vec2> hidePositions;
    
    for (auto p : poly)
    {
        findHidePositions(p.vertices,
                          owner->getWorldPosition(),
                          targetPos,
                          hidePositions);
    }
    
    for (auto e : hidePositions)
    {
        if (!owner->getGame()->isCollideWalls(e))
        {
            hidePos = e;
            return true;
        }
    }
    
    return false;
}


void GoalHide::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    removeAllSubgoals();
    
    if (!_owner->getTargetSys()->isTargetPresent())
    {
        setGoalStatus(GoalStatus::COMPLETED);
        return;
    }
    
    const auto& poly = _owner->getGame()->getNeighborWalls(
                                                           _owner->getWorldPosition(),
                                                           _owner->getSensoryMemory()->getViewRange());
    
    cocos2d::Vec2 hidePos;
    
    bool found = findHidePosition(
                                  poly,
                                  _owner,
                                  _owner->getTargetSys()->getTarget()->getWorldPosition(),
                                  hidePos);
    
    if (found)
        addSubgoal(new GoalMoveToPosition(
                                          _owner,
                                          hidePos,
                                          std::make_shared<ArrivingData>(
                                                                         ArrivingData(_owner->getHeading(), 50.0f, 250.0f))));
    else
        addSubgoal(new GoalRunAway(_owner));
}

GoalStatus GoalHide::process()
{
    if (isInactive())
        activate();
    
    GoalStatus subGoalStatus = processSubgoals();
    return subGoalStatus;
}

void GoalHide::terminate()
{
    removeAllSubgoals();
}

int GoalHide::evaluate(HumanBase* const owner)
{
    if (!owner->getTargetSys()->isTargetPresent())
        return 0;
    
    auto target = owner->getTargetSys()->getTarget();
    
    if (!target->getSensoryMemory()->isEntityViewable(owner))
        return 0;
    
    int enemyCnt = (int)_owner->getSensoryMemory()->getListOfRecentlySensedEntities(false).size();
    int allyCnt = (int)_owner->getSensoryMemory()->getListOfRecentlySensedEntities(true).size();
    
    return std::max(enemyCnt - (allyCnt + 1), 0) * 10;
}

