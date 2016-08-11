//
//  EntityManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"
#include "Types.hpp"

namespace realtrick
{
    namespace client
    {
     
        class Game;
        class GameResource;
        class EntityBase;
        class EntityPlayer;
        
        class EntityManager : public cocos2d::Ref
        {
            
        public:
        
            explicit EntityManager(Game* game) :
            _game(game),
            _player(nullptr)
            {}
            
            virtual ~EntityManager() = default;
            static EntityManager* createWithResouce(Game* game, GameResource* res, PlayerType ownPlayer)
            {
                auto ret = new (std::nothrow) EntityManager(game);
                if ( ret && ret->initWithResource(res, ownPlayer) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool initWithResource(GameResource* res, PlayerType ownPlayer);
            
            void addEntity(EntityBase* entity);
            bool removeEntity(EntityBase* entity);
            EntityBase* getEntityFromID(int ID);
            
            const std::map<int, EntityBase*>& getEntities() const { return _entities; }
            int getNextValidID() { static int nextValidID = 0; return nextValidID++; }
            
            EntityPlayer* getPlayerPtr() const { return _player; }
            
            void update(float dt);
            
        private:
            
            Game* _game;
            std::map<int, EntityBase*> _entities;
            EntityPlayer* _player;
            PlayerType _playerType;
            
        };
        
    }
}









