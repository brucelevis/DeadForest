//
//  WeaponBase.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "WeaponBase.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "HumanOwnedAnimations.hpp"
#include "InventoryData.hpp"
using namespace cocos2d;
using namespace realtrick::client;

WeaponBase::WeaponBase(Game* game) : ItemBase(game),
_numOfLeftRounds(0),
_maxRounds(0),
_damage(0),
_range(0),
_delay(0.0f),
_isReadyToAttack(true),
_reservedBullet(0)
{
    ADD_FAMILY_MASK(_familyMask, WEAPON_BASE);
}


WeaponBase::~WeaponBase()
{
}


WeaponBase::WeaponBase(const WeaponBase& rhs) : ItemBase(rhs)
{
    _numOfLeftRounds = rhs._numOfLeftRounds;
    _maxRounds = rhs._maxRounds;
    _damage = rhs._damage;
    _range = rhs._range;
    _delay = rhs._delay;
    _isReadyToAttack = rhs._isReadyToAttack;
    _bulletType = rhs._bulletType;
    
    // 재장전될 총알의 갯수를 백업하는 용도이므로, 이 변수는 복사하지 않아야한다.
    _reservedBullet = 0;
}


void WeaponBase::use()
{
    auto equipedWeapon = _owner->getEquipedWeapon();
    if( equipedWeapon == nullptr )
    {
        // 주먹일 때, 주먹을 집어넣고 무기를 세팅한다.
        _owner->getFSM()->changeState(&HumanFistIn::getInstance());
        _owner->setEquipedWeapon(this);
    }
    else if ( equipedWeapon == this )
    {
        // 장착 무기를 nullptr(맨손)으로 만들고 무기를 집어넣음;
        _owner->setEquipedWeapon(nullptr);
        this->inWeapon();
    }
    else
    {
        // 장착 중인 무기를 집어넣고, 장착 무기를 바꾼다.
        _owner->getEquipedWeapon()->inWeapon();
        _owner->setEquipedWeapon(this);
    }
}


void WeaponBase::reload()
{
    int leftRounds = getNumOfLeftRounds();
    int maxRounds = getMaxRounds();

	int offset = getReloadedBulletOnce();
    int ownedRound = 0;
    auto item = _owner->getInventoryData()->getItemType(getBulletType());
    if ( item ) ownedRound = item->getAmount();
	int bulletNum = std::min(ownedRound, offset);

    if ( leftRounds != maxRounds )
    {
        if ( ownedRound != 0 )
        {
			setReservedBullets(bulletNum);
            _game->sendMessage(0.0, _owner, this, MessageType::RELOAD_WEAPON, nullptr);
            
            double animatedTime = _owner->getAnimator()->getCurrAnimation()->getMaxFrame() * _owner->getAnimator()->getCurrAnimation()->getFrameSwapTime();
            _game->pushLogic(animatedTime, MessageType::RELOAD_COMPLETE, nullptr);
        }
        else
        {
            _game->addLog(StringUtils::format("<WeaponBase::reload> has not bullets."));
            experimental::AudioEngine::play2d("client/sounds/reload_fail.mp3");
        }
    }
    else
    {
        _game->addLog(StringUtils::format("<WeaponBase::reload> bullet is already full."));
        experimental::AudioEngine::play2d("client/sounds/reload_fail.mp3");
    }
}


void WeaponBase::attack()
{
    dropCartiridges();
    
    // 공격 흐름 분리
    ItemAndOwner item_owner;
    item_owner.item = this;
    item_owner.owner = _owner;
    _game->pushLogic(0.0, MessageType::ATTACK_BY_WEAPON, &item_owner);
}









