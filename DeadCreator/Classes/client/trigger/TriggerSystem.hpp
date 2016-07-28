//
//  TriggerSystem.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 28..
//
//

#pragma once

#include <vector>

#include "cocos2d.h"
#include "GameTrigger.hpp"
#include "Conditions.hpp"
#include "Actions.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GameManager;
        
        class TriggerSystem
        {
            
        public:
            
            explicit TriggerSystem(GameManager* mgr) : _gameMgr(mgr)
            {}
            virtual ~TriggerSystem() = default;
            
            void update(float dt)
            {
                for (auto& trigger : _triggers)
                {
                    if ( trigger->isReady() )
                    {
                        trigger->doAction();
                    }
                }
            }
            
            void addTrigger(GameTrigger* trigger) { _triggers.pushBack(trigger); }
            
        private:
            
            GameManager* _gameMgr;
            cocos2d::Vector<GameTrigger*> _triggers;
            
        };
        
    }
}









