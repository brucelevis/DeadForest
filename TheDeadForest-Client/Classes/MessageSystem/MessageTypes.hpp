//
//  MessageTypes.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//
//

#pragma once

#include <string>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "JoystickEx.hpp"

namespace realtrick
{
    
    class EntityHuman;
    class ItemBase;
    class WeaponBase;
    
    namespace userinterface
    {
        class ItemSlot;
    }
    
    enum MessageType : int
    {
        JOYSTICK_CONTROLED = 0,
        RELOAD_WEAPON,
        HITTED_BY_GUN,
        HITTED_BY_AXE,
        WEAPON_READY,
        REMOVE_SELF,
        SHOW_NEXT_FRAME,
        REPEAT_FRAMES,
        PLAY_SOUND,
        VICTORY_GAME,
        DIE,
        LOAD_GAME_PLAYER,
        LOAD_GAME_COMPLETE,
        MOVE_JOYSTICK_INPUT,
        ATTACK_JOYSTICK_INPUT,
        PUSH_ITEM_TO_INVENTORY,
        PRESS_EQUIP_WEAPON_BUTTON,
        PRESS_RELEASE_WEAPON_BUTTON,
        PRESS_RELOAD_BUTTON,
        RELOAD_COMPLETE,
        BEZEL_DIRECTION_TRIGGERED,
        BEZEL_CLICK_INPUT,
        M16A2_SHOOT,
        CROSS_HAIR_EVENT,
        SCREEN_VIBRATE_EVENT
    };
        
    enum HumanBehaviorType
    {
        NONE                            = 1 << 0,
        MOVE                            = 1 << 1,
        RUN                             = 1 << 2,
        ATTACK_BEGAN                    = 1 << 3,
        ATTACK_ENDED                    = 1 << 4,
        TURN                            = 1 << 5
    };
        
    struct ReceiverSenderDamage
    {
        int receiverID;
        int senderID;
        int damage;
    };
        
    struct MoveJoystickData
    {
        cocos2d::Ref* ref;
        cocos2d::Vec2 dir;
        JoystickEx::ClickEventType type;
    };
    
    struct AttJoystickData
    {
        cocos2d::Ref* ref;
        cocos2d::ui::Widget::TouchEventType type;
    };
    
    struct ItemAndOwner
    {
        EntityHuman* owner;
        ItemBase* item;
    };
    
    struct ItemSlotData
    {
        userinterface::ItemSlot* slot;
    };
    
    struct ReloadData
    {
        WeaponBase* reloadWeapon;
    };
    
    struct BezelDirectionTriggerData
    {
        cocos2d::Ref* ref;
        cocos2d::Vec2 dir;
    };
    
    struct BezelInputData
    {
        cocos2d::Ref* ref;
        cocos2d::ui::Widget::TouchEventType type;
    };
    
}
    
    
    
    
    
    
    
