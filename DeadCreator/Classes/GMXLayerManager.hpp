//
//  GMXLayerManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#ifndef GMXLayerManager_hpp
#define GMXLayerManager_hpp

#include <string>
#include <list>

#include "cocos2d.h"

#include "tinyxml2/tinyxml2.h"

class GMXFile;
class GMXLayer;

class GMXLayerManager : public cocos2d::Node
{
    
public:
    
    GMXLayerManager();
    
    virtual ~GMXLayerManager() = default;
    
    CREATE_FUNC(GMXLayerManager);
    
    bool saveGMXFile(GMXFile* file, const std::string& fileName);

    bool loadGMXFile(GMXFile* file, const std::string& fileName);
    
    void addLayer(GMXLayer* layer);
    
    void closeLayer();
    
    void onResize();
    
    void onCenterView(float x, float y);
    
    GMXLayer* getCurrentLayer() const { return _currLayer; }
    
private:
    
    tinyxml2::XMLDocument _document;
    
    GMXLayer* _currLayer;
    
};

#endif /* GMXLayerManager_hpp */









