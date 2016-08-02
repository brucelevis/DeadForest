//
//  ComponentBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"
#include "Types.hpp"
#include "MessageNode.hpp"


namespace realtrick
{
    namespace client
    {
     
        class Telegram;
        
        class ComponentBase : public cocos2d::Ref, public MessageNode
        {
            
        public:
            
            virtual ~ComponentBase() = default;
            
            ComponentType getType() const { return _type; }
            
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            
            virtual void update(float dt) = 0;
            virtual void clear() = 0;
            
        protected:
            
            ComponentType _type;
            
        };
        
    }
}