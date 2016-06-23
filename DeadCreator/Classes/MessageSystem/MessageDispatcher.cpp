//
//  MessageDispatcher.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//
//

#include <chrono>

#include "MessageDispatcher.hpp"
#include "MessageNode.hpp"
#include "Telegram.hpp"

#include "cocos2d.h"


void MessageDispatcher::discharge(const Telegram& msg)
{
    bool ret = msg.receiver->handleMessage(msg);
    if( ret == false)
    {
        cocos2d::log("<MessageDispatcher:_discharge> Message is not handled. MessageType: [%d]", static_cast<int>(msg.msg));
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
        discharge(tel);
    }
    else
    {
        std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
        tel.dispatchTime += curr;
        _pq.push(tel);
    }
}


void MessageDispatcher::dispatchDelayedMessages()
{
    std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
    
    while (!_pq.empty() &&
           (_pq.top().dispatchTime < curr) &&
           (_pq.top().dispatchTime >= std::chrono::duration<double>::zero()))
    {
        const Telegram& telegram = _pq.top();
        discharge(telegram);
        _pq.pop();
    }
}


void MessageDispatcher::clearQueue()
{
   while( !_pq.empty() ) { _pq.pop(); }
}


MessageNode* MessageDispatcher::getNodePtr(int type) const
{
    auto iter = _nodes.find(type);
    if ( iter != _nodes.end() )
    {
        return iter->second;
    }
    return nullptr;
}


bool MessageDispatcher::registerNode(int nodeType, MessageNode* ptr)
{
    auto iter = _nodes.find(nodeType);
    if ( iter == _nodes.end() )
    {
        _nodes.insert( {nodeType, ptr} );
        return true;
    }
    return false;
}


bool MessageDispatcher::removeNode(int nodeType)
{
    auto iter = _nodes.find(nodeType);
    if ( iter != _nodes.end() )
    {
        _nodes.erase(iter);
        return true;
    }
    return false;
}










