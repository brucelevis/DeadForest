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

using namespace cocos2d;

namespace realtrick
{
    
    int GameManager::_nextValidID = 0;
    
    void GameManager::update(float dt)
    {
        for( const auto& entity : _entities )
        {
            entity.second->update(dt);
        }
        
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
        _gameWorld->addChild(_debugNode, numeric_limits<int>::max() - 1);
    }
    
    
    GameManager::~GameManager()
    {
        this->clear();
    }
    
    
    void GameManager::loadSingleGameMap(const char* fileName)
    {
        if( _gameMap != nullptr )
        {
            throw std::runtime_error("<GameManager::loadGameMap> GameMap is already exist.");
        }

        _cellSpace = new CellSpacePartition(Prm.getValueAsInt("worldWidth"),
                                            Prm.getValueAsInt("worldHeight"),
                                            Prm.getValueAsInt("cellWidth"),
                                            Prm.getValueAsInt("cellHeight"),
                                            Prm.getValueAsInt("maxEntity"));
        
        _gameMap = GameMap::create(this, fileName);
        addStaticEntity(_gameMap, Z_ORDER_GAME_MAP, getNextValidID());
        
        std::vector<Vec2> startingPoints = getGameMap()->getStartingLocationData();
        log("<GameManager::loadGameMap> Created Human at (%.0f, %.0f)", startingPoints.front().x, startingPoints.front().y);
        EntityHuman* human = EntityHuman::create(this);
        human->setWorldPosition(startingPoints.front());
        addDynamicEntity(human, Z_ORDER_HUMAN + 1, getNextValidID());
        setPlayer(human);
        
        for(int i = 1 ; i < startingPoints.size() ; ++ i)
        {
            log("<GameManager::loadGameMap> Created Human at (%.0f, %.0f)", startingPoints[i].x, startingPoints[i].y);
            EntityHuman* human = EntityHuman::create(this);
            human->setWorldPosition(startingPoints[i]);
            addDynamicEntity(human, Z_ORDER_HUMAN, getNextValidID());
        }
        
        std::vector<NameCoordAmount> items = getGameMap()->getItemData();
        for(int i = 0 ; i < items.size() ; ++ i)
        {
            log("<GameManager::loadGameMap> Created Items at (%.0f, %.0f)", items[i].pos.x, items[i].pos.y);
            
            if ( items[i].name == "Glock22" )
            {
                ItemGlock17* glock17 = ItemGlock17::create(this, "Glock17.png", "Glock17.png", "Glock17.png",
                                                           ui::Widget::TextureResType::PLIST);
                glock17->setWorldPosition(items[i].pos);
                glock17->setAmount(items[i].amount);
                addDynamicEntity(glock17, Z_ORDER_ITEMS, getNextValidID());
            }
            else if (items[i].name == "M16A4" )
            {
                ItemM16A2* m16a2 = ItemM16A2::create(this, "M16A2.png", "M16A2.png", "M16A2.png",
                                                     ui::Widget::TextureResType::PLIST);
                m16a2->setWorldPosition(items[i].pos);
                m16a2->setAmount(items[i].amount);
                addDynamicEntity(m16a2, Z_ORDER_ITEMS, getNextValidID());
            }
            else if (items[i].name == "Axe" )
            {
                ItemAxe* axe = ItemAxe::create(this, "Axe.png", "Axe.png", "Axe.png",
                                               ui::Widget::TextureResType::PLIST);
                axe->setWorldPosition(items[i].pos);
                axe->setAmount(items[i].amount);
                addDynamicEntity(axe, Z_ORDER_ITEMS, getNextValidID());
            }
            else if (items[i].name == "M1897" )
            {
                ItemM1897* m1897 = ItemM1897::create(this, "M1897.png", "M1897.png", "M1897.png",
                                                     ui::Widget::TextureResType::PLIST);
                m1897->setWorldPosition(items[i].pos);
                m1897->setAmount(items[i].amount);
                addDynamicEntity(m1897, Z_ORDER_ITEMS, getNextValidID());
            }
            else if (items[i].name == "5_56mm")
            {
                Bullet556mm* bullet556mm = Bullet556mm::create(this, "5_56mm.png", "5_56mm.png", "5_56mm.png", ui::Widget::TextureResType::PLIST);
                bullet556mm->setWorldPosition(items[i].pos);
                bullet556mm->setAmount(items[i].amount);
                addDynamicEntity(bullet556mm, Z_ORDER_ITEMS, getNextValidID());
            }
            else if (items[i].name == "9mm")
            {
                Bullet9mm* bullet9mm = Bullet9mm::create(this, "9mm.png", "9mm.png", "9mm.png", ui::Widget::TextureResType::PLIST);
                bullet9mm->setWorldPosition(items[i].pos);
                bullet9mm->setAmount(items[i].amount);
                addDynamicEntity(bullet9mm, Z_ORDER_ITEMS, getNextValidID());
            }
            else if (items[i].name == "Shell")
            {
                BulletShell* shotgunShell = BulletShell::create(this, "Shell.png", "Shell.png", "Shell.png", ui::Widget::TextureResType::PLIST);
                shotgunShell->setWorldPosition(items[i].pos);
                shotgunShell->setAmount(items[i].amount);
                addDynamicEntity(shotgunShell, Z_ORDER_ITEMS, getNextValidID());
            }
        }
    
    }
    
    
    void GameManager::loadMultiGameMap(const char* fileName)
    {
        if( _gameMap != nullptr )
        {
            throw std::runtime_error("<GameManager::loadGameMap> GameMap is already exist.");
        }
        
        _cellSpace = new CellSpacePartition(Prm.getValueAsInt("worldWidth"),
                                            Prm.getValueAsInt("worldHeight"),
                                            Prm.getValueAsInt("cellWidth"),
                                            Prm.getValueAsInt("cellHeight"),
                                            Prm.getValueAsInt("maxEntity"));
        
        _gameMap = GameMap::create(this, fileName);
        addStaticEntity(_gameMap, Z_ORDER_GAME_MAP, 1000000 + getNextValidID());
        
        std::vector<NameCoordAmount> items = getGameMap()->getItemData();
        for(int i = 0 ; i < items.size() ; ++ i)
        {
            log("<GameManager::loadGameMap> Created Items at (%.0f, %.0f)", items[i].pos.x, items[i].pos.y);
            
            if ( items[i].name == "Glock22" )
            {
                ItemGlock17* glock17 = ItemGlock17::create(this, "Glock17.png", "Glock17.png", "Glock17.png",
                                                           ui::Widget::TextureResType::PLIST);
                glock17->setWorldPosition(items[i].pos);
                glock17->setAmount(items[i].amount);
                addDynamicEntity(glock17, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
            else if (items[i].name == "M16A4" )
            {
                ItemM16A2* m16a2 = ItemM16A2::create(this, "M16A2.png", "M16A2.png", "M16A2.png",
                                                     ui::Widget::TextureResType::PLIST);
                m16a2->setWorldPosition(items[i].pos);
                m16a2->setAmount(items[i].amount);
                addDynamicEntity(m16a2, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
            else if (items[i].name == "Axe" )
            {
                ItemAxe* axe = ItemAxe::create(this, "Axe.png", "Axe.png", "Axe.png",
                                               ui::Widget::TextureResType::PLIST);
                axe->setWorldPosition(items[i].pos);
                axe->setAmount(items[i].amount);
                addDynamicEntity(axe, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
            else if (items[i].name == "M1897" )
            {
                ItemM1897* m1897 = ItemM1897::create(this, "M1897.png", "M1897.png", "M1897.png",
                                                     ui::Widget::TextureResType::PLIST);
                m1897->setWorldPosition(items[i].pos);
                m1897->setAmount(items[i].amount);
                addDynamicEntity(m1897, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
            else if (items[i].name == "5_56mm")
            {
                Bullet556mm* bullet556mm = Bullet556mm::create(this, "5_56mm.png", "5_56mm.png", "5_56mm.png", ui::Widget::TextureResType::PLIST);
                bullet556mm->setWorldPosition(items[i].pos);
                bullet556mm->setAmount(items[i].amount);
                addDynamicEntity(bullet556mm, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
            else if (items[i].name == "9mm")
            {
                Bullet9mm* bullet9mm = Bullet9mm::create(this, "9mm.png", "9mm.png", "9mm.png", ui::Widget::TextureResType::PLIST);
                bullet9mm->setWorldPosition(items[i].pos);
                bullet9mm->setAmount(items[i].amount);
                addDynamicEntity(bullet9mm, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
            else if (items[i].name == "Shell")
            {
                BulletShell* shotgunShell = BulletShell::create(this, "Shell.png", "Shell.png", "Shell.png", ui::Widget::TextureResType::PLIST);
                shotgunShell->setWorldPosition(items[i].pos);
                shotgunShell->setAmount(items[i].amount);
                addDynamicEntity(shotgunShell, Z_ORDER_ITEMS, 1000000 + getNextValidID());
            }
        }
        
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
    
    
    int GameManager::getRemainHuman() const
    {
        int ret = 0;
        for( const auto& ent : _entities )
        {
            if ( ent.second->getEntityType() == EntityType::ENTITY_HUMAN )
            {
                EntityHuman* h = (EntityHuman*)ent.second;
                if ( h->isAlive() ) ret ++;
            }
        }
        return ret;
    }
    
    
    std::string GameManager::getCurrentLocation() const
    {
        Vec2 pos = _gameCamera->getCameraPos();
        int xidx = (int)( ( pos.x + _cellSpace->getCellWidth() ) / _cellSpace->getCellWidth() );
        int yidx = (int)( ( pos.y + _cellSpace->getCellHeight() ) / _cellSpace->getCellHeight() );
        
        return _to_string(xidx) + ", " + _to_string(yidx);
    }
    
    
    void GameManager::pushLogic(double delaySeconds, MessageType type, void* extraInfo)
    {
        Dispatch.pushMessage(delaySeconds, _gameWorld->getLogicStream(), nullptr, type, extraInfo);
    }
    
}













