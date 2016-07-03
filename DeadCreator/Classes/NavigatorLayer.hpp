//
//  NavigatorLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#pragma once

class GMXLayer2;

#include "TileBase.hpp"
#include "ImGuiLayer.h"

class NavigatorLayer : public cocos2d::Node
{
    
public:
    
    NavigatorLayer(GMXLayer2& gmxLayer);
    
    virtual ~NavigatorLayer();
    
    static NavigatorLayer* create(GMXLayer2& gmxLayer);
    
    virtual bool init() override;
    
    void showLayer(bool* opened);
    
    void setTile(int x, int y, const TileBase& tile);
    
private:
    
    GMXLayer2& _gmxLayer;
    
    cocos2d::Size _layerSize;
    cocos2d::Vec2 _layerPosition;
    cocos2d::Rect _boundingBoxPadding;
    
    cocos2d::Vec2 _centerViewParam;
    
    std::map<int, ImTextureID> _tileMarks;
    std::map<int, ImTextureID> _entityMarks;
    
    TileType _defaultTile;
    GLint _defaultTileTextureName;
    ImTextureID _dirtMarkTextureName;
    ImTextureID _grassMarkTextureName;
    ImTextureID _waterMarkTextureName;
    ImTextureID _hillMarkTextureName;
    
};









