//
//  GameObject2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

#include "Types.hpp"
#include "ComponentBase.hpp"
#include "MessageDispatcher.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        class ComponentBase;
    
        class GameObject2
        {
            
        public:
            
            GameObject2(Game* game, int id) :
            _game(game),
            _id(id),
            _name(""),
            _worldPosition(cocos2d::Vec2::ZERO),
            _rotation(0.0f)
            {
            }
            
            virtual ~GameObject2()
            {
                CC_SAFE_DELETE(_messenger);
                
                for (auto iter = std::begin(_components) ; iter != std::end(_components); ++iter)
                    (*iter)->clear();
                _components.clear();
            }
            
            virtual bool init()
            {
                _components.resize(static_cast<int>(ComponentType::MAX));
                _messenger = new MessageDispatcher();
                
                return true;
            }
            
            void addComponent(ComponentBase* component)
            {
                int index = static_cast<int>(component->getType());
                if ( _components[index] ) throw std::runtime_error("same component already exist.");
                _components[index] = component;
            }
            
            void eraseComponent(ComponentType type)
            {
                int index = static_cast<int>(type);
                if ( !_components[index] ) throw std::runtime_error("component is not existing.");
                _components[index]->clear();
            }
            
            Game* getGame() const { return _game; }
            
            int getID() const { return _id; }
            void setID(int id) { _id = id; }
            
            std::string getName() const { return _name; }
            void setName(const std::string& name) { _name = name; }
        
            cocos2d::Vec2 getWorldPosition() const { return _worldPosition; }
            void setWorldPosition(const cocos2d::Vec2& pos) { _worldPosition = pos; }
            
            float getRotation() const { return _rotation; }
            void setRotation(float rot) { _rotation = rot; }
            
            virtual void update(float dt) final
            {
                // update derived class's implemented update() if needed.
                updateImpl(dt);
                
                for( auto& component : _components )
                {
                    if (component) component->update(dt);
                }
                
                _messenger->dispatchDelayedMessages();
            }
            
            virtual void updateImpl(float dt) { }
            
            void sendMessage(double delaySeconds,
                             ComponentType receiver, ComponentType sender,
                             MessageType type, void* extraInfo)
            {
                int receiverIndex = static_cast<int>(receiver);
                if ( _components[receiverIndex] )
                {
                    int senderIndex = static_cast<int>(sender);
                    _messenger->pushMessage(delaySeconds, _components[receiverIndex], _components[senderIndex], type, extraInfo);
                }
            }
            
        protected:
            
            Game* _game;
            
            // tags
            int _id;
            std::string _name;
            
            // transform
            cocos2d::Vec2 _worldPosition;
            float _rotation;
        
        private:
            
            std::vector<ComponentBase*> _components;
            MessageDispatcher* _messenger;
            
        };
        
    }
}









