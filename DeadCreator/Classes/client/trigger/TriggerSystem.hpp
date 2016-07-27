//
//  TriggerSystem.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 28..
//
//

#pragma once

#include <vector>

#include "GameTrigger.hpp"

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
                        trigger->doAction();
                }
            }
            
        private:
            
            GameManager* _gameMgr;
            std::vector<GameTrigger*> _triggers;
            
        };
        
    }
}