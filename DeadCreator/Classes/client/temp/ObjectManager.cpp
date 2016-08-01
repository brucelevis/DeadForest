//
//  ObjectManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "ObjectManager.hpp"
#include "GameObject2.hpp"
#include "GameResource.hpp"
using namespace realtrick::client;
using namespace cocos2d;


bool ObjectManager::initWithResource(GameResource* res)
{
    // load entities
//    bool isFirst = true;
//    for (auto entity = res->_entities.begin(); entity != res->_entities.end(); ++ entity)
//    {
//        EntityType entityType = static_cast<EntityType>(entity->entityType);
//        PlayerType playerType = static_cast<PlayerType>(entity->playerType);
//        Vec2 position(entity->position);
//        
//        GameObject2* object;
        
//        if ( entityType == EntityType::ENTITY_HUMAN )
//        {
//            object = ObjectFactory::createSheriff();
//            
//            if ( isFirst )
//            {
////                setPlayer(human);
//                isFirst = false;
//            }
//        }
//        else
//        {
//            if ( entityType == EntityType::BULLET_556MM )
//            {
//                Bullet556mm* item = Bullet556mm::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//            
//            else if ( entityType == EntityType::BULLET_9MM )
//            {
//                Bullet9mm* item = Bullet9mm::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//            
//            else if ( entityType == EntityType::BULLET_SHELL )
//            {
//                BulletShell* item = BulletShell::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//            
//            else if ( entityType == EntityType::ITEM_AXE )
//            {
//                ItemAxe* item = ItemAxe::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//            
//            else if ( entityType == EntityType::ITEM_GLOCK17 )
//            {
//                ItemGlock17* item = ItemGlock17::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//            
//            else if ( entityType == EntityType::ITEM_M16A2 )
//            {
//                ItemM16A2* item = ItemM16A2::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//            
//            else if ( entityType == EntityType::ITEM_M1897 )
//            {
//                ItemM1897* item = ItemM1897::create(this);
//                item->setWorldPosition(position);
//                item->setPlayerType(PlayerType::NEUTRAL);
//                addEntity(item, Z_ORDER_ITEMS, getNextValidID());
//            }
//        }
//    }
    
    return true;
}


void ObjectManager::addObject(GameObject2* object)
{
    int id = getNextValidID();
    object->setID(id);
    _objects.insert( {id, object} );
}


bool ObjectManager::removeObject(int id)
{
    auto iter = _objects.find(id);
    if ( iter != _objects.end() )
    {
        _objects.erase(iter);
        return true;
    }
    return false;
}


GameObject2* ObjectManager::getEntityFromID(int id)
{
    auto iter = _objects.find(id);
    if( iter != _objects.end() )
        return iter->second;
    else
        return nullptr;
}









