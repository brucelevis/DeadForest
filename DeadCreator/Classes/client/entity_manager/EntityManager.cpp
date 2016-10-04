//
//  EntityManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "EntityManager.hpp"
#include "GameResource.hpp"
#include "EntityZombie.hpp"
#include "EntityZombie2.hpp"
#include "EntityPlayer.hpp"
#include "Items.hpp"
#include "Brains.hpp"
#include "Types.hpp"
using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;


bool EntityManager::initWithResource(GameResource* res, PlayerType ownPlayer)
{
    // load entities
    for (auto entity = res->_entities.begin(); entity != res->_entities.end(); ++ entity)
    {
        int id = entity->id;
        EntityType entityType = static_cast<EntityType>(entity->entityType);
        PlayerType playerType = static_cast<PlayerType>(entity->playerType);
        Vec2 position(entity->position);
        
        if ( entityType == EntityType::ENTITY_PLAYER )
        {
            EntityPlayer* human = EntityPlayer::create(_game);
            human->setWorldPosition(position);
            human->setPlayerType(playerType);
            human->setForce(res->_playerInfos[static_cast<int>(playerType)].force);
            addEntity(human, id);
            
			if (res->_playerInfos[static_cast<int>(playerType)].owner == Owner::COMPUTER)
			{
				human->setBrain(new PursuerBrain(human));
				human->getFSM()->setCurrState(&HumanFistIdleLoop::getInstance());
				human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
				human->setEquipedWeapon(nullptr);
			}
            
            if ( playerType == ownPlayer ) _player = human;
        }
        
        else if ( entityType == EntityType::ENTITY_ZOMBIE )
        {
            EntityZombie* zombie = EntityZombie::create(_game);
            zombie->setWorldPosition(position);
            zombie->setPlayerType(playerType);
            zombie->setForce(res->_playerInfos[static_cast<int>(playerType)].force);
            addEntity(zombie, id);
            
            if ( res->_playerInfos[static_cast<int>(playerType)].owner == Owner::COMPUTER )
                zombie->setBrain(ZombieBrain::createDefault(zombie));
        }
        
        else if ( entityType == EntityType::ENTITY_ZOMBIE2 )
        {
            EntityZombie2* zombie = EntityZombie2::create(_game);
            zombie->setWorldPosition(position);
            zombie->setPlayerType(playerType);
            zombie->setForce(res->_playerInfos[static_cast<int>(playerType)].force);
            addEntity(zombie, id);
            
            if ( res->_playerInfos[static_cast<int>(playerType)].owner == Owner::COMPUTER )
                zombie->setBrain(ZombieBrain::createDefault(zombie));
        }
        
        else if ( entityType == EntityType::ENTITY_ZOMBIE3 )
        {
            EntityZombie* zombie = EntityZombie::create(_game);
            zombie->setWorldPosition(position);
            zombie->setPlayerType(playerType);
            zombie->setForce(res->_playerInfos[static_cast<int>(playerType)].force);
            addEntity(zombie, id);
            
            if ( res->_playerInfos[static_cast<int>(playerType)].owner == Owner::COMPUTER )
                zombie->setBrain(ZombieBrain::createDefault(zombie));
        }
        
        else if ( entityType == EntityType::BULLET_556MM )
        {
            Bullet556mm* item = Bullet556mm::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
        
        else if ( entityType == EntityType::BULLET_9MM )
        {
            Bullet9mm* item = Bullet9mm::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
        
        else if ( entityType == EntityType::BULLET_SHELL )
        {
            BulletShell* item = BulletShell::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
        
        else if ( entityType == EntityType::ITEM_AXE )
        {
            ItemAxe* item = ItemAxe::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
        
        else if ( entityType == EntityType::ITEM_GLOCK17 )
        {
            ItemGlock17* item = ItemGlock17::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
        
        else if ( entityType == EntityType::ITEM_M16A2 )
        {
            ItemM16A2* item = ItemM16A2::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
        
        else if ( entityType == EntityType::ITEM_M1897 )
        {
            ItemM1897* item = ItemM1897::create(_game);
            item->setWorldPosition(position);
            item->setPlayerType(PlayerType::NEUTRAL);
            addEntity(item, id);
        }
    }
    
    for ( auto& ent : _entities )
    {
        auto currentEntity = ent.second;
        auto familyMask = currentEntity->getFamilyMask();
        if ( isMasked(familyMask, FamilyMask::BULLET_BASE) )
        {
            // ...
        }
    }
    
    
    return true;
}


void EntityManager::addEntity(EntityBase* entity)
{
    int id = getNextValidID();
    addEntity(entity, id);
}


void EntityManager::addEntity(EntityBase* entity, int id)
{
    if ( getEntityFromID(id) ) throw std::runtime_error("entity is already exist");
    
    entity->setTag(id);
    _entities.insert( {id, entity} );
}


bool EntityManager::removeEntity(EntityBase* entity)
{
    int id = entity->getTag();
    auto ent = getEntityFromID(id);
    if ( ent )
    {
        _entities.erase(id);
        return true;
    }
    
    return false;
}


EntityBase* EntityManager::getEntityFromID(int id)
{
    auto iter = _entities.find(id);
    if( iter != _entities.end() )
        return iter->second;
    else
        return nullptr;
}


void EntityManager::update(float dt)
{
    for (const auto& entity : _entities)
        entity.second->update(dt);
}


int EntityManager::getNextValidID()
{
    int numOfTry = 1000000;
    while ( numOfTry-- )
    {
        int randomID = cocos2d::random(0, 2100000000);
        if ( getEntityFromID(randomID) == nullptr ) return randomID;
    }
    return -1;
}


HumanBase* EntityManager::getPlayerPtr(PlayerType type)
{
    for ( const auto& entity : _entities )
    {
        auto currEntity = entity.second;
        
        if ( currEntity->getPlayerType() == type )
        {
            return static_cast<HumanBase*>(currEntity);
        }
    }
    return nullptr;
}







