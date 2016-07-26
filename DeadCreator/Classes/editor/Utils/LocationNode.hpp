//
//  LocationNode.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 14..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class GMXLayer;
    class GMXFile;
    
    class LocationNode : public cocos2d::Node
    {
        
    private:
        
        float dummySpace = 10.0f;
        
    public:
        
        explicit LocationNode(GMXLayer& layer);
        virtual ~LocationNode();
        
        static LocationNode* create(GMXLayer& layer);
        virtual bool init() override;
        
        void setLocationSize(int x, int y);
        std::pair<int, int> getLocationSize() const { return { _sizeX, _sizeY }; }
        
        void updateRects();
        void update(const cocos2d::Vec2& mouseWorldPosition);
        
        void renderOutline();
        
        void setPositionFromIndex(int x, int y);
        void setPositionFromWorldPosition(const cocos2d::Vec2& pos);
        void setPositionFromWorldPosition(float x, float y);
        
        cocos2d::Rect getAABBBox() const;
        
        bool isSelected() const { return _isSelected; }
        void setSelected(bool selected) { _isSelected = selected; if ( !selected ) clearLocationGrabFlags(); }
        
        int getLocationZOrder() const { return _locationZOrder; }
        void setLocationZOrder(int zOrder) { _locationZOrder = zOrder; }
        
        void setLocationName(const std::string& name) {_locationName->setString(name); }
        std::string getLocationName() const { return _locationName->getString(); }
        
        const GMXLayer& getGMXLayer() const { return _gmxLayer; }
        
    private:
        
        virtual void setPosition(float x, float y) override;
        virtual void setPosition(const cocos2d::Vec2& pos) override;
        
        std::function<bool(int offsetX, int offsetY)> resizeFuncs[9];
        void clearLocationGrabFlags();
        bool isOnlyGrabbedLocation(int index);
        
    private:
        
        GMXLayer& _gmxLayer;
        const GMXFile& _file;
        
        cocos2d::Rect _aabb;
        cocos2d::Rect _rects[9];
        cocos2d::Color4F _colors[9];
        
        cocos2d::Node* dummy;
        cocos2d::ClippingRectangleNode* _clipNode;
        cocos2d::DrawNode* _aabbNode;
        cocos2d::ui::Text* _locationName;
        
        int _sizeX = 1;
        int _sizeY = 1;
        
        bool _isLocationGrabbed[9];
        
        std::pair<int, int> _oldRectangleIndex = {0, 0};
        int _locationZOrder = 0;
        bool _isSelected = false;
    
    };
    
}









