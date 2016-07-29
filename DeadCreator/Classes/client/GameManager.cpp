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
#include "UiLayer.hpp"
#include "TriggerSystem.hpp"
#include "SingleStream.hpp"
#include "NetworkStream.hpp"
using namespace cocos2d;
using namespace realtrick::client;

#include "GMXFile_generated.h"
#include "util.h"


GameManager::GameManager(GameWorld* world) :
_gameWorld(world),
_winSize(Size::ZERO),
_player(nullptr),
_cellSpace(nullptr),
_gameMap(nullptr),
_gameCamera(nullptr),
_debugNode(nullptr),
_triggerSystem(nullptr),
_clipNode(nullptr),
_rootNode(nullptr),
_uiLayer(nullptr),
_bgmID(0),
_isPaused(true)
{
}


GameManager::~GameManager()
{
    this->clear();
}


void GameManager::clear()
{
    _entities.clear();
    CC_SAFE_DELETE(_cellSpace);
    CC_SAFE_DELETE(_triggerSystem);
    CC_SAFE_DELETE(_gameCamera);
    CC_SAFE_DELETE(_logicStream);
    _player = nullptr;
    
    experimental::AudioEngine::stop(_bgmID);
}


GameManager* GameManager::create(GameWorld* world)
{
    auto ret = new (std::nothrow) GameManager(world);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GameManager::init()
{
    if ( !Node::init() )
        return false;
    
    _winSize = Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);

    _triggerSystem = new TriggerSystem(this);
    _gameCamera = new Camera2D();
    
    // single or network (?)
    if( Prm.getValueAsBool("useNetwork") ) { _logicStream = new NetworkStream(this); }
    else { _logicStream = new SingleStream(this); }
    
    _clipNode = ClippingRectangleNode::create(cocos2d::Rect(0,0, _winSize.width, _winSize.height));
    addChild(_clipNode);
    
    _rootNode = Node::create();
    _rootNode->setPosition(_winSize / 2);
    _clipNode->addChild(_rootNode);
    
    _debugNode = DrawNode::create();
    _debugNode->setPosition(_winSize / 2);
    _debugNode->setVisible(Prm.getValueAsBool("useDebug"));
    _clipNode->addChild(_debugNode, std::numeric_limits<int>::max() - 1);
    
    this->pushLogic(0.0, MessageType::LOAD_GAME_PLAYER, nullptr);
    
    return true;
}


void GameManager::update(float dt)
{
    // """IMPORTANT"""
    // logicStream's update() must call before checking pause.
    // because network stream will load data through this method (although game is puased, load game must process)
    _logicStream->update(dt);
    
    // checking pause is done
    if ( _isPaused ) return ;
    
    // draw debug node
    _debugNode->clear();
    drawCellSpaceDebugNode();
    
    pair<int, int> oldIndex = getFocusedTileIndex(_gameCamera->getCameraPos(), _gameMap->getTileWidth(), _gameMap->getTileHeight(), DUMMY_TILE_SIZE);
    
    // 1. update entities
    for( const auto& entity : _entities )
        entity.second->update(dt);
    
    // 2. trigger update and execute
    _triggerSystem->update(dt);
    
    // 3. set game camera position and chunk update (if cell space is changed)
    _gameCamera->setCameraPos(_player->getWorldPosition());
    if ( oldIndex != getFocusedTileIndex(_gameCamera->getCameraPos(), _gameMap->getTileWidth(), _gameMap->getTileHeight(), DUMMY_TILE_SIZE) )
        _gameMap->updateChunk(_gameCamera->getCameraPos());
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

void GameManager::addEntity(EntityBase* entity, int zOrder, int id)
{
    auto iter = _entities.find(id);
    if( iter != _entities.end())
    {
        throw std::runtime_error("<GameManager::registEntity> ID is already exist.");
    }
    
    entity->setTag(id);
    _entities.insert( {id, entity} );
    _cellSpace->addEntity(entity);
    _rootNode->addChild(entity, zOrder);
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


void GameManager::loadUiLayer()
{
    _uiLayer = UiLayer::create(this);
    _clipNode->addChild(_uiLayer);
    
    _bgmID = experimental::AudioEngine::play2d("rainfall.mp3", true);
    experimental::AudioEngine::setVolume(_bgmID, 0.3f);
    
    setZoom(Prm.getValueAsFloat("cameraZoom"));
}


void GameManager::setZoom(float r)
{
    _rootNode->setScale(r);
    _debugNode->setScale(r);
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
    if ( !_gameCamera ) throw std::runtime_error("camera not exist.");
    return p - _gameCamera->getCameraPos();
}


cocos2d::Vec2 GameManager::worldToLocal(const cocos2d::Vec2& p, const cocos2d::Vec2& camera) const
{
    if ( !_gameCamera ) throw std::runtime_error("camera not exist.");
    return p - camera;
}


void GameManager::pushLogic(double delaySeconds, MessageType type, void* extraInfo)
{
    Dispatch.pushMessage(delaySeconds, _logicStream, nullptr, type, extraInfo);
}


void GameManager::loadGMXFile(const std::string& filePath)
{
    std::string loadedData;
    auto ret = flatbuffers::LoadFile(filePath.c_str(), true, &loadedData);
    if ( ret )
    {
        const DeadCreator::GMXFile* file = DeadCreator::GetGMXFile(loadedData.c_str());
        
        // load cell space partition
        Size worldSize = Size(file->tile_size()->width() * file->number_of_tiles()->x(),
                              file->tile_size()->height() * file->number_of_tiles()->y());
        
        _cellSpace = new CellSpacePartition(worldSize.width, worldSize.height,
                                            file->cell_space_size()->width(),
                                            file->cell_space_size()->height());
        
        
        // load tiles
        _gameMap = GameMap::createWithGMXFile(this, file);
        _rootNode->addChild(_gameMap, Z_ORDER_GAME_MAP, getNextValidID());
        
        
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
        
        // load locations
        for ( auto location = file->locations()->begin(); location != file->locations()->end(); ++location)
        {
            std::string name = location->name()->str();
            Size size(location->size()->width(), location->size()->height());
            Vec2 pos(location->pos()->x(), location->pos()->y());
            cocos2d::Rect rect(pos.x, pos.y, size.width * file->tile_size()->width() / 4, size.height * file->tile_size()->height() / 4);
            
            log("name: [%s], size: [%.0f, %.0f], pos: [%.0f, %.0f]", name.c_str(), size.width, size.height, pos.x, pos.y);
            
            _locations.insert( { name, rect });
        }
        
        
        // load triggers
        for ( auto trigger = file->triggers()->begin() ; trigger != file->triggers()->end() ; ++trigger)
        {
            GameTrigger* newTrigger = new GameTrigger(this);
            
            // set players
            for(auto index = trigger->players()->begin(); index != trigger->players()->end(); ++index)
            {
                newTrigger->addPlayer(*index);
            }
            
            // set conditions
            for(auto cond = trigger->conditions()->begin() ; cond != trigger->conditions()->end(); ++cond)
            {
                auto condType = cond->condition_type();
                switch (condType)
                {
                    case DeadCreator::ConditionBase_Bring:
                    {
                        auto conditionObject = static_cast<const DeadCreator::Bring*>(cond->condition());
                        auto conditionBring = ConditionBring::create(this,
                                                                     static_cast<PlayerType>(conditionObject->player()),
                                                                     static_cast<ApproximationType>(conditionObject->approximation()),
                                                                     conditionObject->number(),
                                                                     static_cast<EntityType>(conditionObject->entity_type()),
                                                                     _locations[conditionObject->location_name()->str()]);
                        newTrigger->addCondition(conditionBring);
                        
                        break;
                    }
                    default: { cocos2d::log("invalid condition type"); break;}
                }
            }
            
            // set actions
            for(auto act = trigger->actions()->begin() ; act != trigger->actions()->end(); ++act)
            {
                auto actType = act->action_type();
                switch (actType)
                {
                    case DeadCreator::ActionBase_DisplayText:
                    {
//                        auto actionObject = static_cast<const DeadCreator::DisplayText*>(act->action());
//                        auto actionDisplayText = new ActionDisplayText();
//                        actionDisplayText->setText(actionObject->text()->str());
//                        newTrigger->addAction(actionDisplayText);
                        break;
                    }
                    default: { cocos2d::log("invalid action type"); break;}
                }
            }
            
            _triggerSystem->addTrigger(newTrigger);
        }
    }
}


void GameManager::drawCellSpaceDebugNode()
{
    if ( _debugNode->isVisible() )
    {
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











