//
//  ItemBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#pragma once

#include "GameObject.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        class EntityHuman;
        class Game;
        
        class ItemBase : public GameObject
        {
            
        public:
            
            explicit ItemBase(Game* game);
            ItemBase(const ItemBase& rhs);
            
            virtual ~ItemBase();
            
            void setOwner(EntityHuman* owner) { _owner=  owner; }
            EntityHuman* getOwner() const { return _owner; }
            
            void setAmount(int amount) { _amount = amount; }
            int getAmount() const { return _amount; }
            
            void setInSlotFrameName(const std::string& name) { _inSlotFrameName = name; }
            void setInSlotSpriteSize(const cocos2d::Size& size) { _inSlotSpriteSize = size; }
            
            std::string getInSlotFrameName() const { return _inSlotFrameName; }
            cocos2d::Size getInSlotSpriteSize() const { return _inSlotSpriteSize; }
            void setInGameFrameName_n(const std::string& name) { _inGameImageName_n = name; }
            void setInGameFrameName_s(const std::string& name) { _inGameImageName_s = name; }
            std::string getInGameFrameName_n() const { return _inGameImageName_n; }
            std::string getInGameFrameName_s() const { return _inGameImageName_s; }
            
            cocos2d::ui::Widget::TextureResType getTexType() const { return _texType; }
            
            void enableNormal(bool enable) override { _inGameNormal->setVisible(enable); }
            
            virtual ItemBase* clone() const = 0;
            virtual void use() = 0;
            virtual void discard() = 0;
            virtual void releaseWeapon() {}
            
        protected:
            
            EntityHuman*                            _owner;
            int                                     _amount;
            
            std::string                             _inSlotFrameName;
            cocos2d::Size                           _inSlotSpriteSize;
            std::string                             _inGameImageName_n;
            std::string                             _inGameImageName_s;
            
            cocos2d::Sprite*                        _inGameImage_n;
            cocos2d::Sprite*                        _inGameImage_s;
            cocos2d::Sprite*                        _inSlotImage;
            cocos2d::Sprite*                        _inGameNormal;
            
            cocos2d::ui::Widget::TextureResType     _texType;
            
        protected:
            
            bool init(const std::string& inGameImage_n, const std::string& inGameImage_s,
                      const std::string& inSlotImage, cocos2d::ui::Widget::TextureResType texResType);
            
        };
        
    }
}









