//
//  UiLayer.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 20..
//
//

#include "UiLayer.hpp"
#include "JoystickEx.hpp"
#include "GameManager.hpp"
#include "EntityHuman.hpp"
#include "MessageTypes.hpp"
#include "Inventory.hpp"
#include "Items.hpp"
#include "EntityRainDrop.hpp"
#include "Camera2D.hpp"
#include "WeaponStatus.hpp"
#include "CircularBezel.hpp"
#include "AimingNode.hpp"
#include "HpBar.hpp"
#include "InfoSystem.hpp"

using namespace realtrick;
using namespace cocos2d;


UiLayer::UiLayer(GameManager* mgr) : _gameMgr(mgr)
{
    
}


UiLayer::~UiLayer()
{
}


UiLayer* UiLayer::create(GameManager* mgr)
{
    auto ret = new (std::nothrow) UiLayer(mgr);
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
    
    _winSize = Director::getInstance()->getVisibleSize();
    
    _moveJoystick = JoystickEx::create("wooden_handle_n.png", "wooden_handle_n.png");
    _moveJoystick->setJoystickPad("wooden_pad.png");
    _moveJoystick->setRotationType(realtrick::JoystickEx::RotationType::ROTATION_8);
    _moveJoystick->setPosition(Vec2(150.0f, 150.0f));
    _moveJoystick->enableDoubleClick(false);
    _moveJoystick->setChangedDirectionAndStatusCallback([&](Ref* ref, const Vec2& dir, JoystickEx::ClickEventType type) {
        
        MoveJoystickData data;
        data.ref = ref;
        data.dir = dir;
        data.type = type;
        _gameMgr->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data);
        
        // 이동방향과 베젤방향을 동기화 시킨다.
        if ( _gameMgr->getPlayerPtr()->getInputMask() == HumanBehaviorType::RUN ) _bezel->setBezelDirection(data.dir);
        
    });
    addChild(_moveJoystick, Z_ORDER_UI - 1);
    
    _attackJoystick = JoystickEx::create("wooden_attack_handle_n.png", "wooden_attack_handle_n.png");
    _attackJoystick->enableDoubleClick(false);
    _attackJoystick->setPosition(Vec2(_winSize.width - 200.0f, 150.0f));
    _attackJoystick->setClickCallback([&](Ref* ref, cocos2d::ui::Widget::TouchEventType type) {
        
        AttJoystickData data;
        data.ref = ref;
        data.type = type;
        _gameMgr->pushLogic(0.0, MessageType::ATTACK_JOYSTICK_INPUT, &data);
        
    });
    addChild(_attackJoystick, Z_ORDER_UI - 1);
    
    
    _bezel = CircularBezel::create("wooden_attack_pad.png");
    _bezel->setPosition(Vec2(_winSize.width - 200.0f, 150.0f));
    _bezel->setTriggerRadius( {40.0f, 250.0f} );
    _bezel->enableBezelMode(false);
    _bezel->enableDebugNode(false);
    _bezel->addTriggerCallback([this](Ref* ref, const Vec2& dir) {
       
        BezelDirectionTriggerData data;
        data.ref = ref;
        data.dir = dir;
        _gameMgr->pushLogic(0.0, MessageType::BEZEL_DIRECTION_TRIGGERED, &data);
        
    });
    _bezel->addClickedCallback([this](Ref* ref, ui::Widget::TouchEventType type){
        
        BezelInputData data;
        data.ref = ref;
        data.type = type;
        _gameMgr->pushLogic(0.0, MessageType::BEZEL_CLICK_INPUT, &data);
        
    });
    addChild(_bezel, Z_ORDER_UI - 1);
    
    _weaponStatus = _gameMgr->getPlayerPtr()->getWeaponStatus();
    _weaponStatus->setPosition(Vec2(_winSize.width * 0.9f, _winSize.height * 0.9f));
    addChild(_weaponStatus, Z_ORDER_UI - 1);
    
    
    _inventory = _gameMgr->getPlayerPtr()->getInventory();
    _inventory->setPosition(Vec2(_winSize / 2));
    addChild(_inventory, Z_ORDER_UI);
    
    
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
                                               
                                               EntityHuman* player = _gameMgr->getPlayerPtr();
                                               
                                               MoveJoystickData data1;
                                               data1.ref = ref;
                                               data1.dir = player->getMoving();
                                               data1.type = JoystickEx::ClickEventType::ENDED;
                                               _gameMgr->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data1);
                                               
                                               AttJoystickData data2;
                                               data2.ref = ref;
                                               data2.type = ui::Widget::TouchEventType::ENDED;
                                               _gameMgr->pushLogic(0.0, MessageType::MOVE_JOYSTICK_INPUT, &data2);
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
    
    for(int i = 0 ; i < 300; ++ i)
    {
        EntityRainDrop* rain = EntityRainDrop::create(_gameMgr, { "rain0.png", "rain1.png", "rain2.png" }, 1.0f, ui::Widget::TextureResType::PLIST);
        rain->setWorldPosition(_gameMgr->getGameCamera()->getCameraPos() + Vec2(random(-_winSize.width * 0.5f, _winSize.width * 0.5f), random(-_winSize.height * 0.5f, _winSize.height * 0.5f)));
        _gameMgr->addDynamicEntity(rain, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
    }
    
    
    _inGameUIRoot = Node::create();
    _inGameUIRoot->setPosition(_winSize / 2);
    _inGameUIRoot->setScale(Prm.getValueAsFloat("cameraZoom"));
    addChild(_inGameUIRoot);
    
    
    _aimingNode = AimingNode::create(_gameMgr);
    _aimingNode->setRange(100.0f);
    _inGameUIRoot->addChild(_aimingNode);
    
    
    _locationText = ui::Text::create("", "SpecialElite.TTF", 40);
    _locationText->setPosition(Vec2(_winSize.width * 0.92, _winSize.height * 0.9));
    _locationText->setOpacity(80);
    addChild(_locationText);

    
    _hpBar = userinterface::HpBar::create(_gameMgr);
    _hpBar->setPosition(Vec2(_winSize.width * 0.03f, _winSize.height * 0.9f));
    addChild(_hpBar);
    
    
    _infoSystemBottom = userinterface::InfoSystem::create("SpecialElite.TTF", 20.0f , Color3B::WHITE, 2);
    _infoSystemBottom->setPosition(Vec2(_winSize.width / 2.0f, _winSize.height * 0.15f));
    addChild(_infoSystemBottom);
    
    
    _infoSystemTop = userinterface::InfoSystem::create("SpecialElite.TTF", 25.0f, Color3B::RED, 1);
    _infoSystemTop->setPosition(Vec2(_winSize.width / 2.0f, _winSize.height * 0.8f));
    addChild(_infoSystemTop);
    
    return true;
}


void UiLayer::pushMessageToBottom(const std::string& infoText)
{
    _infoSystemBottom->pushMessage(infoText);
}


void UiLayer::pushMessageToTop(const std::string& infoText)
{
    _infoSystemTop->pushMessage(infoText);
}









