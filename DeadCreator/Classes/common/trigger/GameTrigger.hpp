//
//  GameTrigger.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 18..
//
//

#pragma once

#include <vector>
#include <string>

#include "EntityType.hpp"
#include "ConditionBase.hpp"
#include "ActionBase.hpp"


#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        struct GameTrigger
        {
            
            bool isSelected;
            
            bool isPlayerSelected[8];
            std::vector<ConditionBase*> conditions;
            std::vector<ActionBase*> actions;
            
            GameTrigger() { for ( int i = 0 ; i < 8 ; ++ i) isPlayerSelected[i] = false; }
            GameTrigger(const GameTrigger& rhs) { copyFrom(rhs); }
            GameTrigger& operator=(const GameTrigger& rhs)
            {
                if ( &rhs != this )
                {
                    reset();
                    copyFrom(rhs);
                }
                return *this;
            }
            
            virtual ~GameTrigger() { reset(); }
            
            void copyFrom(const GameTrigger& rhs)
            {
                for(int i = 0 ; i < 8 ; ++ i ) isPlayerSelected[i] = rhs.isPlayerSelected[i];
                for(auto& cond : rhs.conditions) conditions.push_back(cond->clone());
                for(auto& act : rhs.actions) actions.push_back(act->clone());
            }
            
            void reset()
            {
                for(int i = 0 ; i < 8 ; ++ i ) isPlayerSelected[i] = false;
                for(auto& cond : conditions) CC_SAFE_DELETE(cond);
                for(auto& act : actions) CC_SAFE_DELETE(act);
                conditions.clear();
                actions.clear();
            }
            
            bool drawSelectableTrigger()
            {
                std::string label;
                label += "Conditions:\n";
                for(int i = 0 ; i < conditions.size(); ++ i)
                {
                    label += conditions[i]->getSummaryString();
                    label += '\n';
                }
                
                label += "Actions:\n";
                for(int i = 0 ; i < actions.size() ; ++ i)
                {
                    label += actions[i]->getSummaryString();
                    label += '\n';
                }
                
                label.pop_back();
                
                bool ret = ImGui::Selectable(label.c_str(), &isSelected, ImGuiSelectableFlags_AllowDoubleClick);
                ImGui::Separator();
                return ret;
            }
            
            void addCondition(ConditionBase* cond) { if ( cond ) conditions.push_back(cond); }
            void addAction(ActionBase* act) { if ( act ) actions.push_back(act); }
            
            void eraseCondition(int idx)
            {
                auto iter = std::find(conditions.begin(), conditions.end(), conditions[idx]);
                if ( iter != conditions.end() )
                {
                    CC_SAFE_DELETE(*iter);
                    conditions.erase(iter);
                }
            }
            
            void eraseAction(int idx)
            {
                auto iter = std::find(actions.begin(), actions.end(), actions[idx]);
                if ( iter != actions.end() )
                {
                    CC_SAFE_DELETE(*iter);
                    actions.erase(iter);
                }
            }
            
            int getSelectedPlayerSize() const
            {
                int ret = 0;
                for(int i = 0 ; i < 8 ; ++ i) if (isPlayerSelected[i]) ret ++;
                return ret;
            }
            
            std::vector<int> getSelectedPlayers() const
            {
                std::vector<int> ret;
                for(int i = 0 ; i < 8 ; ++i) if ( isPlayerSelected[i] ) ret.push_back(i);
                return ret;
            }
            
            GameTrigger* clone() const { return new GameTrigger(*this); }
            
        };
        
    }
    
    namespace client
    {
        
        class Game;
        
        class GameTrigger : public cocos2d::Ref
        {
            
        public:
            
            explicit GameTrigger(Game* game) :
            _game(game)
            {}
            virtual ~GameTrigger() {}
            
            static GameTrigger* create(Game* game)
            {
                auto ret = new (std::nothrow) GameTrigger(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init()
            {
                return true;
            }
            
            bool isReady()
            {
                for (auto& cond : _conditions)
                {
                    if ( !cond->isReady() ) return false;
                }
                return true;
            }
            
            void doAction()
            {
                for(auto& act : _actions)
                {
                    act->doAction();
                }
            }
            
            void addPlayer(int player) { _players.set(player); }
            void addCondition(ConditionBase* condition) { condition->setOwner(this); _conditions.pushBack(condition); }
            void addAction(ActionBase* action) { action->setOwner(this); _actions.pushBack(action); }
            
            Game* getGameManager() const { return _game; }
            std::bitset<9> getPlayers() const { return _players; }
            
        private:
            
            Game* _game;
            
            std::bitset<9> _players;
            cocos2d::Vector<ConditionBase*> _conditions;
            cocos2d::Vector<ActionBase*> _actions;
            
        };
        
    }
}









