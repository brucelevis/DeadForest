//
//  EntityPlayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#pragma once

#include "HumanBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityPlayer : public HumanBase
        {
            
        public:
            
            explicit EntityPlayer(Game* game);
            virtual ~EntityPlayer();
            
            virtual bool init() override;
            static EntityPlayer* create(Game* game);

            virtual bool handleMessage(const Telegram& msg) override;
            virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other, cocos2d::Vec2& additionalVelocity) override;
            
            virtual void update(float dt) override;
            virtual void suicide() override;
            
        };
        
    }
}









