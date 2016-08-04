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
                auto data = static_cast<ConditionBringData*>(*cond);
                auto condition = ConditionBring::create(_game,
                                                        data->player,
                                                        data->approximation,
                                                        data->number,
                                                        data->entity,
                                                        data->location);
                newTrigger->addCondition(condition);
            }
            
            else if ( (*cond)->type == TriggerComponentType::CONDITION_ALWAYS)
            {
                auto condition = ConditionAlways::create(_game);
                newTrigger->addCondition(condition);
            }
            
            else if ( (*cond)->type == TriggerComponentType::CONDITION_NEVER)
            {
                auto condition = ConditionNever::create(_game);
                newTrigger->addCondition(condition);
            }
        }
        
        // set actions
        for(auto act = currTrigger.actions.begin() ; act != currTrigger.actions.end(); ++act)
        {
            if ( (*act)->type == TriggerComponentType::ACTION_DISPLAY_TEXT)
            {
                auto data = static_cast<ActionDisplayTextData*>(*act);
                auto action = ActionDisplayText::create(_game, data->text);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_PRESERVE_TRIGGER)
            {
                auto action = ActionPreserveTrigger::create(_game);
                newTrigger->addAction(action);
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
    
    if ( _isRemoveListDirty )
    {
        for( auto& removeID : _removeIDList )
        {
            _triggers.erase(removeID);
        }
        _removeIDList.clear();
        _isRemoveListDirty = false;
    }
}


void TriggerSystem::removeTrigger(GameTrigger* trigger)
{
    _removeIDList.push_back(trigger->getTriggerID());
    _isRemoveListDirty = true;
}









