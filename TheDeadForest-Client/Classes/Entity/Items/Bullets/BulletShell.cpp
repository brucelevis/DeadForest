//
//  BulletShell.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 3..
//
//

#include "BulletShell.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"

namespace realtrick
{
    
    BulletShell::BulletShell(GameManager* mgr) : BulletBase(mgr)
    {
        setEntityType(BULLET_SHELL);
        setMaxBandedNumber(30);
    }
    
    
    BulletShell::~BulletShell()
    {}
    
    
    BulletShell* BulletShell::create(GameManager* mgr,
                                 const char* inGameImage_n, const char* inGameImage_s, const char* inSlotImage,
                                 cocos2d::ui::Widget::TextureResType texResType)
    {
        BulletShell* ret = new (std::nothrow)BulletShell(mgr);
        if( ret && ret->init(inGameImage_n, inGameImage_s, inSlotImage, texResType))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    
    BulletShell* BulletShell::clone() const
    {
        return new BulletShell(*this);
    }
    
    
    void BulletShell::discard()
    {
        BulletShell* item = BulletShell::create(_gameMgr, getInGameFrameName_n().c_str(), getInGameFrameName_s().c_str(), getInSlotFrameName().c_str(), ui::Widget::TextureResType::PLIST);
        item->setAmount( getAmount() );
        item->setPosition(Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
        _gameMgr->addDynamicEntity(item, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
    }
    
}







