//
//  EntityBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
		class PhysicsBase;
        class IPhysics
        {
		public:
			virtual PhysicsBase& getPhysicsBase() const = 0;
			virtual cocos2d::Vec2 getWorldPosition() const = 0;
			virtual void setWorldPosition(const cocos2d::Vec2& pos) = 0;
        };
        
    }
}









