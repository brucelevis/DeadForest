#include <chrono>

#include "MessageDispatcher.hpp"
#include "MessageNode.hpp"
#include "Telegram.hpp"

namespace realtrick
{
    
    void MessageDispatcher::_discharge(const Telegram& msg)
    {
        bool ret = msg.receiver->handleMessage(msg);
        if( ret == false)
        {
            std::cout << "<MessageDispatcher:_discharge> Message is not handled. MessageType: [" << static_cast<int>(msg.msg) << "]" << std::endl;
        }
    }
    
    
    void MessageDispatcher::pushMessage(double delaySeconds,
                                        MessageNode* receiver,
                                        MessageNode* sender,
                                        MessageType type,
                                        void* extraInfo)
    {
        Telegram tel(delaySeconds, receiver, sender, type, extraInfo);
        if(tel.dispatchTime <= std::chrono::duration<double>::zero())
        {
            _discharge(tel);
        }
        else
        {
            std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
            tel.dispatchTime += curr;
            _priorityQ.insert(tel);
        }
    }
    
    
    void MessageDispatcher::dispatchDelayedMessages()
    {
        std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
        
        while (!_priorityQ.empty() &&
               (_priorityQ.begin()->dispatchTime < curr) &&
               (_priorityQ.begin()->dispatchTime >= std::chrono::duration<double>::zero()))
        {
            const Telegram& telegram = *_priorityQ.begin();
            _discharge(telegram);
            _priorityQ.erase(_priorityQ.begin());
        }
    }
    
    
    void MessageDispatcher::clearQueue()
    {
        _priorityQ.clear();
    }
    
}


