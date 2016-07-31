//
//  RenderTarget.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 3..
//
//

#include "RenderTarget.hpp"
#include "Game.hpp"
#include "OcculusionBuilder.hpp"
#include "LightEffect.hpp"
#include "EffectSprite.hpp"
#include "EntityBase.hpp"
#include "SizeProtocol.h"
using namespace realtrick::client;
using namespace cocos2d;


RenderTarget::RenderTarget(Game* game):
_staticTarget(nullptr),
_dynamicTarget(nullptr),
_game(game),
_winSize(Size::ZERO),
_normalTex(nullptr),
_staticTex(nullptr),
_dynamicTex(nullptr),
_occulusionTex(nullptr),
_normalBg(nullptr),
_deferredRenderTarget(nullptr),
_effect(nullptr),
_isPaused(true)
{}


RenderTarget::~RenderTarget()
{
    CC_SAFE_RELEASE(_effect);
    CC_SAFE_RELEASE(_normalTex);
    CC_SAFE_RELEASE(_staticTex);
    CC_SAFE_RELEASE(_staticTarget);
    CC_SAFE_RELEASE(_dynamicTex);
    CC_SAFE_RELEASE(_occulusionTex);
    CC_SAFE_RELEASE(_normalBg);
}


RenderTarget* RenderTarget::create(Game* game)
{
    RenderTarget* ret = new (std::nothrow) RenderTarget(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool RenderTarget::init()
{
    if ( !Node::init() )
        return false;
    
    _winSize = Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    
    _effect = LightEffect::create("lights.fsh");
    _effect->retain();
    
    DirectionalLight* dl = DirectionalLight::create(Vec3(0, -0.7f, -0.7f));
    dl->setColor(Vec3(0.7f, 0.7f, 0.7f));
    dl->setIntensity(1.0f);
    _effect->addLight(dl);

    _deferredRenderTarget = EffectSprite::create("normal_bg.png");
    _deferredRenderTarget->setPosition(_winSize / 2);
    _deferredRenderTarget->setEffect(_effect);
    _deferredRenderTarget->setFlippedY(true);
    addChild(_deferredRenderTarget);

    _staticTarget = Node::create();
    _dynamicTarget = Node::create();
    
    _staticTarget->setPosition(_winSize/2);
    _dynamicTarget->setPosition(_winSize/2);
    
    _staticTarget->retain();
    _dynamicTarget->retain();

    _normalTex = RenderTexture::create(_winSize.width, _winSize.height);
    _staticTex = RenderTexture::create(_winSize.width, _winSize.height);
    _dynamicTex = RenderTexture::create(_winSize.width, _winSize.height);
    _occulusionTex = RenderTexture::create(_winSize.width, _winSize.height);

    _occulusionBuilder = OcculusionBuilder::create(_winSize);
    
    _normalTex->retain();
    _staticTex->retain();
    _dynamicTex->retain();
    _occulusionTex->retain();
    _occulusionBuilder->retain();
    
    _normalBg = Sprite::create("normal_bg.png");
    _normalBg->setPosition(_winSize / 2);
    _normalBg->retain();
    
    return true;
}


cocos2d::Texture2D* RenderTarget::getNormalTex() const
{
    for ( auto &child : _dynamicTarget->getChildren() )
    {
        EntityBase* ent = dynamic_cast<EntityBase*>(child);
        if ( ent ) ent->enableNormal(true);
    }
    
    _normalTex->beginWithClear(0.0, 0.0, 0.0, 0.0);
    
    _normalBg->visit();
    _dynamicTarget->visit();
    
    _normalTex->end();
    
    for ( auto &child : _dynamicTarget->getChildren() )
    {
        EntityBase* ent = dynamic_cast<EntityBase*>(child);
        if ( ent ) ent->enableNormal(false);
    }
    
    return _normalTex->getSprite()->getTexture();
}


cocos2d::Texture2D* RenderTarget::getStaticTex() const
{
    _staticTex->beginWithClear(0.0, 0.0, 0.0, 0.0);
    
    _staticTarget->visit();
    
    _staticTex->end();
    
    return _staticTex->getSprite()->getTexture();
}


cocos2d::Texture2D* RenderTarget::getDynamicTex() const
{
    _dynamicTex->beginWithClear(0.0, 0.0, 0.0, 0.0);
    
    _dynamicTarget->visit();
    
    _dynamicTex->end();
    
    return _dynamicTex->getSprite()->getTexture();
}


cocos2d::Texture2D* RenderTarget::getOcculusionTex() const
{
    OcculusionBuilder::FieldOfView fov;
    fov.aroundCircleRadius = 40.0f;
    fov.aroundCircleSlice = 30;
    fov.entryDegree = 110.0f;
    fov.heading = _game->getPlayerPtr()->getHeading();
    fov.isEnable = Prm.getValueAsBool("isFovMode");
    
    EntityHuman* player = _game->getPlayerPtr();
    _occulusionBuilder->invalidateTexture(_occulusionTex, player->getWorldPosition(), _game->getNeighborWalls(player->getWorldPosition(), _winSize), _winSize, fov);
    
    return _occulusionTex->getSprite()->getTexture();
}


void RenderTarget::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
    if ( _isPaused ) return ;
    
    _effect->setOcculusionMap(getOcculusionTex());
    _effect->setStaticMap(getStaticTex());
    _effect->setDynamicMap(getDynamicTex());
    _effect->setNormalMap(getNormalTex());
    
    Node::visit(renderer, parentTransform, parentFlags);
}

void RenderTarget::setZoom(float ratio)
{
    _deferredRenderTarget->setScale(ratio);
    _occulusionTex->setScale(ratio);
}


void RenderTarget::addDynamicEntity(EntityBase* entity, int zOrder)
{
    _dynamicTarget->addChild(entity, zOrder);
}


void RenderTarget::addStaticEntity(cocos2d::Node* entity, int zOrder)
{
    _staticTarget->addChild(entity, zOrder);
}









