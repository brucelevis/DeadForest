//
//  InputMoveEnd.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 4..
//
//

#pragma once

#include "cocos2d.h"

#include "InputCommandBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class InputMoveEnd : public InputCommandBase
        {
            
        public:
            
            explicit InputMoveEnd(HumanBase* human) :
            _human(human)
            {
                
            }
            virtual ~InputMoveEnd() = default;
            
            virtual void execute() override
            {
                _human->removeInputMask(HumanBehaviorType::MOVE);
            }
            
        private:
            
            HumanBase* _human;
            
        };
        
    }
}









