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
    
    _human = Sprite::create("client/sample/grossini.png");
    _human->setPosition(800, 480);
    addChild(_human);
    
    auto joystick = JoystickEx::create("client/ui/wooden_handle_n.png", "client/ui/wooden_handle_n.png");
    joystick->setJoystickPad("client/ui/wooden_pad.png");
    joystick->setRotationType(JoystickEx::RotationType::ROTATION_8);
    joystick->setPosition(Vec2(300, 300));
    joystick->enableDoubleClick(false);
    joystick->setChangedDirectionAndStatusCallback([this, joystick](Ref* ref, const Vec2& dir, JoystickEx::ClickEventType type) {
        
        if ( type == JoystickEx::ClickEventType::BEGAN || type == JoystickEx::ClickEventType::MOVED ) {
            _humanHeading = joystick->getDirection();
            _humanSpeed = 100.0f;
        }
        else {
            _humanSpeed = 0.0f;
        }
        
    });
    addChild(joystick);
    
    auto bezel = CircularBezel::create("client/ui/wooden_attack_pad.png");
    bezel->setPosition(Vec2(1300, 300));
    bezel->setTriggerRadius( {40.0f, 250.0f} );
    bezel->enableBezelMode(true);
    bezel->enableDebugNode(true);
    bezel->addTriggerCallback([this](Ref* ref, const Vec2& dir, cocos2d::ui::Widget::TouchEventType type) {
        
        if ( type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED ){
            _humanRotation += 10.0f;
        }
        
    });
    addChild(bezel);
    
    auto keyboard = cocos2d::EventListenerKeyboard::create();
    keyboard->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode k, cocos2d::Event* e){
        if (k == cocos2d::EventKeyboard::KeyCode::KEY_0)
        {
            cocos2d::log("0 pressed");
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

    return true;
}


void ShaderTestScene::update(float dt)
{
    _human->setPosition ( _human->getPosition() + _humanHeading * +_humanSpeed * dt );
    _human->setRotation( _humanRotation );
}









