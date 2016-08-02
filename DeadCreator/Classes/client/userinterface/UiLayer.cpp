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
#include "Inventory.hpp"
#include "Items.hpp"
#include "Camera2D.hpp"
#include "WeaponStatus.hpp"
#include "CircularBezel.hpp"
#include "HpBar.hpp"
#include "InfoSystem.hpp"
#include "SizeProtocol.h"
#include "ParamLoader.hpp"
using namespace realtrick::client;
using namespace cocos2d;


UiLayer::UiLayer(Game* game) : _game(game)
{
    
}


UiLayer::~UiLayer()
{
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
    
    Vec2 joystickScale(GAME_SCREEN_WIDTH / 1136, GAME_SCREEN_HEIGHT / 640);
    Vec2 joystickPos = Vec2(150.0f * joystickScale.x, 150.0f * joystickScale.y);
    
    _moveJoystick = JoystickEx::create("wooden_handle_n.png", "wooden_handle_n.png");
    _moveJoystick->setJoystickPad("wooden_pad.png");
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
        if ( _game->getPlayerPtr()->getInputMask() == HumanBehaviorType::RUN ) _bezel->setBezelDirection(data.dir);
        
    });
    addChild(_moveJoystick, Z_ORDER_UI - 1);
    
    _attackJoystick = JoystickEx::create("wooden_attack_handle_n.png", "wooden_attack_handle_n.png");
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
    
    
    _bezel = CircularBezel::create("wooden_attack_pad.png");
    _bezel->setPosition(Vec2(_winSize.width - joystickPos.x, joystickPos.y));
    _bezel->setTriggerRadius( {40.0f, 250.0f} );
    _bezel->setScale(joystickScale.x, joystickScale.y);
    _bezel->enableBezelMode(false);
    _bezel->enableDebugNode(false);
    _bezel->addTriggerCallback([this](Ref* ref, const Vec2& dir) {
        
        BezelDirectionTriggerData data;
        data.ref = ref;
        data.dir = dir;
        _game->pushLogic(0.0, MessageType::BEZEL_DIRECTION_TRIGGERED, &data);
        
    });
    _bezel->addClickedCallback([this](Ref* ref, ui::Widget::TouchEventType type){
        
        BezelInputData data;
        data.ref = ref;
        data.type = type;
        _game->pushLogic(0.0, MessageType::BEZEL_CLICK_INPUT, &data);
        
    });
    addChild(_bezel, Z_ORDER_UI - 1);
    
    auto player = _game->getPlayerPtr();
    if ( player )
    {
        _weaponStatus = player->getWeaponStatus();
        _weaponStatus->setPosition(Vec2(_winSize.width * 0.9f, _winSize.height * 0.9f));
        addChild(_weaponStatus, Z_ORDER_UI - 1);
        
        
        _inventory = player->getInventory();
        _inventory->setPosition(Vec2(_winSize / 2));
        addChild(_inventory, Z_ORDER_UI);
    }
    
    _inventorySwitch = ui::CheckBox::create("inventory_n.png", "inventory_s.png");
    _inventorySwitch->setPosition(Vec2(_winSize.width / 2.0f, 50.0f));
    _inventorySwitch->setScale(1.3f);
    _inventorySwitch->addEventListener([this](Ref* ref, ui::CheckBox::EventType type)
                                       {
                                           if ( type == ui::CheckBox::EventType::SELECTED )
                                           {
                                               // 인벤토리를 열면 모든 조이스틱을 비활성화 시킨다.
                                               _inventory->open();
                                               _attackJoystick->disableJoystick();
                                               _moveJoystick->disableJoystick();
                                               _weaponStatus->disableButton();
                                               
                                               EntityPlayer* player = _game->getPlayerPtr();
                                               
                                               MoveJoystickData data1;
                                               data1.ref = ref;
                                               data1.dir = player->getMoving();
                                               data1.type = JoystickEx::ClickEventType::ENDED;
                                               _game->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data1);
                                               
                                               AttJoystickData data2;
                                               data2.ref = ref;
                                               data2.type = ui::Widget::TouchEventType::ENDED;
                                               _game->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data2);
                                           }
                                           else
                                           {
                                               _inventory->close();
                                               _attackJoystick->enableJoystick();
                                               _moveJoystick->enableJoystick();
                                               _weaponStatus->enableButton();
                                           }
                                       });
    addChild(_inventorySwitch, Z_ORDER_UI);
    
    
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
    
    return true;
}









