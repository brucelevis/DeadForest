
#include "HumanFistStates.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"
#include "MessageDispatcher.hpp"
#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "GameManager.hpp"

namespace realtrick
{
    
    //
    // HumanFistIdleLoop
    //
    void HumanFistIdleLoop::enter(EntityHuman* human)
    {
    }
    
    void HumanFistIdleLoop::execute(EntityHuman* human)
    {        
    }
    
    void HumanFistIdleLoop::exit(EntityHuman* human)
    {
        human->getBodyAnimator()->clearFrameQueue();
    }
    
    bool HumanFistIdleLoop::onMessage(EntityHuman* human, const Telegram& msg)
    {
        return false;
    }
    
    
    
    //
    // HumanFistOut
    //
    void HumanFistOut::enter(EntityHuman* human)
    {
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanFistOut::getInstance());
    }
    
    void HumanFistOut::execute(EntityHuman* human)
    {
    }
    
    void HumanFistOut::exit(EntityHuman* human)
    {
        human->getBodyAnimator()->clearFrameQueue();
    }
    
    bool HumanFistOut::onMessage(EntityHuman* human, const Telegram& msg) { return false; }
    
    
    
    //
    // HumanFistIn
    //
    void HumanFistIn::enter(EntityHuman* human)
    {
    }
    
    void HumanFistIn::execute(EntityHuman* human)
    {
    }
    
    void HumanFistIn::exit(EntityHuman* human)
    {
        human->getBodyAnimator()->clearFrameQueue();
    }
    
    bool HumanFistIn::onMessage(EntityHuman* human, const Telegram& msg) { return false; }
    
    
    
    //
    // HumanFistMoveLoop
    //
    void HumanFistMoveLoop::enter(EntityHuman* human)
    {
        human->getBodyAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 0, 5);
        human->setRunStats(true);
    }
    
    void HumanFistMoveLoop::execute(EntityHuman* human)
    {        
    }
    
    void HumanFistMoveLoop::exit(EntityHuman* human)
    {
        human->setRunStats(false);
        human->getBodyAnimator()->clearFrameQueue();
    }
    
    bool HumanFistMoveLoop::onMessage(EntityHuman* human, const Telegram& msg) { return false; }
    
    //
    // HumanFistAttack
    //
    void HumanFistAttack::enter(EntityHuman* human)
    {
    }
    
    void HumanFistAttack::execute(EntityHuman* human)
    {
    }
    
    void HumanFistAttack::exit(EntityHuman* human)
    {
        human->getBodyAnimator()->clearFrameQueue();
    }
    
    bool HumanFistAttack::onMessage(EntityHuman* human, const Telegram& msg) { return false; }
}









