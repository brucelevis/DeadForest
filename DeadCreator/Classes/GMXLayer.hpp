//
//  GMXLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#ifndef GMXLayer_hpp
#define GMXLayer_hpp

#include "cocos2d.h"
#include "GMXFile.hpp"
#include "MinimapLayer.hpp"

class GMXFileManager;
class EntityBase;
class Location;

class GMXLayer : public cocos2d::Node
{
    
public:
    
    GMXLayer();
    
    virtual ~GMXLayer();
    
    static GMXLayer* create(const std::string& fileName);
    
    bool init(const std::string& fileName);
    
    cocos2d::Size getWorldSize() const { return _file.worldSize; }
    
    // todo list
    
    int getFocusedTileIndex(float mouseX, float mouseY) const  { return getFocusedTileIndex(cocos2d::Vec2(mouseX, mouseY)); }
    
    int getFocusedTileIndex(const cocos2d::Vec2& mousePos) const { return 0; }
    
    void putTile(int type, int index) {}
    
    void addEntity(EntityBase* entity) {}
    
    void addLocation(const cocos2d::Rect& rect, const std::string name) {}
    
    void setViewRect(const cocos2d::Rect& rect) {}
    
    void centerView(float x, float y) { centerView(cocos2d::Vec2(x,y)); }
    
    void centerView(const cocos2d::Vec2& params);
    
    cocos2d::Rect getClippingRegion() const { return _clipNode->getClippingRegion(); }
    
    void setClippingRegion(const cocos2d::Rect& rect) { _clipNode->setClippingRegion(rect); }
    
    cocos2d::Vec2 getCenterViewParams() const { return _centerViewParams; }
    
    void setMinimapPtr(MinimapLayer* minimap) { _minimap = minimap; }
    
    void onResize();
    
private:
    
    GMXFile _file;
    
    GMXFileManager* _gmxFileManager;
    
    MinimapLayer* _minimap;
    
    cocos2d::ClippingRectangleNode* _clipNode;
    
    cocos2d::Node* _tileRoot;
    
    std::vector<std::vector<cocos2d::Sprite*>> _tileImages;
    
    cocos2d::Vec2 _centerViewParams;
    
};

#endif /* GMXLayer_hpp */









