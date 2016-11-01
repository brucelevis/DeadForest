//
//  Wall.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include "cocos2d.h"
#include "PhysicsBase.hpp"

namespace realtrick
{
    namespace client
    {

        class Wall : public cocos2d::Ref, public PhysicsBase
        {

		public:

			explicit Wall(b2World* world);
			virtual ~Wall();

			static Wall* create(b2World* world, const std::vector<cocos2d::Vec2>& vertices);
			bool init(const std::vector<cocos2d::Vec2>& vertices);

			virtual cocos2d::Vec2 getVelocity() const override;
			virtual void setVelocity(const cocos2d::Vec2& velocity) override;

			virtual cocos2d::Vec2 getWorldPosition() const override;
			virtual void setWorldPosition(const cocos2d::Vec2& pos) override;

			virtual cocos2d::Vec2 getHeading() const override;
			virtual float getRotationZ() const override;

		private:

			virtual void setHeading(const cocos2d::Vec2& heading) override;

		protected:

			b2World* _world;
			b2Body* _body;
        };
        
    }
}









