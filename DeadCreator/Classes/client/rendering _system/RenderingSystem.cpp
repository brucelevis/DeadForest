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
#include "Camera2D.hpp"
#include "EntityBase.hpp"
#include "UiLayer.hpp"
using namespace cocos2d;
using namespace realtrick::client;


RenderingSystem::RenderingSystem(Game* game) : _game(game)
{
}


RenderingSystem::~RenderingSystem()
{
    CC_SAFE_DELETE(_camera);
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
    
    _clipNode = ClippingRectangleNode::create(cocos2d::Rect(0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
    addChild(_clipNode);
    
    _renderingNode = Node::create();
    _renderingNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _clipNode->addChild(_renderingNode);
    
    _terrain = Terrain::create(_game);
    _renderingNode->addChild(_terrain);
    
    _camera = new Camera2D();
    
    return true;
}


cocos2d::Vec2 RenderingSystem::getCameraPosition() const
{
    return _camera->getCameraPos();
}


void RenderingSystem::setCameraPosition(const cocos2d::Vec2& pos)
{
    _camera->setCameraPos(pos);
}


void RenderingSystem::updateChunk()
{
    _terrain->updateChunk(getCameraPosition());
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
    _clipNode->addChild(node, 100);
}


void RenderingSystem::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    _renderingNode->setScale(_gameScreenScale.x * _zoomScale, _gameScreenScale.y * _zoomScale);
    
    for( auto& entity : _renderingNode->getChildren() )
    {
        auto ent = static_cast<EntityBase*>(entity);
        ent->setPosition( ent->getWorldPosition() - getCameraPosition() );
    }
    Node::visit(renderer, transform, flags);
}









