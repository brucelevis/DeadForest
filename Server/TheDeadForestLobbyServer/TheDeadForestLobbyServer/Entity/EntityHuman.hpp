
#pragma once

#include <stack>
#include <string>

#include "DynamicEntity.hpp"
#include "StateMachine.hpp"
#include "AnimationPlayer.hpp"
#include "MessageNode.hpp"
#include "ParamLoader.hpp"
#include "GameManager.hpp"
#include "Room.h"
#include "HumanGlobalState.hpp"

namespace realtrick
{
    
    class GameManager;
    
    class EntityHuman : public DynamicEntity
    {
        
    public:
        
        static EntityHuman* create(GameManager* mgr);

		StateMachine<EntityHuman>* getFSM() const { return _FSM; }

		AnimationPlayer* getBodyAnimator() const { return _bodyAnimationPlayer; }
        
        bool isFovOn() const                                        { return _isFovOn; }
        
        void  setBodyRot(float rot)                                 { _bodyRot = rot; }
        
        float getBodyRot() const                                    { return _bodyRot; }
        
        bool isAlive() const                                        { return _isAlive; }
        
        void setDead();
        
        void setAlive();
        
        int getBlood() const                                        { return _blood; }
        
        void setBlood(int blood)                                    { _blood = blood; }

		int getInputMask() const { return _inputMask; }

		void setInputMask(int mask) { _inputMask = mask; }

		void addInputMask(int mask) { _inputMask |= mask; }

		void removeInputMask(int mask) { if (isInputMasked(mask)) _inputMask ^= mask; }

		bool isInputMasked(int mask) { return ((_inputMask & mask) == mask); }
        
        float getWalkSpeed() const { return _walkSpeed; }
        
        void setWalkSpeed(float speed) { _walkSpeed = speed; }
        
        float getRunSpeed() const { return _runSpeed; }
        
        void setRunSpeed(float speed) { _runSpeed = speed; }
        
        virtual bool handleMessage(const Telegram& msg) override;
        
        virtual void update(float dt) override;
        
        virtual bool isIntersectOther(const Vec2& futurePosition, EntityBase* other) override;
        
        virtual bool isIntersectWall(const Vec2& futurePosition, const geometry::Polygon& wall) override;
        
        virtual void rotateEntity(float dt) override;
        
        void setRunStats(bool enable) { _isRun = enable; }
        
        bool isRun() const { return _isRun; }

		bool getIsLoad() const { return _isLoad; }

		void setIsLoad(int isLoad) { _isLoad = isLoad; }
		
    private:    

		StateMachine<EntityHuman>*      _FSM;

		AnimationPlayer*                _bodyAnimationPlayer;

		int                             _pId;

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
        
        float                           _footGauge;

		bool                            _isLoad;
        
    private:
        
        EntityHuman(GameManager* mgr);
        
        virtual ~EntityHuman();
        
        virtual bool init();

    };
    
}





