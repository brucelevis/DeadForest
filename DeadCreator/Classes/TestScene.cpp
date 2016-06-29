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
    file->numOfTileY = 30;
    file->numOfTileX = 30;
    file->tileWidth = 128;
    file->tileHeight = 128;
    file->worldSize = Size(file->numOfTileX * file->tileWidth, file->numOfTileY * file->tileHeight);
    
    _layer = GMXLayer2::create(*file);
    addChild(_layer);
    
    auto btn = ui::Button::create("CloseNormal.png", "CloseSelected.png");
    btn->setPosition(Vec2(500, 300));
    btn->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type) {
        
        if ( type == ui::Widget::TouchEventType::ENDED )
        {
            _layer->setVisible( !_layer->isVisible() );
        }
        
    });
    addChild(btn, 1);
    
    return true;
}









