//
//  DeferredRendering.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#include "DeferredRendering.hpp"
#include "RenderTarget.hpp"
#include "RenderingSystem.hpp"
#include "Effects.hpp"
#include "EntityBase.hpp"
#include "OcculusionBaker.hpp"
#include "Game.hpp"
#include "GameResource.hpp"
using namespace cocos2d;
using namespace realtrick::client;

#include "SimpleProfiler.hpp"


DeferredRendering::DeferredRendering(Game* game) : _game(game)
{
}


DeferredRendering::~DeferredRendering()
{
    CC_SAFE_RELEASE_NULL(_dynamicEntities);
    CC_SAFE_RELEASE_NULL(_staticEntities);
    CC_SAFE_RELEASE_NULL(_dynamicTexture);
    CC_SAFE_RELEASE_NULL(_staticTexture);
    CC_SAFE_RELEASE_NULL(_normalTexture);
    CC_SAFE_RELEASE_NULL(_occlusionTexture);
    CC_SAFE_RELEASE_NULL(_occulusionBaker);
}


DeferredRendering* DeferredRendering::create(Game* game, const std::string& basicTextureName)
{
    auto ret = new (std::nothrow) DeferredRendering(game);
    if ( ret && ret->initWithFile(basicTextureName) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool DeferredRendering::initWithFile(const std::string& basicTextureName)
{
    if ( !EffectSprite::initWithFile(basicTextureName) )
        return false;
    
    setFlippedY(true);
    setEffect(EffectDeferredRendering::create());
    
    _dynamicEntities = Node::create();
    _dynamicEntities->setPosition(getContentSize() / 2);
    _dynamicEntities->retain();
    
    _staticEntities = Node::create();
    _staticEntities->setPosition(getContentSize() / 2);
    _staticEntities->retain();
    
    _dynamicTexture = RenderTexture::create(getContentSize().width, getContentSize().height);
    _dynamicTexture->retain();
    
    _staticTexture = RenderTexture::create(getContentSize().width, getContentSize().height);
    _staticTexture->retain();
    
    _normalTexture = RenderTexture::create(getContentSize().width, getContentSize().height);
    _normalTexture->retain();
    
    _occlusionTexture = RenderTexture::create(getContentSize().width, getContentSize().height);
    _occlusionTexture->retain();
    
    _occulusionBaker = OcculusionBaker::create(getContentSize());
    _occulusionBaker->retain();
    
    return true;
}


void DeferredRendering::prepareToRender(const cocos2d::Vec2& zoomScale, const cocos2d::Vec2& cameraPos)
{
    // dynamic entities transform
    _dynamicEntities->setScale(zoomScale.x, zoomScale.y);
    for( auto& entity : _dynamicEntities->getChildren() )
    {
        auto ent = static_cast<EntityBase*>(entity);
        ent->setPosition( ent->getWorldPosition() - cameraPos );
    }
    
    // static entities transform
    _staticEntities->setScale(zoomScale.x, zoomScale.y);
    for( auto& entity : _staticEntities->getChildren() )
    {
        auto ent = static_cast<EntityBase*>(entity);
        ent->setPosition( ent->getWorldPosition() - cameraPos );
    }
    
    // visits
    _dynamicTexture->beginWithClear(0.0, 0.0, 0.0, 0.0);
    _dynamicEntities->visit();
    _dynamicTexture->end();
    
    _staticTexture->beginWithClear(0.0, 0.0, 0.0, 0.0);
    _staticEntities->visit();
    _staticTexture->end();
    
    // bake normal map
//    for ( auto& entity : _dynamicEntities->getChildren() )
//        static_cast<EntityBase*>(entity)->enableNormal(true);
    
//    _normalTexture->beginWithClear(0.0, 0.0, 0.0, 0.0);
//    _dynamicEntities->visit();
//    _normalTexture->end();
    
//    for ( auto& entity : _dynamicEntities->getChildren() )
//        static_cast<EntityBase*>(entity)->enableNormal(false);
    
    // bake occlusion texture
    auto player = _game->getPlayerPtr();
    OcculusionBaker::FieldOfView fov;
    fov.aroundCircleRadius = 40.0f;
    fov.aroundCircleSlice = 30;
    fov.entryDegree = 110.0f;
    fov.heading = player->getHeading();
    fov.isEnable = true;
    PROFILE_BEGIN("bake occlusion map");
    _occulusionBaker->bakeTexture(_occlusionTexture,
                                  player->getWorldPosition(),
                                  zoomScale,
                                  _game->getNeighborWalls(player->getWorldPosition(), Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT)),
                                  Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT), fov);
    PROFILE_END("bake occlusion map");
    
    getGLProgramState()->setUniformTexture("u_dynamicTex", _dynamicTexture->getSprite()->getTexture());
    getGLProgramState()->setUniformTexture("u_staticTex", _staticTexture->getSprite()->getTexture());
//    getGLProgramState()->setUniformTexture("u_normalTex", _normalTexture->getSprite()->getTexture());
    getGLProgramState()->setUniformTexture("u_occlusionTex", _occlusionTexture->getSprite()->getTexture());
}


void DeferredRendering::addDynamicEntity(EntityBase* node, int zOrder)
{
    _dynamicEntities->addChild(node, zOrder);
}


void DeferredRendering::addStaticEntity(EntityBase* node, int zOrder)
{
    _staticEntities->addChild(node, zOrder);
}









