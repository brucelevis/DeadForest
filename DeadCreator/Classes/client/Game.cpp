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
#include "Tileset.hpp"
#include "TileHelperFunctions.hpp"
#include "PathPlanner.h"
#include "MainMenu3.hpp"
#include "RewardScene.hpp"
#include "SensoryMemory.h"
#include "GoalNetwork.h"
#include "ClipperWrapper.hpp"
#include "SimulatorLayer.hpp"

using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::client;

#include "GMXFile_generated.h"
#include "util.h"


Game::Game() :
_winSize(Size::ZERO),
_triggerSystem(nullptr),
_bgmID(0),
_isPaused(true),
_elapsedTime(0.0f)
{
}


Game::~Game()
{
    this->clear();
}


void Game::clear()
{
    CC_SAFE_DELETE(_logicStream);
	CC_SAFE_DELETE(_graph);
    CC_SAFE_DELETE(_physicsWorld);
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


Game* Game::createWithSimulator(editor::SimulatorLayer* simulator)
{
    auto ret = new (std::nothrow) Game();
    if ( ret && ret->initWithSimulator(simulator) )
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
    
    _camera = Camera2D::create();
    addChild(_camera);
    
    if ( UserDefault::getInstance()->getBoolForKey("useNetwork", false) )
    {
        _logicStream = new ServerStream(this);
    }
    else
    {
        _logicStream = new SingleStream(this);
    }
    
    _physicsWorld = new b2World(b2Vec2(0.0f, 0.0f));
    
    this->pushLogic(0.0, MessageType::LOAD_GAME_PLAYER, nullptr);
    
	GoalNetwork::staticInitConstants();

    return true;
}


bool Game::initWithSimulator(editor::SimulatorLayer* simulator)
{
    if ( !init() ) return false;
    
    _physicsWorld->SetDebugDraw(simulator);
    
    return true;
}


void Game::update(float dt)
{
    _elapsedTime += dt;
    
    // update physics
    _physicsWorld->Step(0.016f, 8, 3);
    
    // update logic stream
    _logicStream->update(dt);
    
    // trigger update and execute
    _triggerSystem->update(dt);
    
    
    if ( !_isPaused )
    {
        pair<int, int> oldIndex = getFocusedTileIndex(_camera->getCameraPos(),
                                                      _gameResource->getTileWidth(),
                                                      _gameResource->getTileHeight(), DUMMY_TILE_SIZE);
        
        // update entities
        _entityManager->update(dt);
        
        // set game camera position and chunk update (if cell space is changed)
        _camera->setCameraPos(_entityManager->getPlayerPtr()->getWorldPosition());
        if ( oldIndex != getFocusedTileIndex(_camera->getCameraPos(), _gameResource->getTileWidth(), _gameResource->getTileHeight(), DUMMY_TILE_SIZE) )
        {
            _renderingSystem->updateChunk(_camera);
        }
        
        _messenger->dispatchDelayedMessages();
    }
}


void Game::loadBGM()
{
    _bgmID = experimental::AudioEngine::play2d("client/sounds/rainfall.mp3", true);
    experimental::AudioEngine::setVolume(_bgmID, 0.3f);
}


std::vector<EntityBase*> Game::getNeighbors(const cocos2d::Vec2& position) const
{
    return getNeighborsOnMove(position, 0.0f);
}


std::vector<EntityBase*> Game::getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const
{
    std::vector<EntityBase*> ret;
    return ret;
}


std::vector<EntityBase*> Game::getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const
{
    std::vector<EntityBase*> ret;
    return ret;
}


std::vector<EntityBase*> Game::getNeighborsEntities(const cocos2d::Vec2& pos, const cocos2d::Rect& rect) const
{
    std::vector<EntityBase*> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborWalls(const cocos2d::Vec2& pos) const
{
	std::vector<realtrick::Polygon> ret;
	return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborWalls(const cocos2d::Vec2& position, float speed) const
{
    std::vector<realtrick::Polygon> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const
{
    std::vector<realtrick::Polygon> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborWalls(const cocos2d::Vec2& position, const Segment& ray) const
{
    std::vector<realtrick::Polygon> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborSimpleWalls(const cocos2d::Vec2& pos) const
{
	std::vector<realtrick::Polygon> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborSimpleWalls(const cocos2d::Vec2& position, float speed) const
{
    std::vector<realtrick::Polygon> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborSimpleWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const
{
    std::vector<realtrick::Polygon> ret;
    return ret;
}


std::vector<realtrick::Polygon> Game::getNeighborSimpleWalls(const cocos2d::Vec2& position, const Segment& ray) const
{
    std::vector<realtrick::Polygon> ret;
    return ret;
}

bool Game::isCollideSimpleWalls(const cocos2d::Vec2& pos) const
{
	const auto& walls = getNeighborSimpleWalls(pos);
	for (const auto& wall : walls)
		if (wall.containPoint(pos))
			return true;
	return false;
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
    
	_graph = new Graph();
	generateIsometricGridGraph(
		_gameResource->getNumOfTileX(),
		_gameResource->getNumOfTileY(),
		_gameResource->getTileWidth(),
		_gameResource->getTileHeight(),
		DUMMY_TILE_SIZE);

    _entityManager = EntityManager::createWithResouce(this, _gameResource, ownPlayer);
    _releasePool.addObject(_entityManager);
    
    _renderingSystem = RenderingSystem::create(this, _gameResource);
    _renderingSystem->setZoom(Prm.getValueAsFloat("cameraZoom"));
    addChild(_renderingSystem);
    
    _triggerSystem = TriggerSystem::createWithResouce(this, _gameResource);
    _releasePool.addObject(_triggerSystem);
    
    initCell(_gameResource);
    const auto& walls = _gameResource->getCollisionData();
    for (const auto& wall : walls )
    {
        addWall(wall);
    }
    
    const auto& entities = _entityManager->getEntities();
    for ( const auto& entity : entities )
    {
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
    
    _entityManager->getPlayerPtr()->setUiLayer(_uiLayer);
}


void Game::sendMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo)
{
    _messenger->pushMessage(delaySeconds, receiver, sender, type, extraInfo);
}


void Game::addEntity(EntityBase* ent, int zOrder)
{
    _entityManager->addEntity(ent);
    _renderingSystem->addEntity(ent, zOrder);
}


void Game::removeEntity(EntityBase* ent)
{
	for (auto e : _entityManager->getEntities())
	{
		if (isMasked(e.second->getFamilyMask(), FamilyMask::HUMAN_BASE))
		{
			HumanBase* human = static_cast<HumanBase*>(e.second);

			if (isMasked(ent->getFamilyMask(), FamilyMask::HUMAN_BASE))
			{
				HumanBase* removingHuman = static_cast<HumanBase*>(ent);
				human->getSensoryMemory()->removeBotFromMemory(removingHuman);
			}
			else if (isMasked(ent->getFamilyMask(), FamilyMask::ITEM_BASE))
			{
				ItemBase* removingItem = static_cast<ItemBase*>(ent);
				human->getSensoryMemory()->removeItemFromMemory(removingItem);
			}
		}
	}

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


void Game::teleportEntity(EntityBase* ent, const cocos2d::Rect& rect)
{
    auto suitablePosition = getSuiatablePosition(Circle(ent->getWorldPosition(), ent->getBoundingRadius()), rect);
    ent->setWorldPosition(suitablePosition);
}


cocos2d::Vec2 Game::getSuiatablePosition(const Circle& circle, const cocos2d::Rect& maximumRegion)
{
//    const auto& entities = getNeighbors(<#const cocos2d::Vec2 &position#>)
    return Vec2::ZERO;
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


HumanBase* Game::getPlayerPtr() const
{
    return _entityManager->getPlayerPtr();
}


HumanBase* Game::getPlayerPtr(PlayerType type) const
{
    return _entityManager->getPlayerPtr(type);
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


void Game::generateIsometricGridGraph(
	int numOfTileX,
	int numOfTileY,
	float tileWidth,
	float tileHeight,
	int numOfDummy)
{
	int tileNumX = numOfTileX + numOfDummy * 2;
	int tileNumY = numOfTileY * 2 + numOfDummy * 4;
	for (int i = 0; i < tileNumY; i++)
	{
		for (int j = 0; j < tileNumX; j++)
		{
			cocos2d::Vec2 pos = indexToPosition(j, i, tileWidth, tileHeight, numOfDummy);
			_graph->addNode(NavGraphNode(_graph->getNextFreeNodeIndex(), pos));
		}
	}


	// Settings for graph loading..

	//       2
	//     1   3
	//   0 (x,y) 4
	//     7   5
	//       6
	//	tail == "1"		!U
	//	tail == "2"		!R
	//	tail == "3"		!D
	//	tail == "4"		!L
	//	tail == "12"	!(U, RU, R)
	//	tail == "13"	∞Ê∑Œ x
	//	tail == "14"	!(L, LU, U)
	//	tail == "23"	!(R, RD, D)
	//	tail == "24"	∞Ê∑Œ x
	//	tail == "34"	!(L, LD, D)
	//	tail == "123"	LU, L, LD
	//	tail == "124"	LD, D, RD
	//	tail == "134"	RU, R, RD
	//	tail == "234"	LU, U, RU
    std::map<std::string, std::vector<bool> > pushByTypeMap;
    pushByTypeMap.insert({"1", { true, true, false, true, true, true, true, true } });
    pushByTypeMap.insert({"2", { true, true, true, true, false, true, true, true } });
    pushByTypeMap.insert({"3", { true, true, true, true, true, true, false, true } });
    pushByTypeMap.insert({"4", { false, true, true, true, true, true, true, true } });
    pushByTypeMap.insert({"12", { true, true, false, false, false, true, true, true }});
    pushByTypeMap.insert({"13", { false, false, false, false, false, false, false, false }});
    pushByTypeMap.insert({"14", { false, false, false, true, true, true, true, true }});
    pushByTypeMap.insert({"23", { true, true, true, true, false, false, false, true }});
    pushByTypeMap.insert({"24", { false, false, false, false, false, false, false, false }});
    pushByTypeMap.insert({"34", { false, true, true, true, true, true, false, false }});
    pushByTypeMap.insert({"123", { true, true, false, false, false, false, false, true }});
    pushByTypeMap.insert({"124", { false, false, false, false, false, true, true, true }});
    pushByTypeMap.insert({"134", { false, false, false, true, true, true, false, false }});
    pushByTypeMap.insert({"234", { false, true, true, true, false, false, false, false }});
    pushByTypeMap.insert({"1234", { false, false, false, false, false, false, false, false }});
    pushByTypeMap.insert({"not_hill", { true, true, true, true, true, true, true, true }});

	const auto& tileData = _gameResource->getTileData();

	for (auto n = std::begin(_graph->getNodes()); n != std::end(_graph->getNodes()); ++ n)
	{
		int from = n->getIndex();
		auto index = numberToIndex(from, numOfTileX, numOfDummy);

		if (!isValidIndex(index.first, index.second, numOfTileX, numOfTileY))
			continue;

		realtrick::TileType tileType = tileData[index.second][index.first].getTileType();
		std::string tail = tileData[index.second][index.first].getTileTail();
		const auto& adj = getNeighborTiles(index.first, index.second);

		if (tileType != TileType::HILL)
			tail = "not_hill";

		const auto& pushList(pushByTypeMap[tail]);

		for (size_t dir = 0; dir < pushList.size(); dir++)
		{
			if (!pushList[dir])
				continue;

			int toJ = adj[dir].first;
			int toI = adj[dir].second;

			if (!isValidIndex(toJ, toI, numOfTileX, numOfTileY))
				continue;

			int to = indexToNumber(toJ, toI, numOfTileX, numOfDummy);

			_graph->addEdge(realtrick::NavGraphEdge(from, to, _graph
				->getNode(from).getPos().getDistance(_graph->getNode(to).getPos())));
		}
	}

	// Now remove the edges which is non-bi-directional.
	for (auto n = std::begin(_graph->getNodes()); n != std::end(_graph->getNodes()); ++n)
	{
		int from = n->getIndex();
		auto e = std::begin(_graph->getEdges(from));

		while (e != std::end(_graph->getEdges(from)))
		{
			int to = e->getTo();
			auto iter = std::find_if(std::begin(_graph->getEdges(to)), std::end(_graph->getEdges(to)), 
				[from](const GraphEdge& e) { return e.getTo() == from; });

			if (iter != std::end(_graph->getEdges(to)))
				++e;
			else
				e = _graph->eraseEdge(from, e);
		}
	}
}


void Game::replaceVictoryScene(float delay)
{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    _isGameEnded = true;
#endif
    
    auto coroutineNode = Sprite::create("client/ui/black_bg.png");
    coroutineNode->setOpacity(0);
    coroutineNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    coroutineNode->runAction(Spawn::create(FadeTo::create(delay, 255),
                                           Sequence::create(DelayTime::create(delay),
                                                            CallFunc::create([this]{
                                            
                                               UserDefault::getInstance()->setBoolForKey("isVictory", true);
                                               Director::getInstance()->replaceScene(RewardScene::createScene());
                                               
                                           }),
                                                            RemoveSelf::create(),
                                                            nullptr),
                                           nullptr));
    addChild(coroutineNode);
}


void Game::replaceDefeatScene(float delay)
{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    _isGameEnded = true;
#endif
    
    auto coroutineNode = Sprite::create("client/ui/black_bg.png");
    coroutineNode->setOpacity(0);
    coroutineNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    coroutineNode->runAction(Spawn::create(FadeTo::create(delay, 255),
                                           Sequence::create(DelayTime::create(delay),
                                                            CallFunc::create([this]{
                                               
                                               UserDefault::getInstance()->setBoolForKey("isVictory", false);
                                               Director::getInstance()->replaceScene(RewardScene::createScene());
                                               
                                           }),
                                                            RemoveSelf::create(),
                                                            nullptr),
                                           nullptr));
    addChild(coroutineNode);
}


bool Game::isAllyState(PlayerType src, PlayerType dest)
{
    int p1 = static_cast<int>(src);
    int p2 = static_cast<int>(dest);
    return _gameResource->_playerInfos[p1].isAllyWith[p2];
}


bool Game::isSharedVisionState(PlayerType src, PlayerType dest)
{
    int p1 = static_cast<int>(src);
    int p2 = static_cast<int>(dest);
    return _gameResource->_playerInfos[p1].isSharedVision[p2];
}


void Game::setAllyState(PlayerType src, PlayerType dest, bool enable)
{
    int p1 = static_cast<int>(src);
    int p2 = static_cast<int>(dest);
    _gameResource->_playerInfos[p1].isAllyWith[p2] = enable;
}


void Game::setSharedVisionState(PlayerType src, PlayerType dest, bool enable)
{
    int p1 = static_cast<int>(src);
    int p2 = static_cast<int>(dest);
    _gameResource->_playerInfos[p1].isSharedVision[p2] = enable;
}


const int Game::getCountdownTimer() const
{
    return _uiLayer->getCountdownTimer();
}


void Game::setCountdownTimer(unsigned int seconds)
{
    _uiLayer->setCountdownTimer(seconds);
}


void Game::addCountdownTimer(unsigned int seconds)
{
    _uiLayer->addCountdownTimer(seconds);
}


void Game::subtractCountdownTimer(unsigned int seconds)
{
    _uiLayer->subtractCountdownTimer(seconds);
}


SwitchStatus Game::getSwitchStatus(int index) const
{
    CCASSERT((index >= 0 && index < 256), "out of switch's index");
    return _gameResource->_switchs[index].status;
}


void Game::setSwitchStatus(int index, SwitchStatus status)
{
    CCASSERT((index >= 0 && index < 256), "out of switch's index");
    _gameResource->_switchs[index].status = status;
}


//---------------------------- isLOSOkay --------------------------------------
//
//  returns true if the ray between A and B is unobstructed.
//------------------------------------------------------------------------------
bool Game::isLOSOkay(cocos2d::Vec2 A, cocos2d::Vec2 B) const
{
	const auto& cols = getNeighborSimpleWalls(A, Segment(A, B));

	bool collide = false;
	for (const auto& col : cols)
	{
		if (physics::intersect(col, Segment(A, B)))
		{
			collide = true;
			break;
		}
	}

	return !collide;
}


bool Game::isLOSOkay(cocos2d::Vec2 A, cocos2d::Vec2 B, float radius) const
{
	Vec2 left = (B - A).getPerp().getNormalized() * radius * 0.5f;
	Vec2 right = -left;

	return isLOSOkay(A + left, B + left) && isLOSOkay(A + right, B + right);
}


void Game::initCell(GameResource* res)
{
    auto worldWidth = res->getTileWidth() * res->getNumOfTileX();
    auto worldHeight = res->getTileHeight() * res->getNumOfTileY();
    auto cellWidth = res->getCellWidth();
    auto cellHeight = res->getCellHeight();
    
    auto numOfCellsX = (worldWidth  / cellWidth) + 2;
    auto numOfCellsY = (worldHeight / cellHeight) + 2;
    
    for (int y = 0; y < numOfCellsY; ++y)
    {
        for (int x = 0; x < numOfCellsX; ++x)
        {
            float left  = x * cellWidth;
            float bot   = y * cellHeight;
            _cellAABBs.push_back(cocos2d::Rect(left - cellWidth, bot - cellHeight, cellWidth, cellHeight));
        }
    }
}


void Game::addWall(const realtrick::Polygon& wall)
{
    //
    //      -------- -------- --------
    //     | 1      | 2      | 3      |
    //     |        |  ------|-. max  |                      _____       _
    //     |       -|-       | |      |            -        -     |     | |
    //     |      | |        | |      |        1: |_|    2:|______|  3: |_|
    //      -------- -------- --------             _                     _
    //     | 4    | | 5      |6|      |        4: |_|    5: ------   6: | |
    //     |       -|----    | |      |                    |      |     | |
    //     |        |    |   | |      |                     ---   |     | |
    //     |        |    |   | |      |                        |__|     |_|
    //      -------- -------- --------                          __       _
    //     | 7      | 8  |   |9|      |        7:        8:    |  |  9: | |
    //     |      . |     ---|-       |                         --       -
    //     |     min|        |        |
    //     |        |        |        |
    //      -------- -------- --------
    //
    //      해당 벽의 최대 최소 x, y를 각각 구해 AABB 를 만든다.
    //      공간들을 순회하면서 겹치는 공간이 있다면, 클리핑한다.
    //      그리고 해당 공간에 클리핑된 벽들을 각각 저장해 놓는다.
    
    float minx = std::min_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.x < v2.x);
    })->x;
    
    float miny = std::min_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.y < v2.y);
    })->y;
    
    float maxx = std::max_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.x < v2.x);
    })->x;
    
    float maxy = std::max_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.y < v2.y);
    })->y;
    
    cocos2d::Vec2 minVertex = cocos2d::Vec2(minx, miny);
    cocos2d::Vec2 maxVertex = cocos2d::Vec2(maxx, maxy);
    cocos2d::Rect wallAABB = cocos2d::Rect(minVertex, cocos2d::Size(maxVertex - minVertex));
    
    for( auto& aabb : _cellAABBs )
    {
        if ( aabb.intersectsRect(wallAABB) )
        {
            std::vector<realtrick::Polygon> clippedWalls = clipping::getClippedPolygons(wall, aabb);
            for( const auto& clippedWall : clippedWalls )
            {
                // add box2d polygons
                b2BodyDef groundDef;
                groundDef.type = b2BodyType::b2_staticBody;
                
                auto ground = _physicsWorld->CreateBody(&groundDef);
                
                int32 size = static_cast<int32>(clippedWall.vertices.size());
                b2Vec2 *v = new b2Vec2[size];
                for(int i = 0 ; i < size ; ++ i)
                {
                    v[i].Set(clippedWall.vertices[i].x, clippedWall.vertices[i].y);
                }
                
                b2ChainShape chain;
                chain.CreateChain(v, size);
                
                delete[] v;
                
                b2FixtureDef groundFixture;
                groundFixture.shape = &chain;
                groundFixture.restitution = 0.5f;
                
                ground->CreateFixture(&groundFixture);
            }
        }
    }
}



