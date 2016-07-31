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

namespace realtrick
{
    namespace client
    {
        
        class ComponentBase;
    
        class GameObject2
        {
            
        public:
            
            GameObject2() = default;
            virtual ~GameObject2() = default;
            
            virtual bool init()
            {
                for(int i = 0 ; i < static_cast<int>(ComponentType::MAX) ; ++ i)
                    _components.pushBack(nullptr);
                
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
                if ( !_components[index] ) throw std::runtime_error("component is not exist.");
                _components.erase(index);
            }
            
            int getID() const { return _id; }
            void setID(int id) { _id = id; }
            
            std::string getName() const { return _name; }
            void setName(const std::string& name) { _name = name; }
            
            virtual void update(float dt) = 0;
            
        protected:
            
            int _id;
            std::string _name;
            cocos2d::Vector<ComponentBase*> _components;
        
        };
        
    }
}









