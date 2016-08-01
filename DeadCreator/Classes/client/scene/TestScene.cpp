//
//  TestScene.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 9..
//
//

#include "TestScene.hpp"
#include "Server.hpp"
#include "MainMenu3.hpp"
#include "GeneratedPackets.hpp"
#include "SizeProtocol.h"
using namespace realtrick::client;
using namespace cocos2d;


TestScene::TestScene()
{
}


TestScene* TestScene::create()
{
    auto ret = new (std::nothrow) TestScene();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


Scene* TestScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TestScene::create();
    scene->addChild(layer);
    return scene;
}


bool TestScene::init()
{
    if ( !Layer::init() )
        return false;
    
    this->scheduleUpdate();
    
    return true;
}


void TestScene::update(float dt)
{
    
}









