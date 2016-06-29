//
//  TileHelperFunctions.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 30..
//
//

#pragma once

#include "cocos2d.h"


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
