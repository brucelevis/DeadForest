//
//  InputRotated.hpp
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
        
        class InputBezelBegin : public InputCommandBase
        {
            
        public:
            
            InputBezelBegin(HumanBase* human, const cocos2d::Vec2& dir) :
            _rotateDir(dir),
            _human(human)
            {
                
            }
            virtual ~InputBezelBegin() = default;
            
            virtual void execute() override
            {
                if ( _human->isInventoryOpened() || !_human->isAlive() ) return ;
            
                _human->setTargetHeading(_rotateDir);
                _human->addInputMask(HumanBehaviorType::TURN);
                
            }
            
        private:
            
            HumanBase* _human;
            cocos2d::Vec2 _rotateDir;
            
        };
        
    }
}









