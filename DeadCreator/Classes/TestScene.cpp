//
//  TestScene.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 26..
//
//

#include "TestScene.hpp"
#include "GMXFile.hpp"
#include "ui/CocosGUI.h"
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
    
    GMXFile* file = new GMXFile();
    file->numOfTileY = 32;
    file->numOfTileX = 32;
    file->tileWidth = 128;
    file->tileHeight = 128;
    file->worldSize = Size(file->numOfTileX * file->tileWidth, file->numOfTileY * file->tileHeight);
    
    _layer = GMXLayer2::create(*file);
    addChild(_layer);
    
    auto l2 = GMXLayer2::create(*file);
    addChild(l2);
    
    return true;
}









