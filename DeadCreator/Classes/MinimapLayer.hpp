//
//  MinimapLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#ifndef MinimapLayer_hpp
#define MinimapLayer_hpp

#include "cocos2d.h"

class MinimapLayer : public cocos2d::Node
{
    
public:
    
    MinimapLayer();
    
    virtual ~MinimapLayer();
    
    virtual bool init() override;
    
    static MinimapLayer* create();
    
private:
    
    cocos2d::DrawNode* _focusWindowRenderer;
    
};

#endif /* MinimapLayer_hpp */
