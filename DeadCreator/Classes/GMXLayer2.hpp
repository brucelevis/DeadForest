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
    
    virtual void setTerrain(int x, int y, TerrainBase* terrain) override;
    
    void showWindow();
    
    void updateChunk(int x, int y);
    
    bool isUpdateChunk(const cocos2d::Vec2& newPos, const cocos2d::Vec2& oldPos);
    
    void setLayerMaxSize(const cocos2d::Size& maxSize) { _layerMaxSize = maxSize; }
    
    cocos2d::Vec2 getRootTileWorldPosition() const { return _rootTileWorldPosition; }
    
    virtual void setVisible(bool visible) override;
    
    void initFile();
    
private:
    
    GMXFile& _file;
    
    cocos2d::DrawNode* _debugNode;
    
    cocos2d::Size _visibleSize;
    
    cocos2d::Size _layerSize;
    
    cocos2d::Size _layerMaxSize;
    
    cocos2d::Vec2 _layerPosition;
    
    cocos2d::Vec2 _centerViewParam;
    
    cocos2d::Vec2 _centerViewPosition;
    
    cocos2d::Vec2 _rootTileWorldPosition;
    
    cocos2d::Vec2 _cameraDirection;
    
    Camera2D* _camera;
    
    float _windowSpeed;
    
    cocos2d::Node* _root;
    
    CellSpacePartition* _cellSpacePartition;
    
    std::vector< std::vector<TileImage*> > _tileImages;
    
    cocos2d::ClippingRectangleNode* _clipNode;
    
    bool _isShowWindow = true;
    
    bool _isKeyDown[256];
    
};

#endif /* GMXLayer2_hpp */









