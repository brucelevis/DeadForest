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
#include "imgui.h"

class GMXLayer;

class MinimapLayer : public cocos2d::Node
{
    
public:
    
    MinimapLayer();
    
    virtual ~MinimapLayer();
    
    bool init(const cocos2d::Size& layerSize);
    
    static MinimapLayer* create(const cocos2d::Size& layerSize);
    
    void setFocusWindowSize(float width, float height) { setFocusWindowSize(cocos2d::Size(width, height)); }
    
    void setFocusWindowSize(const cocos2d::Size focusWindowSize) { _focusWindowSize = focusWindowSize; }
    
    cocos2d::Size getFocusWindowSize() const { return _focusWindowSize; }
    
    void onCenterView(float x, float y) { onCenterView(cocos2d::Vec2(x,y)); }
    
    void onCenterView(const cocos2d::Vec2& params);
    
    void setDefaultImage(const std::string& fileName);
    
    void setGMXLayer(GMXLayer* layer);
    
    void onResize();
    
    void disableFocusWindow() { _focusWindowRenderer->clear(); }
    
private:
    
    GMXLayer* _gmxLayer;
    
    cocos2d::DrawNode* _focusWindowRenderer;
    
    cocos2d::Size _focusWindowSize;
    
    cocos2d::Sprite* _defaultImage;
    
};

#endif /* MinimapLayer_hpp */









