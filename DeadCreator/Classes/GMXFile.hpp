//
//  GMXFile.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#ifndef GMXFile_hpp
#define GMXFile_hpp

#include <string>
#include <vector>

struct GMXFile
{
    
    std::string fileName;
    
    int tileWidth = -1;
    
    int tileHeight = -1;
    
    int numOfTileX = -1;
    
    int numOfTileY = -1;
    
    std::vector<std::vector<std::string>> tileInfos;
    
    cocos2d::Size worldSize;
    
    GMXFile() = default;
    
    virtual ~GMXFile() = default;
    
};

#endif /* GMXFile_hpp */
