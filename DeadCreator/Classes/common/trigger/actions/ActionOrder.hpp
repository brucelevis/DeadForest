//
//  ActionOrder.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 11. 14..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"
#include "GameResource.hpp"
#include "Goals.hpp"
#include "HumanBase.hpp"
#include "BrainBase.hpp"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class ActionOrder : public ActionBase
        {
            
        public:
            
            explicit ActionOrder(GMXLayer* gmxLayer) : ActionBase(gmxLayer),
            _entity(gmxLayer),
            _playerType(gmxLayer),
            _sourceLocation(gmxLayer),
            _orderType(gmxLayer),
            _destLocation(gmxLayer)
            {
                name() = "Order";
            }
            ActionOrder(const ActionOrder& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionOrder& operator=(const ActionOrder& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionOrder& rhs)
            {
                ActionBase::copyFrom(rhs);
                _entity = rhs._entity;
                _playerType = rhs._playerType;
                _sourceLocation = rhs._sourceLocation;
                _orderType = rhs._orderType;
                _destLocation = rhs._destLocation;
            }
            
            virtual ~ActionOrder() = default;
            virtual bool drawEditMode(void* opt) override
            {
                
                ImGui::Text("Issue order to all");
                
                // entity combo box
                ImGui::PushID(0);
                ImGui::SameLine(); _entity.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("owned by");
                
                // player combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("at");
                
                // location combo box
                ImGui::PushID(2);
                ImGui::SameLine(); _sourceLocation.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text(":");
                
                ImGui::PushID(3);
                _orderType.drawImGui();
                ImGui::PopID();
                
                ImGui::PushID(4);
                ImGui::SameLine(); _destLocation.drawImGui(opt);
                ImGui::PopID();
                
                return (_entity.isItemSelected() &&
                        _playerType.isItemSelected() &&
                        _sourceLocation.isItemSelected() &&
                        _orderType.isItemSelected() &&
                        _destLocation.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Issue order to all ";
                ret += "'" + _entity.getParameterName() + "' ";
                ret += "owned by ";
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "at ";
                ret += "'" + _sourceLocation.getParameterName() + "': ";
                ret += "'" + _orderType.getParameterName() + "' ";
                ret += "to ";
                ret += "'" + _destLocation.getParameterName() + "'.";
                return ret;
            }
            
            virtual void reset() override
            {
                _entity.reset();
                _playerType.reset();
                _sourceLocation.reset();
                _orderType.reset();
                _destLocation.reset();
            }
            
            virtual ActionOrder* clone() const override { return new ActionOrder(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateOrder(builder,
                                                    static_cast<int32_t>(_entity.getEntityType()),
                                                    static_cast<int32_t>(_playerType.getPlayerType()),
                                                    builder.CreateString(_sourceLocation.getLocation()->getLocationName()),
                                                    static_cast<DeadCreator::OrderType>(_orderType.getOrderType()),
                                                    builder.CreateString(_destLocation.getLocation()->getLocationName()));
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_Order, obj.Union());
            }
            
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setSourceLocation(LocationNode* loc) { _sourceLocation.setLocation(loc); }
            void setOrderType(OrderType type) { _orderType.setOrderType(type); }
            void setDestLocation(LocationNode* loc) { _destLocation.setLocation(loc); }
            
        private:
            
            TriggerParameterEntity _entity;
            TriggerParameterPlayerType _playerType;
            TriggerParameterLocation _sourceLocation;
            TriggerParameterOrderType _orderType;
            TriggerParameterLocation _destLocation;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionOrderData: public TriggerDataBase
        {
            EntityType entity;
            PlayerType player;
            std::string srcLocation;
            OrderType order;
            std::string destLocation;
            
            ActionOrderData() { type = TriggerComponentType::ACTION_ORDER; }
        };
        
        class ActionOrder : public ActionBase
        {
            
        public:
            
            explicit ActionOrder(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionOrder() = default;
            
            static ActionOrder* create(Game* game, EntityType entity, PlayerType player, const std::string& srcLocation, OrderType order, const std::string& destLocation)
            {
                auto ret = new (std::nothrow) ActionOrder(game);
                if ( ret && ret->init(entity, player, srcLocation, order, destLocation) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(EntityType entity, PlayerType player, const std::string& srcLocation, OrderType order, const std::string& destLocation)
            {
                _params.entity = entity;
                _params.player = player;
                _params.srcLocation = srcLocation;
                _params.order = order;
                _params.destLocation = destLocation;
                
                return true;
            }
            
            virtual void doAction()
            {
                if ( _params.player == PlayerType::CURRENT_PLAYER ) _maskedPlayer = _owner->getPlayers();
                else _maskedPlayer.set(static_cast<int>(_params.player));
                
                const auto& entities = _game->getEntityManager()->getEntities();
                const auto& locations = _game->getGameResource()->getLocations();
                
                auto destRectCenter = cocos2d::Vec2(locations.at(_params.destLocation).getMidX() + cocos2d::random(-5.0f, 5.0f),
                                                    locations.at(_params.destLocation).getMidY() + cocos2d::random(-5.0f, 5.0f));
                
                for( const auto& ent : entities )
                {
                    auto currEntity = ent.second;
                    
                    if ( !_maskedPlayer.test(static_cast<int>(currEntity->getPlayerType())) ) continue;
                    if ( _params.entity != currEntity->getEntityType() ) continue;
                    if ( _params.player != currEntity->getPlayerType() ) continue;
                    if ( !isMasked(currEntity->getFamilyMask(), FamilyMask::HUMAN_BASE) ) continue;
                    
                    auto human = static_cast<HumanBase*>(currEntity);
                    if ( locations.at(_params.srcLocation).intersectsCircle(human->getWorldPosition(), human->getBoundingRadius()) )
                    {
                        if ( _params.order == OrderType::MOVE )
                        {
                            auto heading = (destRectCenter - human->getWorldPosition()).getNormalized();
                            GoalMoveToPosition* moveGoal = new GoalMoveToPosition(human, destRectCenter,
                                                                                  std::make_shared<ArrivingData>(ArrivingData(heading, 100.0f, 50.0f)));
                            human->getBrain()->executeGoal(moveGoal);
                        }
                        else // if ( _params.order == OrderType::ATTACK )
                        {
                            GoalAttackToDestination* attackGoal = new GoalAttackToDestination(human, destRectCenter, 50.0f);
                            human->getBrain()->executeGoal(attackGoal);
                        }
                    }
                }
            }
            
        private:
            
            ActionOrderData _params;
            std::bitset<30> _maskedPlayer;
            
        };
        
    }
}









