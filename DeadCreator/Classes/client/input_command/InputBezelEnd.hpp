//
//  InputBezelEnd.hpp
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
        
        class InputBezelEnd : public InputCommandBase
        {
            
        public:
            
            explicit InputBezelEnd(HumanBase* human) :
            _human(human)
            {
                
            }
            virtual ~InputBezelEnd() = default;
            
            virtual void execute() override
            {
                _human->removeInputMask(HumanBehaviorType::TURN);
            }
            
        private:
            
            HumanBase* _human;
            
        };
        
    }
}









