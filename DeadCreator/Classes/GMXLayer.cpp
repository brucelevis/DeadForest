//
//  GMXLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include <boost/any.hpp>

#include "GMXLayer.hpp"
#include "GMXFileManager.hpp"
#include "MinimapLayer.hpp"
using namespace cocos2d;

#include "CCImGui.h"


GMXLayer::GMXLayer() :
_centerViewParams(Vec2::ZERO)
{
}


GMXLayer::~GMXLayer()
{
}


GMXLayer* GMXLayer::create(const std::string& fileName)
{
    auto ret = new (std::nothrow) GMXLayer();
    if ( ret && ret->init(fileName) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


bool GMXLayer::init(const std::string& fileName)
{
    if ( !Node::init() )
        return false;
    
    _gmxFileManager = new (std::nothrow) GMXFileManager(&_file);
    if ( !_gmxFileManager || !_gmxFileManager->loadGMXFile(fileName) )
    {
        CC_SAFE_FREE(_gmxFileManager);
        return false;
    }
    
    _clipNode = ClippingRectangleNode::create(Rect(0, 0, 0, 0));
    addChild(_clipNode);
    
    _tileRoot = Node::create();
    _clipNode->addChild(_tileRoot);
    
    _tileImages.resize(_file.numOfTileY);
    for(int i = 0 ; i < _file.numOfTileY ; ++ i)
    {
        _tileImages[i].resize(_file.numOfTileX);
    }
    
    for(int i = 0 ; i < _file.numOfTileY ; ++ i)
    {
        for(int j = 0 ; j < _file.numOfTileX ; ++ j)
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
    
    return true;
}


void GMXLayer::centerView(const cocos2d::Vec2& params)
{
    // todo
    _tileRoot->setPosition(-Vec2(params.x * (_file.worldSize.width - _clipNode->getClippingRegion().size.width),
                                 params.y * (_file.worldSize.height - _clipNode->getClippingRegion().size.height)));
    
}


void GMXLayer::onResize()
{
    float padding = boost::any_cast<float>(CCIMGUI->getValue("windowPadding"));
    float statusBarHeight = boost::any_cast<float>(CCIMGUI->getValue("statusBarHeight"));
    float menuBarHeight = boost::any_cast<float>(CCIMGUI->getValue("menuBarHeight"));
    auto visibleSize = _director->getVisibleSize();
    
    Rect clipRect = Rect(0, 0,
                         visibleSize.width - _minimap->getLayerSize().width - padding * 3,
                         visibleSize.height - menuBarHeight - statusBarHeight - padding * 2);
    
    setClippingRegion(clipRect);
}








