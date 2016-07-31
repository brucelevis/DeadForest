//TileType GameMap::getStepOnTileType(const cocos2d::Vec2& pos)
//{
//    std::pair<int, int> idx = getFocusedTileIndex(pos, _tileWidth, _tileHeight, DUMMY_TILE_SIZE);
//    Tileset& tile = _tileData[idx.second][idx.first];
//    Vec2 center = indexToPosition(idx.first, idx.second, _tileWidth, _tileHeight, DUMMY_TILE_SIZE);
//    
//    Vec2 region1 = center + Vec2(0.0f, _tileHeight / 4.0f);
//    Vec2 region2 = center + Vec2(_tileWidth / 4.0f, 0.0f);
//    Vec2 region3 = center - Vec2(0.0f, _tileHeight / 4.0f);
//    Vec2 region4 = center - Vec2(_tileWidth / 4.0f, 0.0f);
//    
//    std::string tileType = tile.getTileTail();
//    
//    if ( physics::isContainPointInDiamond(region1, _tileWidth / 4.0f, pos) && (tileType.find('1') != std::string::npos) )
//    {
//        return tile.getTileType();
//    }
//    else if ( physics::isContainPointInDiamond(region2, _tileWidth / 4.0f, pos) && (tileType.find('2') != std::string::npos) )
//    {
//        return tile.getTileType();
//    }
//    else if ( physics::isContainPointInDiamond(region3, _tileWidth / 4.0f, pos) && (tileType.find('3') != std::string::npos) )
//    {
//        return tile.getTileType();
//    }
//    else if ( physics::isContainPointInDiamond(region4, _tileWidth / 4.0f, pos) && (tileType.find('4') != std::string::npos) )
//    {
//        return tile.getTileType();
//    }
//    
//    return TileType::DIRT;
//}










