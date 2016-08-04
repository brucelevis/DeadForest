//
//  TriggerSystem.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
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
        
        class Game;
        class GameResource;
        
        class TriggerSystem : public cocos2d::Ref
        {
            
        public:
            
            explicit TriggerSystem(Game* game) : _game(game)
            {}
            virtual ~TriggerSystem() = default;
            
            static TriggerSystem* createWithResouce(Game* game, GameResource* res)
            {
                auto ret = new (std::nothrow) TriggerSystem(game);
                if ( ret && ret->initWithResource(res) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool initWithResource(GameResource* res);
            
            void removeTrigger(GameTrigger* trigger);
            void update(float dt);
            int getNextValidTriggerID() { static int nextValidID = 0; return nextValidID++; }
            
        private:
            
            Game* _game;
            cocos2d::Map<int, GameTrigger*> _triggers;
            std::vector<int> _removeIDList;
            
        };
        
    }
}









