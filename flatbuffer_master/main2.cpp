//
//  main2.cpp
//  flatbuffer_master
//
//  Created by mac on 2016. 7. 9..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;

#include "GMXFile_generated.h"
using namespace DeadCreator;

int main()
{
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<TileInfo>> tiles;
    Coord coord(42, 24);
    auto tile = CreateTileInfo(builder, builder.CreateString("1_1_1234"), &coord);
    tiles.push_back(tile);
    Coord numOfTiles(30, 30);
    Size tileSize(128, 128);
    auto file = CreateGMXFile(builder, TileType::TileType_Dirt, builder.CreateVector(tiles), &numOfTiles, &tileSize);
    builder.Finish(file);

    auto GMXFile = GetGMXFile(builder.GetBufferPointer());
    cout << GMXFile->default_type() << endl;
    cout << GMXFile->number_of_tiles()->x() << " " << GMXFile->number_of_tiles()->y()  << endl;
    for( auto iter = GMXFile->tiles()->begin() ; iter != GMXFile->tiles()->end() ; ++ iter)
    {
        cout << iter->number()->str() << endl;
        cout << iter->indices()->x() << " " << iter->indices()->y() << endl;
    }
    cout << GMXFile->tile_size()->width() << " " << GMXFile->tile_size()->height() << endl;
}

