//
//  InventoryView.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 9. 2..
//
//

#include "InventoryView.hpp"
#include "InventoryData.hpp"
#include "Game.hpp"
#include "ItemBase.hpp"
#include "StringHelper.hpp"
using namespace realtrick::client;
using namespace cocos2d;

// constant variables
const Vec2 ORIGIN(40.0f, 30.0f);
const float PAD = 30.0f;
const int SLOT_SIZE = 12;

InventoryView::InventoryView(Game* game):
_game(game)
{
}


InventoryView::~InventoryView()
{
}


InventoryView* InventoryView::create(Game* game)
{
    auto ret = new (std::nothrow) InventoryView(game);
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
    for(int i = 0 ; i < SLOT_SIZE ; ++ i)
    {
        int x = i % 4;
        int y = i / 4;
        
        _slots[i] = ui::Button::create("client/ui/inventory_view_slot_n.png", "client/ui/inventory_view_slot_s.png");
        _slots[i]->setZoomScale(0.0f);
        _slots[i]->setAnchorPoint(Vec2::ZERO);
        _slots[i]->setTag(i);
        _slots[i]->setPosition(Vec2(ORIGIN.x + (100.0f + PAD) * x, ORIGIN.y + (100.0f + PAD) * y));
        _slots[i]->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type){
            
            auto self = static_cast<ui::Button*>(ref);
            if ( type == ui::Widget::TouchEventType::ENDED ) {
                int slot = self->getTag();
                _game->pushLogic(0.0, MessageType::USE_ITEM_FROM_INVENTORY, &slot);
            }
            
        });
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
        _slots[slot]->removeAllChildren();
        
        auto item = data->getItem(slot);
        if ( item )
        {
            auto spr = Sprite::createWithSpriteFrameName(item->getInSlotFrameName());
            spr->setPosition(Vec2(64.0f, 64.0f));
            spr->setScale(2.0f);
            _slots[slot]->addChild(spr);
            
            int amount = item->getAmount();
            if ( amount > 1 )
            {
                auto numberLabel = ui::Text::create(_to_string(item->getAmount()), "fonts/SpecialElite.TTF", 20);
                numberLabel->setPosition(Vec2(64.0f, 30.0f));
                numberLabel->setTextColor(Color4B::BLACK);
                _slots[slot]->addChild(numberLabel);
            }
        }
    }
}









