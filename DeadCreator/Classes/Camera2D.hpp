//
//  Camera2D.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 29..
//
//

#ifndef Camera2D_hpp
#define Camera2D_hpp

#include "cocos2d.h"

class Camera2D : public cocos2d::Node
{
    
public:
    
    Camera2D() = default;
    
    virtual ~Camera2D() = default;
    
    CREATE_FUNC(Camera2D);
    
};

#endif /* Camera2D_hpp */
