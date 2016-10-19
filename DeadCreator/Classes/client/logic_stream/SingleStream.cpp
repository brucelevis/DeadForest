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
#include "HumanOwnedAnimations.hpp"
#include "Items.hpp"
#include "InputCommands.hpp"
#include "AnimatedFiniteEntity.hpp"
#include "InventoryData.hpp"
#include "Camera2D.hpp"
using namespace cocos2d;
using namespace realtrick::client;

bool SingleStream::handleMessage(const Telegram& msg)
{
    // loading methods
    if ( msg.msg == MessageType::LOAD_GAME_PLAYER)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
        _game->loadGMXFileFromPath("temp_game_map");
#elif ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        _game->loadGMXFileFromPath(FileUtils::getInstance()->fullPathForFilename("client/final_client2.gmx").c_str());
#endif
        _game->loadGameContents(PlayerType::PLAYER1);
        _game->pushLogic(0.0, MessageType::LOAD_GAME_COMPLETE, nullptr);
        
        return true;
    }
    
    else if ( msg.msg == MessageType::LOAD_GAME_COMPLETE )
    {
        _game->loadBGM();
        _game->resumeGame();
        
        return true;
    }
    
    // input commands
    else if ( msg.msg == MessageType::MOVE_JOYSTICK_INPUT )
    {
        MoveJoystickData* data = static_cast<MoveJoystickData*>(msg.extraInfo);
        
        HumanBase* human = _game->getPlayerPtr();
        human->setMoving(data->dir);
        
        if (data->type == JoystickEx::ClickEventType::BEGAN ||
            data->type == JoystickEx::ClickEventType::MOVED )
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
        // TODO:
        // 1. 재장전 중일때 무기를 못 바꾼다.
        // 2. 재장전 중일때 프로그래스바 생성한다.
        // 3. 재장전 중일때 재장전버튼을 못누른다.
        InputReload inputCommand(_game->getPlayerPtr());
        inputCommand.execute();
        
        return true;
    }
    
    // callback funcs
    else if ( msg.msg == MessageType::PUSH_ITEM_TO_INVENTORY )
    {
        ItemAndOwner* data = static_cast<ItemAndOwner*>(msg.extraInfo);
        data->item->setOwner(data->owner);

        HumanBase* player = data->owner;

        int slot = player->addItem(data->item);
        if ( slot != -1 )
        {
            // 인벤토리에 공간이 있을 경우, 화면에서 지우고 소리를 출력한다.
            _game->removeEntity(data->item);
            
            if ( data->item->getEntityType() == EntityType::ITEM_AXE )
            {
                SoundSource s;
                s.fileName = "AxeGet.mp3";
                s.position = player->getWorldPosition();
                s.soundRange = 200.0f;
                _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
            }
            else
            {
                SoundSource s;
                s.fileName = "M16A2Enter.mp3";
                s.position = player->getWorldPosition();
                s.soundRange = 200.0f;
                _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
            }
        
            // ui 갱신
            _game->sendMessage(0.0, player, nullptr, MessageType::SYNC_INVENTORY_WEAPON_VIEW, nullptr);
        }
        else
        {
            // 아이템창이 부족함
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::USE_ITEM_FROM_INVENTORY )
    {
        int slot = *static_cast<int*>(msg.extraInfo);
        
        HumanBase* player = _game->getPlayerPtr();
        auto item = player->getInventoryData()->getItem(slot);
        player->useItem(static_cast<EntityType>(item->getEntityType()));
        
        // 무기정보UI의 무기를 갱신한다.
        _game->sendMessage(0.0, player, nullptr, MessageType::SYNC_INVENTORY_WEAPON_VIEW, nullptr);
        
        return true;
    }
    
    else if ( msg.msg == MessageType::ATTACK_BY_WEAPON )
    {
        auto owner = static_cast<HumanBase*>(msg.extraInfo);
        owner->attackByWeapon();
        
        return true;
    }
    
    else if ( msg.msg == MessageType::ATTACK_BY_FIST )
    {
        auto owner = static_cast<HumanBase*>(msg.extraInfo);
        owner->attackByFist();
        
        return true;
    }
    
    else if ( msg.msg == MessageType::PLAY_SOUND )
    {
        SoundSource* s =  static_cast<SoundSource*>(msg.extraInfo);
        float t = (1.0f - (s->position - _game->getCamera()->getCameraPos()).getLength() / s->soundRange) * s->volume;
        if ( t >= 0.0f )
        {
            experimental::AudioEngine::setVolume( experimental::AudioEngine::play2d("client/sounds/" + s->fileName), t);
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::PAUSE_GAME )
    {
        _game->pauseGame();
        
        return true;
    }
    
    else if ( msg.msg == MessageType::RESUME_GAME )
    {
        _game->resumeGame();
        
        return true;
    }
    
    return false;
}









