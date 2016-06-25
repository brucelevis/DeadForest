//
//  TestScene.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 26..
//
//

#ifndef TestScene_hpp
#define TestScene_hpp

#include "cocos2d.h"

class TestScene : public cocos2d::Layer
{
    
public:
    
    CREATE_FUNC(TestScene);
    
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    
};

#endif /* TestScene_hpp */
