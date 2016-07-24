//
//  EntityHuman.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 15..
//
//

#pragma once

#include <stack>
#include <string>

#include "DynamicEntity.hpp"
#include "StateMachine.hpp"
#include "AnimationPlayer.hpp"
#include "MessageNode.hpp"
#include "AimingSystem.hpp"

namespace realtrick
{
    
    class WeaponBase;
    class GameManager;
    
    namespace userinterface
    {
        class Inventory;
        class WeaponStatus;
    }
    
    class EntityHuman : public DynamicEntity
    {
        
    public:
        
        static EntityHuman* create(GameManager* mgr);
        
        StateMachine<EntityHuman>* getFSM() const                   { return _FSM; }
        
        AnimationPlayer* getBodyAnimator() const                    { return _bodyAnimationPlayer; }
        
        bool isFovOn() const                                        { return _isFovOn; }
        
        void  setBodyRot(float rot)                                 { _bodyRot = rot; }
        
        float getBodyRot() const                                    { return _bodyRot; }
        
        bool isAlive() const                                        { return _isAlive; }
        
        void setDead();
        
        void setAlive();
        
        int getBlood() const                                        { return _blood; }
        
        void setBlood(int blood)                                    { _blood = blood; }
        
        WeaponBase* getEquipedWeapon() const                        { return _equipedWeapon; }
        
        void setEquipedWeapon(WeaponBase* newWeapon)                { _equipedWeapon = newWeapon; }
        
        int getInputMask() const                                    { return _inputMask; }
        
        void setInputMask(int mask)                                 { _inputMask = mask; }
        
        void addInputMask(int mask)                                 { _inputMask |= mask; }
        
        void removeInputMask(int mask)                              { if( isInputMasked(mask) ) _inputMask ^= mask; }
        
        bool isInputMasked(int mask)                                { return ((_inputMask & mask) == mask); }
        
        float getWalkSpeed() const { return _walkSpeed; }
        
        void setWalkSpeed(float speed) { _walkSpeed = speed; }
        
        float getRunSpeed() const { return _runSpeed; }
        
        void setRunSpeed(float speed) { _runSpeed = speed; }
        
        void enableWeaponEquipped(bool enable) { _isWeaponEquipCompletly = enable; }
        
        virtual bool handleMessage(const Telegram& msg) override;
        
        virtual void update(float dt) override;
        
        virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other) override;
        
        virtual bool isIntersectWall(const cocos2d::Vec2& futurePosition, const Polygon& wall) override;
        
        virtual void rotateEntity() override;
        
        std::string getCurrBodySpriteFrameName() const { return _bodyAnimationPlayer->getCurrFrameName(); }
        
        cocos2d::Sprite* getCurrBodySprite() const { return _bodyAnimationPlayer->getBaseSprite(); }
        
        cocos2d::Sprite* getCurrBodyShadowSprite() const { return _bodyAnimationPlayer->getShadowSprite(); }
        
        void setRunStats(bool enable) { _isRun = enable; }
        
        bool isRun() const { return _isRun; }
        
        bool isAiming() const { return _isAiming; }
        
        void enableAiming(bool enable) { _isAiming = enable; }
        
        void enableNormal(bool enable) override;
        
        void disableShadow();
        
        void setFootGauge(float g);
        
        float getFootGauge() const { return _footGauge; }
        
        userinterface::Inventory* getInventory() const { return _inventory; }
        
        userinterface::WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
        
        AimingSystem* getAimingSystem() const { return _aimingSystem; }
        
        void setUserNickName(const std::string name) { _userNickName = name; }
        
        std::string getUserNickName() const { return _userNickName; }
        
    private:
        
        StateMachine<EntityHuman>*      _FSM;
        
        AnimationPlayer*                _bodyAnimationPlayer;
        
        bool                            _isWeaponEquipCompletly;
        
        WeaponBase*                     _equipedWeapon;
        
        int                             _inputMask;
        
        int                             _blood;
        
        int                             _maxBlood;
        
        std::stack<int>                 _bleedingStack;
        
        std::stack<int>                 _healingStack;
        
        bool                            _isAlive;
        
        bool                            _isFovOn;
        
        float                           _bodyRot;
        
        float                           _walkSpeed;
        
        float                           _runSpeed;
        
        float                           _noEventTime;
        
        float                           _enduranceTime;
        
        bool                            _isRun;
        
        bool                            _isAiming;
        
        float                           _footGauge;
        
        userinterface::Inventory*       _inventory;
        
        userinterface::WeaponStatus*    _weaponStatus;
        
        AimingSystem*                   _aimingSystem;
        
        std::string                     _userNickName;
        
    private:
        
        EntityHuman(GameManager* mgr);
        
        virtual ~EntityHuman();
        
        virtual bool init() override;

    };
    
}





