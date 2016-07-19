//
//  EditorEntity.hpp
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
    
    struct EntityData
    {
        EntityType entityType;
        std::string entityName;
        std::string fileName;
        
        EntityData() = default;
        
        EntityData(EntityType type, const std::string& entName, const std::string& fName) :
        entityType(type),
        entityName(entName),
        fileName(fName)
        {}
    };
    
    class EditorEntity : public cocos2d::Node
    {
        
    public:
        
        EditorEntity() = default;
        virtual ~EditorEntity() = default;
        
        bool init(int ID, EntityType type)
        {
            if ( !Node::init() )
                return false;
            
            _id = ID;
            _type = type;
            
            _image = cocos2d::Sprite::create(getEntityTableByType().at(type).fileName);
            addChild(_image);
            
            _selectedCircle = cocos2d::Sprite::create("circle.png");
            _selectedCircle->setVisible(false);
            addChild(_selectedCircle);
            
            return true;
        }
        
        static EditorEntity* create(int ID, EntityType type)
        {
            auto ret = new (std::nothrow) EditorEntity();
            if ( ret && ret->init(ID, type) )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        int getID() const { return _id; }
        
        void setPlayerType(PlayerType type) { _playerType = type; }
        PlayerType getPlayerType() const { return _playerType; }
        
        void setSelected(bool enable)
        {
            if ( _selectedCircle ) _selectedCircle->setVisible(enable);
        }
        
        EntityType getEntityType() const { return _type; }
        
        static const std::map<EntityType, EntityData>& getEntityTableByType()
        {
            static std::map<EntityType, EntityData> table;
            static bool isFirstCall = true;
            if ( isFirstCall )
            {
                table[EntityType::ENTITY_HUMAN] = EntityData(EntityType::ENTITY_HUMAN, "Sheriff", "sheriff.png");
                table[EntityType::ITEM_M16A2] = EntityData(EntityType::ITEM_M16A2, "M16A2", "M16A2.png");
                table[EntityType::ITEM_M1897] = EntityData(EntityType::ITEM_M1897, "M1897", "M1897.png");
                table[EntityType::ITEM_GLOCK17] = EntityData(EntityType::ITEM_GLOCK17, "Glock17", "Glock17.png");
                table[EntityType::ITEM_AXE] = EntityData(EntityType::ITEM_AXE, "Axe", "Axe.png");
                table[EntityType::BULLET_556MM] = EntityData(EntityType::BULLET_556MM, "5.56mm", "5_56mm.png");
                table[EntityType::BULLET_9MM] = EntityData(EntityType::BULLET_9MM, "9mm", "9mm.png");
                table[EntityType::BULLET_SHELL] = EntityData(EntityType::BULLET_SHELL, "Shotgun Shell", "Shell.png");
                
                isFirstCall = false;
            }
            return table;
        }
        
        static const std::map<std::string, EntityData> getEntityTableByName()
        {
            static std::map<std::string, EntityData> table;
            static bool isFirstCall = true;
            if ( isFirstCall )
            {
                table["Sheriff"] = EntityData(EntityType::ENTITY_HUMAN, "Sheriff", "sheriff.png");
                table["M16A2"] = EntityData(EntityType::ITEM_M16A2, "M16A2", "M16A2.png");
                table["M1897"] = EntityData(EntityType::ITEM_M1897, "M1897", "M1897.png");
                table["Glock17"] = EntityData(EntityType::ITEM_GLOCK17, "Glock17", "Glock17.png");
                table["Axe"] = EntityData(EntityType::ITEM_AXE, "Axe", "Axe.png");
                table["5.56mm"] = EntityData(EntityType::BULLET_556MM, "5.56mm", "5_56mm.png");
                table["9mm"] = EntityData(EntityType::BULLET_9MM, "9mm", "9mm.png");
                table["Shotgun Shell"] = EntityData(EntityType::BULLET_SHELL, "Shotgun Shell", "Shell.png");
                
                isFirstCall = false;
            }
            return table;
        }
        
    private:
        
        int _id;
        EntityType _type;
        cocos2d::Sprite* _image;
        PlayerType _playerType;
        cocos2d::Sprite* _selectedCircle = nullptr;
        
    };
        
}
        
        
        
        
        
        
        
        
