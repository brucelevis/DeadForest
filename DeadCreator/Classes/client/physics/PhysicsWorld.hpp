//
//  PhysicsWorld.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 27..
//
//

#pragma once

#include "Box2D/Box2D.h"

#include "cocos2d.h"


namespace realtrick
{
    namespace client
    {
        
        class Game;
        class PhysicsBody;
        
        class PhysicsWorld
        {
            
        public:
            
            explicit PhysicsWorld(Game* game) : _game(game) {}
            virtual ~PhysicsWorld()
            {
                CC_SAFE_DELETE(_world);
            }
            
            void update(float dt)
            {
                
            }
            
            void addBody(PhysicsBody* body)
            {
                b2BodyDef bd;
                _world->CreateBody(&bd);
            }
            
            void removeBody(PhysicsBody* body)
            {
//                _world->DestroyBody(b2Body *body)
            }
            
        private:
            
            Game* _game;
            b2World* _world;
            
        };
        
    }
}
