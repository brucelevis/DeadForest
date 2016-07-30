//
//  EntityZombie.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 30..
//
//

#pragma once

#include "DynamicEntity.hpp"
#include "StateMachine.hpp"
#include "AnimationPlayer.hpp"
#include "MessageNode.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityZombie : public DynamicEntity
        {
            
        public:
            
            explicit EntityZombie(GameManager* mgr);
            virtual ~EntityZombie();
            
            virtual bool init() override;
            static EntityZombie* create(GameManager* mgr);
            
            StateMachine<EntityZombie>* getFSM() const { return _FSM; }
            AnimationPlayer* getBodyAnimator() const { return _bodyAnimationPlayer; }
            
            void setBodyRot(float rot) { _bodyRot = rot; }
            float getBodyRot() const { return _bodyRot; }
            
            bool isAlive() const { return _isAlive; }
            void setDead();
            void setAlive();
            
            int getBlood() const { return _blood; }
            void setBlood(int blood) { _blood = blood; }
            
            float getWalkSpeed() const { return _walkSpeed; }
            void setWalkSpeed(float speed) { _walkSpeed = speed; }
            
            float getRunSpeed() const { return _runSpeed; }
            void setRunSpeed(float speed) { _runSpeed = speed; }
            
            virtual bool handleMessage(const Telegram& msg) override;
            
            virtual void update(float dt) override;
            
            virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other) override;
            virtual bool isIntersectWall(const cocos2d::Vec2& futurePosition, const Polygon& wall) override;
            
            virtual void rotateEntity() override;
            
            void setRunStats(bool enable) { _isRun = enable; }
            bool isRun() const { return _isRun; }
            
            void enableNormal(bool enable) override;
            
            void setFootGauge(float g);
            float getFootGauge() const { return _footGauge; }
            
            void setStateName(const std::string& name) { _stateName = name; }
            std::string getStateName() const { return _stateName; }
            
        private:
            
            StateMachine<EntityZombie>*     _FSM;
            AnimationPlayer*                _bodyAnimationPlayer;
            
            int                             _blood;
            int                             _maxBlood;
            
            bool                            _isAlive;
            bool                            _isRun;
            
            float                           _bodyRot;
            float                           _walkSpeed;
            float                           _runSpeed;
            float                           _footGauge;
            
            std::string                     _stateName;
            
        };
        
    }
}









