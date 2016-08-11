//
//  InputAttackBegin.hpp
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
        
        class InputAttackBegin : public InputCommandBase
        {
            
        public:
            
            explicit InputAttackBegin(HumanBase* human) :
            _human(human)
            {
                
            }
            virtual ~InputAttackBegin() = default;
            
            virtual void execute() override
            {
                if ( !_human->isInventoryOpened() ) _human->addInputMask(HumanBehaviorType::ATTACK);
            }
            
        private:
            
            HumanBase* _human;
            
        };
        
    }
}









