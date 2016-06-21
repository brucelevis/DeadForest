//
//  GMXFileManager.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include <boost/filesystem.hpp>

#include "GMXFileManager.hpp"
#include "GMXFile.hpp"

#include "cocos2d.h"
using namespace cocos2d;

using namespace tinyxml2;

bool GMXFileManager::saveGMXFile(const std::string &fileName)
{
    XMLElement* rootNode = _document.NewElement("Root");
    _document.InsertFirstChild(rootNode);
    
    auto element = _document.NewElement("BasicAttr");
    element->SetAttribute("tileWidth", _file->tileWidth);
    element->SetAttribute("tileHeight", _file->tileHeight);
    element->SetAttribute("numOfTileX", _file->numOfTileX);
    element->SetAttribute("numOfTileY", _file->numOfTileY);
    rootNode->InsertEndChild(element);
    
    boost::filesystem::path path(boost::filesystem::initial_path());
    path /= fileName;
    
    auto ret = _document.SaveFile(path.native().c_str());
        
    if (ret != 0) return false;
    else return true;
}


bool GMXFileManager::loadGMXFile(const std::string &fileName)
{
    boost::filesystem::path path(boost::filesystem::initial_path());
    path /= fileName;

    auto ret = _document.LoadFile(path.native().c_str());
    if ( ret != 0)
    {
        cocos2d::log("file is not exist");
        return false;
    }
    
    auto root = _document.FirstChild();
    auto element = root->FirstChildElement("BasicAttr");
    _file->tileWidth = atoi(element->Attribute("tileWidth"));
    _file->tileHeight = atoi(element->Attribute("tileHeight"));
    _file->numOfTileX = atoi(element->Attribute("numOfTileX"));
    _file->numOfTileY = atoi(element->Attribute("numOfTileY"));
    _file->worldSize = Size(_file->tileWidth * _file->numOfTileX, _file->tileHeight / 2 * _file->numOfTileY);
    
    return true;
}









