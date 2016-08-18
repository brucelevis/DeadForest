//
//  RenderingSystem.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "RenderingSystem.hpp"
#include "SizeProtocol.h"
#include "Terrain.hpp"
#include "EntityBase.hpp"
#include "UiLayer.hpp"
#include "Camera2D.hpp"
using namespace cocos2d;
using namespace realtrick::client;


RenderingSystem::RenderingSystem(Game* game) : _game(game)
{
}


RenderingSystem::~RenderingSystem()
{
}


RenderingSystem* RenderingSystem::create(Game* game, GameResource* res)
{
    auto ret = new (std::nothrow) RenderingSystem(game);
    if ( ret && ret->init(res) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool RenderingSystem::init(GameResource* res)
{
    if ( !Node::init() )
        return false;
    
    _gameScreenScale = Vec2(GAME_SCREEN_WIDTH / 1136, GAME_SCREEN_HEIGHT / 640);
    
    _renderingNode = Node::create();
    _renderingNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _renderingNode->retain();
    
    _terrain = Terrain::create(_game);
    _renderingNode->addChild(_terrain);

    _fbo = RenderTexture::create(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    _fbo->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    addChild(_fbo, 5);
    
    return true;
}


void RenderingSystem::updateChunk(Camera2D* camera)
{
    _terrain->updateChunk(camera->getCameraPos());
}


void RenderingSystem::addEntity(EntityBase* entity, int zOrder)
{
    _renderingNode->addChild(entity, zOrder);
}


void RenderingSystem::removeEntity(EntityBase* entity)
{
    entity->removeFromParentAndCleanup(true);
}


void RenderingSystem::addUINode(cocos2d::Node* node)
{
    this->addChild(node, 100);
}


void RenderingSystem::render(Camera2D* camera)
{
    _fbo->beginWithClear(0.0, 0.0, 1.0, 0.2);
    
    cameraTransform(camera);
    _renderingNode->visit();
    
    _fbo->end();
}


void RenderingSystem::cameraTransform(Camera2D* camera)
{
    _renderingNode->setScale(_gameScreenScale.x * _zoomScale, _gameScreenScale.y * _zoomScale);
    for( auto& entity : _renderingNode->getChildren() )
    {
        auto ent = static_cast<EntityBase*>(entity);
        ent->setPosition( ent->getWorldPosition() - camera->getCameraPos() );
    }
}









