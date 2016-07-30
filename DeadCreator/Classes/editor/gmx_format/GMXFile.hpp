//
//  GMXFile.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 21..
//
//

#pragma once

#include <string>
#include <vector>

namespace realtrick
{
    namespace editor
    {
        
        struct GMXFile
        {
            
            std::string fileName;
            int tileWidth = -1;
            int tileHeight = -1;
            int numOfTileX = -1;
            int numOfTileY = -1;
            std::vector<std::vector<std::string>> tileInfos;
            cocos2d::Size worldSize;
            int defaultTile = -1;
            
            GMXFile() = default;
            virtual ~GMXFile() = default;
            
        };
        
    }
}









