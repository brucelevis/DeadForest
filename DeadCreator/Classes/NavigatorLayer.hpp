//
//  NavigatorLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#pragma once

#include "MutableUiBase.hpp"

class EditScene2;
class GMXLayer2;

class NavigatorLayer : public MutableUiBase, public cocos2d::Node
{
    
public:
    
    NavigatorLayer(EditScene2& imguiLayer, GMXLayer2& gmxLayer);
    
    virtual ~NavigatorLayer();
    
    static NavigatorLayer* create(EditScene2& imguiLayer, GMXLayer2& gmxLayer);
    
    virtual bool init() override;
    
    void showLayer(bool* opened);
    
private:
    
    EditScene2& _imguiLayer;
    GMXLayer2& _gmxLayer;
    
    bool _isShowWindow = true;
    cocos2d::Size _layerSize;
    cocos2d::Vec2 _layerPosition;
    cocos2d::Vec2 _centerViewParam;
    cocos2d::Vec2 _centerViewPosition;
    cocos2d::Rect _boundingBoxPadding;
    cocos2d::Vec2 _mousePosInCanvas;
    
};



