//
//  HumanBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 15..
//
//

#pragma once

#include "StateMachine.hpp"
#include "Animator.hpp"
#include "Physics.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        class BrainBase;
		class PathPlanner;
        
        class HumanBase : public EntityBase
        {
            
        public:
            
            explicit HumanBase(Game* game);
            virtual ~HumanBase();
            
            virtual bool init() override;
            static HumanBase* create(Game* game);
            
            void setFootGauge(float g);
            
            virtual void update(float dt) override;
            virtual void enableNormal(bool enable) override;
            virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other);
            virtual bool isIntersectWall(const cocos2d::Vec2& futurePosition, const realtrick::Polygon& wall);
            virtual void rotateEntity();
            virtual void moveEntity();
            virtual void suicide() {}

            virtual bool handleMessage(const Telegram& msg) override;
        
            Animator* getAnimator() const { return _animator; }
            StateMachine* getFSM() const { return _FSM; }
			PathPlanner* getPathPlanner() const { return _pathPlanner; }
            
            cocos2d::Vec2 getHeading() const { return _heading; }
            
            cocos2d::Vec2 getTargetHeading() const { return _targetHeading; }
            void setTargetHeading(const cocos2d::Vec2& target) { _targetHeading = target; }
            
            cocos2d::Vec2 getLeft() const { return _heading.getPerp(); }
            cocos2d::Vec2 getRight() const { return _heading.getRPerp(); }
            
            cocos2d::Vec2 getMoving() const { return _moving; }
            void setMoving(const cocos2d::Vec2& moving) { _moving = moving; }
            
            cocos2d::Vec2 getVelocity() const { return _velocity; }
            void setVelocity(const cocos2d::Vec2 velocity) { _velocity = velocity; }
            
            float getTurnSpeed() const { return _turnSpeed; }
            void setTurnSpeed(float speed) { _turnSpeed = speed; }
            
            bool isAlive() const { return _isAlive; }
            void setDead() { setVelocity(cocos2d::Vec2::ZERO); _isAlive = false; }
            void setAlive() { _isAlive = true; _blood = _maxBlood; }
            
            int getBlood() const { return _blood; }
            void setBlood(int blood) { _blood = blood; }
            
            int getInputMask() const { return _inputMask; }
            void setInputMask(int mask) { _inputMask = mask; }
            void addInputMask(int mask) { _inputMask |= mask; }
            void removeInputMask(int mask) { if( isInputMasked(mask) ) _inputMask ^= mask; }
            bool isInputMasked(int mask) { return ((_inputMask & mask) == mask); }
            
            float getWalkSpeed() const { return _walkSpeed; }
            void setWalkSpeed(float speed) { _walkSpeed = speed; }
            
            float getRunSpeed() const { return _runSpeed; }
            void setRunSpeed(float speed) { _runSpeed = speed; }
            
            void setRotationZ(float rot) { _rotation = rot; }
            float getRotationZ() const { return _rotation; }
            
            void setRunStats(bool enable) { _isRun = enable; }
            bool isRun() const { return _isRun; }
            
            void setStateName(const std::string& name) { _stateName = name; }
            std::string getStateName() const { return _stateName; }
            
            void setInventoryOpened(bool enable) { _isInventoryOpened = enable; }
            bool isInventoryOpened() const { return _isInventoryOpened; }
            
            void setBrain(BrainBase* brain);
            
            void hittedByWeapon(EntityType type, int damage);
            
        private:
        
            void setHeading(const cocos2d::Vec2 heading) { _heading = heading; _heading.normalize(); }
            
        protected:
            
            Animator*                       _animator;
            StateMachine*                   _FSM;
            BrainBase*                      _brain;
			PathPlanner*					_pathPlanner;

            cocos2d::Vec2                   _heading;
            cocos2d::Vec2                   _targetHeading;
            cocos2d::Vec2                   _moving;
            cocos2d::Vec2                   _left;
            cocos2d::Vec2                   _right;
            cocos2d::Vec2                   _velocity;
            
            float                           _turnSpeed;
            float                           _speed;
            
            int                             _inputMask;
            int                             _blood;
            int                             _maxBlood;
            
            bool                            _isAlive;
            bool                            _isRun;
            bool                            _isFovOn;
            bool                            _isInventoryOpened;

            float                           _walkSpeed;
            float                           _runSpeed;
            float                           _footGauge;
            float                           _rotation;
        
            std::string                     _stateName;
            
        };
        
    }
}









