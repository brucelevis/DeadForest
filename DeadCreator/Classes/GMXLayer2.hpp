//
//  GMXLayer2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#ifndef GMXLayer2_hpp
#define GMXLayer2_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CellSpacePartition.hpp"
#include "TileImage.hpp"
#include "Camera2D.hpp"
#include "GMXFile.hpp"
#include "TileBase.hpp"
#include "CommandQueue.hpp"

#define DUMMY_TILE_SIZE 4

class PaletteLayer;
class NavigatorLayer;
class EditScene2;
class CommandBase;
class TileToolCommand;

class GMXLayer2 : public cocos2d::Layer
{
    
public:
    
    explicit GMXLayer2(EditScene2& _imguiLayer, GMXFile& file);
    
    virtual ~GMXLayer2();
    
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    static GMXLayer2* create(EditScene2& imguiLayer, GMXFile& file);
    
    virtual bool init() override;

    virtual void update(float dt) override;
    
    const GMXFile& getFile() const { return _file; }
    
    void setTile(int x, int y, const TileBase& tile, bool isExecCommand = false);
    
    void showWindow();
    
    void updateChunk(const cocos2d::Vec2& pivot);
    
    bool isUpdateChunk(const cocos2d::Vec2& newPos, const cocos2d::Vec2& oldPos);
    
    void putTile(TileType type, int x, int y);
    
    void setLayerPosition(const cocos2d::Vec2& pos) { _layerPosition = pos; }
    
    cocos2d::Vec2 getTileRootWorldPosition() const { return _tileRootWorldPosition; }
    
    cocos2d::Vec2 getCameraPosition() const { return _camera->getPosition(); }
    
    cocos2d::Vec2 getMousePosInWorld() const { return _mousePosInWorld; }
    
    cocos2d::Vec2 getCenterViewParameter() const { return _centerViewParam; }
    
    void setCenterViewParameter(const cocos2d::Vec2& p);
    
    cocos2d::Size getCanvasSize() const { return _canvasSize; }
    
    cocos2d::Size getLayerSize() const { return _layerSize; }
    
    cocos2d::Size getWorldSize() const { return _file.worldSize; }
    
    const TileBase& getTile(int x, int y) const { return _tiles[y][x]; }
    
    void initFile();
    
    bool& isShowPalette() { return _isShowPalette; }
    
    bool& isShowNavigator() { return _isShowNavigator; }
    
private:
    
    EditScene2& _imguiLayer;
    GMXFile& _file;
    
    cocos2d::DrawNode* _worldDebugNode;
    cocos2d::DrawNode* _localDebugNode;
    cocos2d::DrawNode* _hoveredTileRegion;
    cocos2d::ClippingRectangleNode* _clipNode;
    cocos2d::Size _visibleSize;
    cocos2d::Size _canvasSize;
    cocos2d::Size _layerSize;
    cocos2d::Vec2 _layerPosition;
    cocos2d::Vec2 _centerViewParam;
    cocos2d::Vec2 _tileRootWorldPosition;
    cocos2d::Vec2 _cameraDirection;
    Camera2D* _camera;
    float _windowSpeed;
    cocos2d::Node* _tileRoot;
    cocos2d::Node* _rootNode;
    
    CellSpacePartition* _cellSpacePartition;
    std::vector<std::vector<TileBase>> _tiles;
    std::vector< std::vector<TileImage*> > _tileImages;
    std::vector< std::vector<cocos2d::ui::Text*> > _tileIndices;
    int _viewX;
    int _viewY;
    
    bool _isShowWindow = true;
    bool _isKeyDown[256];
    
    cocos2d::Vec2 _mousePosInWorld;
    cocos2d::Vec2 _mousePosInCanvas;
    
    PaletteLayer* _paletteLayer;
    bool _isShowPalette = true;
    
    NavigatorLayer* _navigatorLayer;
    bool _isShowNavigator = true;
    
    CommandQueue _commandQueue;
    TileToolCommand* _tileToolCommand;
    
};

#endif /* GMXLayer2_hpp */









