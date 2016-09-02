//
//  UiLayer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 20..
//
//

#include "UiLayer.hpp"
#include "JoystickEx.hpp"
#include "Game.hpp"
#include "EntityPlayer.hpp"
#include "MessageTypes.hpp"
#include "Items.hpp"
#include "Camera2D.hpp"
#include "CircularBezel.hpp"
#include "HpBar.hpp"
#include "InfoSystem.hpp"
#include "SizeProtocol.h"
#include "ParamLoader.hpp"
#include "InputCommands.hpp"
#include "RenderingSystem.hpp"
#include "CrossHair.hpp"
#include "InventoryView.hpp"
using namespace realtrick::client;
using namespace cocos2d;


UiLayer::UiLayer(Game* game) : _game(game)
{
}


UiLayer::~UiLayer()
{
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}


UiLayer* UiLayer::create(Game* game)
{
    auto ret = new (std::nothrow) UiLayer(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool UiLayer::init()
{
    if ( !Node::init() )
        return false;
    
    _winSize = Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    this->scheduleUpdate();
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    
    Vec2 joystickScale(GAME_SCREEN_WIDTH / 1136, GAME_SCREEN_HEIGHT / 640);
    Vec2 joystickPos = Vec2(150.0f * joystickScale.x, 150.0f * joystickScale.y);
    
    _moveJoystick = JoystickEx::create("client/ui/wooden_handle_n.png", "client/ui/wooden_handle_n.png");
    _moveJoystick->setJoystickPad("client/ui/wooden_pad.png");
    _moveJoystick->setRotationType(JoystickEx::RotationType::ROTATION_8);
    _moveJoystick->setPosition(Vec2(joystickPos.x, joystickPos.y));
    _moveJoystick->setScale(joystickScale.x, joystickScale.y);
    _moveJoystick->enableDoubleClick(false);
    _moveJoystick->setChangedDirectionAndStatusCallback([&](Ref* ref, const Vec2& dir, JoystickEx::ClickEventType type) {
        
        MoveJoystickData data;
        data.ref = ref;
        data.dir = dir;
        data.type = type;
        _game->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data);
        
        // 이동방향과 베젤방향을 동기화 시킨다.
        if ( _game->getPlayerPtr()->getInputMask() == HumanBehaviorType::MOVE ) _bezel->setBezelDirection(data.dir);
        
    });
    addChild(_moveJoystick, Z_ORDER_UI - 1);
    
    _attackJoystick = JoystickEx::create("client/ui/wooden_attack_handle_n.png", "client/ui/wooden_attack_handle_n.png");
    _attackJoystick->enableDoubleClick(false);
    _attackJoystick->setPosition(Vec2(_winSize.width - joystickPos.x, joystickPos.y));
    _attackJoystick->setScale(joystickScale.x, joystickScale.y);
    _attackJoystick->setClickCallback([&](Ref* ref, cocos2d::ui::Widget::TouchEventType type) {
        
        AttJoystickData data;
        data.ref = ref;
        data.type = type;
        _game->pushLogic(0.0, MessageType::ATTACK_JOYSTICK_INPUT, &data);
        
    });
    addChild(_attackJoystick, Z_ORDER_UI - 1);
    
    _bezel = CircularBezel::create("client/ui/wooden_attack_pad.png");
    _bezel->setPosition(Vec2(_winSize.width - joystickPos.x, joystickPos.y));
    _bezel->setTriggerRadius( {40.0f, 250.0f} );
    _bezel->setScale(joystickScale.x, joystickScale.y);
    _bezel->enableBezelMode(false);
    _bezel->addTriggerCallback([this](Ref* ref, const Vec2& dir, cocos2d::ui::Widget::TouchEventType type) {
        
        BezelDirectionTriggerData data;
        data.ref = ref;
        data.dir = dir;
        data.type = type;
        _game->pushLogic(0.0, MessageType::BEZEL_DIRECTION_TRIGGERED, &data);
        
    });
    addChild(_bezel, Z_ORDER_UI - 1);
    
#elif ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    
    auto mouse = EventListenerMouse::create();
    mouse->onMouseDown = [this](Event* event){
        
        EventMouse* e = static_cast<EventMouse*>(event);
        Vec2 screenPos = Vec2(e->getCursorX(), e->getCursorY());
        Vec2 localPos = getParent()->convertToNodeSpace(screenPos);
        
        if ( localPos.x > 0.0f && localPos.x < GAME_SCREEN_WIDTH && localPos.y > 0.0f && localPos.y < GAME_SCREEN_HEIGHT )
        {
                AttJoystickData data;
                data.ref = this;
                data.type = ui::Widget::TouchEventType::BEGAN;
                _game->pushLogic(0.0, MessageType::ATTACK_JOYSTICK_INPUT, &data);
        }
        
    };
    
    mouse->onMouseUp = [this](Event* event){
        
        AttJoystickData data;
        data.ref = this;
        data.type = ui::Widget::TouchEventType::ENDED;
        _game->pushLogic(0.0, MessageType::ATTACK_JOYSTICK_INPUT, &data);
        
    };
    
    mouse->onMouseMove = [this](Event* event) {
        
        EventMouse* e = static_cast<EventMouse*>(event);
        Vec2 screenPos = Vec2(e->getCursorX(), e->getCursorY());
        Vec2 localPos = getParent()->convertToNodeSpace(screenPos);
        
        if ( localPos.x > 0.0f && localPos.x < GAME_SCREEN_WIDTH && localPos.y > 0.0f && localPos.y < GAME_SCREEN_HEIGHT )
        {
            _mouseDirection = Vec2(localPos.x - GAME_SCREEN_WIDTH / 2, localPos.y - GAME_SCREEN_HEIGHT / 2).getNormalized();
            _isInputMaskDirty = true;
        }
        
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, this);
    
    auto keyboard = EventListenerKeyboard::create();
    keyboard->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        
        auto oldMask = _inputMask;
        
        if ( keyCode == EventKeyboard::KeyCode::KEY_W ) _inputMask.set(InputMask::UP);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_S ) _inputMask.set(InputMask::DOWN);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_A ) _inputMask.set(InputMask::LEFT);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_D ) _inputMask.set(InputMask::RIGHT);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_SHIFT ) _inputMask.set(InputMask::RUNNING);
		else if (keyCode == EventKeyboard::KeyCode::KEY_R)
		{
			_game->pushLogic(0.0, MessageType::PRESS_RELOAD_BUTTON, nullptr);
		}
        if ( oldMask != _inputMask ) _isInputMaskDirty = true;
        
    };
    
    keyboard->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        
        auto oldMask = _inputMask;
        
        if ( keyCode == EventKeyboard::KeyCode::KEY_W ) _inputMask.reset(InputMask::UP);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_S ) _inputMask.reset(InputMask::DOWN);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_A ) _inputMask.reset(InputMask::LEFT);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_D ) _inputMask.reset(InputMask::RIGHT);
        else if ( keyCode == EventKeyboard::KeyCode::KEY_SHIFT ) _inputMask.reset(InputMask::RUNNING);
        
        if ( oldMask != _inputMask ) _isInputMaskDirty = true;
        
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);
    
#endif    
    
    _inGameUiLayer = Node::create();
    _inGameUiLayer->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _inGameUiLayer->setScale(_game->getRenderingSysetm()->getZoomScale().x, _game->getRenderingSysetm()->getZoomScale().y);
    addChild(_inGameUiLayer);
    
    _crossHair = CrossHair::create("client/ui/aim_point.png");
    _crossHair->setScale(0.8f);
    _crossHair->setRange(90.0f);
    
    _crossHair->setEffect("hit", CrossHairEffect({
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair1.png",
        "client/ui/cross_hair2.png",
        "client/ui/cross_hair3.png",
        "client/ui/cross_hair4.png",
        "client/ui/cross_hair5.png",
        "client/ui/cross_hair6.png",
        "client/ui/cross_hair7.png" }, 0.035f));
    
    _crossHair->setEffect("fire", CrossHairEffect({
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png",
        "client/ui/cross_hair0.png" }, 0.035f));
    
    _inGameUiLayer->addChild(_crossHair);
    
    
    _infoSystem = InfoSystem::create("fonts/SpecialElite.TTF");
    _infoSystem->setPosition(Vec2(0, -100.0f));
    _inGameUiLayer->addChild(_infoSystem);
    
    //
    // Rain Effect
    //
    for(int i = 0 ;i < 3 ; ++ i)
    {
        auto rain = ParticleRain::create();
        rain->setRotation(20);
        rain->setScaleY(random(3,6));
        rain->setSpeed(300 + random(0,30));
        addChild(rain, Z_ORDER_UI - 2);
    }
    
    _hpBar = HpBar::create(_game);
    _hpBar->setPosition(Vec2(_winSize.width * 0.03f, _winSize.height * 0.9f));
    addChild(_hpBar);
    
    _inventoryView = InventoryView::create();
    _inventoryView->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
    addChild(_inventoryView);

    
    return true;
}


void UiLayer::update(float dt)
{
    _crossHair->setRotation(-physics::getAngleFromZero(_game->getPlayerPtr()->getHeading()));
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    if ( _isInputMaskDirty )
    {
        Vec2 moveDirection(Vec2::ZERO);
        if ( _inputMask.test(InputMask::UP) ) moveDirection += Vec2::UNIT_Y;
        if ( _inputMask.test(InputMask::DOWN) ) moveDirection -= Vec2::UNIT_Y;
        if ( _inputMask.test(InputMask::LEFT) ) moveDirection -= Vec2::UNIT_X;
        if ( _inputMask.test(InputMask::RIGHT) ) moveDirection += Vec2::UNIT_X;
        
        if ( _inputMask.test(InputMask::RUNNING) )
        {
            BezelDirectionTriggerData data;
            data.ref = this;
            data.dir = Vec2::ZERO;
            data.type = ui::Widget::TouchEventType::ENDED;
            _game->pushLogic(0.0, MessageType::BEZEL_DIRECTION_TRIGGERED, &data);
        }
        else
        {
            BezelDirectionTriggerData data;
            data.ref = this;
            data.dir = _mouseDirection;
            data.type = ui::Widget::TouchEventType::MOVED;
            _game->pushLogic(0.0, MessageType::BEZEL_DIRECTION_TRIGGERED, &data);
        }
        
        if ( !moveDirection.isZero() )
        {
            MoveJoystickData data;
            data.ref = this;
            data.dir = moveDirection.getNormalized();
            data.type = JoystickEx::ClickEventType::BEGAN;
            _game->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data);
        }
        else
        {
            MoveJoystickData data;
            data.ref = this;
            data.dir = Vec2::ZERO;
            data.type = JoystickEx::ClickEventType::ENDED;
            _game->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data);
        }
        
        _isInputMaskDirty = false;
    }
#endif
}


void UiLayer::runCrossHairEffect(const std::string& name)
{
    _crossHair->runEffect(name);
}


void UiLayer::displayText(const std::string& text)
{
    _infoSystem->pushMessage(text);
}









