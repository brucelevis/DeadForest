//
//  EditorEntityBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class GMXLayer2;
    
    enum class EditorEntityType : int
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
        INVALID = -1,
        PLAYER1 = 0,
        PLAYER2,
        PLAYER3,
        PLAYER4,
        PLAYER5,
        PLAYER6,
        PLAYER7,
        PLAYER8,
        NEUTRAL = 12,
    };
            
    class EditorEntityBase : public cocos2d::Node
    {
        
    public:
        
        EditorEntityBase(GMXLayer2& layer, int id);
        
        virtual ~EditorEntityBase();
        
        virtual bool init() override;
        
        int getID() const { return _id; }
        
        void setPlayerType(PlayerType type) { _playerType = type; }
        
        PlayerType getPlayerType() const { return _playerType; }
        
        void setSelected(bool enable)
        {
            if ( _selectedCircle ) _selectedCircle->setVisible(enable);
        }
        
        EditorEntityType getEntityType() const { return _type; }
            
    protected:
            
        GMXLayer2& _gmxLayer;
        
        int _id;
        EditorEntityType _type;
        cocos2d::Sprite* _body;
        cocos2d::Sprite* _shadow;
        cocos2d::ui::Widget::TextureResType _resType;
        PlayerType _playerType;
        cocos2d::Sprite* _selectedCircle = nullptr;
        
    };
        
}
        
        
        
        
        
        
        
        
