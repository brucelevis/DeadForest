//
//  SingleStream.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 4. 3..
//
//

#include "SingleStream.hpp"
#include "Game.hpp"
#include "EntityPlayer.hpp"
#include "UiLayer.hpp"
#include "Inventory.hpp"
#include "ItemSlot.hpp"
#include "WeaponStatus.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Items.hpp"
#include "InputCommands.hpp"
using namespace cocos2d;
using namespace realtrick::client;

bool SingleStream::handleMessage(const Telegram& msg)
{
    // loading methods
    if ( msg.msg == MessageType::LOAD_GAME_PLAYER)
    {
        _game->loadGMXFile("temp_game_map");
        _game->pushLogic(0.0, MessageType::LOAD_GAME_COMPLETE, nullptr);
        
        return true;
    }
    
    else if ( msg.msg == MessageType::LOAD_GAME_COMPLETE )
    {
        _game->loadUiLayer();
        _game->resumeGame();
    
        return true;
    }
    
    // input commands
    else if ( msg.msg == MessageType::MOVE_JOYSTICK_INPUT )
    {
        MoveJoystickData* data = static_cast<MoveJoystickData*>(msg.extraInfo);
        
        HumanBase* human = _game->getPlayerPtr();
        human->setMoving(data->dir);
        
        if ( data->type == JoystickEx::ClickEventType::BEGAN || data->type == JoystickEx::ClickEventType::MOVED )
        {
            InputMoveBegin inputCommand(human, data->dir);
            inputCommand.execute();
        }
        else if ( data->type == JoystickEx::ClickEventType::ENDED )
        {
            InputMoveEnd inputCommand(human);
            inputCommand.execute();
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::ATTACK_JOYSTICK_INPUT )
    {
        AttJoystickData* data = static_cast<AttJoystickData*>(msg.extraInfo);
        HumanBase* human = _game->getPlayerPtr();
        
        if ( data->type == ui::Widget::TouchEventType::BEGAN )
        {
            InputAttackBegin inputCommand(human);
            inputCommand.execute();
        }
        
        else if ( data->type == ui::Widget::TouchEventType::ENDED || data->type == ui::Widget::TouchEventType::CANCELED )
        {
            InputAttackEnd inputCommand(human);
            inputCommand.execute();
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::BEZEL_DIRECTION_TRIGGERED )
    {
        BezelDirectionTriggerData* data= static_cast<BezelDirectionTriggerData*>(msg.extraInfo);
        HumanBase* human = _game->getPlayerPtr();
        
        if ( data->type == ui::Widget::TouchEventType::BEGAN || data->type == ui::Widget::TouchEventType::MOVED )
        {
            InputBezelBegin inputCommand(human, data->dir);
            inputCommand.execute();
        }
        else if ( data->type == ui::Widget::TouchEventType::ENDED || data->type == ui::Widget::TouchEventType::CANCELED )
        {
            InputBezelEnd inputCommand(human);
            inputCommand.execute();
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::PRESS_RELOAD_BUTTON )
    {
        /*
         TODO:
         1. 재장전 중일때 무기를 못 바꾼다.
         2. 재장전 중일때 프로그래스바 생성한다.
         3. 재장전 중일때 재장전버튼을 못누른다.
         */
        
        InputReload inputCommand(_game->getPlayerPtr());
        inputCommand.execute();
        
        return true;
    }
    
    // callback funcs
    else if ( msg.msg == MessageType::PUSH_ITEM_TO_INVENTORY )
    {
        ItemAndOwner* data = static_cast<ItemAndOwner*>(msg.extraInfo);
            data->item->setOwner(data->owner);
        _game->getPlayerPtr()->getInventory()->pushItem(data->item);
        
        EntityPlayer* player = _game->getPlayerPtr();
        
        if ( data->item->getEntityType() == EntityType::ITEM_AXE )
        {
            SoundSource s;
            s.fileName = "AxeGet.mp3";
            s.position = player->getWorldPosition();
            s.soundRange = 200.0f;
            _game->sendMessage(0.0, player, player, MessageType::PLAY_SOUND, &s);
        }
        else
        {
            SoundSource s;
            s.fileName = "M16A2Enter.mp3";
            s.position = player->getWorldPosition();
            s.soundRange = 200.0f;
            _game->sendMessage(0.0, player, player, MessageType::PLAY_SOUND, &s);
        }
        
        if ( isMasked(data->item->getFamilyMask(), FamilyMask::BULLET_BASE) )
        {
            // 내가 장착하고 있는 총알 종류를 먹었으면 무기정보를 업데이트한다.
            WeaponBase* equipedWeapon = _game->getPlayerPtr()->getEquipedWeapon();
            if ( equipedWeapon != nullptr )
            {
                // 주먹이 아니고 무기에 맞는 총알을 먹었으면 업데이트한다.
                EntityType bulletType = static_cast<EntityType>(data->item->getEntityType());
                if ( equipedWeapon->getBulletType() == bulletType )
                {
                    _game->getPlayerPtr()->getWeaponStatus()->setEntryBullet(bulletType);
                }
            }
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::PRESS_EQUIP_WEAPON_BUTTON )
    {
        // 아이템을 장착한다.
        ItemSlotData* data = static_cast<ItemSlotData*>(msg.extraInfo);
        data->slot->enableEquippedItem();
        data->slot->useItem();
        
        // 무기 정보 UI를 업데이트 한다.
        WeaponBase* weapon = static_cast<WeaponBase*>(data->slot->getItemPtr());
        _game->getPlayerPtr()->getWeaponStatus()->setWeaponStatus(weapon);
        
        return true;
    }
    
    else if ( msg.msg == MessageType::PRESS_RELEASE_WEAPON_BUTTON )
    {
        ItemSlotData* data = static_cast<ItemSlotData*>(msg.extraInfo);
        data->slot->disableEquippedItem();
        data->slot->releaseWeapon();
        
        // 무기 정보 UI를 업데이트 한다.
        _game->getPlayerPtr()->getWeaponStatus()->setWeaponStatus(nullptr);
        
        return true;
    }
    
    else if ( msg.msg == MessageType::RELOAD_COMPLETE )
    {
        EntityPlayer* player = _game->getPlayerPtr();
        WeaponBase* equipedWeapon = player->getEquipedWeapon();
        
        // 인벤토리에있는 총알아이템을 소모한다.
        Inventory* inventory = player->getInventory();
        if ( equipedWeapon->getEntityType() == EntityType::ITEM_GLOCK17 )
        {
            inventory->setItemAmount(Bullet9mm::create(nullptr),
                                     inventory->getItemAmount(equipedWeapon->getBulletType()) -  equipedWeapon->getReservedBullets());
        }
        else if ( equipedWeapon->getEntityType() == EntityType::ITEM_M16A2 )
        {
            inventory->setItemAmount(Bullet556mm::create(nullptr),
                                     inventory->getItemAmount(equipedWeapon->getBulletType()) -  equipedWeapon->getReservedBullets());
        }
        else if ( equipedWeapon->getEntityType() == EntityType::ITEM_M1897 )
        {
            inventory->setItemAmount(BulletShell::create(nullptr),
                                     inventory->getItemAmount(equipedWeapon->getBulletType()) -  equipedWeapon->getReservedBullets());
        }
        
        // 무기의 탄창을 채운다.
        equipedWeapon->setNumOfLeftRounds(equipedWeapon->getNumOfLeftRounds() +  equipedWeapon->getReservedBullets());
        equipedWeapon->setReservecBullets(0);
        
        // 무기정보UI의 무기를 갱신한다.
        WeaponStatus* weaponStatus = _game->getPlayerPtr()->getWeaponStatus();
        weaponStatus->setWeaponStatus(equipedWeapon);
        weaponStatus->enableButton();
        
        return true;
    }
    
    return false;
}









