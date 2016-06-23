//
//  MessageTypes.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//
//

#pragma once

#include <string>

#include "cocos2d.h"


enum MessageNodeType
{
    PALETTE_WINDOW,
    TRIGGER_WINDOW,
    GMX_LAYER,
};

enum MessageType : int
{
    REDO_TILE, // 앞으로 가기 Ctrl + Y
    UNDO_TILE, // 뒤로 가기 Ctrl + Z
};




