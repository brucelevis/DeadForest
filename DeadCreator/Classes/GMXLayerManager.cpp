//
//  GMXLayerManager.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include <boost/filesystem.hpp>

#include "GMXLayerManager.hpp"
#include "GMXFile.hpp"

#include "cocos2d.h"
using namespace cocos2d;

using namespace tinyxml2;

bool GMXLayerManager::saveGMXFile(GMXFile* file,const std::string &fileName)
{
    XMLElement* rootNode = _document.NewElement("Root");
    _document.InsertFirstChild(rootNode);
    
    auto element = _document.NewElement("BasicAttr");
    element->SetAttribute("tileWidth", file->tileWidth);
    element->SetAttribute("tileHeight", file->tileHeight);
    element->SetAttribute("numOfTileX", file->numOfTileX);
    element->SetAttribute("numOfTileY", file->numOfTileY);
    rootNode->InsertEndChild(element);
    
    boost::filesystem::path path(boost::filesystem::initial_path());
    path /= fileName;
    
    auto ret = _document.SaveFile(path.native().c_str());
        
    if (ret != 0) return false;
    else return true;
}


bool GMXLayerManager::loadGMXFile(GMXFile* file,const std::string &fileName)
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
    file->tileWidth = atoi(element->Attribute("tileWidth"));
    file->tileHeight = atoi(element->Attribute("tileHeight"));
    file->numOfTileX = atoi(element->Attribute("numOfTileX"));
    file->numOfTileY = atoi(element->Attribute("numOfTileY"));
    file->worldSize = Size(file->tileWidth * file->numOfTileX, file->tileHeight / 2 * file->numOfTileY);
    
    return true;
}


void GMXLayerManager::pushFile(GMXFile* file)
{
}


void GMXLayerManager::openFile(GMXFile* file)
{
}


void GMXLayerManager::closeFile(GMXFile* file)
{
}


void GMXLayerManager::removeFile(GMXFile* file)
{
}









