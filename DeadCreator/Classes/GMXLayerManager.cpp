//
//  GMXLayerManager.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include "FileSystem.hpp"
#include "GMXLayerManager.hpp"
#include "GMXFile.hpp"
#include "GMXLayer.hpp"

#include "cocos2d.h"
using namespace cocos2d;

using namespace tinyxml2;


GMXLayerManager::GMXLayerManager() :
_currLayer(nullptr)
{}


bool GMXLayerManager::saveGMXFile(const std::string &fileName)
{
    GMXFile* file = _currLayer->getFile();
    
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
    
    comment = _document.NewComment("Tile Informations");
    rootNode->InsertEndChild(comment);
    
    std::string defaultTiles[4] = {"Dirt", "Grass", "Water", "Hill"};
    element = _document.NewElement("defaultTile");
    text = _document.NewText(defaultTiles[file->defaultTile].c_str());
    element->InsertEndChild(text);
    rootNode->InsertEndChild(element);
    
    char tileHeader;
    if ( file->defaultTile == 0 ) tileHeader = '1';
    else if ( file->defaultTile == 1) tileHeader = '2';
    else if ( file->defaultTile == 2) tileHeader = '3';
    else if ( file->defaultTile == 3) tileHeader = '5';
    
    element = _document.NewElement("tileList");
    int changedTileSize = 0;
    for(int i = 0 ; i < file->numOfTileY * 2 + DUMMY_TILE_SIZE * 4; ++ i )
    {
        for(int j = 0; j < file->numOfTileX + DUMMY_TILE_SIZE * 2; ++ j)
        {
            if ( file->tileInfos[i][j][0] == tileHeader &&
                file->tileInfos[i][j].substr(4, file->tileInfos[i][j].size() - 4) == "1234")
            {
                continue;
            }
            
            auto listElement = _document.NewElement("tileInfo");
            listElement->SetAttribute("x", j);
            listElement->SetAttribute("y", i);
            listElement->SetAttribute("tile", file->tileInfos[i][j].c_str());
            
            element->InsertEndChild(listElement);
            
            changedTileSize++;
        }
    }
    element->SetAttribute("tileSize", changedTileSize);
    rootNode->InsertEndChild(element);
    
    std::string filePath = FileSystem::getParentPath(FileSystem::getInitialPath()) + "/maps/" + fileName;
    log("%s", filePath.c_str());
    auto ret = _document.SaveFile(filePath.c_str());
        
    if (ret != 0) return false;
    else return true;
}


bool GMXLayerManager::loadGMXFile(GMXFile* file, const std::string& fileName)
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
    
    element = root->FirstChildElement("defaultTile");
    
    std::string defaultTile = element->GetText();
    std::string header;
    if ( defaultTile == "Dirt" ) file->defaultTile = 0, header += "1_";
    else if ( defaultTile == "Grass" ) file->defaultTile = 1, header += "2_";
    else if ( defaultTile == "Water" ) file->defaultTile = 2, header += "3_";
    else if ( defaultTile == "Hill" ) file->defaultTile = 3, header += "5_";
    log("default type: %s (%d)", defaultTile.c_str(), file->defaultTile);
    
    
    int x = file->numOfTileX + DUMMY_TILE_SIZE * 2;
    int y = file->numOfTileY * 2 + DUMMY_TILE_SIZE * 4;
    
    file->tileInfos.resize(y);
    for(int i = 0 ; i < y ; ++ i)
    {
        file->tileInfos[i].resize(x);
    }
    
    for(int i = 0 ; i < y; ++ i)
    {
        for(int j = 0 ; j < x; ++ j)
        {
            file->tileInfos[i][j] = header + std::to_string(random(1,3)) + "_1234";
        }
    }
    
    element = root->FirstChildElement("tileList");
    auto listElement = element->FirstChildElement("tileInfo");
    while ( listElement != nullptr )
    {
        int x = atoi(listElement->Attribute("x"));
        int y = atoi(listElement->Attribute("y"));
        std::string tile = listElement->Attribute("tile");
        file->tileInfos[y][x] = tile;
        
        log("%d, %d : %s", x, y, tile.c_str());
        
        listElement = listElement->NextSiblingElement("tileInfo");
    }
    
    return true;
}


void GMXLayerManager::addChild(GMXLayer* layer)
{
    _currLayer = layer;
    Node::addChild(layer);
}


void GMXLayerManager::closeLayer()
{
    _currLayer->closeFile();
    _currLayer->removeFromParent();
    _currLayer = nullptr;
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









