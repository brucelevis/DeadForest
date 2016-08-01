//
//  Game.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 27..
//
//

#include "Game.hpp"
#include "EntityHuman.hpp"
#include "Items.hpp"
#include "Camera2D.hpp"
#include "RenderTarget.hpp"
#include "GameObject.hpp"
#include "LogicStream.hpp"
#include "UiLayer.hpp"
#include "TriggerSystem.hpp"
#include "RenderingSystem.hpp"
#include "SingleStream.hpp"
#include "NetworkStream.hpp"
#include "GameResource.hpp"
#include "ObjectManager.hpp"
using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::client;

#include "GMXFile_generated.h"
#include "util.h"


Game::Game() :
_winSize(Size::ZERO),
_player(nullptr),
_cellSpace(nullptr),
_triggerSystem(nullptr),
_bgmID(0),
_isPaused(true)
{
}


Game::~Game()
{
    this->clear();
}


void Game::clear()
{
    _entities.clear();
    CC_SAFE_DELETE(_logicStream);
    _player = nullptr;
    
    CC_SAFE_DELETE(_messenger);
    experimental::AudioEngine::stop(_bgmID);
}


Game* Game::create()
{
    auto ret = new (std::nothrow) Game();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


Scene* Game::createScene()
{
    auto scene = Scene::create();
    auto node = Game::create();
    scene->addChild(node);
    return scene;
}


bool Game::init()
{
    if ( !Node::init() )
        return false;
    
    this->scheduleUpdate();
    _winSize = Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    
    _messenger = new MessageDispatcher();
    
    // single or network (?)
    if( Prm.getValueAsBool("useNetwork") ) { _logicStream = new NetworkStream(this); }
    else { _logicStream = new SingleStream(this); }
    
    this->pushLogic(0.0, MessageType::LOAD_GAME_PLAYER, nullptr);
    
    return true;
}


void Game::update(float dt)
{
    // """IMPORTANT"""
    // logicStream's update() must call before checking pause.
    // because network stream will load data through this method (although game is puased, load game must process)
    _logicStream->update(dt);
    
    // checking pause is done
    if ( _isPaused ) return ;
    
    pair<int, int> oldIndex = getFocusedTileIndex(_renderingSystem->getCameraPosition(),
                                                  _gameResource->getTileWidth(),
                                                  _gameResource->getTileHeight(), DUMMY_TILE_SIZE);
    
    // 1. update entities
    for( const auto& entity : _entities )
        entity.second->update(dt);
    
    // 2. trigger update and execute
    _triggerSystem->update(dt);
    
    // 3. set game camera position and chunk update (if cell space is changed)
    _renderingSystem->setCameraPosition(_player->getWorldPosition());
    
    if ( oldIndex != getFocusedTileIndex(_renderingSystem->getCameraPosition(),
                                         _gameResource->getTileWidth(),
                                         _gameResource->getTileHeight(), DUMMY_TILE_SIZE) )
    {
        _renderingSystem->updateChunk();
    }
    
    _messenger->dispatchDelayedMessages();
}


GameObject* Game::getEntityFromID(int ID)
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

void Game::addEntity(GameObject* entity, int zOrder, int id)
{
    auto iter = _entities.find(id);
    if( iter != _entities.end())
    {
        throw std::runtime_error("<Game::registEntity> ID is already exist.");
    }
    
    entity->setTag(id);
    _entities.insert( {id, entity} );
    _cellSpace->addEntity(entity);
    _renderingSystem->addEntity(entity, zOrder);
}


void Game::removeEntity(int id)
{
    auto iter = _entities.find(id);
    if(iter == _entities.end())
    {
        throw std::runtime_error("<Game::removeEntity> ID is not exist.");
    }
    
    iter->second->removeFromParentAndCleanup(true);
    _entities.erase(iter);
    _cellSpace->removeEntityFromCell(iter->second);
}


void Game::loadUiLayer()
{
    _bgmID = experimental::AudioEngine::play2d("rainfall.mp3", true);
    experimental::AudioEngine::setVolume(_bgmID, 0.3f);
}


std::list<GameObject*> Game::getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const
{
    std::list<GameObject*> ret;
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
        }
    }
    return ret;
}


std::list<GameObject*> Game::getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const
{
    std::list<GameObject*> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCellsNotCurrent(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( physics::intersect(realtrick::Rect(currCell.boundingBox.origin.x,
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
    
    return ret;
}

std::vector<Polygon> Game::getNeighborWalls(const cocos2d::Vec2& position, float speed) const
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
    return ret;
}


std::vector<Polygon> Game::getNeighborWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const
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
    return ret;
}


std::vector<Polygon> Game::getNeighborWalls(const cocos2d::Vec2& position, const Segment& ray) const
{
    std::vector<Polygon> ret;
    std::vector<int> cellIndices = _cellSpace->getNeighborCellsNotCurrent(position);
    for ( const int idx : cellIndices )
    {
        const Cell& currCell = _cellSpace->getCell(idx);
        if ( physics::intersect(realtrick::Rect(currCell.boundingBox.origin, currCell.boundingBox.size.width, currCell.boundingBox.size.height), ray) )
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
    
    return ret;
}


void Game::pushLogic(double delaySeconds, MessageType type, void* extraInfo)
{
    _messenger->pushMessage(delaySeconds, _logicStream, nullptr, type, extraInfo);
}


void Game::loadResource(const std::string& filePath)
{
    std::string loadedData;
    auto ret = flatbuffers::LoadFile(filePath.c_str(), true, &loadedData);
    if ( ret )
    {
        const DeadCreator::GMXFile* file = DeadCreator::GetGMXFile(loadedData.c_str());
        
        // load entities
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
                addEntity(human, Z_ORDER_HUMAN + 1, getNextValidID());
                
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
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::BULLET_9MM )
                {
                    Bullet9mm* item = Bullet9mm::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::BULLET_SHELL )
                {
                    BulletShell* item = BulletShell::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_AXE )
                {
                    ItemAxe* item = ItemAxe::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_GLOCK17 )
                {
                    ItemGlock17* item = ItemGlock17::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_M16A2 )
                {
                    ItemM16A2* item = ItemM16A2::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
                
                else if ( entityType == EntityType::ITEM_M1897 )
                {
                    ItemM1897* item = ItemM1897::create(this);
                    item->setWorldPosition(position);
                    item->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(item, Z_ORDER_ITEMS, getNextValidID());
                }
            }
        }
        
    }
}


void Game::loadGMXFile(const std::string& path)
{
    _gameResource = GameResource::createWithGMXFile(path);
    _releasePool.addObject(_gameResource);
    
    _renderingSystem = RenderingSystem::create(this, _gameResource);
    _renderingSystem->setZoom(Prm.getValueAsFloat("cameraZoom"));
    addChild(_renderingSystem);
    
    _triggerSystem = TriggerSystem::createWithResouce(this, _gameResource);
    _releasePool.addObject(_triggerSystem);
    
    _cellSpace = CellSpacePartition::createWithResource(_gameResource);
    _releasePool.addObject(_cellSpace);
    
    const auto& walls = _gameResource->getCollisionData();
    for (const auto& wall : walls)
    {
        _cellSpace->addWall(wall);
    }
    
    _objectManager = ObjectManager::createWithResouce(this, _gameResource);
    _releasePool.addObject(_objectManager);
}


void Game::sendMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo)
{
    _messenger->pushMessage(delaySeconds, receiver, sender, type, extraInfo);
}


TileType Game::getStepOnTileType(const cocos2d::Vec2& pos)
{
    const auto& tileData = _gameResource->getTileData();
    const auto tileWidth = _gameResource->getTileWidth();
    const auto tileHeight = _gameResource->getTileHeight();
    
    std::pair<int, int> idx = getFocusedTileIndex(pos, tileWidth, tileHeight, DUMMY_TILE_SIZE);
    const auto& tile = tileData[idx.second][idx.first];
    Vec2 center = indexToPosition(idx.first, idx.second, tileWidth, tileHeight, DUMMY_TILE_SIZE);

    Vec2 region1 = center + Vec2(0.0f, tileHeight / 4.0f);
    Vec2 region2 = center + Vec2(tileWidth / 4.0f, 0.0f);
    Vec2 region3 = center - Vec2(0.0f, tileHeight / 4.0f);
    Vec2 region4 = center - Vec2(tileWidth / 4.0f, 0.0f);

    std::string tileType = tile.getTileTail();

    if ( physics::isContainPointInDiamond(region1, tileWidth / 4.0f, pos) && (tileType.find('1') != std::string::npos) )
    {
        return tile.getTileType();
    }
    else if ( physics::isContainPointInDiamond(region2, tileWidth / 4.0f, pos) && (tileType.find('2') != std::string::npos) )
    {
        return tile.getTileType();
    }
    else if ( physics::isContainPointInDiamond(region3, tileWidth / 4.0f, pos) && (tileType.find('3') != std::string::npos) )
    {
        return tile.getTileType();
    }
    else if ( physics::isContainPointInDiamond(region4, tileWidth / 4.0f, pos) && (tileType.find('4') != std::string::npos) )
    {
        return tile.getTileType();
    }

    return TileType::DIRT;
}









