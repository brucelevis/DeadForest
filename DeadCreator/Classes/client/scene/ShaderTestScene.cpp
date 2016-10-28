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
//    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
    auto scene = Scene::create();
    auto node = ShaderTestScene::create();
    scene->addChild(node);
    return scene;
}


bool ShaderTestScene::init()
{
    if ( !Node::init() )
        return false;
    
    this->scheduleUpdate();
    
//    auto contactListener = EventListenerPhysicsContact::create();
//    contactListener->onContactBegin = [this](PhysicsContact& contact)->bool{
//        
//        auto a = contact.getShapeA()->getBody();
//        auto b = contact.getShapeB()->getBody();
//        
//        if ( (1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) ||
//            (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()) )
//        {
//            log("collision has occured %d", random(0,100));
//        }
//        
//        return true;
//    };
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
//    
//    auto edgeBody = PhysicsBody::createEdgeBox(Size(1136, 640), PHYSICSBODY_MATERIAL_DEFAULT, 1);
//    auto edgeNode = Node::create();
//    edgeNode->setPosition(800, 490);
//    edgeNode->setPhysicsBody(edgeBody);
//    addChild(edgeNode);
//    
//    
//    auto spr = Sprite::create("client/sample/grossini.png");
//    spr->setPosition(850, 500);
//    addChild(spr);
//    
//    auto physicsBody = PhysicsBody::createBox(spr->getContentSize(), PhysicsMaterial(0, 1, 0));
//    physicsBody->setDynamic(true);
//    physicsBody->setCollisionBitmask(1);
//    physicsBody->setContactTestBitmask(true);
////    physicsBody->setVelocity(Vec2(500, 547));
////    physicsBody->setVelocityLimit( 500 );
//    physicsBody->setLinearDamping(0.5);
////    physicsBody->applyForce( Vec2(100, 78) );
//    physicsBody->applyImpulse(Vec2(400, 700));
//    spr->setPhysicsBody(physicsBody);
//    
//    {
//        auto spr = Sprite::create("client/sample/grossini.png");
//        spr->setPosition(850, 300);
//        addChild(spr);
//        
//        auto physicsBody = PhysicsBody::createBox(spr->getContentSize(), PhysicsMaterial(0, 1, 0));
//        physicsBody->setCollisionBitmask(2);
//        physicsBody->setContactTestBitmask(true);
//        physicsBody->setDynamic(false);
//        
//        spr->setPhysicsBody(physicsBody);
//    }
    
    _isMoved = false;
    _dir = Vec2::UNIT_X;
    
    auto keyboard = EventListenerKeyboard::create();
    keyboard->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
    {
        if ( keyCode == EventKeyboard::KeyCode::KEY_W )
        {
            _dir += Vec2(0, 2000000);
            _isMoved = true;
        }
        
        if ( keyCode == EventKeyboard::KeyCode::KEY_S )
        {
            _dir += Vec2(0, -2000000);
            _isMoved = true;
        }
        
        if ( keyCode == EventKeyboard::KeyCode::KEY_A )
        {
            _dir += Vec2(-2000000, 0);
            _isMoved = true;
        }
        
        if ( keyCode == EventKeyboard::KeyCode::KEY_D )
        {
            _dir += Vec2(2000000, 0);
            _isMoved = true;
        }
    };
    
    keyboard->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
    {
        if ( keyCode == EventKeyboard::KeyCode::KEY_W ||
            keyCode == EventKeyboard::KeyCode::KEY_S ||
            keyCode == EventKeyboard::KeyCode::KEY_A ||
            keyCode == EventKeyboard::KeyCode::KEY_D )
        {
            _isMoved = false;
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);
    
    auto mouse = EventListenerMouse::create();
    mouse->onMouseMove = [this](cocos2d::EventMouse* e)
    {
        auto p = e->getLocation();
        p.y = _director->getVisibleSize().height - p.y;
        
        _body->ApplyTorque(3000000, true);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse, this);
    
    _world = new b2World(b2Vec2(0.0f, 0.0f));
    
    // sample
    _sprite = Sprite::create("client/sample/grossini.png");
    _sprite->setPosition(800, 490);
    addChild(_sprite);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.bullet = false;
    bodyDef.angle = 0.0f;
    bodyDef.allowSleep = true;
    
    _body = _world->CreateBody(&bodyDef);
    _body->SetLinearDamping(3);
    
    b2PolygonShape shape;
    shape.SetAsBox(_sprite->getContentSize().width / 2.0f, _sprite->getContentSize().height / 2.0f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.1f;
    
    _body->CreateFixture(&fixtureDef);
    _body->SetTransform(b2Vec2(_sprite->getPositionX(), _sprite->getPositionY()), b2_pi * 0.25f);
    _sprite->setRotation(CC_RADIANS_TO_DEGREES(b2_pi * 0.25f));
    
    
    // ground
    b2BodyDef groundDef;
    groundDef.type = b2BodyType::b2_staticBody;
    
    auto ground = _world->CreateBody(&groundDef);
    
    b2Vec2 v[4];
    v[0].Set(0.0f, 0.0f);
    v[1].Set(1600.0f, 0.0f);
    v[2].Set(1600.0f, 980.0f);
    v[3].Set(0.0f, 980.0f);
    
    b2ChainShape chain;
    chain.CreateChain(v, 4);
    
    b2FixtureDef groundFixture;
    groundFixture.shape = &chain;
    groundFixture.restitution = 0.5f;
    
    ground->CreateFixture(&groundFixture);
    
    return true;
}


void ShaderTestScene::update(float dt)
{
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 10;
    int32 positionIterations = 8;
    _world->Step(timeStep, velocityIterations, positionIterations);
    
    _sprite->setPosition(_body->GetPosition().x, _body->GetPosition().y);
    _sprite->setRotation(CC_RADIANS_TO_DEGREES(_body->GetAngle()));
    
    _world->ClearForces();
    log("%f %f", _body->GetPosition().x, _body->GetPosition().y);
    
    if ( _isMoved )
    {
        _body->SetLinearVelocity(b2Vec2(_dir.x, _dir.y));
    }
    
    _dir = Vec2(_body->GetLinearVelocity().x, _body->GetLinearVelocity().y);
}









