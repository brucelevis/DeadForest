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

class GMXFile;

class GMXLayer2 : public MutableUiBase, public ImGuiLayer
{
    
public:
    
    GMXLayer2() = default;
    
    virtual ~GMXLayer2() = default;
    
    static GMXLayer2* create(const cocos2d::Size& size);
    
    bool init(const cocos2d::Size& size);

    virtual void update(float dt) override;
    
    virtual void setTerrain(int x, int y, TerrainBase* terrain) override;
    
    void showWindow();
    
private:
    
    std::vector< std::vector<cocos2d::Sprite*> > _tileImages;
    
    cocos2d::Size _layerSize;
    
    bool _isShowWindow = true;
    
};

#endif /* GMXLayer2_hpp */
