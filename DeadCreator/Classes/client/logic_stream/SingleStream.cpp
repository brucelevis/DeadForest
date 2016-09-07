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
        auto player = _game->getPlayerPtr();
        auto equipedWeapon = player->getEquipedWeapon();
        if ( equipedWeapon && equipedWeapon->getEntityType() != EntityType::ITEM_AXE )
        {
            equipedWeapon->setNumOfLeftRounds( equipedWeapon->getNumOfLeftRounds() - 1 );
            _game->sendMessage(0.0, _game->getPlayerPtr(), nullptr, MessageType::SYNC_INVENTORY_WEAPON_VIEW, nullptr);
        }
        
        ItemAndOwner itemowner = *static_cast<ItemAndOwner*>(msg.extraInfo);
        EntityType type = static_cast<EntityType>(itemowner.item->getEntityType());
        HumanBase* owner = itemowner.owner;
        WeaponBase* weapon = static_cast<WeaponBase*>(itemowner.item);
        
        if ( type == EntityType::ITEM_AXE )
        {
            _game->addLog("<ItemAxe:attack> attack!");
            Vec2 worldPos = owner->getWorldPosition();
            
            // 엔티티들과의 충돌처리
            bool isHit = false;
            Vec2 shootAt = owner->getHeading();
            const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, shootAt, weapon->getRange());
            for (const auto &d : members)
            {
                if ( d == owner ) continue;
                
                if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                {
                    HumanBase* human = static_cast<HumanBase*>(d);
                    if( human->isAlive() && physics::intersect(Segment(worldPos,
                                                                       worldPos + owner->getHeading() * weapon->getRange()),
                                                               Circle(d->getWorldPosition(), human->getBoundingRadius())) )
                    {
                        ReceiverSenderDamage s;
                        s.damage = weapon->getDamage();
                        s.receiver = human;
                        s.sender = owner;
                        _game->sendMessage(0.0, human, owner, MessageType::HITTED_BY_AXE, &s);
                        
                        isHit = true;
                    }
                }
            }
            
            if ( isHit )
            {
                SoundSource s;
                s.fileName = "AxeHit" + _to_string(random(0,2)) + ".mp3";
                s.position = worldPos;
                s.soundRange = 200.0f;
                _game->sendMessage(0.0, owner, owner, MessageType::PLAY_SOUND, &s);
                _game->sendMessage(0.0, owner, owner, MessageType::HIT, nullptr);
            }
            else
            {
                SoundSource s;
                s.fileName = "AxeSwing" + _to_string(random(0,2)) + ".mp3";
                s.position = worldPos;
                s.soundRange = 200.0f;
                _game->sendMessage(0.0, owner, owner, MessageType::PLAY_SOUND, &s);
                _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
            }
        }
        else if ( type == EntityType::ITEM_M16A2 )
        {
            _game->addLog("<ItemM16A2:attack> attack!");
            std::vector<std::pair<float, EntityBase*>> closestIntersectPoint;
            Vec2 worldPos = owner->getWorldPosition();
            
            // 엔티티들과의 충돌처리
            Mat3 rotMat;
            rotMat.rotate(MATH_DEG_TO_RAD(random(-3.0f, 3.0f)));
            Vec2 shootAt = rotMat.getTransformedVector(owner->getHeading());
            Segment bulletRay = Segment(worldPos, worldPos + shootAt * weapon->getRange());
            
            const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, shootAt, weapon->getRange());
            for (const auto &d : members)
            {
                if ( d == owner ) continue;
                
                if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                {
                    HumanBase* human = static_cast<HumanBase*>(d);
                    if( human->isAlive() && physics::intersect(Segment(worldPos,
                                                                       worldPos + owner->getHeading() * weapon->getRange()), Circle(d->getWorldPosition(), 20.0f)) )
                    {
                        closestIntersectPoint.push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
                    }
                }
            }
            
            // 벽과의 충돌처리
            const std::vector<Polygon> walls = _game->getNeighborWalls(owner->getWorldPosition(), bulletRay);
            float dist;
            for( const auto& wall : walls )
            {
                for( int i = 0 ; i < wall.vertices.size() - 1 ; ++i)
                {
                    if ( physics::intersectGet(bulletRay, Segment(wall.vertices[i], wall.vertices[i + 1]), dist) )
                    {
                        closestIntersectPoint.push_back(std::make_pair(dist, nullptr));
                    }
                }
                if ( physics::intersectGet(bulletRay, Segment(wall.vertices.back(), wall.vertices.front()),dist) )
                {
                    closestIntersectPoint.push_back(std::make_pair(dist, nullptr));
                }
            }
            
            if ( closestIntersectPoint.empty() == false )
            {
                auto collider = *(min_element(std::begin(closestIntersectPoint), std::end(closestIntersectPoint), [](const std::pair<float, EntityBase*>& p1, const std::pair<float, EntityBase*>& p2) {
                    return p1.first < p2.first;
                }));
                
                // 최소거리에 충돌된 충돌체가 사람이면 처리. (벽일수도있음)
                if ( collider.second != nullptr )
                {
                    ReceiverSenderDamage d;
                    d.receiver = static_cast<HumanBase*>(collider.second);
                    d.sender = owner;
                    d.damage = weapon->getDamage();
                    _game->sendMessage(0.0, collider.second, owner, MessageType::HITTED_BY_GUN, &d);
                    
                    SoundSource s;
                    s.fileName = "GunShotAt" + _to_string(random(0, 1)) + ".mp3";
                    s.position = worldPos + shootAt * collider.first;
                    s.soundRange = 2000.0f;
                    _game->sendMessage(0.0, owner, owner, MessageType::PLAY_SOUND, &s);
                    
                    _game->sendMessage(0.0, owner, owner, MessageType::HIT, nullptr);
                }
                else
                {
                    _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
                }
            }
            else
            {
                _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
            }
        }
        else if ( type == EntityType::ITEM_M1897 )
        {
            int numOfShells = static_cast<ItemM1897*>(weapon)->getNumOfShells();
            
            _game->addLog("<ItemM1897:attack> attack!");
            vector<vector<pair<float, EntityBase*>>> closestIntersectPoint(numOfShells);
            Vec2 worldPos = owner->getWorldPosition();
            
            // 엔티티들과의 충돌처리
            Vec2 typicalHeading = owner->getHeading();
            Segment bulletRay = Segment(worldPos, worldPos + typicalHeading * weapon->getRange());
            vector<pair<float, Vec2>> shootAts(numOfShells);
            for(int i = 0 ; i < numOfShells ; ++ i)
            {
                Mat3 randomRotation;
                randomRotation.rotate(MATH_DEG_TO_RAD(random(-15.0f, 15.0f)));
                shootAts[i].first = random(weapon->getRange() / 3.0f, weapon->getRange());
                shootAts[i].second = randomRotation.getTransformedVector(typicalHeading);
            }
            
            const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, typicalHeading, weapon->getRange());
            for (const auto &d : members)
            {
                if ( d == owner ) continue;
                
                if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                {
                    for(int s = 0 ; s < shootAts.size() ; ++ s)
                    {
                        HumanBase* human = static_cast<HumanBase*>(d);
                        if( human->isAlive() && physics::intersect(Segment(worldPos, worldPos + shootAts[s].second * shootAts[s].first), Circle(d->getWorldPosition(), human->getBoundingRadius())) )
                        {
                            closestIntersectPoint[s].push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
                        }
                    }
                }
            }
            
            // 벽과의 충돌처리
            const std::vector<Polygon> walls = _game->getNeighborWalls(owner->getWorldPosition(), bulletRay);
            float dist;
            for( const auto& wall : walls )
            {
                for(int s  = 0 ; s < shootAts.size() ; ++ s)
                {
                    for( int i = 0 ; i < wall.vertices.size() - 1 ; ++i)
                    {
                        if ( physics::intersectGet(bulletRay, Segment(wall.vertices[i], wall.vertices[i + 1]), dist) )
                        {
                            closestIntersectPoint[s].push_back(std::make_pair(dist, nullptr));
                        }
                    }
                    if ( physics::intersectGet(bulletRay, Segment(wall.vertices.back(), wall.vertices.front()),dist) )
                    {
                        closestIntersectPoint[s].push_back(std::make_pair(dist, nullptr));
                    }
                }
            }
            
            bool isHit = false;
            for(int s = 0 ; s < shootAts.size() ; ++ s)
            {
                if ( closestIntersectPoint[s].empty() == false )
                {
                    auto collider = *(min_element(std::begin(closestIntersectPoint[s]), std::end(closestIntersectPoint[s]), [](const std::pair<float, EntityBase*>& p1, const std::pair<float, EntityBase*>& p2) {
                        return p1.first < p2.first;
                    }));
                    
                    // 최소거리에 충돌된 충돌체가 사람이면 처리. (벽일수도있음)
                    if ( collider.second != nullptr )
                    {
                        ReceiverSenderDamage s;
                        s.receiver = static_cast<HumanBase*>(collider.second);
                        s.sender = owner;
                        s.damage = weapon->getDamage();
                        _game->sendMessage(0.0, collider.second, owner, MessageType::HITTED_BY_GUN, &s);
                        
                        isHit = true;
                    }
                }
            }
            
            if ( isHit ) _game->sendMessage(0.0, owner, owner, MessageType::HIT, nullptr);
            else _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
        }
        else if ( type == EntityType::ITEM_GLOCK17 )
        {
            _game->addLog("<ItemGlock17:attack> attack!");
            std::vector<std::pair<float, EntityBase*>> closestIntersectPoint;
            Vec2 worldPos = owner->getWorldPosition();
            
            // 엔티티들과의 충돌처리
            Mat3 rotMat;
            rotMat.rotate(MATH_DEG_TO_RAD(random(-3.0f, 3.0f)));
            Vec2 shootAt = rotMat.getTransformedVector(owner->getHeading());
            Segment bulletRay = Segment(worldPos, worldPos + shootAt * weapon->getRange());
            
            const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, shootAt, weapon->getRange());
            for (const auto &d : members)
            {
                if ( d == owner ) continue;
                
                if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                {
                    HumanBase* human = static_cast<HumanBase*>(d);
                    if( human->isAlive() && physics::intersect(Segment(worldPos,
                                                                       worldPos + owner->getHeading() * weapon->getRange()), Circle(d->getWorldPosition(), 20.0f)) )
                    {
                        closestIntersectPoint.push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
                    }
                }
            }
            
            // 벽과의 충돌처리
            const std::vector<Polygon> walls = _game->getNeighborWalls(owner->getWorldPosition(), bulletRay);
            float dist;
            for( const auto& wall : walls )
            {
                for( int i = 0 ; i < wall.vertices.size() - 1 ; ++i)
                {
                    if ( physics::intersectGet(bulletRay, Segment(wall.vertices[i], wall.vertices[i + 1]), dist) )
                    {
                        closestIntersectPoint.push_back(std::make_pair(dist, nullptr));
                    }
                }
                if ( physics::intersectGet(bulletRay, Segment(wall.vertices.back(), wall.vertices.front()),dist) )
                {
                    closestIntersectPoint.push_back(std::make_pair(dist, nullptr));
                }
            }
            
            if ( closestIntersectPoint.empty() == false )
            {
                auto collider = *(min_element(std::begin(closestIntersectPoint), std::end(closestIntersectPoint), [](const std::pair<float, EntityBase*>& p1, const std::pair<float, EntityBase*>& p2) {
                    return p1.first < p2.first;
                }));
                
                // 최소거리에 충돌된 충돌체가 사람이면 처리. (벽일수도있음)
                if ( collider.second != nullptr )
                {
                    ReceiverSenderDamage d;
                    d.receiver = static_cast<HumanBase*>(collider.second);
                    d.sender = owner;
                    d.damage = weapon->getDamage();
                    _game->sendMessage(0.0, collider.second, owner, MessageType::HITTED_BY_GUN, &d);
                    
                    SoundSource s;
                    s.fileName = "GunShotAt" + _to_string(random(0, 1)) + ".mp3";
                    s.position = worldPos + shootAt * collider.first;
                    s.soundRange = 2000.0f;
                    _game->sendMessage(0.0, owner, owner, MessageType::PLAY_SOUND, &s);
                    
                    _game->sendMessage(0.0, owner, owner, MessageType::HIT, nullptr);
                }
                else
                {
                    _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
                }
            }
            else
            {
                _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
            }
        }
        
        return true;
    }
    
    return false;
}









