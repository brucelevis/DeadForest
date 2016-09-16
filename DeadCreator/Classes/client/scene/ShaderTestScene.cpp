//
//  ShaderTestScene.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#include "ShaderTestScene.hpp"
#include "DeferredRendering.hpp"
using namespace realtrick::client;
using namespace cocos2d;

ShaderTestScene::ShaderTestScene()
{
}


Scene* ShaderTestScene::createScene()
{
    auto scene = Scene::create();
    auto node = ShaderTestScene::create();
    scene->addChild(node);
    return scene;
}


bool ShaderTestScene::init()
{
    if ( !Node::init() )
        return false;
    
    this->scheduleUpdate();
    
    auto human = Sprite::create("client/sample/grossini.png");
    human->setPosition(0, 100);
    addChild(human);
    
    return true;
}


void ShaderTestScene::update(float dt)
{
    
}









