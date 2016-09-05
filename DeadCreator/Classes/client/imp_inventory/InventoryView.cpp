//
//  InventoryView.cpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 2..
//
//

#include "InventoryView.hpp"
#include "InventoryData.hpp"
#include "ItemBase.hpp"
#include "StringHelper.hpp"
using namespace realtrick::client;
using namespace cocos2d;

// constant variables
const Vec2 ORIGIN(40.0f, 30.0f);
const float PAD = 30.0f;
const int SLOT_SIZE = 12;

InventoryView* InventoryView::create()
{
    auto ret = new (std::nothrow) InventoryView();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool InventoryView::init()
{
    if ( !Node::init() )
        return false;
    
    _background = Sprite::create("client/ui/inventory_view_bg.png");
    addChild(_background);
    
    _slots.resize(SLOT_SIZE);
    for(int i = 0 ; i < 12 ; ++ i)
    {
        int x = i % 4;
        int y = i / 4;
        
        _slots[i] = ui::Button::create("client/ui/inventory_view_slot.png", "client/ui/inventory_view_slot.png");
        _slots[i]->setAnchorPoint(Vec2::ZERO);
        _slots[i]->setPosition(Vec2(ORIGIN.x + (100.0f + PAD) * x, ORIGIN.y + (100.0f + PAD) * y));
        _background->addChild(_slots[i]);
    }
    
    _infoView = Sprite::create("client/ui/inventory_view_info.png");
    _infoView->setAnchorPoint(Vec2::ZERO);
    _infoView->setPosition(Vec2(ORIGIN.x + (100.0f + PAD) * 4 + PAD - 10.0f, ORIGIN.y - 5.0f));
    _background->addChild(_infoView);
    
    return true;
}


void InventoryView::syncItemView(InventoryData* data)
{
    for(int slot = 0 ; slot < data->getMaxItemSlot() ; ++ slot)
    {
        if ( _background->getChildByTag(slot) ) _background->removeChildByTag(slot);
        
        auto item = data->getItem(slot);
        if ( item )
        {
            int x = slot % 4;
            int y = slot / 4;
            
            auto spr = Sprite::createWithSpriteFrameName(item->getInSlotFrameName());
            spr->setAnchorPoint(Vec2::ZERO);
            spr->setPosition(Vec2(ORIGIN.x + (100.0f + PAD) * x, ORIGIN.y + (100.0f + PAD) * y));
            spr->setTag(slot);
            _background->addChild(spr);
            
            int amount = item->getAmount();
            if ( amount > 1 )
            {
                auto numberLabel = ui::Text::create(_to_string(item->getAmount()), "fonts/SpecialElite.TTF", 20);
                numberLabel->setPosition(Vec2(0.0f, 15.0f));
                numberLabel->setTextColor(Color4B::BLACK);
                spr->addChild(numberLabel);
            }
        }
    }
}









