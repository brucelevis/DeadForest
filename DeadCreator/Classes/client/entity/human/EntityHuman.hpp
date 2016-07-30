//
//  EntityHuman.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 15..
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
        
        class WeaponBase;
        class GameManager;
        class Inventory;
        class WeaponStatus;
        
        class EntityHuman : public DynamicEntity
        {
            
        public:
            
            explicit EntityHuman(GameManager* mgr);
            virtual ~EntityHuman();
            
            virtual bool init() override;
            static EntityHuman* create(GameManager* mgr);
            
            void setFootGauge(float g);
            
            virtual void update(float dt) override;
            virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other) override;
            virtual bool isIntersectWall(const cocos2d::Vec2& futurePosition, const Polygon& wall) override;
            virtual void rotateEntity() override;

            virtual bool handleMessage(const Telegram& msg) override { return _FSM->handleMessage(msg); }
            virtual void enableNormal(bool enable) override { getBodyAnimator()->enableNormal(enable); }
            
            StateMachine<EntityHuman>* getFSM() const { return _FSM; }
            AnimationPlayer* getBodyAnimator() const { return _bodyAnimationPlayer; }
            
            bool isFovOn() const { return _isFovOn; }
            
            bool isAlive() const { return _isAlive; }
            void setDead() { _isAlive = false; }
            void setAlive() { _isAlive = true; _blood = _maxBlood; }
            
            int getBlood() const { return _blood; }
            void setBlood(int blood) { _blood = blood; }
            
            WeaponBase* getEquipedWeapon() const { return _equipedWeapon; }
            void setEquipedWeapon(WeaponBase* newWeapon) { _equipedWeapon = newWeapon; }
            
            int getInputMask() const { return _inputMask; }
            void setInputMask(int mask) { _inputMask = mask; }
            void addInputMask(int mask) { _inputMask |= mask; }
            void removeInputMask(int mask) { if( isInputMasked(mask) ) _inputMask ^= mask; }
            bool isInputMasked(int mask) { return ((_inputMask & mask) == mask); }
            
            float getWalkSpeed() const { return _walkSpeed; }
            void setWalkSpeed(float speed) { _walkSpeed = speed; }
            
            float getRunSpeed() const { return _runSpeed; }
            void setRunSpeed(float speed) { _runSpeed = speed; }
            
            void setBodyRot(float rot) { _bodyRot = rot; }
            float getBodyRot() const { return _bodyRot; }
            
            void setRunStats(bool enable) { _isRun = enable; }
            bool isRun() const { return _isRun; }
            
            Inventory* getInventory() const { return _inventory; }
            WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
            
            void setUserNickName(const std::string& name) { _userNickName = name; }
            std::string getUserNickName() const { return _userNickName; }
            
            void setStateName(const std::string& name) { _stateName = name; }
            std::string getStateName() const { return _stateName; }

        private:
            
            StateMachine<EntityHuman>*      _FSM;
            AnimationPlayer*                _bodyAnimationPlayer;
            WeaponBase*                     _equipedWeapon;
            
            int                             _inputMask;
            int                             _blood;
            int                             _maxBlood;
            
            bool                            _isAlive;
            bool                            _isRun;
            bool                            _isFovOn;

            float                           _walkSpeed;
            float                           _runSpeed;
            float                           _footGauge;
            float                           _bodyRot;
            
            Inventory*                      _inventory;
            WeaponStatus*                   _weaponStatus;
            
            std::string                     _userNickName;
            std::string                     _stateName;
            
        };
        
    }
}









