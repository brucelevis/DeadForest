//
//  GMXLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include "GMXLayer.hpp"
#include "GMXLayerManager.hpp"
#include "MinimapLayer.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;

#include "CCImGui.h"


GMXLayer::GMXLayer() :
_isOpened(false)
{
}


GMXLayer::~GMXLayer()
{
}


GMXLayer* GMXLayer::create()
{
    auto ret = new (std::nothrow) GMXLayer();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


bool GMXLayer::init()
{
    if ( !Node::init() )
        return false;
    
    auto visibleSize = _director->getVisibleSize();
    _clipNode = ClippingRectangleNode::create(Rect(0, 0,
                                                   visibleSize.width - MINIMAP_SIZE - WINDOW_PADDING * 3,
                                                   visibleSize.height - MENUBAR_HEIGHT - STATUSBAR_HEIGHT - WINDOW_PADDING * 2));
    addChild(_clipNode);
    
    _tileRoot = Node::create();
    _clipNode->addChild(_tileRoot);
    
    return true;
}


void GMXLayer::openFile(GMXFile* file)
{
    _isOpened = true;
    _file = file;
    
    int x = file->numOfTileX;
    int y = file->numOfTileY * 2;
    
    
    _tileImages.resize(y);
    for(int i = 0 ; i < y ; ++ i)
    {
        _tileImages[i].resize(x);
    }
    
    for(int i = 0 ; i < y; ++ i)
    {
        for(int j = 0 ; j < x ; ++ j)
        {
            Vec2 tilePosition;
            if ( i % 2 == 0)
            {
                tilePosition.setPoint(j * 128, i * 64);
            }
            else
            {
                tilePosition.setPoint(64 + j * 128, i * 64);
            }
            if ( rand() % 2  == 0)
            {
                _tileImages[i][j] = Sprite::create("dirt.png");
            }
            else
            {
                _tileImages[i][j] = Sprite::create("grass.png");
            }
            _tileImages[i][j]->setPosition(tilePosition);
            _tileRoot->addChild(_tileImages[i][j]);
        }
    }
}


void GMXLayer::onCenterView(const cocos2d::Vec2& params)
{
    // todo
    if ( isOpened() )
    {
        _tileRoot->setPosition(-Vec2(params.x * (_file->worldSize.width - _clipNode->getClippingRegion().size.width),
                                     params.y * (_file->worldSize.height - _clipNode->getClippingRegion().size.height)));
    }
}


void GMXLayer::onResize()
{
    auto visibleSize = _director->getVisibleSize();
    Rect clipRect = Rect(0, 0,
                         visibleSize.width - MINIMAP_SIZE - WINDOW_PADDING * 3,
                         visibleSize.height - MENUBAR_HEIGHT - STATUSBAR_HEIGHT - WINDOW_PADDING * 2);
    
    setClippingRegion(clipRect);
}








