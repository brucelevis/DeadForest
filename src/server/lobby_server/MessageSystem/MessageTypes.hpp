#pragma once

#include <string>

namespace realtrick
{
    class EntityHuman;
    //class ItemBase;
    //class WeaponBase;

    namespace userinterface
    {
        class ItemSlot;
    }
    
    enum MessageType : int
    {
        JOYSTICK_CONTROLED = 0,
        RELOAD_WEAPON = 1,
        HITTED_BY_GUN = 2,
        HITTED_BY_AXE = 3,
        WEAPON_READY = 4,
        REMOVE_SELF = 5,
        SHOW_NEXT_FRAME = 6,
        REPEAT_FRAMES = 7,
        PLAY_SOUND = 8,
        VICTORY_GAME = 9,
        DIE = 10,
        LOAD_GAME = 11,
        MOVE_JOYSTICK_INPUT = 12,
        ATTACK_JOYSTICK_INPUT = 13,
        PUSH_ITEM_TO_INVENTORY = 14,
        PRESS_EQUIP_WEAPON_BUTTON = 15,
        PRESS_RELEASE_WEAPON_BUTTON = 16,
        PRESS_RELOAD_BUTTON = 17,
        RELOAD_COMPLETE = 18,
        BEZEL_DIRECTION_TRIGGERED = 19,
        BEZEL_CLICK_INPUT = 20
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
       /*
    struct ReceiverSenderDamage
    {
        std::string receiver;
        std::string sender;
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
    */
}
    
    
    
    
    
    
    
