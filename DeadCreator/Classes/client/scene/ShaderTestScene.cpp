//
//  ShaderTestScene.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#include "ShaderTestScene.hpp"
#include "DeferredRendering.hpp"
#include "CircularBezel.hpp"
using namespace realtrick::client;
using namespace cocos2d;

ShaderTestScene::ShaderTestScene()
{
}


Scene* ShaderTestScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
    
    auto node = ShaderTestScene::create();
    scene->addChild(node);
    return scene;
}


bool ShaderTestScene::init()
{
    if ( !Node::init() )
        return false;
    
    this->scheduleUpdate();
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact& contact)->bool{
        
        auto a = contact.getShapeA()->getBody();
        auto b = contact.getShapeB()->getBody();
        
        if ( (1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) ||
            (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()) )
        {
            log("collision has occured %d", random(0,100));
        }
        
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    auto edgeBody = PhysicsBody::createEdgeBox(Size(1136, 640), PHYSICSBODY_MATERIAL_DEFAULT, 1);
    auto edgeNode = Node::create();
    edgeNode->setPosition(800, 490);
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);
    
    
    auto spr = Sprite::create("client/sample/grossini.png");
    spr->setPosition(850, 500);
    addChild(spr);
    
    auto physicsBody = PhysicsBody::createBox(spr->getContentSize(), PhysicsMaterial(0, 1, 0));
    physicsBody->setDynamic(true);
    physicsBody->setCollisionBitmask(1);
    physicsBody->setContactTestBitmask(true);
//    physicsBody->setVelocity(Vec2(500, 547));
//    physicsBody->setVelocityLimit( 500 );
    physicsBody->setLinearDamping(0.5);
//    physicsBody->applyForce( Vec2(100, 78) );
    physicsBody->applyImpulse(Vec2(400, 700));
    spr->setPhysicsBody(physicsBody);
    
    {
        auto spr = Sprite::create("client/sample/grossini.png");
        spr->setPosition(850, 300);
        addChild(spr);
        
        auto physicsBody = PhysicsBody::createBox(spr->getContentSize(), PhysicsMaterial(0, 1, 0));
        physicsBody->setCollisionBitmask(2);
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        
        spr->setPhysicsBody(physicsBody);
    }
    
    return true;
}


void ShaderTestScene::update(float dt)
{
    
}









