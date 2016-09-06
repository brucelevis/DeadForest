//
//  ItemExtensionButtons.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 23..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class ItemBase;
        class Game;
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
            
            explicit ItemExtensionButtons(Game* game);
            virtual ~ItemExtensionButtons();
            
            static ItemExtensionButtons* create(Game* game, ItemSlot* slot);
            
            bool init(ItemSlot* slot);
            
            void showButtons();
            void hideButtons();
            void setButtonSize(const cocos2d::Size& size);
            
        private:
            
            Game*                    _game;
            ItemSlot*                       _ownSlot;
            std::vector<AnimatedButton*>    _buttons;
            
        };
        
    }
}









