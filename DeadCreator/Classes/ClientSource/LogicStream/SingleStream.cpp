//
//  SingleStream.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 4. 3..
//
//

#include "SingleStream.hpp"
#include "GameManager.hpp"
#include "EntityHuman.hpp"
#include "GameWorld.hpp"
#include "UiLayer.hpp"
#include "Inventory.hpp"
#include "ItemSlot.hpp"
#include "WeaponStatus.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Items.hpp"

namespace realtrick
{
    
    bool SingleStream::handleMessage(const Telegram& msg)
    {
        if ( msg.msg == MessageType::LOAD_GAME_PLAYER)
        {
//            _gameMgr->loadSingleGameMap("simple_server_map.txt");
//            _gameMgr->pushLogic(0.0, MessageType::LOAD_GAME_COMPLETE, nullptr);
            
            _gameMgr->loadGMXFile("/Users/jun/Desktop/test_map.gmx");
            
            return true;
        }
        else if ( msg.msg == MessageType::LOAD_GAME_COMPLETE )
        {
            
            _gameMgr->getGameWorld()->loadUiLayer();
            _gameMgr->getGameWorld()->resumeGame();
            
            return true;
        }
        
        else if ( msg.msg == MessageType::MOVE_JOYSTICK_INPUT )
        {
            MoveJoystickData data = *static_cast<MoveJoystickData*>(msg.extraInfo);
            EntityHuman* player = _gameMgr->getPlayerPtr();
            player->setMoving(data.dir);
            
            switch ( data.type )
            {
                case JoystickEx::ClickEventType::BEGAN:
                {
                    player->addInputMask(HumanBehaviorType::RUN);
                    break;
                }
                case JoystickEx::ClickEventType::ENDED:
                {
                    player->removeInputMask(HumanBehaviorType::RUN);
                    break;
                }
                    
                default: break;
            }
            
            return true;
        }
        else if ( msg.msg == MessageType::ATTACK_JOYSTICK_INPUT )
        {
            AttJoystickData data = *static_cast<AttJoystickData*>(msg.extraInfo);
            EntityHuman* player = _gameMgr->getPlayerPtr();
            
            switch ( data.type )
            {
                case ui::Widget::TouchEventType::BEGAN:
                {
                    player->addInputMask(HumanBehaviorType::ATTACK_BEGAN);
                    break;
                }
                case ui::Widget::TouchEventType::ENDED:
                {
                    player->removeInputMask(HumanBehaviorType::ATTACK_BEGAN);
                    player->addInputMask(HumanBehaviorType::ATTACK_ENDED);
                    break;
                }
                case ui::Widget::TouchEventType::CANCELED:
                {
                    player->removeInputMask(HumanBehaviorType::ATTACK_BEGAN);
                    break;
                }
                    
                default: break;
            }
            
            return true;
        }
        else if ( msg.msg == MessageType::BEZEL_DIRECTION_TRIGGERED )
        {
            BezelDirectionTriggerData data= *static_cast<BezelDirectionTriggerData*>(msg.extraInfo);
            EntityHuman* player = _gameMgr->getPlayerPtr();
            player->setTargetHeading(data.dir);
            
            return true;
        }
        else if ( msg.msg == MessageType::BEZEL_CLICK_INPUT )
        {
            BezelInputData data = *static_cast<BezelInputData*>(msg.extraInfo);
            EntityHuman* player = _gameMgr->getPlayerPtr();
            
            if ( data.type == ui::Widget::TouchEventType::BEGAN )
            {
                player->addInputMask(HumanBehaviorType::TURN);
            }
            else if ( data.type == ui::Widget::TouchEventType::ENDED )
            {
                player->removeInputMask(HumanBehaviorType::TURN);
            }
            
            return true;
        }
        else if ( msg.msg == MessageType::PUSH_ITEM_TO_INVENTORY )
        {
            ItemAndOwner data = *static_cast<ItemAndOwner*>(msg.extraInfo);
            data.item->setOwner(data.owner);
            _gameMgr->getPlayerPtr()->getInventory()->pushItem(data.item);
            
            EntityHuman* player = _gameMgr->getPlayerPtr();
            
            if ( data.item->getEntityType() == EntityType::ITEM_AXE )
            {
                SoundSource s;
                s.fileName = "AxeGet.mp3";
                s.position = player->getWorldPosition();
                s.soundRange = 200.0f;
                Dispatch.pushMessage(0.0, player, player, MessageType::PLAY_SOUND, &s);
            }
            else
            {
                SoundSource s;
                s.fileName = "M16A2Enter.mp3";
                s.position = player->getWorldPosition();
                s.soundRange = 200.0f;
                Dispatch.pushMessage(0.0, player, player, MessageType::PLAY_SOUND, &s);
            }
            
            if ( isMasked(data.item->getFamilyMask(), FamilyMask::BULLET_BASE) )
            {
                // 내가 장착하고 있는 총알 종류를 먹었으면 무기정보를 업데이트한다.
                WeaponBase* equipedWeapon = _gameMgr->getPlayerPtr()->getEquipedWeapon();
                if ( equipedWeapon != nullptr )
                {
                    // 주먹이 아니고 무기에 맞는 총알을 먹었으면 업데이트한다.
                    EntityType bulletType = static_cast<EntityType>(data.item->getEntityType());
                    if ( equipedWeapon->getBulletType() == bulletType )
                    {
                        _gameMgr->getPlayerPtr()->getWeaponStatus()->setEntryBullet(bulletType);
                    }
                }
            }
            
            return true;
        }
        else if ( msg.msg == MessageType::PRESS_EQUIP_WEAPON_BUTTON)
        {
            // 아이템을 장착한다.
            ItemSlotData data = *static_cast<ItemSlotData*>(msg.extraInfo);
            data.slot->enableEquippedItem();
            data.slot->useItem();
            
            // 무기 정보 UI를 업데이트 한다.
            WeaponBase* weapon = static_cast<WeaponBase*>(data.slot->getItemPtr());
            _gameMgr->getPlayerPtr()->getWeaponStatus()->setWeaponStatus(weapon);
            
            return true;
        }
        else if ( msg.msg == MessageType::PRESS_RELEASE_WEAPON_BUTTON)
        {
            ItemSlotData data = *static_cast<ItemSlotData*>(msg.extraInfo);
            data.slot->disableEquippedItem();
            data.slot->releaseWeapon();
            
            // 무기 정보 UI를 업데이트 한다.
            _gameMgr->getPlayerPtr()->getWeaponStatus()->setWeaponStatus(nullptr);
            
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
            
            EntityHuman* player = _gameMgr->getPlayerPtr();
            WeaponBase* equipedWeapon = player->getEquipedWeapon();
            
            if ( equipedWeapon != nullptr && equipedWeapon->getEntityType() != EntityType::ITEM_AXE )
                equipedWeapon->reload();
            
            return true;
        }
        else if ( msg.msg == MessageType::RELOAD_COMPLETE )
        {
            EntityHuman* player = _gameMgr->getPlayerPtr();
            WeaponBase* equipedWeapon = player->getEquipedWeapon();
            
            // 인벤토리에있는 총알아이템을 소모한다.
            userinterface::Inventory* inventory = player->getInventory();
            if ( equipedWeapon->getEntityType() == EntityType::ITEM_GLOCK17 )
            {
                inventory->setItemAmount(Bullet9mm::create(nullptr, "9mm.png", "9mm.png", "9mm.png", ui::Widget::TextureResType::PLIST),
                                         inventory->getItemAmount(equipedWeapon->getBulletType()) -  equipedWeapon->getReservedBullets());
            }
            else if ( equipedWeapon->getEntityType() == EntityType::ITEM_M16A2 )
            {
                inventory->setItemAmount(Bullet556mm::create(nullptr, "5_56mm.png", "5_56mm.png", "5_56mm.png", ui::Widget::TextureResType::PLIST),
                                         inventory->getItemAmount(equipedWeapon->getBulletType()) -  equipedWeapon->getReservedBullets());
            }
            else if ( equipedWeapon->getEntityType() == EntityType::ITEM_M1897 )
            {
                inventory->setItemAmount(BulletShell::create(nullptr, "Shell.png", "Shell.png", "Shell.png", ui::Widget::TextureResType::PLIST),
                                         inventory->getItemAmount(equipedWeapon->getBulletType()) -  equipedWeapon->getReservedBullets());
            }
            
            // 무기의 탄창을 채운다.
            equipedWeapon->setNumOfLeftRounds(equipedWeapon->getNumOfLeftRounds() +  equipedWeapon->getReservedBullets());
            equipedWeapon->setReservecBullets(0);
            
            // 무기정보UI의 무기를 갱신한다.
            userinterface::WeaponStatus* weaponStatus = _gameMgr->getPlayerPtr()->getWeaponStatus();
            weaponStatus->setWeaponStatus(equipedWeapon);
            weaponStatus->enableButton();
            
            return true;
        }
        
        return false;
    }
    
}









