#pragma once

#include "EntityBase.hpp"
#include "Physics.hpp"

namespace realtrick
{
    
    class GameManager;
    
    class DynamicEntity : public EntityBase
    {
        
    public:
        
        DynamicEntity(GameManager* gameMgr);
        
        virtual ~DynamicEntity();
        
        Vec2 getHeading() const										{ return _heading; }
        
        inline void setHeading(const Vec2 heading);
        
        Vec2 getTargetHeading() const								{ return _targetHeading; }
        
        void setTargetHeading(const Vec2& target)					{ _targetHeading = target; }
        
        Vec2 getLeft() const										{ return _heading.getPerp(); }
        
        Vec2 getRight() const										{ return _heading.getRPerp(); }
        
        Vec2 getMoving() const										{ return _moving; }
        
        void setMoving(const Vec2& moving)							{ _moving = moving; }
        
        Vec2 getVelocity() const									{ return _velocity; }
        
        void setVelocity(const Vec2 velocity)						{ _velocity = velocity; }
        
        Vec2 getForce() const										{ return _force; }
        
        void setForce(const Vec2 force)								{ _force = force; }
        
        Vec2 getAcceleration() const								{ return _acceleration; }
        
        void setAcceleration(const Vec2 ac)							{ _acceleration = ac; }
        
        float getMass() const                                       { return _mass; }
        
        void setMass(float mass)                                    { _mass = mass; }
        
        float getMaxSpeed() const                                   { return _maxSpeed; }
        
        void setMaxSpeed(float maxSpeed)                            { _maxSpeed = maxSpeed; }
        
        float getTurnSpeed() const                                  { return _turnSpeed; }
        
        void setTurnSpeed(float speed)                              { _turnSpeed = speed; }
        
        float getBoundingRadius() const                             { return _boundingRadius; }
        
        void setBoundingRadius(float r)                             { _boundingRadius = r; }
        
        virtual bool handleMessage(const Telegram& msg) override    { return false; }
        
        virtual void update(float dt) override {}
        
        virtual void moveEntity(float dt) final;
        
        virtual bool isIntersectOther(const Vec2& futurePosition, EntityBase* other) { return true; }
        
        virtual bool isIntersectWall(const Vec2& futurePosition, const geometry::Polygon& wall) { return true; }
        
        virtual void rotateEntity(float d) {}
        
    protected:
        
        Vec2						_heading;
    
        Vec2						_targetHeading;
    
        Vec2						_moving;
    
        Vec2						_left;
    
        Vec2						_right;
    
        Vec2						_velocity;
    
        Vec2						_force;
    
        Vec2						_acceleration;
        
        float                       _mass;
        
        float                       _maxSpeed;
        
        float                       _turnSpeed;
        
        float                       _boundingRadius;
        
        float                       _speed;
        
    };
    
    inline void DynamicEntity::setHeading(const Vec2 heading)
    {
        _heading = heading;
        _heading.normalize();
    }

}






