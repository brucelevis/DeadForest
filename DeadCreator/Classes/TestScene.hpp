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

#include "GMXLayer2.hpp"
#include "CellSpacePartition.hpp"

class TestScene : public cocos2d::Layer
{
    
public:
    
    CREATE_FUNC(TestScene);
    
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    
private:
    
    CellSpacePartition* _cellSpace;
    
    GMXLayer2* _layer;
    
};

#endif /* TestScene_hpp */
