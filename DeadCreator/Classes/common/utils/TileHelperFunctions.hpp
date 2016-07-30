//
//  TileHelperFunctions.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 30..
//
//

#pragma once

#include "cocos2d.h"

namespace
{
    
    int indexToNumber(int x, int y, int numberOfTileX, int numOfDummy)
    {
        return x + (numberOfTileX + numOfDummy * 2) * y;
    }
    
    std::pair<int, int> numberToIndex(int number, int numberOfTileX, int numOfDummy)
    {
        int width = numberOfTileX + numOfDummy * 2;
        return std::make_pair(number % width, number / width);
    }
    
    cocos2d::Vec2 indexToPosition(int x, int y, int tileWidth, int tileHeight, int numOfDummy)
    {
        cocos2d::Vec2 tilePosition;
        if ( y % 2 == 0 )
        {
            tilePosition.setPoint(x * tileWidth - (tileWidth * numOfDummy), y * tileHeight / 2 - (tileHeight * numOfDummy));
        }
        else
        {
            tilePosition.setPoint(tileWidth / 2 + x * tileWidth - (tileWidth * numOfDummy), y * tileHeight / 2 - (tileHeight * numOfDummy));
        }
        
        return tilePosition;
    }
    
    std::vector<std::pair<int, int>> getNeighborTiles(int x, int y)
    {
        //       2
        //     1   3
        //   0 (x,y) 4
        //     7   5
        //       6
        std::vector<std::pair<int, int>> ret;
        if (y % 2 == 0)
        {
            ret.push_back( {x - 1, y} );        // 0
            ret.push_back( {x - 1, y + 1} );    // 1
            ret.push_back( {x, y + 2} );        // 2
            ret.push_back( {x, y + 1} );        // 3
            ret.push_back( {x + 1, y} );        // 4
            ret.push_back( {x, y - 1} );        // 5
            ret.push_back( {x, y - 2} );        // 6
            ret.push_back( {x - 1, y - 1} );    // 7
        }
        else
        {
            ret.push_back( {x - 1, y} );        // 0
            ret.push_back( {x, y + 1} );        // 1
            ret.push_back( {x, y + 2} );        // 2
            ret.push_back( {x + 1, y + 1} );    // 3
            ret.push_back( {x + 1, y} );        // 4
            ret.push_back( {x + 1, y - 1} );    // 5
            ret.push_back( {x, y - 2} );        // 6
            ret.push_back( {x, y - 1} );        // 7
        }
        return ret;
    }
    
    
    bool isContainPointInDiamond(const cocos2d::Vec2& diamondCenter, const cocos2d::Size& halfLen, const cocos2d::Vec2& p)
    {
        int m = 1;
        float b[4];
        
        cocos2d::Vec2 leftPoint = cocos2d::Vec2(diamondCenter.x - halfLen.width, diamondCenter.y);
        cocos2d::Vec2 rightPoint = cocos2d::Vec2(diamondCenter.x + halfLen.width, diamondCenter.y);
        cocos2d::Vec2 topPoint = cocos2d::Vec2(diamondCenter.x, diamondCenter.y + halfLen.height);
        cocos2d::Vec2 botPoint = cocos2d::Vec2(diamondCenter.x, diamondCenter.y - halfLen.height);
        
        b[0] = leftPoint.y - m * leftPoint.x;
        b[1] = topPoint.y + m * topPoint.x;
        b[2] = rightPoint.y - m * rightPoint.x;
        b[3] = botPoint.y + m * botPoint.x;
        
        if (p.y - (m * p.x) - b[0] <= 0.f
            && p.y - (-m * p.x) - b[1] <= 0.f
            && p.y - (m * p.x) - b[2] >= 0.f
            && p.y - (-m * p.x) - b[3] >= 0.f)
        {
            return true;
        }
        
        return false;
    }
    
    
    std::pair<int,int> getFocusedTileIndex(const cocos2d::Vec2& worldPos, float tileWidth, float tileHeight, int numOfDummy)
    {
        int centerTileIndexX = static_cast<int>((worldPos.x) / tileWidth) + numOfDummy;        // not exact index!
        int centerTileIndexY = static_cast<int>((worldPos.y) / (tileHeight / 2))  + numOfDummy * 2;  // not exact index!
        
        for(int i = centerTileIndexY - 2 ; i < centerTileIndexY + 2 ; ++ i)
        {
            for(int j = centerTileIndexX - 2 ; j < centerTileIndexX + 2 ; ++ j)
            {
                cocos2d::Vec2 pivot;
                if( i % 2 == 0 ) pivot.setPoint(j * tileWidth - (tileWidth * numOfDummy), i * tileHeight/2 - (tileHeight * numOfDummy));
                else pivot.setPoint(tileWidth / 2 + j * tileWidth - (tileWidth * numOfDummy), i * tileHeight/2 - (tileHeight * numOfDummy));
                
                if(isContainPointInDiamond(pivot, cocos2d::Size(tileWidth / 2, tileHeight / 2), worldPos))
                {
                    centerTileIndexX = j; // exact index!
                    centerTileIndexY = i; // exact index!
                    return std::make_pair(j, i);
                }
            }
        }
        
        return {0, 0};
    }
    
    
    std::pair<int, int> getRectangleTileIndex(const cocos2d::Vec2& worldPos, float tileWidth, float tileHeight)
    {
        return { worldPos.x / (tileWidth / 4), worldPos.y / (tileHeight / 4) };
    }
    
    
    cocos2d::Vec2 getPositionFromRectangleIndex(int x, int y, float tileWidth, float tileHeight)
    {
        return cocos2d::Vec2(x * tileWidth / 4, y * tileHeight / 4);
    }
    
}









