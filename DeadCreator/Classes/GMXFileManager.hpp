//
//  GMXFileManager.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#ifndef GMXFileManager_hpp
#define GMXFileManager_hpp

#include <string>

#include "tinyxml2/tinyxml2.h"

class GMXFile;

class GMXFileManager
{
    
public:
    
    explicit GMXFileManager(GMXFile* file) { _file = file; }
    
    virtual ~GMXFileManager() = default;
    
    bool saveGMXFile(const std::string& fileName);

    bool loadGMXFile(const std::string& fileName);
    
private:
    
    GMXFile* _file;
    
    tinyxml2::XMLDocument _document;
    
};

#endif /* GMXFileManager_hpp */









