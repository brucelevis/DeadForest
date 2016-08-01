//
//  ObjectManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
     
        class Game;
        class GameResource;
        class GameObject2;
        
        class ObjectManager : public cocos2d::Ref
        {
            
        public:
        
            explicit ObjectManager(Game* game) : _game(game) {}
            virtual ~ObjectManager() = default;
            static ObjectManager* createWithResouce(Game* game, GameResource* res)
            {
                auto ret = new (std::nothrow) ObjectManager(game);
                if ( ret && ret->initWithResource(res) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool initWithResource(GameResource* res);
            
            void addObject(GameObject2* object);
            bool removeObject(int id);
            GameObject2* getEntityFromID(int ID);
            
            const std::map<int, GameObject2*>& getObjects() const { return _objects; }
            int getNextValidID() { static int nextValidID = 0; return nextValidID++; }
            
        private:
            
            Game* _game;
            std::map<int, GameObject2*> _objects;
            
        };
        
    }
}









