//
//  Game.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 27..
//
//

#include "Game.hpp"
#include "EntityPlayer.hpp"
#include "Items.hpp"
#include "Camera2D.hpp"
#include "EntityBase.hpp"
#include "LogicStream.hpp"
#include "UiLayer.hpp"
#include "Terrain.hpp"
#include "TriggerSystem.hpp"
#include "RenderingSystem.hpp"
#include "SingleStream.hpp"
#include "ServerStream.hpp"
#include "GameResource.hpp"
#include "EntityManager.hpp"
using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::client;

#include "GMXFile_generated.h"
#include "util.h"


Game::Game() :
_winSize(Size::ZERO),
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
    CC_SAFE_DELETE(_logicStream);
    CC_SAFE_DELETE(_camera);
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

    _camera = new Camera2D();
    
    
//    if ( Prm.getValueAsBool("useNetwork") ) _logicStream = new ServerStream(this);
//    else _logicStream = new SingleStream(this);
    
    _logicStream = new SingleStream(this);
    
    this->pushLogic(0.0, MessageType::LOAD_GAME_PLAYER, nullptr);
    
    return true;
}


void Game::update(float dt)
{
    // """IMPORTANT"""
    // logicStream's update() must call before checking pause.
    // because server stream will load data through this method (although game is puased, load game must process)
    _logicStream->update(dt);
    
    // checking pause is done
    if ( _isPaused ) return ;
    
    pair<int, int> oldIndex = getFocusedTileIndex(_camera->getCameraPos(),
                                                  _gameResource->getTileWidth(),
                                                  _gameResource->getTileHeight(), DUMMY_TILE_SIZE);
    
    // 1. update entities
    _entityManager->update(dt);
    
    // 2. set game camera position and chunk update (if cell space is changed)
    _camera->setCameraPos(_entityManager->getPlayerPtr()->getWorldPosition());
    
    if ( oldIndex != getFocusedTileIndex(_camera->getCameraPos(),
                                         _gameResource->getTileWidth(),
                                         _gameResource->getTileHeight(), DUMMY_TILE_SIZE) )
    {
        _renderingSystem->updateChunk(_camera);
    }
    
    // 3. trigger update and execute
    _triggerSystem->update(dt);
    
    _messenger->dispatchDelayedMessages();
}


void Game::loadBGM()
{
    _bgmID = experimental::AudioEngine::play2d("rainfall.mp3", true);
    experimental::AudioEngine::setVolume(_bgmID, 0.3f);
}


std::list<EntityBase*> Game::getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const
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
        }
    }
    return ret;
}


std::list<EntityBase*> Game::getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const
{
    std::list<EntityBase*> ret;
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


void Game::loadGMXFileFromPath(const std::string& path)
{
    _gameResource = GameResource::createWithGMXFile(path);
    _releasePool.addObject(_gameResource);
}


void Game::loadGMXFileFromBinary(const char* binary)
{
    _gameResource = GameResource::createWithBinary(binary);
    _releasePool.addObject(_gameResource);
}


void Game::loadGameContents(PlayerType ownPlayer)
{
    _messenger = MessageDispatcher::create(this);
    _releasePool.addObject(_messenger);
    
    _entityManager = EntityManager::createWithResouce(this, _gameResource, ownPlayer);
    _releasePool.addObject(_entityManager);
    
    _renderingSystem = RenderingSystem::create(this, _gameResource);
    _renderingSystem->setZoom(Prm.getValueAsFloat("cameraZoom"));
    addChild(_renderingSystem);
    
    _triggerSystem = TriggerSystem::createWithResouce(this, _gameResource);
    _releasePool.addObject(_triggerSystem);
    
    _cellSpace = CellSpacePartition::createWithResource(_gameResource);
    _releasePool.addObject(_cellSpace);
    
    const auto& walls = _gameResource->getCollisionData();
    for (const auto& wall : walls )
    {
        _cellSpace->addWall(wall);
    }
    
    const auto& entities = _entityManager->getEntities();
    for ( const auto& entity : entities )
    {
        _cellSpace->addEntity(entity.second);
        
        int zOrder = 0;
        if ( isMasked(entity.second->getFamilyMask(), FamilyMask::ITEM_BASE) )
        {
            zOrder = Z_ORDER_ITEMS;
        }
        else if ( isMasked(entity.second->getFamilyMask(), FamilyMask::HUMAN_BASE) )
        {
            zOrder = Z_ORDER_HUMAN;
        }
        _renderingSystem->addEntity(entity.second, zOrder);
    }
    
    _uiLayer = UiLayer::create(this);
    _renderingSystem->addUINode(_uiLayer);
}


void Game::sendMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo)
{
    _messenger->pushMessage(delaySeconds, receiver, sender, type, extraInfo);
}


void Game::addEntity(EntityBase* ent, int zOrder)
{
    _cellSpace->addEntity(ent);
    _entityManager->addEntity(ent);
    _renderingSystem->addEntity(ent, zOrder);
}


void Game::removeEntity(EntityBase* ent)
{
    _cellSpace->removeEntityFromCell(ent);
    _entityManager->removeEntity(ent);
    _renderingSystem->removeEntity(ent);
}


void Game::killEntity(EntityBase* ent)
{
    if ( isMasked(ent->getFamilyMask(), FamilyMask::HUMAN_BASE) )
    {
        auto human = static_cast<HumanBase*>(ent);
        if ( human->isAlive() ) human->suicide();
    }
    else
    {
        removeEntity(ent);
    }
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


EntityPlayer* Game::getPlayerPtr() const
{
    return _entityManager->getPlayerPtr();
}


EntityBase* Game::getEntityFromID(int id) const
{
    return _entityManager->getEntityFromID(id);
}


void Game::addLog(const std::string& log)
{
    std::string newLog = log;
    int numOfOverlap = 1;
    if ( !_logs.empty() && _logs.back().first == log )
    {
        numOfOverlap = _logs.back().second + 1;
        newLog = log + " (" + _to_string(numOfOverlap) + ")";
        
        _logs.pop_back();
        
        std::string recentString;
        if ( numOfOverlap != 2) recentString = std::string(log + " (" + _to_string(numOfOverlap - 1) + ")");
        else recentString = log;
        
        _logString.pop_back(); // 'n'
        auto recentStringSize = recentString.size();
        while ( recentStringSize-- ) _logString.pop_back();
    }
    
    _logs.push_back( {log, numOfOverlap} );
    _logString += (newLog + '\n');
    _isLogAdded = true;
}


void Game::clearLogs()
{
    _logs.clear();
    _logString.clear();
    _isLogAdded = false;
}


void Game::runCrossHairEffect(const std::string& name)
{
    _uiLayer->runCrossHairEffect(name);
}


void Game::displayText(const std::string& text)
{
    _uiLayer->displayText(text);
}









