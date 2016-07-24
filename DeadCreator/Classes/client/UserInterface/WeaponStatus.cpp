//
//  WeaponStatus.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 5. 14..
//
//

#include "WeaponStatus.hpp"
#include "WeaponBase.hpp"
#include "GameManager.hpp"
#include "GameWorld.hpp"
#include "UiLayer.hpp"
#include "Inventory.hpp"

using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::userinterface;


WeaponStatus::WeaponStatus(GameManager* mgr) :
_gameMgr(mgr),
_numOfEntryBullet(0),
_numOfRemainBullet(0)
{
}


WeaponStatus* WeaponStatus::create(GameManager* mgr)
{
    auto ret = new (std::nothrow) WeaponStatus(mgr);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


bool WeaponStatus::init()
{
    if ( !Node::init() )
        return false;

    _reloadButton = ui::Button::create("punch_reload_n.png", "punch_reload_n.png");
    _reloadButton->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type)
                                         {
                                             if ( type == ui::Widget::TouchEventType::ENDED )
                                             {
                                                 _gameMgr->pushLogic(0.0, MessageType::PRESS_RELOAD_BUTTON, nullptr);
                                             }
                                         });
    addChild(_reloadButton);
    
    _remainBulletText = ui::Text::create("", "SpecialElite.TTF", 20);
    _remainBulletText->setPosition(Vec2(-30.0f, -70.0f));
    addChild(_remainBulletText);
    
    _entryBulletText = ui::Text::create("", "SpecialElite.TTF", 20);
    _entryBulletText->setPosition(Vec2(30.0f, -70.0f));
    addChild(_entryBulletText);

    return true;
}


void WeaponStatus::setWeaponStatus(WeaponBase* weapon)
{
    _displayedWeapon = weapon;
    
    if ( weapon )
    {
        switch ( weapon->getEntityType() )
        {
            case EntityType::ITEM_GLOCK17:
            {
                _reloadButton->loadTextures("glock_reload_n.png", "glock_reload_s.png");
                setRemainBullet(weapon->getNumOfLeftRounds());
                setEntryBullet(weapon);
                
                break;
            }
            case EntityType::ITEM_M16A2:
            {
                _reloadButton->loadTextures("m16a2_reload2_n.png", "m16a2_reload2_s.png");
                setRemainBullet(weapon->getNumOfLeftRounds());
                setEntryBullet(weapon);

                break;
            }
            case EntityType::ITEM_M1897:
            {
                _reloadButton->loadTextures("m1897_reload2_n.png", "m1897_reload2_s.png");
                setRemainBullet(weapon->getNumOfLeftRounds());
                setEntryBullet(weapon);
                
                break;
            }
            case EntityType::ITEM_AXE:
            {
                _reloadButton->loadTextures("axe_reload2_n.png", "axe_reload2_n.png");
                
                _remainBulletText->setString("");
                _entryBulletText->setString("");
                _numOfRemainBullet = 2100000000; // infinite
                _numOfEntryBullet  = 2100000000;
                
                break;
            }
                
            default: break;
                
        }
    }
    else
    {
        _reloadButton->loadTextures("punch_reload_n.png", "punch_reload_n.png");
        
        _remainBulletText->setString("");
        _entryBulletText->setString("");
        _numOfRemainBullet = 2100000000; // infinite
        _numOfEntryBullet  = 2100000000;

    }
}


void WeaponStatus::setEntryBullet(WeaponBase* weapon)
{
    int ownedBullet = _gameMgr->getPlayerPtr()->getInventory()->getItemAmount(weapon->getBulletType());
    _setEntryBulletNumber(ownedBullet);
}


void WeaponStatus::setEntryBullet(EntityType bulletType)
{
    int ownedBullet = _gameMgr->getPlayerPtr()->getInventory()->getItemAmount(bulletType);
    _setEntryBulletNumber(ownedBullet);
}


void WeaponStatus::disableButton()
{
    _reloadButton->setOpacity(128);
    _reloadButton->setTouchEnabled(false);
}


void WeaponStatus::enableButton()
{
    _reloadButton->setOpacity(255);
    _reloadButton->setTouchEnabled(true);
}









