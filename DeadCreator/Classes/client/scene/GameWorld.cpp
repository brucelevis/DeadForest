//
//  GameWorld.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//
//


#include "GameWorld.hpp"
#include "GameManager.hpp"
#include "MessageDispatcher.hpp"
#include "Camera2D.hpp"
#include "RenderTarget.hpp"
#include "UiLayer.hpp"
#include "SingleStream.hpp"
#include "NetworkStream.hpp"
#include "TileBase.hpp"
#include "TileHelperFunctions.hpp"
using namespace cocos2d;
using namespace realtrick::client;

GameWorld::~GameWorld()
{
    CC_SAFE_DELETE(_gameMgr);
    CC_SAFE_DELETE(_logicStream);
}


Scene* GameWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = GameWorld::create();
    scene->addChild(layer);
    return scene;
}


bool GameWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _winSize = Director::getInstance()->getVisibleSize();
    this->scheduleUpdate();
    
    _gameMgr = new GameManager(this);
    
    _renderTarget = RenderTarget::create(_gameMgr);
    addChild(_renderTarget);
    
    _gameCamera = Camera2D::create(_gameMgr);
    addChild(_gameCamera);
    _gameMgr->setGameCamera(_gameCamera);
    
    // 일단 중지
    pauseGame();
    
    // single or network (?)
    if( Prm.getValueAsBool("useNetwork") ) { _logicStream = new NetworkStream(_gameMgr); }
    else { _logicStream = new SingleStream(_gameMgr); }
    
    this->pushLogic(0.0, MessageType::LOAD_GAME_PLAYER, nullptr);
    
    return true;
}


void GameWorld::loadUiLayer()
{
    // palyer를 세팅하고 Ui를 로딩한다.
    setPlayerPtr(_gameMgr->getPlayerPtr());
    
    _uiLayer = UiLayer::create(_gameMgr);
    addChild(_uiLayer);
    
    // init game map
    _gameMgr->getGameMap()->updateChunk(_player->getWorldPosition());
    _gameCamera->setCameraPos(_player->getWorldPosition());
    
    int id = experimental::AudioEngine::play2d("rainfall.mp3", true);
    experimental::AudioEngine::setVolume(id, 0.3f);
    
    this->setZoom(Prm.getValueAsFloat("cameraZoom"));
}


void GameWorld::setZoom(float r)
{
    _gameMgr->getDebugNode()->setScale(r);
    _renderTarget->setZoom(r);
}


void GameWorld::update(float dt)
{
    _logicStream->processPacket(dt);
    
    if ( _isPaused ) return ;
    
    _gameMgr->getDebugNode()->clear();
    
    Vec2 oldCameraPos = _gameCamera->getCameraPos();
    pair<int, int> oldIndex = getFocusedTileIndex(oldCameraPos, _gameMgr->getGameMap()->getTileWidth(),
                                                  _gameMgr->getGameMap()->getTileHeight(),
                                                  DUMMY_TILE_SIZE);
    
    _gameMgr->update(dt);
    _gameCamera->setCameraPos(_player->getWorldPosition());
    
    if ( oldIndex != getFocusedTileIndex(_gameCamera->getCameraPos(), _gameMgr->getGameMap()->getTileWidth(),
                                         _gameMgr->getGameMap()->getTileHeight(),
                                         DUMMY_TILE_SIZE))
    {
        _gameMgr->getGameMap()->updateChunk(oldCameraPos);
    }
    
    Dispatch.dispatchDelayedMessages();
}

void GameWorld::pushLogic(double delaySeconds, MessageType type, void* extraInfo)
{
    Dispatch.pushMessage(delaySeconds, _logicStream, nullptr, type, extraInfo);
}

void GameWorld::pauseGame()
{
    _isPaused = true;
    _renderTarget->pauseGame();
}

void GameWorld::resumeGame()
{
    _isPaused = false;
    _renderTarget->resumeGame();
}









