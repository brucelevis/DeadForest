
#include "EntityBase.hpp"
#include "GameManager.hpp"

using namespace realtrick;

EntityBase::EntityBase(GameManager* gameMgr) :
_gameMgr(gameMgr),
_entityType(0)
{
}
    
EntityBase::EntityBase(const EntityBase& rhs)
{
    _gameMgr = rhs._gameMgr;
    _entityType = rhs._entityType;
    _tag = rhs._tag;
}
