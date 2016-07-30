//
//  GameWorld.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//


#include "GameWorld.hpp"
#include "GameManager.hpp"
#include "MessageDispatcher.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;
using namespace realtrick::client;

GameWorld::GameWorld() :
_winSize(Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT)),
_gameMgr(nullptr)
{}


GameWorld::~GameWorld()
{
    Dispatch.clearQueue();
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
    
    return true;
}


void GameWorld::update(float dt)
{
    // game loop
    _gameMgr->update(dt);
    Dispatch.dispatchDelayedMessages();
}









