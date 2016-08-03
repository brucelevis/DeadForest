//
//  InputAttackEnd.hpp
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
        
        class InputAttackEnd : public InputCommandBase
        {
            
        public:
            
            explicit InputAttackEnd(HumanBase* human) :
            _human(human)
            {
                
            }
            virtual ~InputAttackEnd() = default;
            
            virtual void execute() override
            {
                _human->removeInputMask(HumanBehaviorType::ATTACK);
            }
            
        private:
            
            HumanBase* _human;
            
        };
        
    }
}