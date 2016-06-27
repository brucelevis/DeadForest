//
//  TestScene.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 26..
//
//

#include "TestScene.hpp"
using namespace cocos2d;

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
    
    _layer = GMXLayer2::create(Size(300, 300));
    addChild(_layer);
    
    return true;
}









