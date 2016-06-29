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
#include "MutableUiBase.hpp"
#include "CellSpacePartition.hpp"
#include "TileImage.hpp"
#include "Camera2D.hpp"

#define DUMMY_TILE_SIZE 4

class GMXFile;
class PaletteLayer;
class NavigatorLayer;
class EditScene2;

class GMXLayer2 : public MutableUiBase, public cocos2d::Layer
{
    
public:
    
    explicit GMXLayer2(EditScene2& _imguiLayer, GMXFile& file);
    
    virtual ~GMXLayer2() = default;
    
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    static GMXLayer2* create(EditScene2& imguiLayer, GMXFile& file);
    
    virtual bool init() override;

    virtual void update(float dt) override;
    
    virtual void setTile(int x, int y, TileBase* tile) override;
    
    void showWindow();
    
    void updateChunk(const cocos2d::Vec2& pivot);
    
    bool isUpdateChunk(const cocos2d::Vec2& newPos, const cocos2d::Vec2& oldPos);
    
    void setLayerPosition(const cocos2d::Vec2& pos) { _layerPosition = pos; }
    
    cocos2d::Vec2 getTileRootWorldPosition() const { return _tileRootWorldPosition; }
    
    void initFile();
    
    bool& isShowPalette() { return _isShowPalette; }
    
    bool& isShowNavigator() { return _isShowNavigator; }
    
private:
    
    EditScene2& _imguiLayer;
    GMXFile& _file;
    
    cocos2d::DrawNode* _worldDebugNode;
    cocos2d::DrawNode* _localDebugNode;
    cocos2d::ClippingRectangleNode* _clipNode;
    cocos2d::Size _visibleSize;
    cocos2d::Size _layerSize;
    cocos2d::Vec2 _layerPosition;
    cocos2d::Vec2 _centerViewParam;
    cocos2d::Vec2 _centerViewPosition;
    cocos2d::Vec2 _tileRootWorldPosition;
    cocos2d::Vec2 _cameraDirection;
    Camera2D* _camera;
    float _windowSpeed;
    cocos2d::Node* _tileRoot;
    cocos2d::Node* _rootNode;
    
    CellSpacePartition* _cellSpacePartition;
    std::vector< std::vector<TileImage*> > _tileImages;
    int _viewX;
    int _viewY;
    
    bool _isShowWindow = true;
    bool _isKeyDown[256];
    
    PaletteLayer* _paletteLayer;
    bool _isShowPalette = true;
    
    NavigatorLayer* _navigatorLayer;
    bool _isShowNavigator = true;
    
};

#endif /* GMXLayer2_hpp */









