//
//  ConditionBring.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include "ConditionBase.hpp"
#include "TriggerParameters.hpp"
#include "EditorEntity.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ConditionBring : public ConditionBase
        {
            
        public:
            
            ConditionBring() { name() = "Bring"; }
            ConditionBring(const ConditionBring& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionBring& operator=(const ConditionBring& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionBring& rhs)
            {
                ConditionBase::copyFrom(rhs);
                
                _playerType = rhs._playerType;
                _approximation = rhs._approximation;
                _number = rhs._number;
                _entity = rhs._entity;
                _location = rhs._location;
            }
            
            virtual ~ConditionBring() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                
                // player combo box
                ImGui::PushID(0);
                _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("brings");
                
                // approximation combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _approximation.drawImGui();
                ImGui::PopID();
                
                // drag int
                ImGui::PushID(2);
                ImGui::SameLine(); _number.drawImGui();
                ImGui::PopID();
                
                // entity combo box
                ImGui::PushID(3);
                ImGui::SameLine(); _entity.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("to");
                
                // location combo box
                ImGui::PushID(4);
                _location.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::EndChild();
                
                return (_playerType.isItemSelected() &&
                        _approximation.isItemSelected() &&
                        _entity.isItemSelected() &&
                        _location.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "brings ";
                ret += "'" + _approximation.getParameterName() + "' ";
                ret += "'" + _number.getParameterName() + "' ";
                ret += "'" + _entity.getParameterName() + "' ";
                ret += "to ";
                ret += "'" + _location.getParameterName() + "'";
                return ret;
            }
            
            virtual void reset() override
            {
                _playerType.reset();
                _approximation.reset();
                _number.reset();
                _entity.reset();
                _location.reset();
            }
            
            virtual ConditionBring* clone() const override
            {
                return new ConditionBring(*this);
            }
            
            virtual void deepCopy(TriggerComponentProtocol* copy) override
            {
                ConditionBase::deepCopy(copy);
                
                auto p = static_cast<ConditionBring*>(copy);
                _playerType = p->_playerType;
                _approximation = p->_approximation;
                _number = p->_number;
                _entity = p->_entity;
                _location = p->_location;
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto locationPtr = _location.getLocation();
                auto obj = DeadCreator::CreateBring(builder,
                                                    static_cast<int>(_playerType.getPlayerType()),
                                                    static_cast<DeadCreator::Approximation>(_approximation.getApproximationType()),
                                                    _number.getNumber(),
                                                    _entity.getEntityType(),
                                                    builder.CreateString(locationPtr->getLocationName()));
                
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_Bring, obj.Union());
            }
            
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setApproximation(TriggerParameterApproximation::Type type) { _approximation.setApproximationType(type); }
            void setNumber(int number) { _number.setNumber(number); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setLocation(LocationNode* loc) { _location.setLocation(loc); }
            
        private:
            
            TriggerParameterPlayerType _playerType;
            TriggerParameterApproximation _approximation;
            TriggerParameterNumber _number;
            TriggerParameterEntity _entity;
            TriggerParameterLocation _location;
            
        };
        
    }
    
    namespace client
    {
        
        class ConditionBring : public ConditionBase
        {
            
        public:
            
            virtual bool isReady() override
            {
                return true;
            }
            
        private:
            
            int _player;
            int _approximation;
            int _number;
            int _entity;
            int _location;
            
        };
        
    }
    
}









