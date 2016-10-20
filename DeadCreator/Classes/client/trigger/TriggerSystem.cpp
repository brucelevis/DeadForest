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
            
            else if ( (*cond)->type == TriggerComponentType::CONDITION_COUNTDOWN_TIMER )
            {
                auto data = static_cast<ConditionCountdownTimerData*>(*cond);
                auto condition = ConditionCountdownTimer::create(_game, data->approximation, data->number);
                newTrigger->addCondition(condition);
            }
            
            else if ( (*cond)->type == TriggerComponentType::CONDITION_ELAPSED_TIME )
            {
                auto data = static_cast<ConditionElapsedTimeData*>(*cond);
                auto condition = ConditionElapsedTime::create(_game, data->approximation, data->number);
                newTrigger->addCondition(condition);
            }
            
            else if ( (*cond)->type == TriggerComponentType::CONDITION_SWITCH )
            {
                auto data = static_cast<ConditionSwitchData*>(*cond);
                auto condition = ConditionSwitch::create(_game, data->info);
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
            
            else if ( (*act)->type == TriggerComponentType::ACTION_VICTORY)
            {
                auto action = ActionVictory::create(_game);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_DEFEAT)
            {
                auto action = ActionDefeat::create(_game);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_MOVE_LOCATION )
            {
                auto data = static_cast<ActionMoveLocationData*>(*act);
                auto action = ActionMoveLocation::create(_game,
                                                         data->destLocation,
                                                         data->entity,
                                                         data->player,
                                                         data->sourceLocation);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_KILL_ENTITY_AT_LOCATION )
            {
                auto data = static_cast<ActionKillEntityAtLocationData*>(*act);
                auto action = ActionKillEntityAtLocation::create(_game,
                                                                 data->number,
                                                                 data->entity,
                                                                 data->player,
                                                                 data->location);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_PLAY_SOUND )
            {
                auto data = static_cast<ActionPlaySoundData*>(*act);
                auto action = ActionPlaySound::create(_game, data->fileName);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_PLAY_SOUND_AT_LOCATION )
            {
                auto data = static_cast<ActionPlaySoundAtLocationData*>(*act);
                auto action = ActionPlaySoundAtLocation::create(_game,
                                                                data->fileName,
                                                                data->location);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_MOVE_ENTITY )
            {
                auto data = static_cast<ActionMoveEntityData*>(*act);
                auto action = ActionMoveEntity::create(_game,
                                                       data->number,
                                                       data->entity,
                                                       data->player,
                                                       data->sourceLocation,
                                                       data->destLocation);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_PAUSE_GAME )
            {
                auto action = ActionPauseGame::create(_game);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_RESUME_GAME )
            {
                auto action = ActionResumeGame::create(_game);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_SET_COUNTDOWN_TIMER )
            {
                auto data = static_cast<ActionSetCountdownTimerData*>(*act);
                auto action = ActionSetCountdownTimer::create(_game,
                                                              data->arithmetical,
                                                              data->number);
                newTrigger->addAction(action);
            }
            
            else if ( (*act)->type == TriggerComponentType::ACTION_SET_SWITCH )
            {
                auto data = static_cast<ActionSetSwitchData*>(*act);
                auto action = ActionSetSwitch::create(_game, data->info);
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
    std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
    if ( (curr - _lastTriggerExecutedTime).count() < _triggerExecuteTime ) return ;
    
    for (auto& trigger : _triggers)
    {
        if ( trigger.second->isReady() )
        {
            trigger.second->doAction();
            _lastTriggerExecutedTime = std::chrono::system_clock::now().time_since_epoch();
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









