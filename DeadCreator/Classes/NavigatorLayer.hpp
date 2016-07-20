//
//  NavigatorLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#pragma once

#include "TileBase.hpp"
#include "ImGuiLayer.h"

namespace realtrick
{
    
    class GMXLayer;
    class EditorEntity;
    
    class NavigatorLayer : public cocos2d::Node
    {
        
    public:
        
        NavigatorLayer(GMXLayer& gmxLayer);
        virtual ~NavigatorLayer();
        static NavigatorLayer* create(GMXLayer& gmxLayer);
        virtual bool init() override;
        
        void showLayer(bool* opened);
        void setTile(int x, int y, const TileBase& tile);
        
        void addEntity(EditorEntity* ent);
        void eraseEntity(int id);
        
    private:
        
        GMXLayer& _gmxLayer;
        
        cocos2d::Size _layerSize;
        cocos2d::Vec2 _layerPosition;
        cocos2d::Rect _boundingBoxPadding;
        
        cocos2d::Vec2 _centerViewParam;
        
        std::map<int, ImTextureID> _tileMarks;
        std::map<int, EditorEntity*> _entityMarks;
        
        EditorTileType _defaultTile;
        GLint _defaultTileTextureName;
        
    };
    
}









