//
//  EntityBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GMXLayer2;

enum class EntityType : int
{
    INVALID = -1,
    SHERIFF = 0,
    ITEM_556MM,
    ITEM_9MM,
    ITEM_SHELL,
    ITEM_GLOCK17,
    ITEM_M16A2,
    ITEM_M1897,
    ITEM_AXE,
};
    
enum class PlayerType : int
{
    NEUTRAL = 0,
    PLAYER1,
    PLAYER2,
    PLAYER3,
    PLAYER4,
    PLAYER5,
    PLAYER6,
    PLAYER7,
    PLAYER8,
};

class EntityBase : public cocos2d::Node
{
    
public:
    
    EntityBase(GMXLayer2& layer, int id);
    
    virtual ~EntityBase();
    
    virtual bool init() override;
    
    int getID() const { return _id; }
    
    void setPlayerType(PlayerType type) { _playerType = type; }
    
    PlayerType getPlayerType() const { return _playerType; }
    
    virtual cocos2d::Rect getBoundingBox() const override
    {
        return cocos2d::Rect(getPositionX() - _boundingBox.size.width / 2,
                             getPositionY() - _boundingBox.size.height / 2,
                             _boundingBox.size.width,
                             _boundingBox.size.height);
    }
    
    virtual void setBoundingBox(const cocos2d::Rect& aabb);
    
    void setVisibleAABB(bool enable) { if ( _boundingBoxNode ) _boundingBoxNode->setVisible(enable); }
    
    void setSelected(bool enable)
    {
        if ( _selectedCircle ) _selectedCircle->setVisible(enable);
    }
    
protected:
    
    GMXLayer2& _gmxLayer;
    
    int _id;
    EntityType _type;
    cocos2d::Sprite* _body;
    cocos2d::Sprite* _shadow;
    cocos2d::ui::Widget::TextureResType _resType;
    PlayerType _playerType;
    cocos2d::Rect _boundingBox;
    cocos2d::DrawNode* _boundingBoxNode = nullptr;
    cocos2d::Sprite* _selectedCircle = nullptr;
    
};









