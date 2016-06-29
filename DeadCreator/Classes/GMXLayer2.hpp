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
#include "ImGuiLayer.h"
#include "CellSpacePartition.hpp"
#include "TileImage.hpp"
#include "Camera2D.hpp"

#define DUMMY_TILE_SIZE 4

class GMXFile;

class GMXLayer2 : public MutableUiBase, public ImGuiLayer
{
    
public:
    
    explicit GMXLayer2(GMXFile& file);
    
    virtual ~GMXLayer2() = default;
    
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    static GMXLayer2* create(GMXFile& file);
    
    virtual bool init() override;

    virtual void update(float dt) override;
    
    virtual void setTile(int x, int y, TileBase* tile) override;
    
    void showWindow();
    
    void updateChunk(const cocos2d::Vec2& pivot);
    
    bool isUpdateChunk(const cocos2d::Vec2& newPos, const cocos2d::Vec2& oldPos);
    
    void setLayerMaxSize(const cocos2d::Size& maxSize) { _layerMaxSize = maxSize; }
    
    cocos2d::Vec2 getTileRootWorldPosition() const { return _tileRootWorldPosition; }
    
    virtual void setVisible(bool visible) override;
    
    void initFile();
    
    std::pair<int,int> getFocusedTileIndex(const cocos2d::Vec2& worldPos) const;
    
    bool isContainPointInDiamond(const cocos2d::Vec2& diamondCenter, const cocos2d::Size& halfLen, const cocos2d::Vec2& p) const;
    
private:
    
    GMXFile& _file;
    
    cocos2d::DrawNode* _worldDebugNode;
    
    cocos2d::DrawNode* _localDebugNode;
    
    cocos2d::Size _visibleSize;
    
    cocos2d::Size _layerSize;
    
    cocos2d::Size _layerMaxSize;
    
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
    
    cocos2d::ClippingRectangleNode* _clipNode;
    
    bool _isShowWindow = true;
    
    bool _isKeyDown[256];
    
};

#endif /* GMXLayer2_hpp */









