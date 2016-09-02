//
//  InventoryView.cpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 2..
//
//

#include "InventoryView.hpp"
using namespace realtrick::client;
using namespace cocos2d;

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
    
    Vec2 origin = Vec2(40, 30);
    float pad = 30.0f;
    
    _slots.resize(12);
    for(int i = 0 ; i < 12 ; ++ i)
    {
        int x = i % 4;
        int y = i / 4;
        
        _slots[i] = ui::Button::create("client/ui/inventory_view_slot.png", "client/ui/inventory_view_slot.png");
        _slots[i]->setAnchorPoint(Vec2::ZERO);
        _slots[i]->setPosition(Vec2(origin.x + (100.0f + pad) * x, origin.y + (100.0f + pad) * y));
        _background->addChild(_slots[i]);
    }
    
    _infoView = Sprite::create("client/ui/inventory_view_info.png");
    _infoView->setAnchorPoint(Vec2::ZERO);
    _infoView->setPosition(Vec2(origin.x + (100.0f + pad) * 4 + pad - 10.0f, origin.y - 5.0f));
    _background->addChild(_infoView);
    
    _closeButton = ui::Button::create("client/ui/inventory_close_btn_n.png", "client/ui/inventory_close_btn_s.png");
    _closeButton->setPosition(Vec2(0.0f, -250.0f));
    _closeButton->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type){
        
        if ( type == ui::Widget::TouchEventType::ENDED ) {
            this->setVisible(false);
        }
        
    });
    addChild(_closeButton);
    
    return true;
}









