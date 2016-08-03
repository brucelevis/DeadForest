//
//  InputRotated.hpp
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
        
        class InputRotated : public InputCommandBase
        {
            
        public:
            
            InputRotated(HumanBase* human, const cocos2d::Vec2& dir) :
            _rotateDir(dir),
            _human(human)
            {
                
            }
            virtual ~InputRotated() = default;
            
            virtual void execute() override
            {
                _human->setTargetHeading(_rotateDir);
            }
            
        private:
            
            HumanBase* _human;
            cocos2d::Vec2 _rotateDir;
            
        };
        
    }
}