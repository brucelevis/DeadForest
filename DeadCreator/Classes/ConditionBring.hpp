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

namespace realtrick
{
 
    class ConditionBring : public ConditionBase
    {
        
    public:
        
        ConditionBring() : ConditionBase()
        {
            _playerType = new TriggerParameterPlayerType();
            _approximation = new TriggerParameterApproximation();
            _number = new TriggerParameterNumber();
            _entity = new TriggerParameterEntity();
            _location = new TriggerParameterLocation();
            
        }
        
        virtual ~ConditionBring()
        {
            CC_SAFE_DELETE(_playerType);
            CC_SAFE_DELETE(_approximation);
            CC_SAFE_DELETE(_number);
            CC_SAFE_DELETE(_entity);
            CC_SAFE_DELETE(_location);
        }
        
        ConditionBring(const ConditionBring& rhs) : ConditionBase(rhs) {}
        
        virtual void draw() override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
            ImGui::PushID(0);
            ImGui::PushItemWidth(180);
            static const char* items1[9] =
            {
                "Player 1",
                "Player 2",
                "Player 3",
                "Player 4",
                "Player 5",
                "Player 6",
                "Player 7",
                "Player 8",
                "Current Player"
            };
            
            if ( ImGui::Combo("", &_currPlayer, items1, 9, 9) )
            {
                if ( _currPlayer == 0 ) _playerType->setPlayerType(PlayerType::PLAYER1);
                else if ( _currPlayer == 1 ) _playerType->setPlayerType(PlayerType::PLAYER2);
                else if ( _currPlayer == 2 ) _playerType->setPlayerType(PlayerType::PLAYER3);
                else if ( _currPlayer == 3 ) _playerType->setPlayerType(PlayerType::PLAYER4);
                else if ( _currPlayer == 4 ) _playerType->setPlayerType(PlayerType::PLAYER5);
                else if ( _currPlayer == 5 ) _playerType->setPlayerType(PlayerType::PLAYER6);
                else if ( _currPlayer == 6 ) _playerType->setPlayerType(PlayerType::PLAYER7);
                else if ( _currPlayer == 7 ) _playerType->setPlayerType(PlayerType::PLAYER8);
                else if ( _currPlayer == 8 ) _playerType->setPlayerType(PlayerType::CURRENT_PLAYER);
            }
            
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::Text("brings");
            
            ImGui::SameLine();
            ImGui::PushID(1);
            ImGui::PushItemWidth(150);
            static const char* items2[3] =
            {
                "at least",
                "at most",
                "exactly",
            };
            
            if (ImGui::Combo("", &_currApproximation, items2, 3, 3))
            {
                if ( _currApproximation == 0 )
                    _approximation->setApproximationType(TriggerParameterApproximation::Type::AT_LEAST);
                else if ( _currApproximation == 1 )
                    _approximation->setApproximationType(TriggerParameterApproximation::Type::AT_MOST);
                else if ( _currApproximation == 2 )
                    _approximation->setApproximationType(TriggerParameterApproximation::Type::EXACTLY);
            }
            
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::PushID(2);
            ImGui::PushItemWidth(30);
            
            if ( ImGui::DragInt("", &_currNumber, 1, 0, 255) )
            {
                _number->setNumber(_currNumber);
            }
            
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::PushID(3);
            ImGui::PushItemWidth(150);
            
            static std::string entityList;
            static std::vector<std::string> entityNames;
            static bool isFirstCall = true;
            if ( isFirstCall )
            {
                auto entityTable = EditorEntity::getEntityTableByType();
                for(auto iter = entityTable.begin(); iter != entityTable.end() ; ++ iter)
                {
                    entityList += (iter->second.entityName);
                    entityNames.push_back(iter->second.entityName);
                    entityList += '\0';
                }
                isFirstCall = false;
            }
            
            if ( ImGui::Combo("", &_currEntity, entityList.c_str(), 5) )
            {
                _entity->setEntityType(EditorEntity::getEntityTableByName().at(entityNames[_currEntity]).entityType);
                cocos2d::log("entity is %d.", _entity->getEntityType());
            }
            
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::Text("to");
            
            ImGui::SameLine();
            ImGui::PushID(4);
            ImGui::PushItemWidth(150);
            static const char* items4[3] =
            {
                "Location 1",
                "Location 2",
                "Location 3",
            };
            
            if ( ImGui::Combo("", &_currLocation, items4, 3, 3) )
            {
//                _location->setLocation();
            }
            
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            
            ImGui::PopStyleColor();
        }
        
        virtual ConditionBring* clone() const override
        {
            return new ConditionBring(*this);
        }
        
    private:
        
        int _currPlayer = -1;
        TriggerParameterPlayerType* _playerType = nullptr;
        
        int _currApproximation = -1;
        TriggerParameterApproximation* _approximation = nullptr;
        
        int _currNumber = 0;
        TriggerParameterNumber* _number = nullptr;
        
        int _currEntity = -1;
        TriggerParameterEntity* _entity = nullptr;
        
        int _currLocation = -1;
        TriggerParameterLocation* _location = nullptr;
        
    };
    
}









