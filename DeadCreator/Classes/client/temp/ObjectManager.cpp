//
//  ObjectManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "ObjectManager.hpp"
#include "GameObject2.hpp"
using namespace realtrick::client;


bool ObjectManager::initWithResource(GameResource* res)
{
    return true;
}


void ObjectManager::addObject(GameObject2* object)
{
    int id = getNextValidID();
    object->setID(id);
    _objects.insert( {id, object} );
}


bool ObjectManager::removeObject(int id)
{
    auto iter = _objects.find(id);
    if ( iter != _objects.end() )
    {
        _objects.erase(iter);
        return true;
    }
    return false;
}


GameObject2* ObjectManager::getEntityFromID(int id)
{
    auto iter = _objects.find(id);
    if( iter != _objects.end() )
        return iter->second;
    else
        return nullptr;
}









