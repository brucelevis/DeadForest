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
#include "EntityType.hpp"

namespace realtrick
{
    
    class GMXLayer;
            
    class EditorEntityBase : public cocos2d::Node
    {
        
    public:
        
        EditorEntityBase(GMXLayer& layer, int id);
        
        virtual ~EditorEntityBase();
        
        virtual bool init() override;
        
        int getID() const { return _id; }
        
        void setPlayerType(PlayerType type) { _playerType = type; }
        
        PlayerType getPlayerType() const { return _playerType; }
        
        void setSelected(bool enable)
        {
            if ( _selectedCircle ) _selectedCircle->setVisible(enable);
        }
        
        EntityType getEntityType() const { return _type; }
            
    protected:
            
        GMXLayer& _gmxLayer;
        
        int _id;
        EntityType _type;
        cocos2d::Sprite* _body;
        cocos2d::Sprite* _shadow;
        cocos2d::ui::Widget::TextureResType _resType;
        PlayerType _playerType;
        cocos2d::Sprite* _selectedCircle = nullptr;
        
    };
        
}
        
        
        
        
        
        
        
        
