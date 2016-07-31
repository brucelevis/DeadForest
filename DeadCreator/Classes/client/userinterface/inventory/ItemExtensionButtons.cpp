//
//  ItemExtensionButtons.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 23..
//
//

#include "ItemExtensionButtons.hpp"
#include "AnimatedButton.hpp"
#include "ItemSlot.hpp"
#include "ItemBase.hpp"
#include "EntityHuman.hpp"
#include "WeaponBase.hpp"
#include "Game.hpp"
using namespace realtrick::client;
using namespace cocos2d;

ItemExtensionButtons::ItemExtensionButtons(Game* game) :
_game(game),
_ownSlot(nullptr)
{}


ItemExtensionButtons::~ItemExtensionButtons()
{
    
}


ItemExtensionButtons* ItemExtensionButtons::create(Game* game, ItemSlot* slot)
{
    ItemExtensionButtons* ret = new (std::nothrow) ItemExtensionButtons(game);
    if ( ret && ret->init(slot) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ItemExtensionButtons::init(ItemSlot* slot)
{
    if ( !Node::init() )
        return false;
    
    _ownSlot = slot;
    
    _buttons.resize(ButtonType::MAX);
    
    _buttons[ButtonType::USE] = AnimatedButton::create("b2_n.png", "b2_s.png", ui::Widget::TextureResType::PLIST);
    _buttons[ButtonType::USE]->setAnimationFrameNames({"b21.png", "b22.png", "b23.png", "b2_n.png"});
    _buttons[ButtonType::USE]->setBottomInfoString("USE");
    
    _buttons[ButtonType::EQUIP] = AnimatedButton::create("b2_n.png", "b2_s.png", ui::Widget::TextureResType::PLIST);
    _buttons[ButtonType::EQUIP]->setAnimationFrameNames({"b21.png", "b22.png", "b23.png", "b2_n.png"});
    _buttons[ButtonType::EQUIP]->setBottomInfoString("EQUIP");
    
    _buttons[ButtonType::EXAMINE] = AnimatedButton::create("b1_n.png", "b1_s.png", ui::Widget::TextureResType::PLIST);
    _buttons[ButtonType::EXAMINE]->setAnimationFrameNames({"b11.png", "b12.png", "b13.png", "b1_n.png"});
    _buttons[ButtonType::EXAMINE]->setBottomInfoString("EXAMINE");
    
    _buttons[ButtonType::DISCARD] = AnimatedButton::create("b3_n.png", "b3_s.png", ui::Widget::TextureResType::PLIST);
    _buttons[ButtonType::DISCARD]->setAnimationFrameNames({"b31.png", "b32.png", "b33.png", "b3_n.png"});
    _buttons[ButtonType::DISCARD]->setBottomInfoString("DISCARD");
    
    _buttons[ButtonType::RELEASE] = AnimatedButton::create("b4_n.png", "b4_s.png", ui::Widget::TextureResType::PLIST);
    _buttons[ButtonType::RELEASE]->setAnimationFrameNames({"b41.png", "b42.png", "b43.png", "b4_n.png"});
    _buttons[ButtonType::RELEASE]->setBottomInfoString("RELEASE");
    
    for(int i = 0 ; i < ButtonType::MAX ; ++ i)
    {
        _buttons[i]->setAnimationInterval(0.03f);
        _buttons[i]->setButtonSize(Size(128,128));
        _ownSlot->getInventory()->getBackgroundView()->addChild(_buttons[i], 100);
    }
    
    _buttons[EQUIP]->setButtonCallback([&](Ref* ref, ui::Widget::TouchEventType type){
        
        if ( type == ui::Widget::TouchEventType::ENDED )
        {
            log("<ItemExtensionButtons::init> equip");
            ItemSlotData data;
            data.slot = _ownSlot;
            _game->pushLogic(0.0, MessageType::PRESS_EQUIP_WEAPON_BUTTON, reinterpret_cast<void*>(&data));
        }
        
    });
    
    _buttons[RELEASE]->setButtonCallback([&](Ref* ref, ui::Widget::TouchEventType type){
        
        if ( type == ui::Widget::TouchEventType::ENDED )
        {
            log("<ItemExtensionButtons::init> release");
            ItemSlotData data;
            data.slot = _ownSlot;
            _game->pushLogic(0.0, MessageType::PRESS_RELEASE_WEAPON_BUTTON, reinterpret_cast<void*>(&data));
        }
        
    });
    
    _buttons[USE]->setButtonCallback([&](Ref* ref, ui::Widget::TouchEventType type){
        
        if ( type == ui::Widget::TouchEventType::ENDED )
        {
            log("<ItemExtensionButtons::init> use");
        }
        
    });
    
    _buttons[DISCARD]->setButtonCallback([&](Ref* ref, ui::Widget::TouchEventType type){
        
        if ( type == ui::Widget::TouchEventType::ENDED )
        {
            log("<ItemExtensionButtons::init> discard");
        }
        
    });
    
    _buttons[EXAMINE]->setButtonCallback([&](Ref* ref, ui::Widget::TouchEventType type){
        
        if ( type == ui::Widget::TouchEventType::ENDED )
        {
            log("<ItemExtensionButtons::init> examine");
        }
        
    });
    
    
    return true;
}


void ItemExtensionButtons::showButtons()
{
    ItemBase* item = _ownSlot->getItemPtr();
    int familyMask = item->getFamilyMask();
    
    const float PADDING = 80;
    
    if ( isMasked(familyMask, FamilyMask::WEAPON_BASE) )
    {
        if ( _ownSlot->isEquippedItem() )
        {
            // 장착중인 아이템
            // 1. 해제
            _buttons[RELEASE]->setPosition(Vec2(_ownSlot->getPosition().x, _ownSlot->getPosition().y + PADDING));
            _buttons[RELEASE]->show();
        }
        else
        {
            // 장착중이지 않은 아이템
            // 1. 장착
            // 2. 버리기
            // 3. 조사
            _buttons[EQUIP]->setPosition(Vec2(_ownSlot->getPosition().x - PADDING, _ownSlot->getPosition().y));
            _buttons[DISCARD]->setPosition(Vec2(_ownSlot->getPosition().x + PADDING , _ownSlot->getPosition().y));
            _buttons[EXAMINE]->setPosition(Vec2(_ownSlot->getPosition().x , _ownSlot->getPosition().y + PADDING));
            
            _buttons[EQUIP]->show();
            _buttons[DISCARD]->show();
            _buttons[EXAMINE]->show();
        }
    }
}


void ItemExtensionButtons::hideButtons()
{
    for ( const auto& button : _buttons )
    {
        button->hide();
    }
}


void ItemExtensionButtons::setButtonSize(const cocos2d::Size& size)
{
    for ( const auto& button : _buttons )
    {
        button->setButtonSize(size);
    }
}









