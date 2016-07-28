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
#include "SizeProtocol.h"
using namespace cocos2d;
using namespace realtrick::client;

GameWorld::GameWorld() :
_winSize(Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT)),
_gameMgr(nullptr),
_logicStream(nullptr)
{}


GameWorld::~GameWorld()
{
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
        return false;
    
    this->scheduleUpdate();
    
    _gameMgr = GameManager::create(this);
    addChild(_gameMgr);
    
    // single or network (?)
    if( Prm.getValueAsBool("useNetwork") ) { _logicStream = new NetworkStream(_gameMgr); }
    else { _logicStream = new SingleStream(_gameMgr); }
    
    this->pushLogic(0.0, MessageType::LOAD_GAME_PLAYER, nullptr);
    
    return true;
}


void GameWorld::update(float dt)
{
    _logicStream->processPacket(dt);
    
    _gameMgr->update(dt);
    
    Dispatch.dispatchDelayedMessages();
}

void GameWorld::pushLogic(double delaySeconds, MessageType type, void* extraInfo)
{
    Dispatch.pushMessage(delaySeconds, _logicStream, nullptr, type, extraInfo);
}










