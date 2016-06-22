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

#define DUMMY_TILE_SIZE 4

class EntityBase;
class Location;
class Task;

class GMXLayer : public cocos2d::Node
{
    
public:
    
    GMXLayer();
    
    virtual ~GMXLayer();
    
    static GMXLayer* create();
    
    virtual bool init() override;
    
    cocos2d::Size getWorldSize() const { if ( _file ) return _file->worldSize; return cocos2d::Size::ZERO; }
    
    void onCenterView(float x, float y) { onCenterView(cocos2d::Vec2(x,y)); }
    
    void onCenterView(const cocos2d::Vec2& params);
    
    cocos2d::Rect getClippingRegion() const { return _clipNode->getClippingRegion(); }
    
    void setClippingRegion(const cocos2d::Rect& rect) { _clipNode->setClippingRegion(rect); }

    void setMinimapPtr(MinimapLayer* minimap) { _minimap = minimap; }
    
    void onResize();
    
    void openFile(GMXFile* file);
    
    bool isOpened() const { return _isOpened; }
    
    // todo list
    
    int getFocusedTileIndex(float mouseX, float mouseY) const  { return getFocusedTileIndex(cocos2d::Vec2(mouseX, mouseY)); }
    
    int getFocusedTileIndex(const cocos2d::Vec2& mousePos) const { return 0; }
    
    void putTile(int type, int index);
    
    void addEntity(EntityBase* entity) {}
    
    void addLocation(const cocos2d::Rect& rect, const std::string name) {}
    
private:
    
    GMXFile* _file;
    
    MinimapLayer* _minimap;
    
    cocos2d::ClippingRectangleNode* _clipNode;
    
    cocos2d::Node* _tileRoot;
    
    std::vector<std::vector<std::string>> _tileName;
    
    std::vector<std::vector<cocos2d::Sprite*>> _tileImages;
    
    bool _isOpened;
    
    // todo
    
    std::vector<Task*> _taskHistory;
    
    bool _isChanged;
    
};

#endif /* GMXLayer_hpp */









