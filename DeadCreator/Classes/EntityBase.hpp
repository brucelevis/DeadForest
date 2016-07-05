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
    PLAYER1 = 1,
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
    
    int getID() const { return _id; }
    
    virtual EntityBase* clone() const = 0;
    
    void setPlayerType(PlayerType type) { _playerType = type; }
    
    PlayerType getPlayerType() const { return _playerType; }
    
protected:
    
    GMXLayer2& _gmxLayer;
    
    int _id;
    EntityType _type;
    cocos2d::Sprite* _body;
    cocos2d::Sprite* _shadow;
    cocos2d::ui::Widget::TextureResType _resType;
    PlayerType _playerType;
    
};









