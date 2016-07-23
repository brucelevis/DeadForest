//
//  ItemExtensionButtons.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 23..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    class ItemBase;
    class GameManager;
    
    namespace userinterface
    {
    
        class AnimatedButton;
        class ItemSlot;
        
        class ItemExtensionButtons : public cocos2d::Node
        {
            
        private:
            
            enum ButtonType
            {
                USE = 0,
                EQUIP,
                RELEASE,
                DISCARD,
                EXAMINE,
                MAX
            };
            
        public:
            
            static ItemExtensionButtons* create(GameManager* mgr, ItemSlot* slot);
            
            void showButtons();
            
            void hideButtons();
            
            void setButtonSize(const cocos2d::Size& size);
            
        private:
            
            GameManager*                    _gameMgr;
            
            ItemSlot*                       _ownSlot;
            
            std::vector<AnimatedButton*>    _buttons;
            
        private:
            
            ItemExtensionButtons(GameManager* mgr);
            
            virtual ~ItemExtensionButtons();
            
            bool init(ItemSlot* slot);
            
        };
        
    }
}









