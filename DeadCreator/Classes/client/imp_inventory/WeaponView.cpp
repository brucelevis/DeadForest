//
//  WeaponView.cpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 6..
//
//

#include "WeaponView.hpp"
#include "InventoryData.hpp"
#include "Game.hpp"
using namespace realtrick::client;
using namespace cocos2d;

WeaponView::WeaponView(Game* game) :
_game(game)
{
}


WeaponView::~WeaponView()
{
}


WeaponView* WeaponView::create(Game* game)
{
    auto ret = new (std::nothrow) WeaponView(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool WeaponView::init()
{
    if ( !Node::init() )
        return false;
    
    _view = ui::Button::create("client/ui/glock_reload_n.png", "client/ui/glock_reload_s.png");
    _view->setPosition(Vec2(0.0f, 50.0f));
    addChild(_view);
    
    _numOfBullets = ui::Text::create("17 / 51", "fonts/SpecialElite.TTF", 20);
    _numOfBullets->setPosition(Vec2(0.0f, -30.0f));
    addChild(_numOfBullets);
    
    return true;
}


void WeaponView::syncWeaponView(InventoryData* data)
{
    auto player = _game->getPlayerPtr();
    if ( player )
    {
        auto equipedWeapon = player->getEquipedWeapon();
        if ( equipedWeapon )
        {
            std::string remainBulletInInventory = "0";
            auto bullet = data->getItemType(equipedWeapon->getBulletType());
            if ( bullet )
            {
                remainBulletInInventory = _to_string(bullet->getAmount());
            }
            
            std::string reservedBullet = _to_string(equipedWeapon->getNumOfLeftRounds());
        
            _numOfBullets->setString(reservedBullet + " / " + remainBulletInInventory);
            
            int weaponType = equipedWeapon->getEntityType();
            if ( weaponType == EntityType::ITEM_GLOCK17 )
            {
                _view->loadTextures("client/ui/glock_reload_n.png", "client/ui/glock_reload_s.png");
            }
            else if ( weaponType == EntityType::ITEM_M16A2 )
            {
                _view->loadTextures("client/ui/m16a2_reload_n.png", "client/ui/m16a2_reload_s.png");
            }
            else if ( weaponType == EntityType::ITEM_AXE )
            {
                _view->loadTextures("client/ui/axe_reload_n.png", "client/ui/axe_reload_n.png");
                _numOfBullets->setString("");
            }
            else if ( weaponType == EntityType::ITEM_M1897 )
            {
                _view->loadTextures("client/ui/m1897_reload_n.png", "client/ui/m1897_reload_s.png");
            }
        }
        else
        {
            _view->loadTextures("client/ui/punch_reload_n.png", "client/ui/punch_reload_n.png");
            _numOfBullets->setString("");
        }
    }
}









