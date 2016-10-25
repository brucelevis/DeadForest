//
//  EditorEntity.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 4..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Types.hpp"
#include "EditorType.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        struct EntityData
        {
            EntityType entityType;
            PaletteType paletteType;
            std::string entityName;
            std::string fileName;
            
            EntityData() = default;
            
            EntityData(EntityType type, PaletteType palette, const std::string& entName, const std::string& fName) :
            entityType(type),
            paletteType(palette),
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
                
                _selectedCircle = cocos2d::DrawNode::create();
                _selectedCircle->setVisible(false);
                _selectedCircle->drawCircle(cocos2d::Vec2::ZERO, 20.0f, 360.0f, 30, false, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.5f));
                addChild(_selectedCircle);
                
                _boundingCircle = cocos2d::DrawNode::create();
                _boundingCircle->setVisible(false);
                addChild(_boundingCircle);
                
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
            void setSelected(bool enable) { if ( _selectedCircle ) _selectedCircle->setVisible(enable); }
            
            void setBoundingCircle(bool visible, const cocos2d::Color4F& color)
            {
                _boundingCircle->clear();
                _boundingCircle->drawCircle(cocos2d::Vec2::ZERO, _boundingRadius, 360.0f, 30, false, color);
                _boundingCircle->setVisible(visible);
            }
            void setBoundingRadius(float r) { _boundingRadius = r; }
            float getBoundingRadius() const { return _boundingRadius; }
            
            EntityType getEntityType() const { return _type; }
            void addFamilyMask(int mask) { _familyMask |= mask; }
            
            int getFamilyType() const { return _familyMask; }
            
            std::string getEntityName() const { return _entityName; }
            void setEntityName(const std::string& name) { _entityName = name; }
            
            static const std::map<EntityType, EntityData>& getEntityTableByType()
            {
                static std::map<EntityType, EntityData> table;
                static bool isFirstCall = true;
                if ( isFirstCall )
                {
                    table[EntityType::ENTITY_PLAYER] = EntityData(EntityType::ENTITY_PLAYER, PaletteType::HUMAN, "Sheriff", "editor/sheriff.png");
                    table[EntityType::ENTITY_ZOMBIE] = EntityData(EntityType::ENTITY_ZOMBIE, PaletteType::ENEMY, "Zombie1", "editor/zombie1.png");
                    table[EntityType::ENTITY_ZOMBIE2] = EntityData(EntityType::ENTITY_ZOMBIE2, PaletteType::ENEMY, "Zombie2", "editor/zombie2.png");
                    table[EntityType::ENTITY_ZOMBIE3] = EntityData(EntityType::ENTITY_ZOMBIE3, PaletteType::ENEMY, "Zombie3", "editor/zombie3.png");
                    table[EntityType::ITEM_M16A2] = EntityData(EntityType::ITEM_M16A2, PaletteType::ITEM, "M16A2", "editor/M16A2.png");
                    table[EntityType::ITEM_M1897] = EntityData(EntityType::ITEM_M1897, PaletteType::ITEM,"M1897", "editor/M1897.png");
                    table[EntityType::ITEM_GLOCK17] = EntityData(EntityType::ITEM_GLOCK17, PaletteType::ITEM,"Glock17", "editor/Glock17.png");
                    table[EntityType::ITEM_AXE] = EntityData(EntityType::ITEM_AXE, PaletteType::ITEM,"Axe", "editor/Axe.png");
                    table[EntityType::BULLET_556MM] = EntityData(EntityType::BULLET_556MM, PaletteType::ITEM,"5.56mm", "editor/5_56mm.png");
                    table[EntityType::BULLET_9MM] = EntityData(EntityType::BULLET_9MM, PaletteType::ITEM,"9mm", "editor/9mm.png");
                    table[EntityType::BULLET_SHELL] = EntityData(EntityType::BULLET_SHELL, PaletteType::ITEM,"Shotgun Shell", "editor/Shell.png");
                    table[EntityType::CONSUMABLE_BANDAGE] = EntityData(EntityType::CONSUMABLE_BANDAGE, PaletteType::ITEM, "Bandage", "editor/Bandage.png");
                    table[EntityType::CONSUMABLE_MEATCAN] = EntityData(EntityType::CONSUMABLE_MEATCAN, PaletteType::ITEM, "Meat can", "editor/MeatCan.png");
                    
                    isFirstCall = false;
                }
                return table;
            }
            
            static const std::map<std::string, EntityData>& getEntityTableByName()
            {
                static std::map<std::string, EntityData> table;
                static bool isFirstCall = true;
                if ( isFirstCall )
                {
                    table["Sheriff"] = EntityData(EntityType::ENTITY_PLAYER, PaletteType::HUMAN, "Sheriff", "editor/sheriff.png");
                    table["Zombie1"] = EntityData(EntityType::ENTITY_ZOMBIE, PaletteType::ENEMY, "Zombie1", "editor/zombie1.png");
                    table["Zombie2"] = EntityData(EntityType::ENTITY_ZOMBIE2, PaletteType::ENEMY, "Zombie2", "editor/zombie2.png");
                    table["Zombie3"] = EntityData(EntityType::ENTITY_ZOMBIE3, PaletteType::ENEMY, "Zombie3", "editor/zombie3.png");
                    table["M16A2"] = EntityData(EntityType::ITEM_M16A2, PaletteType::ITEM,"M16A2", "editor/M16A2.png");
                    table["M1897"] = EntityData(EntityType::ITEM_M1897, PaletteType::ITEM,"M1897", "editor/M1897.png");
                    table["Glock17"] = EntityData(EntityType::ITEM_GLOCK17, PaletteType::ITEM,"Glock17", "editor/Glock17.png");
                    table["Axe"] = EntityData(EntityType::ITEM_AXE, PaletteType::ITEM,"Axe", "editor/Axe.png");
                    table["5.56mm"] = EntityData(EntityType::BULLET_556MM, PaletteType::ITEM,"5.56mm", "editor/5_56mm.png");
                    table["9mm"] = EntityData(EntityType::BULLET_9MM, PaletteType::ITEM,"9mm", "editor/9mm.png");
                    table["Shotgun Shell"] = EntityData(EntityType::BULLET_SHELL, PaletteType::ITEM, "Shotgun Shell", "editor/Shell.png");
                    table["Bandage"] = EntityData(EntityType::CONSUMABLE_BANDAGE, PaletteType::ITEM, "Bandage", "editor/Bandage.png");
                    table["Meat can"] = EntityData(EntityType::CONSUMABLE_MEATCAN, PaletteType::ITEM, "Meat can", "editor/MeatCan.png");
                    
                    isFirstCall = false;
                }
                return table;
            }
            
        private:
            
            int _id = -1;
            EntityType _type = EntityType::DEFAULT;
            int _familyMask = 0;
            cocos2d::Sprite* _image = nullptr;
            PlayerType _playerType = PlayerType::INVALID;
            cocos2d::DrawNode* _selectedCircle = nullptr;
            cocos2d::DrawNode* _boundingCircle = nullptr;
            float _boundingRadius = 20.0f;
            std::string _entityName;
            
        };
        
    }
}









