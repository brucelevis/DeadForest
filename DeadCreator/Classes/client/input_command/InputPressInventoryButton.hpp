//
//  InputPressInventoryButton.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 6..
//
//

#pragma once

#include "cocos2d.h"

#include "InputCommandBase.hpp"
#include "Game.hpp"
#include "InventoryView.hpp"

namespace realtrick
{
    namespace client
    {
        
        class InputPressInventoryButton : public InputCommandBase
        {
            
        public:
            
            explicit InputPressInventoryButton(InventoryView* view) :
            _view(view)
            {}
            virtual ~InputPressInventoryButton() = default;
            
            virtual void execute() override
            {
                _view->setVisible(!_view->isVisible());
                _view->getGame()->getPlayerPtr()->setInventoryOpened(_view->isVisible());
            }
            
        private:
            
            InventoryView* _view;
            
        };
        
    }
}

