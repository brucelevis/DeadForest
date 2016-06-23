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
#include "ui/CocosGUI.h"
#include "GMXFile.hpp"
#include "MinimapLayer.hpp"
#include "Tiling.hpp"
#include "MessageNode.hpp"
#include "Telegram.hpp"

#define DUMMY_TILE_SIZE 4

class PaletteWindow;
class EntityBase;
class Location;
class Task;
class HistoryQueue;


class GMXLayer : public cocos2d::Node, public MessageNode
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
    
    void closeFile();
    
    std::string getFileName() const { return _file->fileName; }
    
    cocos2d::Vec2 getCenterViewPosition() const { return _centerViewPosition; }
    
    bool isContainPointInDiamond(const cocos2d::Vec2& diamondCenter, const cocos2d::Size& halfLen, const cocos2d::Vec2& p) const;
    
    std::pair<int, int> getFocusedTileIndex(float worldPosX, float worldPosY) const  { return getFocusedTileIndex(cocos2d::Vec2(worldPosX, worldPosY)); }
    
    std::pair<int, int> getFocusedTileIndex(const cocos2d::Vec2& worldPos) const;
    
    cocos2d::Vec2 getTilePosition(int x, int y) { return _tileImages[y][x]->getPosition(); }
    
    void drawSelectRegion(int x, int y);
    
    void disableSelectRegion();
    
    void setSelectTileImage(int tileType);
    
    bool handleMessage(const Telegram& msg) override;
    
    void putTile(int type, int x, int y);
    
    void redo();
    
    void undo();
    
    bool isRedo() const;
    
    bool isUndo() const;
    
    bool isChanged() const;
    
    // todo list
    
    void addEntity(EntityBase* entity) {}
    
    void addLocation(const cocos2d::Rect& rect, const std::string name) {}
    
private:
    
    std::vector<std::pair<int, int>> getNeighborTiles(int x, int y) const;
    
    int indexToNumber(int x, int y) const;
    
    std::pair<int, int> numberToIndex(int number) const;
    
    std::string getTileHeader(int type);
    
    std::string getTileHeader(const std::string& number);
    
    std::string getTileTail(const std::string& number);
    
private:
    
    GMXFile* _file;
    
    MinimapLayer* _minimap;
    
    cocos2d::ClippingRectangleNode* _clipNode;
    
    cocos2d::Node* _tileRoot;
    
    cocos2d::Node* _selectTileRoot;
    
    std::vector<cocos2d::Sprite*> _attachedTileImages;
    
    std::vector<std::vector<cocos2d::Sprite*>> _tileImages;
    
    PaletteWindow* _palette;
    
    cocos2d::Vec2 _centerViewPosition;
    
    cocos2d::DrawNode* _selectRegion;
    
    cocos2d::Vec2 _selectRegionPosition;

    int _oldSelectedTileType;
    
    HistoryQueue* _historyQueue;
    
    
};

#endif /* GMXLayer_hpp */









