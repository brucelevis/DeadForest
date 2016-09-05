//
//  InputMoveBegin.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 4..
//
//

#pragma once

#include "cocos2d.h"

#include "InputCommandBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class InputMoveBegin : public InputCommandBase
        {
          
        public:
            
            InputMoveBegin(HumanBase* human, const cocos2d::Vec2& dir) :
            _human(human),
            _moveDir(dir)
            {}
            virtual ~InputMoveBegin() = default;
            
            virtual void execute() override
            {
                if ( _human->isInventoryOpened() || !_human->isAlive() ) return ;
                
                _human->addInputMask(HumanBehaviorType::MOVE);
                _human->setMoving(_moveDir);
                
            }
            
        private:
            
            HumanBase* _human;
            cocos2d::Vec2 _moveDir;
            
        };
        
    }
}









