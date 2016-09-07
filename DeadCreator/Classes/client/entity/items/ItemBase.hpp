//
//  ItemBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#pragma once

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        class EntityPlayer;
        class Game;
        
        class ItemBase : public EntityBase
        {
            
        public:
            
            explicit ItemBase(Game* game);
            ItemBase(const ItemBase& rhs);
            
            virtual ~ItemBase();
            
            void setOwner(HumanBase* owner) { _owner=  owner; }
            HumanBase* getOwner() const { return _owner; }
            
            void setAmount(int amount) { _amount = amount; }
            int getAmount() const { return _amount; }
            
            void setInSlotFrameName(const std::string& name) { _inSlotFrameName = name; }
            void setInSlotSpriteSize(const cocos2d::Size& size) { _inSlotSpriteSize = size; }
            
            std::string getInSlotFrameName() const { return _inSlotFrameName; }
            cocos2d::Size getInSlotSpriteSize() const { return _inSlotSpriteSize; }
            
            cocos2d::ui::Widget::TextureResType getTexType() const { return _texType; }
            
            virtual ItemBase* clone() const = 0;
            virtual void use() = 0;
            virtual void discard() = 0;
            virtual void releaseWeapon() {}
            
            virtual void enableNormal(bool enable) override;
            
        protected:
            
            HumanBase*                              _owner;
            int                                     _amount;
            
            std::string                             _inSlotFrameName;
            cocos2d::Size                           _inSlotSpriteSize;
            
            std::string                             _spriteName;
            
            cocos2d::Sprite*                        _inGameImage_n;
            cocos2d::Sprite*                        _inGameImage_s;
            cocos2d::Sprite*                        _normalMap;
            cocos2d::Sprite*                        _inSlotImage;
            
            cocos2d::ui::Widget::TextureResType     _texType;
            
        protected:
            
            bool init(const std::string& inGameImage_n, const std::string& inGameImage_s,
                      const std::string& inSlotImage, cocos2d::ui::Widget::TextureResType texResType);
            
        };
        
    }
}









