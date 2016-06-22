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

#include "tinyxml2/tinyxml2.h"

class GMXFile;

class GMXLayerManager
{
    
public:
    
    GMXLayerManager() = default;
    
    virtual ~GMXLayerManager() = default;
    
    bool saveGMXFile(GMXFile* file, const std::string& fileName);

    bool loadGMXFile(GMXFile* file, const std::string& fileName);
    
    void pushFile(GMXFile* file);
    
    void openFile(GMXFile* file);
    
    void closeFile(GMXFile* file);
    
    void removeFile(GMXFile* file);
    
private:
    
    tinyxml2::XMLDocument _document;
    
    std::list<GMXFile*> _openedFiles;
    
};

#endif /* GMXLayerManager_hpp */









