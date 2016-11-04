//
//  PhysicsBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include "cocos2d.h"
#include <Box2D/Box2D.h>

namespace realtrick
{
    namespace client
    {

        class PhysicsBase
        {

		public:

			enum Type
			{
				kHuman, kWall, kItem
			};

			PhysicsBase();
			virtual ~PhysicsBase();

			virtual bool initWithPhysicsBody(b2World* world, const b2BodyDef& bd, const b2FixtureDef& fd, int type);

			virtual cocos2d::Vec2 getWorldPosition() const = 0;
			virtual void setWorldPosition(const cocos2d::Vec2& pos) = 0;

			virtual cocos2d::Vec2 getVelocity() const = 0;
			virtual void setVelocity(const cocos2d::Vec2& velocity) = 0;

			virtual cocos2d::Vec2 getHeading() const = 0;

			virtual float getRotationZ() const = 0;

			inline int getType() const { return _type; }

		protected:

			b2Body* _body;
			int _type;

		private:

			virtual void setHeading(const cocos2d::Vec2& heading) = 0;

        };
        
    }
}









