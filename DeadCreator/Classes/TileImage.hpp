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

class GMXLayer2;

class TileImage : public cocos2d::Sprite
{
    
public:
    
    explicit TileImage(GMXLayer2& layer);
    
    virtual ~TileImage();
    
    static TileImage* create(GMXLayer2& layer);
    
    static TileImage* create(GMXLayer2& layer, const std::string& fileName);
    
    static TileImage* createWithFrameName(GMXLayer2& layer, const std::string& frameName);
    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
    
    cocos2d::Vec2 getWorldPosition() const { return _worldPosition; }
    
    void setWorldPosition(const cocos2d::Vec2& worldPos) { _worldPosition = worldPos; }
    
private:
    
    GMXLayer2& _gmxLayer;
    
    cocos2d::Vec2 _worldPosition;
    
};

#endif /* TileImage_hpp */
