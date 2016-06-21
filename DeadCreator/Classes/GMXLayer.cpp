//
//  GMXLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include "GMXLayer.hpp"
#include "GMXFileManager.hpp"
using namespace cocos2d;


GMXLayer::GMXLayer()
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
    
    _clipNode = ClippingRectangleNode::create(Rect(0, 0, 30, 30));
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









