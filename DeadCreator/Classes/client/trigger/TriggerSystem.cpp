//
//  TriggerSystem.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "TriggerSystem.hpp"
#include "GameResource.hpp"
using namespace cocos2d;
using namespace realtrick::client;

bool TriggerSystem::initWithResource(GameResource* res)
{
    for ( auto trigger = res->_triggers.begin() ; trigger != res->_triggers.end() ; ++trigger)
    {
        TriggerData currTrigger = *trigger;
        GameTrigger* newTrigger = GameTrigger::create(_game);
        
        // set players
        for(auto index = currTrigger.players.begin(); index != currTrigger.players.end(); ++index)
            newTrigger->addPlayer(*index);
        
        // set conditions
        for(auto cond = currTrigger.conditions.begin() ; cond != currTrigger.conditions.end(); ++cond)
        {
            if ( (*cond)->type == TriggerComponentType::CONDITION_BRING)
            {
                auto bringData = static_cast<ConditionBringData*>(*cond);
                auto conditionBring = ConditionBring::create(_game, bringData->player,
                                                             bringData->approximation,
                                                             bringData->number,
                                                             bringData->entity,
                                                             bringData->location);
                newTrigger->addCondition(conditionBring);
            }
        }
        
        // set actions
        for(auto act = currTrigger.actions.begin() ; act != currTrigger.actions.end(); ++act)
        {
            if ( (*act)->type == TriggerComponentType::ACTION_DISPLAY_TEXT)
            {
                auto displayData = static_cast<ActionDisplayTextData*>(*act);
                auto displayObj = ActionDisplayText::create(_game, displayData->text);
                newTrigger->addAction(displayObj);
            }
        }
        
        int triggerID = getNextValidTriggerID();
        newTrigger->setTriggerID(triggerID);
        _triggers.insert(triggerID, newTrigger);
    }
    
    return true;
}


void TriggerSystem::update(float dt)
{
    for (auto& trigger : _triggers)
    {
        if ( trigger.second->isReady() )
        {
            trigger.second->doAction();
        }
    }
    
    for( auto& removeID : _removeIDList )
    {
        _triggers.erase(removeID);
    }
}


void TriggerSystem::removeTrigger(GameTrigger* trigger)
{
    _removeIDList.push_back(trigger->getTriggerID());
}









