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
#include "GMXLayer.hpp"

#include "cocos2d.h"
using namespace cocos2d;

using namespace tinyxml2;


GMXLayerManager::GMXLayerManager() :
_currLayer(nullptr)
{}


bool GMXLayerManager::saveGMXFile(GMXFile* file,const std::string &fileName)
{
    XMLDeclaration* decl = _document.NewDeclaration();
    _document.InsertEndChild(decl);
    
    XMLElement* rootNode = _document.NewElement("Root");
    _document.InsertEndChild(rootNode);
    
    auto comment = _document.NewComment("Setting Basic Informations");
    rootNode->InsertEndChild(comment);
    
    auto element = _document.NewElement("fileName");
    auto text = _document.NewText(file->fileName.c_str());
    element->InsertEndChild(text);
    rootNode->InsertEndChild(element);
    
    element = _document.NewElement("tileSize");
    element->SetAttribute("width", file->tileWidth);
    element->SetAttribute("height", file->tileHeight);
    rootNode->InsertEndChild(element);
    
    element = _document.NewElement("numOfTiles");
    element->SetAttribute("x", file->numOfTileX);
    element->SetAttribute("y", file->numOfTileY);
    rootNode->InsertEndChild(element);
    
    element = _document.NewElement("worldSize");
    element->SetAttribute("width", file->worldSize.width);
    element->SetAttribute("height", file->worldSize.height);
    rootNode->InsertEndChild(element);

    
    boost::filesystem::path path(boost::filesystem::initial_path());
    path /= fileName;
    
//    std::string filePath = path.native();
    std::string filePath = "/Users/jun/Desktop/" + fileName;
    
    auto ret = _document.SaveFile(filePath.c_str());
        
    if (ret != 0) return false;
    else return true;
}


bool GMXLayerManager::loadGMXFile(GMXFile* file,const std::string &fileName)
{
    boost::filesystem::path path(boost::filesystem::initial_path());
    path /= fileName;

    //std::string filePath = path.native();
    std::string filePath = "/Users/jun/Desktop/" + fileName;
    
    auto ret = _document.LoadFile(filePath.c_str());
    if ( ret != 0)
    {
        cocos2d::log("file is not exist");
        return false;
    }
    
    auto root = _document.FirstChildElement("Root");
    auto element = root->FirstChildElement("fileName");
    file->fileName = element->GetText();
    log("fileName: %s", file->fileName.c_str());
    
    element = root->FirstChildElement("tileSize");
    file->tileWidth = atoi(element->Attribute("width"));
    file->tileHeight = atoi(element->Attribute("height"));
    log("tileWidth: %d, tileHeight: %d", file->tileWidth, file->tileHeight);
    
    element = root->FirstChildElement("numOfTiles");
    file->numOfTileX = atoi(element->Attribute("x"));
    file->numOfTileY = atoi(element->Attribute("y"));
    log("numOfTileX: %d, numOfTileY: %d", file->numOfTileX, file->numOfTileY);
    
    element = root->FirstChildElement("worldSize");
    file->worldSize.width = atoi(element->Attribute("width"));
    file->worldSize.height = atoi(element->Attribute("height"));
    log("worldSizeX: %.0f, worldSizeY: %.0f", file->worldSize.width, file->worldSize.height);
    
    return true;
}


void GMXLayerManager::addLayer(GMXLayer* layer)
{
    if ( _currLayer ) _currLayer->setVisible(false);
    
    _gmxlayers.push_back(layer);
    _currLayer = layer;
    addChild(layer);
}


void GMXLayerManager::openLayer(GMXLayer* layer)
{
}


void GMXLayerManager::closeLayer(GMXLayer* layer)
{
}


void GMXLayerManager::removeLayer(GMXLayer* layer)
{
}


void GMXLayerManager::onResize()
{
    if ( _currLayer )
    {
        _currLayer->onResize();
    }
}


void GMXLayerManager::onCenterView(float x, float y)
{
    if ( _currLayer )
    {
        _currLayer->onCenterView(x, y);
    }
}









