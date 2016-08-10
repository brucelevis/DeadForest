//
//  StateMachine.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include "Telegram.hpp"
#include "State.hpp"
#include "HumanOwnedStates.hpp"


namespace realtrick
{
    namespace client
    {
        
        class StateMachine
        {
            
        public:
            
            explicit StateMachine(HumanBase* owner) :
            _owner(owner),
            _currState(nullptr),
            _prevState(nullptr)
            {}
            
            virtual ~StateMachine() = default;
            
            void setCurrState(State* s)     { _currState = s; }
            void setPrevState(State* s)     { _prevState = s; }
            
            State* getCurrState() const     { return _currState; }
            State* getPrevState() const     { return _prevState; }
            
            void update(float dt)
            {
                if (_currState) _currState->execute(_owner);
            }
            
            bool handleMessage(const Telegram& msg)
            {
                return ( _currState && _currState->onMessage(_owner, msg) );
            }
            
            void changeState(State* newState)
            {
                _prevState = _currState;
                _currState->exit(_owner);
                _currState = newState;
                _currState->enter(_owner);
            }
            
            void revertToPreviousState()
            {
                changeState(_prevState);
            }
            
        private:
            
            HumanBase*              _owner;
            State*                  _currState;
            State*                  _prevState;
            
        };
        
    }
}









