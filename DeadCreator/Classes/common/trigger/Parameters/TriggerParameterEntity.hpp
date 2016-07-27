//
//  TriggerParameterEntity.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "EditorEntity.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterEntity : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterEntity() : TriggerParameterBase(),
            _entityType(EntityType::DEFAULT)
            {
                _parameterType = TriggerParameterBase::Type::ENTITY;
                setParameterName("#invalid");
            }
            
            TriggerParameterEntity(const TriggerParameterEntity& rhs) { copyFrom(rhs); }
            TriggerParameterEntity& operator=(const TriggerParameterEntity& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterEntity& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _entityType = rhs._entityType;
                _currEntity = rhs._currEntity;
            }
            
            virtual ~TriggerParameterEntity() = default;
            
            EntityType getEntityType() const { return _entityType; }
            void setEntityType(EntityType entityType)
            {
                setParameterName(EditorEntity::getEntityTableByType().at(entityType).entityName);
                
                _entityType = entityType;
                
                auto& entityTable = EditorEntity::getEntityTableByType();
                int index = 0;
                for(auto iter = entityTable.begin(); iter != entityTable.end() ; ++ iter)
                {
                    if ( iter->first == entityType ) _currEntity = index;
                    ++ index;
                }
            }
            
            virtual TriggerParameterEntity* clone() const override
            {
                return new TriggerParameterEntity(*this);
            }
            
            virtual void reset() override { _currEntity = -1; }
            virtual bool isItemSelected() override { return (_currEntity != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(200);
                std::string entityList;
                std::vector<std::string> entityNames;
                auto& entityTable = EditorEntity::getEntityTableByType();
                for(auto iter = entityTable.begin(); iter != entityTable.end() ; ++ iter)
                {
                    entityList += (iter->second.entityName);
                    entityNames.push_back(iter->second.entityName);
                    entityList += '\0';
                }
                if ( ImGui::Combo("", &_currEntity, entityList.c_str(), 5) )
                {
                    setEntityType(EditorEntity::getEntityTableByName().at(entityNames[_currEntity]).entityType);
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            EntityType _entityType;
            int _currEntity = -1;
            
        };
        
    }
}









