//
//  TileImage.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 29..
//
//

#ifndef TileImage_hpp
#define TileImage_hpp

#include "cocos2d.h"

class TileImage : public cocos2d::Sprite
{
    
public:
    
    TileImage();
    
    virtual ~TileImage();
    
    static TileImage* create();
    
    static TileImage* create(const std::string& fileName);
    
    static TileImage* createWithFrameName(const std::string& frameName);
    
private:
    
};

#endif /* TileImage_hpp */
