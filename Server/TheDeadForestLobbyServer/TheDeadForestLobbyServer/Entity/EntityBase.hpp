#pragma once

#include "Vec2.h"
#include "EntityType.hpp"
#include "MessageNode.hpp"
#include "MessageDispatcher.hpp"

namespace realtrick
{

	class Telegram;
    class GameManager;
    
    class EntityBase : public MessageNode
    {

    public:
        
        EntityBase(const EntityBase& rhs);
        
        EntityBase(GameManager* gameMgr);
        
        virtual ~EntityBase()
        {
            _gameMgr = nullptr;
        }
        
        GameManager*    getGameManager() const      { return _gameMgr; }

		void            setTag(int tag) { _tag = tag; }

		int             getTag() const { return _tag; }
        
        int             getEntityType() const       { return _entityType; }
        
        void            setEntityType(int type)     { _entityType = type; }
        
        Vec2			getWorldPosition() const    { return _worldPosition; }
        
        void            setWorldPosition(const Vec2& pos)  { _worldPosition = pos; }
        
        virtual bool    handleMessage(const Telegram& msg) override { return false; }
        
        virtual void    update(float dt) {}
        
        virtual void    enableNormal(bool enable) {}
        
    protected:
        
        GameManager*            _gameMgr;
        
        int                     _entityType;

		int                     _tag;

        Vec2					_worldPosition;
        
    };    
}









