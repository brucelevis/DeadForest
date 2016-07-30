//
//  MainMenu3.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 1..
//
//

#include "MainMenu3.hpp"
#include "GameWorld.hpp"
#include "GeneratedPackets.hpp"
#include "Server.hpp"
#include "SizeProtocol.h"
using namespace realtrick::client;
using namespace cocos2d;
using namespace cocos2d::ui;


MainMenu3::MainMenu3() :
_winSize(Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT)),
_roomID(0),
_isButtonMoving_multi(false),
_menuCenterIndex_multi(0),
_isButtonMoving_custom(false),
_menuCenterIndex_custom(0)
{
}


MainMenu3::~MainMenu3()
{}


MainMenu3* MainMenu3::create()
{
    auto ret = new (std::nothrow) MainMenu3();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


Scene* MainMenu3::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenu3::create();
    scene->addChild(layer);
    return scene;
}


void MainMenu3::update(float dt)
{
    if ( !Server::getInstance().isQueueEmpty() )
    {
        Packet* packet;
        Server::getInstance().dequeue(packet);
        packet->decode();
        
        switch ( packet->type() )
        {
            case PacketType::LOGIN_SUCCESS:
            {
                log("<MainMenu3::update> login success.");
                auto obj = fpacket::GetPacketLoginSuccess(packet->body());
                _userName->setString(obj->id()->c_str());
                
                break;
            }
            case PacketType::SUCCESS_SEARCH_GAME:
            {
                auto obj = fpacket::GetPacketSuccessSearchGame(packet->body());
                _roomID = obj->roomId();
                log("<MainMenu3::update> success serch game. room id: [%d]", _roomID);
                
                Director::getInstance()->replaceScene(GameWorld::createScene());
                
                break;
            }
            case PacketType::FAIL_SEARCH_GAME:
            {
                log("<MainMenu3::update> fail serch game.");
                
                break;
            }
            default:
            {
                log("<MainMenu3::update> Invalid Packet Arrived. (%d)", static_cast<int>(packet->type()));
                
                break;
            }
        }
        
        CC_SAFE_DELETE(packet);
    }
}


bool MainMenu3::init()
{
    if ( !Layer::init() )
        return false;
    
    _gameTitle = Sprite::create("game_title.png");
    _gameTitle->setPosition(Vec2(_winSize.width / 2.0f, _winSize.height * 0.8f));
    _gameTitle->setScale(2.0f);
    _gameTitle->runAction(Sequence::create(FadeTo::create(0.0f, 0), FadeTo::create(0.5f, 255), nullptr));
    addChild(_gameTitle);
    
    _userName = Label::createWithTTF(UserDefault::getInstance()->getStringForKey("id"), "SpecialElite.TTF", 32);
    _userName->runAction(Sequence::create(FadeTo::create(0.0f, 0), FadeTo::create(0.5f, 255), nullptr));
    
    MenuItemLabel* labelMenu = MenuItemLabel::create(_userName);
    labelMenu->setCallback([this](Ref* ref){
        
        // user name touched.
        flatbuffers::FlatBufferBuilder fbb;
        auto obj = fpacket::CreatePacketSignup(fbb,
                                               UserDefault::getInstance()->getIntegerForKey("pid"),
                                               fbb.CreateString("skawnsgus123"));
        fbb.Finish(obj);
        
        Packet* packet = new Packet();
        packet->encode(fbb.GetBufferPointer(), fbb.GetSize(), PacketType::SIGN_UP);
        Server::getInstance().write(packet);
        
    });
    labelMenu->setAnchorPoint(Vec2(0.0f, 0.5f));
    labelMenu->setPosition(Vec2(_winSize.width * 0.07f, _winSize.height * 0.1f) - Vec2(_winSize / 2.0f));
    
    _menuHolder = Menu::create(labelMenu, nullptr);
    addChild(_menuHolder);
    
    _connectState = DrawNode::create();
    _connectState->drawDot(Vec2(_winSize.width * 0.05f, _winSize.height * 0.1f), 7.0f, Color4F::GREEN);
    addChild(_connectState);
    
    _showMainMenu(0.25f);
    
    this->scheduleUpdate();
    
    return true;
}


void MainMenu3::_showMainMenu(float delay)
{
    _menuHolder->setEnabled(true);
    
    _playButton = ui::Button::create("play_n.png", "play_s.png");
    _playButton->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.5f));
    _playButton->setOpacity(0);
    _playButton->runAction(FadeTo::create(delay, 255));
    _playButton->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hideMainMenuAndShowPlayMenu(delay);
        }
    });
    addChild(_playButton);
    
    _optionButton = ui::Button::create("option_n.png", "option_s.png");
    _optionButton->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.4f));
    _optionButton->setOpacity(0);
    _optionButton->runAction(FadeTo::create(delay, 255));
    _optionButton->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            //_hideMainMenu(delay);
        }
        
    });
    addChild(_optionButton);
    
    _exitButton = ui::Button::create("exit_n.png", "exit_s.png");
    _exitButton->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.3f));
    _exitButton->setOpacity(0);
    _exitButton->runAction(FadeTo::create(delay, 255));
    _exitButton->addTouchEventListener([this](Ref* ref, Widget::TouchEventType type) {
        if ( type == Widget::TouchEventType::ENDED )
        {
//            Director::getInstance()->end();
//            exit(-1);
        }
    });
    addChild(_exitButton);
}


void MainMenu3::_showPlayMenu(float delay)
{
    _singleButton = ui::Button::create("single_n.png", "single_s.png");
    _singleButton->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.55f));
    _singleButton->setOpacity(0);
    _singleButton->runAction(FadeTo::create(delay, 255));
    _singleButton->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hidePlayMenuAndShowSingleMenu(delay);
        }
        
    });
    addChild(_singleButton);
    
    _multiButton = ui::Button::create("multi_n.png", "multi_s.png");
    _multiButton->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.45f));
    _multiButton->setOpacity(0);
    _multiButton->runAction(FadeTo::create(delay, 255));
    _multiButton->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hidePlayMenuAndShowMultiMenu(delay);
        }
        
    });
    addChild(_multiButton);
    
    _customButton = ui::Button::create("custom_n.png", "custom_s.png");
    _customButton->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.35f));
    _customButton->setOpacity(0);
    _customButton->runAction(FadeTo::create(delay, 255));
    _customButton->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hidePlayMenuAndShowCustomMenu(delay);
        }
        
    });
    addChild(_customButton);
    
    _backButton_play = ui::Button::create("back_n.png", "back_s.png");
    _backButton_play->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.25f));
    _backButton_play->setOpacity(0);
    _backButton_play->runAction(FadeTo::create(delay, 255));
    _backButton_play->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hidePlayMenuAndShowMainMenu(delay);
        }
        
    });
    addChild(_backButton_play);
}



void MainMenu3::_showSingleMenu(float delay)
{
    auto scene = GameWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}


void MainMenu3::_showMultiMenu(float delay)
{
    _menuCenterIndex_multi = 3;
    _isButtonMoving_multi = false;
    
    _clipRectNode_multi = ClippingRectangleNode::create(Rect(_winSize.width * 0.5f - 400.0f,
                                                             _winSize.height * 0.5f - 125.0f,
                                                             800, 250));
    addChild(_clipRectNode_multi);
    
    _chapter1 = Sprite::create("menu_sample1.png");
    _chapter1->setPosition(MENU_POSITION_1);
    _chapter1->setScale(0.8f);
    _chapter1->setOpacity(0);
    _chapter1->setAnchorPoint(Vec2(0.5f, 0.0f));
    _chapter1->runAction(FadeTo::create(delay, 80));
    _clipRectNode_multi->addChild(_chapter1);
    
    _chapter2 = Sprite::create("menu_sample2.png");
    _chapter2->setPosition(MENU_POSITION_2);
    _chapter2->setScale(0.8f);
    _chapter2->setOpacity(0);
    _chapter2->runAction(FadeTo::create(delay, 80));
    _chapter2->setAnchorPoint(Vec2(0.5f, 0.0f));
    _clipRectNode_multi->addChild(_chapter2);
    
    _chapter3 = Sprite::create("menu_sample3.png");
    _chapter3->setPosition(MENU_POSITION_3);
    _chapter3->setScale(0.9f);
    _chapter3->setOpacity(0);
    _chapter3->runAction(FadeTo::create(delay, 255));
    _chapter3->setAnchorPoint(Vec2(0.5f, 0.0f));
    _clipRectNode_multi->addChild(_chapter3);
    
    _chapter4 = Sprite::create("menu_sample4.png");
    _chapter4->setPosition(MENU_POSITION_4);
    _chapter4->setScale(0.8f);
    _chapter4->setOpacity(0);
    _chapter4->runAction(FadeTo::create(delay, 80));
    _chapter4->setAnchorPoint(Vec2(0.5f, 0.0f));
    _clipRectNode_multi->addChild(_chapter4);
    
    _chapter5 = Sprite::create("menu_sample1.png");
    _chapter5->setPosition(MENU_POSITION_5);
    _chapter5->setScale(0.8f);
    _chapter5->setOpacity(0);
    _chapter5->runAction(FadeTo::create(delay, 80));
    _chapter5->setAnchorPoint(Vec2(0.5f, 0.0f));
    _clipRectNode_multi->addChild(_chapter5);
    
    _leftButton_multi = ui::Button::create("arrow_n.png", "arrow_s.png");
    _leftButton_multi->setRotation(270.0f);
    _leftButton_multi->setOpacity(0);
    _leftButton_multi->runAction(FadeTo::create(delay, 255));
    _leftButton_multi->setPosition(Vec2(_winSize.width * 0.5f - 475.0f, _winSize.height * 0.48f));
    _leftButton_multi->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( !_isButtonMoving_multi && type == Widget::TouchEventType::ENDED )
        {
            _leftButton_multi->runAction(Sequence::create(CallFunc::create([&]{ _isButtonMoving_multi = true; }),
                                                          DelayTime::create(delay),
                                                          CallFunc::create([&]{ _isButtonMoving_multi = false; }),
                                                          nullptr));
            switch ( _menuCenterIndex_multi )
            {
                case 1:
                {
                    _chapter1->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_4),
                                                       nullptr));
                    
                    _chapter2->runAction(MoveTo::create(delay, MENU_POSITION_5));
                    
                    _chapter3->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_RIGHT_HIDE),
                                                          Place::create(MENU_POSITION_LEFT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_1),
                                                          nullptr));
                    
                    _chapter4->runAction(MoveTo::create(delay, MENU_POSITION_2));
                    
                    _chapter5->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _menuCenterIndex_multi = 5;
                    
                    break;
                }
                case 2:
                {
                    _chapter2->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_4),
                                                       nullptr));
                    
                    _chapter3->runAction(MoveTo::create(delay, MENU_POSITION_5));
                    
                    _chapter4->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_RIGHT_HIDE),
                                                          Place::create(MENU_POSITION_LEFT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_1),
                                                          nullptr));
                    
                    _chapter5->runAction(MoveTo::create(delay, MENU_POSITION_2));
                    
                    _chapter1->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _menuCenterIndex_multi = 1;
                    break;
                }
                case 3:
                {
                    _chapter3->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_4),
                                                       nullptr));
                    
                    _chapter4->runAction(MoveTo::create(delay, MENU_POSITION_5));
                    
                    _chapter5->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_RIGHT_HIDE),
                                                          Place::create(MENU_POSITION_LEFT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_1),
                                                          nullptr));
                    
                    _chapter1->runAction(MoveTo::create(delay, MENU_POSITION_2));
                    
                    _chapter2->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _menuCenterIndex_multi = 2;
                    
                    break;
                }
                case 4:
                {
                    _chapter4->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_4),
                                                       nullptr));
                    
                    _chapter5->runAction(MoveTo::create(delay, MENU_POSITION_5));
                    
                    _chapter1->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_RIGHT_HIDE),
                                                          Place::create(MENU_POSITION_LEFT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_1),
                                                          nullptr));
                    
                    _chapter2->runAction(MoveTo::create(delay, MENU_POSITION_2));
                    
                    _chapter3->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _menuCenterIndex_multi = 3;
                    
                    break;
                }
                case 5:
                {
                    _chapter5->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_4),
                                                       nullptr));
                    
                    _chapter1->runAction(MoveTo::create(delay, MENU_POSITION_5));
                    
                    _chapter2->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_RIGHT_HIDE),
                                                          Place::create(MENU_POSITION_LEFT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_1),
                                                          nullptr));
                    
                    _chapter3->runAction(MoveTo::create(delay, MENU_POSITION_2));
                    
                    _chapter4->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _menuCenterIndex_multi = 4;
                    
                    break;
                }
            }
        }
    });
    addChild(_leftButton_multi);
    
    _rightButton_multi = ui::Button::create("arrow_n.png", "arrow_s.png");
    _rightButton_multi->setRotation(90.0f);
    _rightButton_multi->setPosition(Vec2(_winSize.width * 0.5f + 475.0f, _winSize.height * 0.48f));
    _rightButton_multi->setOpacity(0);
    _rightButton_multi->runAction(FadeTo::create(delay, 255));
    _rightButton_multi->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( !_isButtonMoving_multi && type == Widget::TouchEventType::ENDED )
        {
            _rightButton_multi->runAction(Sequence::create(CallFunc::create([&]{ _isButtonMoving_multi = true; }),
                                                           DelayTime::create(delay),
                                                           CallFunc::create([&]{ _isButtonMoving_multi = false; }),
                                                           nullptr));
            switch ( _menuCenterIndex_multi )
            {
                case 1:
                {
                    _chapter1->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_2),
                                                       nullptr));
                    
                    _chapter2->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _chapter3->runAction(MoveTo::create(delay, MENU_POSITION_4));
                    
                    _chapter4->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_LEFT_HIDE),
                                                          Place::create(MENU_POSITION_RIGHT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_5),
                                                          nullptr));
                    
                    
                    _chapter5->runAction(MoveTo::create(delay, MENU_POSITION_1));
                    
                    _menuCenterIndex_multi = 2;
                    
                    break;
                }
                case 2:
                {
                    _chapter2->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_2),
                                                       nullptr));
                    
                    _chapter3->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _chapter4->runAction(MoveTo::create(delay, MENU_POSITION_4));
                    
                    _chapter5->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_LEFT_HIDE),
                                                          Place::create(MENU_POSITION_RIGHT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_5),
                                                          nullptr));
                    
                    _chapter1->runAction(MoveTo::create(delay, MENU_POSITION_1));
                    
                    _menuCenterIndex_multi = 3;
                    
                    break;
                }
                case 3:
                {
                    _chapter3->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_2),
                                                       nullptr));
                    
                    _chapter4->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _chapter5->runAction(MoveTo::create(delay, MENU_POSITION_4));
                    
                    _chapter1->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_LEFT_HIDE),
                                                          Place::create(MENU_POSITION_RIGHT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_5),
                                                          nullptr));
                    
                    _chapter2->runAction(MoveTo::create(delay, MENU_POSITION_1));
                    
                    _menuCenterIndex_multi = 4;
                    
                    break;
                }
                case 4:
                {
                    _chapter4->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_2),
                                                       nullptr));
                    
                    _chapter5->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _chapter1->runAction(MoveTo::create(delay, MENU_POSITION_4));
                    
                    _chapter2->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_LEFT_HIDE),
                                                          Place::create(MENU_POSITION_RIGHT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_5),
                                                          nullptr));
                    
                    _chapter3->runAction(MoveTo::create(delay, MENU_POSITION_1));
                    
                    _menuCenterIndex_multi = 5;
                    
                    break;
                }
                case 5:
                {
                    _chapter5->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                       ScaleTo::create(delay, 0.8f),
                                                       MoveTo::create(delay, MENU_POSITION_2),
                                                       nullptr));
                    
                    _chapter1->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                       ScaleTo::create(delay, 0.9f),
                                                       MoveTo::create(delay, MENU_POSITION_3),
                                                       nullptr));
                    
                    _chapter2->runAction(MoveTo::create(delay, MENU_POSITION_4));
                    
                    _chapter3->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_LEFT_HIDE),
                                                          Place::create(MENU_POSITION_RIGHT_HIDE),
                                                          MoveTo::create(delay / 2,  MENU_POSITION_5),
                                                          nullptr));
                    
                    _chapter4->runAction(MoveTo::create(delay, MENU_POSITION_1));
                    
                    _menuCenterIndex_multi = 1;
                    
                    break;
                }
            }
        }
    });
    addChild(_rightButton_multi);
    
    _infoText_multi = Text::create("introduce about this mode.", "SpecialElite.TTF", 30);
    _infoText_multi->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.25f));
    _infoText_multi->setOpacity(0);
    _infoText_multi->runAction(FadeTo::create(delay, 255));
    addChild(_infoText_multi);
    
    _searchButton_multi = ui::Button::create("search_n.png", "search_s.png");
    _searchButton_multi->setPosition(Vec2(_winSize.width * 0.4f, _winSize.height * 0.15f));
    _searchButton_multi->setOpacity(0);
    _searchButton_multi->runAction(FadeTo::create(delay, 255));
    _searchButton_multi->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            // 방에 들어간다
            
            
            
            _hideMultiMenuAndShowMultiSearchMenu(delay);
            
        }
        
    });
    addChild(_searchButton_multi);
    
    _backButton_multi = ui::Button::create("back_n.png", "back_s.png");
    _backButton_multi->setPosition(Vec2(_winSize.width * 0.6f, _winSize.height * 0.15f));
    _backButton_multi->setOpacity(0);
    _backButton_multi->runAction(FadeTo::create(delay, 255));
    _backButton_multi->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hideMultiMenuAndShowPlayMenu(delay);
        }
        
    });
    addChild(_backButton_multi);
}


void MainMenu3::_showCustomMenu(float delay)
{
    _menuCenterIndex_custom = 3;
    _isButtonMoving_custom = false;
    
    _clipRectNode_custom = ClippingRectangleNode::create(Rect(_winSize.width * 0.5f - 400.0f,
                                                              _winSize.height * 0.5f - 125.0f,
                                                              800, 250));
    addChild(_clipRectNode_custom);
    
    _survivalMode = Sprite::create("menu_sample1.png");
    _survivalMode->setPosition(MENU_POSITION_2);
    _survivalMode->setScale(0.8f);
    _survivalMode->setOpacity(0);
    _survivalMode->runAction(FadeTo::create(delay, 80));
    _survivalMode->setAnchorPoint(Vec2(0.5f, 0.0f));
    _clipRectNode_custom->addChild(_survivalMode);
    
    _battleRoyalMode = Sprite::create("menu_sample2.png");
    _battleRoyalMode->setPosition(MENU_POSITION_3);
    _battleRoyalMode->setScale(0.9f);
    _battleRoyalMode->setOpacity(0);
    _battleRoyalMode->setAnchorPoint(Vec2(0.5f, 0.0f));
    _battleRoyalMode->runAction(FadeTo::create(delay, 255));
    _clipRectNode_custom->addChild(_battleRoyalMode);
    
    _teamSurvivalMode = Sprite::create("menu_sample3.png");
    _teamSurvivalMode->setPosition(MENU_POSITION_4);
    _teamSurvivalMode->setScale(0.8f);
    _teamSurvivalMode->setOpacity(0);
    _teamSurvivalMode->runAction(FadeTo::create(delay, 80));
    _teamSurvivalMode->setAnchorPoint(Vec2(0.5f, 0.0f));
    _clipRectNode_custom->addChild(_teamSurvivalMode);
    
    _leftButton_custom = ui::Button::create("arrow_n.png", "arrow_s.png");
    _leftButton_custom->setRotation(270.0f);
    _leftButton_custom->setOpacity(0);
    _leftButton_custom->runAction(FadeTo::create(delay, 255));
    _leftButton_custom->setPosition(Vec2(_winSize.width * 0.5f - 475.0f, _winSize.height * 0.48f));
    _leftButton_custom->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( !_isButtonMoving_custom && type == Widget::TouchEventType::ENDED )
        {
            _leftButton_custom->runAction(Sequence::create(CallFunc::create([&]{ _isButtonMoving_custom = true; }),
                                                           DelayTime::create(delay),
                                                           CallFunc::create([&]{ _isButtonMoving_custom = false; }),
                                                           nullptr));
            switch ( _menuCenterIndex_custom )
            {
                case 2:
                {
                    _survivalMode->runAction(Sequence::create(Spawn::create(MoveTo::create(delay / 2, MENU_POSITION_3 + Vec2(50.0f, 0.0f)),
                                                                            ScaleTo::create(delay / 2, 0.85f),
                                                                            nullptr),
                                                              Spawn::create(MoveTo::create(delay / 2, MENU_POSITION_3),
                                                                            ScaleTo::create(delay / 2, 0.9f),
                                                                            nullptr),
                                                              nullptr));
                    
                    _battleRoyalMode->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_4 + Vec2(50.0f, 0.0f)),
                                                                 MoveTo::create(delay / 2, MENU_POSITION_4),
                                                                 nullptr));
                    
                    _teamSurvivalMode->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_5 + Vec2(50.0f, 0.0f)),
                                                                  MoveTo::create(delay / 2, MENU_POSITION_5),
                                                                  nullptr));
                    
                    _menuCenterIndex_custom = 2;

                    break;
                }
                case 3:
                {
                    _survivalMode->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                           ScaleTo::create(delay, 0.9f),
                                                           MoveTo::create(delay, MENU_POSITION_3),
                                                           nullptr));
                    
                    _battleRoyalMode->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                              ScaleTo::create(delay, 0.8f),
                                                              MoveTo::create(delay, MENU_POSITION_4),
                                                              nullptr));
                    
                    _teamSurvivalMode->runAction(MoveTo::create(delay, MENU_POSITION_5));
                    
                    _menuCenterIndex_custom = 2;
                    
                    break;
                }
                case 4:
                {
                    _battleRoyalMode->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                           ScaleTo::create(delay, 0.9f),
                                                           MoveTo::create(delay, MENU_POSITION_3),
                                                           nullptr));
                    
                    _teamSurvivalMode->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                              ScaleTo::create(delay, 0.8f),
                                                              MoveTo::create(delay, MENU_POSITION_4),
                                                              nullptr));
                    
                    _survivalMode->runAction(MoveTo::create(delay, MENU_POSITION_2));
                    
                    _menuCenterIndex_custom = 3;
                    
                    break;
                }
            }
        }
    });
    addChild(_leftButton_custom);
    
    _rightButton_custom = ui::Button::create("arrow_n.png", "arrow_s.png");
    _rightButton_custom->setRotation(90.0f);
    _rightButton_custom->setPosition(Vec2(_winSize.width * 0.5f + 475.0f, _winSize.height * 0.48f));
    _rightButton_custom->setOpacity(0);
    _rightButton_custom->runAction(FadeTo::create(delay, 255));
    _rightButton_custom->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( !_isButtonMoving_custom && type == Widget::TouchEventType::ENDED )
        {
            _leftButton_custom->runAction(Sequence::create(CallFunc::create([&]{ _isButtonMoving_custom = true; }),
                                                           DelayTime::create(delay),
                                                           CallFunc::create([&]{ _isButtonMoving_custom = false; }),
                                                           nullptr));
            switch ( _menuCenterIndex_custom )
            {
                case 2:
                {
                    _battleRoyalMode->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                               ScaleTo::create(delay, 0.9f),
                                                               MoveTo::create(delay, MENU_POSITION_3),
                                                               nullptr));
                    
                    _survivalMode->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                              ScaleTo::create(delay, 0.8f),
                                                              MoveTo::create(delay, MENU_POSITION_2),
                                                              nullptr));
                    
                    _teamSurvivalMode->runAction(MoveTo::create(delay, MENU_POSITION_4));
                    
                    _menuCenterIndex_custom = 3;
                    
                    break;
                }
                case 3:
                {
                    _teamSurvivalMode->runAction(Spawn::create(FadeTo::create(delay, 255),
                                                           ScaleTo::create(delay, 0.9f),
                                                           MoveTo::create(delay, MENU_POSITION_3),
                                                           nullptr));
                    
                    _battleRoyalMode->runAction(Spawn::create(FadeTo::create(delay, 80),
                                                              ScaleTo::create(delay, 0.8f),
                                                              MoveTo::create(delay, MENU_POSITION_2),
                                                              nullptr));
                    
                    _survivalMode->runAction(MoveTo::create(delay, MENU_POSITION_1));
                    
                    _menuCenterIndex_custom = 4;
                    
                    break;
                }
                case 4:
                {
                    _survivalMode->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_1 - Vec2(50.0f, 0.0f)),
                                                              MoveTo::create(delay / 2, MENU_POSITION_1),
                                                              nullptr));
                    
                    _battleRoyalMode->runAction(Sequence::create(MoveTo::create(delay / 2, MENU_POSITION_2 - Vec2(50.0f, 0.0f)),
                                                                 MoveTo::create(delay / 2, MENU_POSITION_2),
                                                                 nullptr));
                    
                    _teamSurvivalMode->runAction(Sequence::create(Spawn::create(MoveTo::create(delay / 2, MENU_POSITION_3 - Vec2(50.0f, 0.0f)),
                                                                                ScaleTo::create(delay / 2, 0.85f),
                                                                                nullptr),
                                                                  Spawn::create(MoveTo::create(delay / 2, MENU_POSITION_3),
                                                                                ScaleTo::create(delay / 2, 0.9f),
                                                                                nullptr),
                                                                  nullptr));
                    
                    _menuCenterIndex_custom = 4;
                    
                    break;
                }
            }
        }
        
    });
    addChild(_rightButton_custom);
    
    _infoText_custom = Text::create("introduce about this mode.", "SpecialElite.TTF", 30);
    _infoText_custom->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.25f));
    _infoText_custom->setOpacity(0);
    _infoText_custom->runAction(FadeTo::create(delay, 255));
    addChild(_infoText_custom);
    
    _searchButton_custom = ui::Button::create("search_n.png", "search_s.png");
    _searchButton_custom->setPosition(Vec2(_winSize.width * 0.4f, _winSize.height * 0.15f));
    _searchButton_custom->setOpacity(0);
    _searchButton_custom->runAction(FadeTo::create(delay, 255));
    _searchButton_custom->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            // 방에 들어간다
            
            flatbuffers::FlatBufferBuilder builder;
            auto obj = fpacket::CreatePacketDoSearchGame(builder, _menuCenterIndex_custom);
            builder.Finish(obj);
            
            Packet* packet = new Packet();
            packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::DO_SEARCH_GAME);
            Server::getInstance().write(packet);
            
            _hideCustomMenuAndShowCustomSearchMenu(delay);
        }
        
    });
    addChild(_searchButton_custom);
    
    _backButton_custom = ui::Button::create("back_n.png", "back_s.png");
    _backButton_custom->setPosition(Vec2(_winSize.width * 0.6f, _winSize.height * 0.15f));
    _backButton_custom->setOpacity(0);
    _backButton_custom->runAction(FadeTo::create(delay, 255));
    _backButton_custom->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hideCustomMenuAndShowPlayMenu(delay);
        }
        
    });
    addChild(_backButton_custom);
}


void MainMenu3::_showMultiSearchMenu(float delay)
{
    _progressDots_multi.resize(11);
    for(auto i = 0 ; i < _progressDots_multi.size(); ++ i)
    {
        _progressDots_multi[i] = Sprite::create("dot.png");
        _progressDots_multi[i]->setScale(0.1f);
        _progressDots_multi[i]->setOpacity(0);
        _progressDots_multi[i]->setColor(Color3B::WHITE);
        _progressDots_multi[i]->setPosition(Vec2(_winSize.width / 2.0f - 100.0f + i * 20.0f, _winSize.height * 0.45f));
        _progressDots_multi[i]->runAction(RepeatForever::create(Spawn::create(FadeTo::create(delay, 255),
                                                                              Sequence::create(DelayTime::create(i * 0.1f),
                                                                                               ScaleTo::create(0.4f, 0.2f),
                                                                                               ScaleTo::create(0.4f, 0.1f),
                                                                                               DelayTime::create(1.0f - (i * 0.1f)),
                                                                                               nullptr),
                                                                              nullptr)));
        addChild(_progressDots_multi[i]);
    }
    
    
    _cancelButton_multiSearch = ui::Button::create("cancel_n.png", "cancel_s.png");
    _cancelButton_multiSearch->setPosition(Vec2(_winSize.width / 2 , _winSize.height * 0.3f));
    _cancelButton_multiSearch->setOpacity(0);
    _cancelButton_multiSearch->runAction(FadeTo::create(delay, 255));
    _cancelButton_multiSearch->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            _hideMultiSearchMenuAndShowMultiMenu(delay);
        }
        
    });
    addChild(_cancelButton_multiSearch);
}


void MainMenu3::_showCustomSearchMenu(float delay)
{
    _progressDots_custom.resize(11);
    for(auto i = 0 ; i < _progressDots_custom.size(); ++ i)
    {
        _progressDots_custom[i] = Sprite::create("dot.png");
        _progressDots_custom[i]->setScale(0.1f);
        _progressDots_custom[i]->setOpacity(0);
        _progressDots_custom[i]->setColor(Color3B::WHITE);
        _progressDots_custom[i]->setPosition(Vec2(_winSize.width / 2.0f - 100.0f + i * 20.0f, _winSize.height * 0.45f));
        _progressDots_custom[i]->runAction(RepeatForever::create(Spawn::create(FadeTo::create(delay, 255),
                                                                              Sequence::create(DelayTime::create(i * 0.1f),
                                                                                               ScaleTo::create(0.4f, 0.2f),
                                                                                               ScaleTo::create(0.4f, 0.1f),
                                                                                               DelayTime::create(1.0f - (i * 0.1f)),
                                                                                               nullptr),
                                                                              nullptr)));
        addChild(_progressDots_custom[i]);
    }
    
    
    _cancelButton_customSearch = ui::Button::create("cancel_n.png", "cancel_s.png");
    _cancelButton_customSearch->setPosition(Vec2(_winSize.width / 2 , _winSize.height * 0.3f));
    _cancelButton_customSearch->setOpacity(0);
    _cancelButton_customSearch->runAction(FadeTo::create(delay, 255));
    _cancelButton_customSearch->addTouchEventListener([this, delay](Ref* ref, Widget::TouchEventType type) {
        
        if ( type == Widget::TouchEventType::ENDED )
        {
            // 검색을 취소한다.
            flatbuffers::FlatBufferBuilder builder;
            auto obj = fpacket::CreatePacketCancelSearchGame(builder, _roomID);
            builder.Finish(obj);
            
            Packet* packet = new Packet();
            packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::CANCEL_SEARCH_GAME);
            Server::getInstance().write(packet);
            
            _hideCustomSearchMenuAndShowCustomMenu(delay);
        }
        
    });
    addChild(_cancelButton_customSearch);
}


void MainMenu3::_hideMainMenu(float delay)
{
    _menuHolder->setEnabled(false);
    _playButton->setTouchEnabled(false);
    _optionButton->setTouchEnabled(false);
    _exitButton->setTouchEnabled(false);
    
    _playButton->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _optionButton->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _exitButton->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
}


void MainMenu3::_hidePlayMenu(float delay)
{
    _singleButton->setTouchEnabled(false);
    _multiButton->setTouchEnabled(false);
    _customButton->setTouchEnabled(false);
    _backButton_play->setTouchEnabled(false);
    
    _singleButton->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _multiButton->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _customButton->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _backButton_play->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
}


void MainMenu3::_hideSingleMenu(float delay)
{
}


void MainMenu3::_hideMultiMenu(float delay)
{
    _leftButton_multi->stopAllActions();
    _rightButton_multi->stopAllActions();
    
    _leftButton_multi->setTouchEnabled(false);
    _rightButton_multi->setTouchEnabled(false);
    _searchButton_multi->setTouchEnabled(false);
    _backButton_multi->setTouchEnabled(false);
    
    _clipRectNode_multi->runAction(Sequence::create(DelayTime::create(delay * 1.1f), RemoveSelf::create(), nullptr));
    _chapter1->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _chapter2->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _chapter3->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _chapter4->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _chapter5->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _leftButton_multi->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _rightButton_multi->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _infoText_multi->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _searchButton_multi->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _backButton_multi->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
}


void MainMenu3::_hideCustomMenu(float delay)
{
    _leftButton_custom->stopAllActions();
    _rightButton_custom->stopAllActions();
    
    _leftButton_custom->setTouchEnabled(false);
    _rightButton_custom->setTouchEnabled(false);
    _searchButton_custom->setTouchEnabled(false);
    _backButton_custom->setTouchEnabled(false);
    
    _clipRectNode_custom->runAction(Sequence::create(DelayTime::create(delay * 1.1f), RemoveSelf::create(), nullptr));
    _battleRoyalMode->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _survivalMode->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _teamSurvivalMode->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _leftButton_custom->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _rightButton_custom->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _infoText_custom->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _searchButton_custom->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    _backButton_custom->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
}


void MainMenu3::_hideMultiSearchMenu(float delay)
{
    for(auto i = 0 ; i < _progressDots_multi.size() ; ++ i)
    {
        _progressDots_multi[i]->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    }
    runAction(Sequence::create(DelayTime::create(delay * 1.1f), CallFunc::create([this]{ _progressDots_multi.clear(); }), NULL));
    
    _cancelButton_multiSearch->stopAllActions();
    _cancelButton_multiSearch->setTouchEnabled(false);
    _cancelButton_multiSearch->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
}


void MainMenu3::_hideCustomSearchMenu(float delay)
{
    for(auto i = 0 ; i < _progressDots_custom.size() ; ++ i)
    {
        _progressDots_custom[i]->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));
    }
    runAction(Sequence::create(DelayTime::create(delay * 1.1f), CallFunc::create([this]{ _progressDots_custom.clear(); }), NULL));
    
    _cancelButton_customSearch->stopAllActions();
    _cancelButton_customSearch->setTouchEnabled(false);
    _cancelButton_customSearch->runAction(Sequence::create(FadeTo::create(delay, 0), RemoveSelf::create(), nullptr));

}


void MainMenu3::_hideMainMenuAndShowPlayMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideMainMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showPlayMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hidePlayMenuAndShowSingleMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hidePlayMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showSingleMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hidePlayMenuAndShowMultiMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hidePlayMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showMultiMenu(delay); }),
                               nullptr));
    
}


void MainMenu3::_hidePlayMenuAndShowCustomMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hidePlayMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showCustomMenu(delay); }),
                               nullptr));
    
}


void MainMenu3::_hidePlayMenuAndShowMainMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hidePlayMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showMainMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hideMultiMenuAndShowPlayMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideMultiMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showPlayMenu(delay); }),
                               nullptr));
    
}


void MainMenu3::_hideMultiMenuAndShowMultiSearchMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideMultiMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showMultiSearchMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hideCustomMenuAndShowPlayMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideCustomMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showPlayMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hideCustomMenuAndShowCustomSearchMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideCustomMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showCustomSearchMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hideMultiSearchMenuAndShowMultiMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideMultiSearchMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showMultiMenu(delay); }),
                               nullptr));
}


void MainMenu3::_hideCustomSearchMenuAndShowCustomMenu(float delay)
{
    runAction(Sequence::create(CallFunc::create([this, delay] { _hideCustomSearchMenu(delay); }),
                               DelayTime::create(delay),
                               CallFunc::create([this, delay] { _showCustomMenu(delay); }),
                               nullptr));
}









