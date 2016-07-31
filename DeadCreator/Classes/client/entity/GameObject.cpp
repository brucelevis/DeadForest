//
//  GameObject.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 3..
//
//

#include "GameObject.hpp"
#include "Game.hpp"
#include "Camera2D.hpp"
using namespace realtrick::client;

GameObject::GameObject(Game* game) :
_game(game),
_familyMask(0),
_entityType(0),
_boundingRadius(0.0f)
{
    ADD_FAMILY_MASK(_familyMask, ENTITY_BASE);
}


GameObject::GameObject(const GameObject& rhs)
{
    _game = rhs._game;
    _familyMask = rhs._familyMask;
    _entityType = rhs._entityType;
    _tag = rhs._tag;
}








