//
//  Human.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 1..
//
//

#include "Human.hpp"
#include "Components.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;


Human::Human(Game* game, int id) :
GameObject2(game, id)
{
}

Human::~Human()
{
}


Human* Human::create(Game* game, int id)
{
    auto ret = new (std::nothrow) Human(game, id);
    if ( ret && ret->init() )
    {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool Human::init()
{
    if ( !GameObject2::init() )
        return false;
    
    RenderableComponent* render = RenderableComponent::create(this, "HumanFistIdleLoop0.png",
                                                              cocos2d::ui::Widget::TextureResType::PLIST);
    
    this->addComponent(render);
    
    return true;
}









