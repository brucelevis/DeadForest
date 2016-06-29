//
//  TileImage.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 29..
//
//

#include "TileImage.hpp"
#include "GMXLayer2.hpp"

TileImage::TileImage(GMXLayer2& layer) :
_gmxLayer(layer)
{
}


TileImage::~TileImage()
{
}


TileImage* TileImage::create(GMXLayer2& layer)
{
    auto ret = new (std::nothrow) TileImage(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


TileImage* TileImage::create(GMXLayer2& layer, const std::string& fileName)
{
    auto ret = new (std::nothrow) TileImage(layer);
    if ( ret && ret->initWithFile(fileName) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


TileImage* TileImage::createWithFrameName(GMXLayer2& layer, const std::string& frameName)
{
    auto ret = new (std::nothrow) TileImage(layer);
    if ( ret && ret->initWithSpriteFrameName(frameName) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void TileImage::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
    setPosition(_worldPosition - _gmxLayer.getRootTileWorldPosition());
    Node::visit(renderer, parentTransform, parentFlags);
}






