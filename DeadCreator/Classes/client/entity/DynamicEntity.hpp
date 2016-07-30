//
//  DynamicEntity.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 2..
//
//

#pragma once

#include "cocos2d.h"
#include "EntityBase.hpp"
#include "Physics.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GameManager;
        
        class DynamicEntity : public EntityBase
        {
            
        public:
            
            explicit DynamicEntity(GameManager* gameMgr);
            virtual ~DynamicEntity() = default;
            
            cocos2d::Vec2 getHeading() const { return _heading; }
            void setHeading(const cocos2d::Vec2 heading) { _heading = heading; _heading.normalize(); }
            
            cocos2d::Vec2 getTargetHeading() const { return _targetHeading; }
            void setTargetHeading(const cocos2d::Vec2& target) { _targetHeading = target; }
            
            cocos2d::Vec2 getLeft() const { return _heading.getPerp(); }
            cocos2d::Vec2 getRight() const { return _heading.getRPerp(); }
            cocos2d::Vec2 getMoving() const { return _moving; }
            void setMoving(const cocos2d::Vec2& moving) { _moving = moving; }
            
            cocos2d::Vec2 getVelocity() const { return _velocity; }
            void setVelocity(const cocos2d::Vec2 velocity) { _velocity = velocity; }
            
            float getMaxSpeed() const { return _maxSpeed; }
            void setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }
            
            float getTurnSpeed() const { return _turnSpeed; }
            void setTurnSpeed(float speed) { _turnSpeed = speed; }
            
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            
            virtual void update(float dt) override {}
            
            virtual void moveEntity() final;
            virtual void rotateEntity() {}
            
            virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other) { return true; }
            virtual bool isIntersectWall(const cocos2d::Vec2& futurePosition, const Polygon& wall) { return true; }
            
        protected:
            
            cocos2d::Vec2               _heading;
            cocos2d::Vec2               _targetHeading;
            cocos2d::Vec2               _moving;
            cocos2d::Vec2               _left;
            cocos2d::Vec2               _right;
            cocos2d::Vec2               _velocity;
            
            float                       _maxSpeed;
            float                       _turnSpeed;
            float                       _speed;
            
        };
        
    }
}









