//
//  GameManager.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 27..
//
//

#include "GameManager.hpp"
#include "EntityHuman.hpp"
#include "Items.hpp"
#include "GameWorld.hpp"
#include "Camera2D.hpp"
#include "EntityBlood.hpp"
#include "RenderTarget.hpp"
#include "EntityEmptyCartridge.hpp"
#include "EntityBase.hpp"
#include "LogicStream.hpp"
#include "TriggerSystem.hpp"
using namespace cocos2d;
using namespace std;
using namespace realtrick::client;

#include "GMXFile_generated.h"
#include "util.h"


int GameManager::_nextValidID = 0;

void GameManager::update(float dt)
{
    for( const auto& entity : _entities )
    {
        entity.second->update(dt);
    }
    
    _triggerSystem->update(dt);
    
    if ( _debugNode->isVisible() )
    {
        // 공간분할 디버그
        const std::vector<Cell>& cells = _cellSpace->getCells();
        int idx = _cellSpace->positionToIndex(_gameCamera->getCameraPos());
        for( int i = 0 ; i < (int)cells.size() ; ++ i)
        {
            if ( idx == i )
            {
                _debugNode->drawSolidRect(worldToLocal(cells[i].boundingBox.origin),
                                          worldToLocal(Vec2(cells[i].boundingBox.getMaxX(), cells[i].boundingBox.getMaxY())),
                                          Color4F(1.0f, 0.0f, 1.0f, 0.1f));
            }
            
            _debugNode->drawRect(worldToLocal(cells[i].boundingBox.origin),
                                 worldToLocal(Vec2(cells[i].boundingBox.getMaxX(), cells[i].boundingBox.getMaxY())),
                                 Color4F::RED);
        }
    }
}

GameManager::GameManager(GameWorld* world) :
_gameWorld(world),
_winSize(Size::ZERO),
_player(nullptr),
_cellSpace(nullptr),
_gameMap(nullptr),
_gameCamera(nullptr),
_debugNode(nullptr)
{
    _winSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    _debugNode = DrawNode::create();
    _debugNode->setPosition(_winSize / 2);
    _debugNode->setVisible(Prm.getValueAsBool("useDebug"));
    _gameWorld->addChild(_debugNode, std::numeric_limits<int>::max() - 1);
    _triggerSystem = new TriggerSystem(this);
}


GameManager::~GameManager()
{
    this->clear();
}


void GameManager::setPlayer(EntityHuman* player)
{
    _player = player;
}


EntityBase* GameManager::getEntityFromID(int ID)
{
    auto iter = _entities.find(ID);
    if(iter != _entities.end())
    {
        return iter->second;
    }
    else
    {
        return nullptr;
    }
}

void GameManager::addDynamicEntity(EntityBase* entity, int zOrder, int id)
{
    auto iter = _entities.find(id);
    if( iter != _entities.end())
    {
        throw std::runtime_error("<GameManager::registEntity> ID is already exist.");
    }
    
    entity->setTag(id);
    _entities.insert( {id, entity} );
    _cellSpace->addEntity(entity);
    _gameWorld->getRenderTarget()->addDynamicEntity(entity, zOrder);
}


void GameManager::addStaticEntity(cocos2d::Node* entity, int zOrder, int id)
{
    entity->setTag(id);
    _gameWorld->getRenderTarget()->addStaticEntity(entity, zOrder);
}


void GameManager::removeEntity(int id)
{
    auto iter = _entities.find(id);
    if(iter == _entities.end())
    {
        throw std::runtime_error("<GameManager::removeEntity> ID is not exist.");
    }
    
    iter->second->removeFromParent();
    _entities.erase(iter);
    _cellSpace->removeEntityFromCell(iter->second);
}


void GameManager::clear()
{
    _entities.clear();
    CC_SAFE_DELETE(_cellSpace);
    CC_SAFE_DELETE(_triggerSystem);
    _player = nullptr;
}


std::list<EntityBase*> GameManager::getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const
{
    std::list<EntityBase*> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCells(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( currCell.boundingBox.intersectsRect(cocos2d::Rect(position.x - speed / 2, position.y - speed / 2, speed, speed)) )
        {
            for ( const auto &entity : currCell.members )
            {
                ret.push_back(entity);
            }
            
            // debug node
            if ( _debugNode->isVisible() )
            {
                _debugNode->drawSolidRect(worldToLocal(currCell.boundingBox.origin),
                                          worldToLocal(Vec2(currCell.boundingBox.origin.x + currCell.boundingBox.size.width,
                                                            currCell.boundingBox.origin.y + currCell.boundingBox.size.height)), Color4F(1.0, 1.0, 0.0, 0.1));
            }
        }
    }
    return std::move(ret);
}


std::list<EntityBase*> GameManager::getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const
{
    std::list<EntityBase*> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCellsNotCurrent(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( physics::intersect(realtrick::client::Rect(currCell.boundingBox.origin.x,
                                                currCell.boundingBox.origin.y,
                                                currCell.boundingBox.size.width,
                                                currCell.boundingBox.size.height), Segment(position, position + dir * range)) )
        {
            for ( const auto &entity : currCell.members )
            {
                ret.push_back(entity);
            }
        }
    }
    
    const Cell& myCell = _cellSpace->getCell(_cellSpace->positionToIndex(position));
    for( const auto& entity : myCell.members )
    {
        ret.push_back(entity);
    }
    
    return std::move(ret);
}

std::vector<Polygon> GameManager::getNeighborWalls(const cocos2d::Vec2& position, float speed) const
{
    std::vector<Polygon> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCells(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( currCell.boundingBox.intersectsRect(cocos2d::Rect(position.x - speed / 2, position.y - speed / 2, speed, speed)) )
        {
            for ( const auto &wall : currCell.walls )
            {
                ret.push_back(wall);
            }
        }
    }
    return std::move(ret);
}


std::vector<Polygon> GameManager::getNeighborWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const
{
    std::vector<Polygon> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCells(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( currCell.boundingBox.intersectsRect(cocos2d::Rect(position - screenSize / 2, screenSize)) )
        {
            for ( const auto &wall : currCell.walls )
            {
                ret.push_back(wall);
            }
        }
    }
    return std::move(ret);
}


std::vector<Polygon> GameManager::getNeighborWalls(const cocos2d::Vec2& position, const Segment& ray) const
{
    std::vector<Polygon> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCellsNotCurrent(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( physics::intersect(realtrick::client::Rect(currCell.boundingBox.origin, currCell.boundingBox.size.width, currCell.boundingBox.size.height), ray) )
        {
            for ( const auto &wall : currCell.walls )
            {
                ret.push_back(wall);
            }
        }
    }
    
    const Cell& myCell = _cellSpace->getCell(_cellSpace->positionToIndex(position));
    for( const auto& wall : myCell.walls )
    {
        ret.push_back(wall);
    }
    
    return std::move(ret);
}


cocos2d::Vec2 GameManager::worldToLocal(const cocos2d::Vec2& p) const
{
    if ( !_gameCamera ) throw runtime_error("camera not exist.");
    
    return p - _gameCamera->getCameraPos();
}


cocos2d::Vec2 GameManager::worldToLocal(const cocos2d::Vec2& p, const cocos2d::Vec2& camera) const
{
    if ( !_gameCamera ) throw runtime_error("camera not exist.");
    return p - camera;
}


void GameManager::pushLogic(double delaySeconds, MessageType type, void* extraInfo)
{
    Dispatch.pushMessage(delaySeconds, _gameWorld->getLogicStream(), nullptr, type, extraInfo);
}


void GameManager::loadGMXFile(const std::string& filePath)
{
    std::string loadedData;
    auto ret = flatbuffers::LoadFile(filePath.c_str(), true, &loadedData);
    if ( ret )
    {
        const DeadCreator::GMXFile* file = DeadCreator::GetGMXFile(loadedData.c_str());
        log("default tile: %d", file->default_type());
        log("tile width: %d", file->tile_size()->width());
        log("tile height: %d", file->tile_size()->height());
        log("number of tile x: %d", file->number_of_tiles()->x());
        log("number of tile y: %d", file->number_of_tiles()->y());
        log("cell space size: %d, %d", file->cell_space_size()->width(), file->cell_space_size()->height());
        
        Size worldSize = Size(file->tile_size()->width() * file->number_of_tiles()->x(),
                              file->tile_size()->height() * file->number_of_tiles()->y());
        
        _cellSpace = new CellSpacePartition(worldSize.width, worldSize.height,
                                            file->cell_space_size()->width(),
                                            file->cell_space_size()->height());
        
        _gameMap = GameMap::createWithGMXFile(this, file);
        addStaticEntity(_gameMap, Z_ORDER_GAME_MAP, getNextValidID());
        
        bool isFirst = true;
        for ( auto entity = file->entities()->begin(); entity != file->entities()->end(); ++ entity )
        {
            EntityType entityType = static_cast<EntityType>(entity->entity_type());
            PlayerType playerType = static_cast<PlayerType>(entity->player_type());
            Vec2 position(entity->pos()->x(), entity->pos()->y());
            if ( entityType == EntityType::ENTITY_HUMAN )
            {
                EntityHuman* human = EntityHuman::create(this);
                human->setPlayerType(playerType);
                human->setWorldPosition(position);
                addDynamicEntity(human, Z_ORDER_HUMAN + 1, getNextValidID());
                
                if ( isFirst )
                {
                    setPlayer(human);
                    isFirst = false;
                }
            }
            else
            {
                if ( entityType == EntityType::BULLET_556MM )
                {
                    Bullet556mm* item = Bullet556mm::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::BULLET_9MM )
                {
                    Bullet9mm* item = Bullet9mm::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::BULLET_SHELL )
                {
                    BulletShell* item = BulletShell::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_AXE )
                {
                    ItemAxe* item = ItemAxe::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_GLOCK17 )
                {
                    ItemGlock17* item = ItemGlock17::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_M16A2 )
                {
                    ItemM16A2* item = ItemM16A2::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_M1897 )
                {
                    ItemM1897* item = ItemM1897::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addDynamicEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
            }
        }
        
    }
}









