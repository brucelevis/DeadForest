//
//  GMXLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include "GMXLayer.hpp"
#include "GMXLayerManager.hpp"
#include "PaletteWindow.hpp"
#include "MinimapLayer.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;

#include "CCImGui.h"


GMXLayer::GMXLayer() :
_oldSelectedTileType(TileType::DIRT)
{
}


GMXLayer::~GMXLayer()
{
}


GMXLayer* GMXLayer::create()
{
    auto ret = new (std::nothrow) GMXLayer();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


bool GMXLayer::init()
{
    if ( !Node::init() )
        return false;
    
    auto visibleSize = _director->getVisibleSize();
    _clipNode = ClippingRectangleNode::create(Rect(0, 0,
                                                   visibleSize.width - MINIMAP_SIZE - WINDOW_PADDING * 3,
                                                   visibleSize.height - MENUBAR_HEIGHT - STATUSBAR_HEIGHT - WINDOW_PADDING * 2));
    addChild(_clipNode);
    
    _tileRoot = Node::create();
    _clipNode->addChild(_tileRoot);
    
    _selectRegion = DrawNode::create();
    _tileRoot->addChild(_selectRegion , 10);
    
    _selectTileRoot = Node::create();
    _tileRoot->addChild(_selectTileRoot, 8);
    
    _centerViewPosition = Vec2(_clipNode->getClippingRegion().size / 2);
    
    return true;
}


void GMXLayer::openFile(GMXFile* file)
{
    _file = file;
    
    int x = file->numOfTileX + DUMMY_TILE_SIZE * 2;
    int y = file->numOfTileY * 2 + DUMMY_TILE_SIZE * 4;
    
    _tileImages.resize(y);
    for(int i = 0 ; i < y ; ++ i)
    {
        _tileImages[i].resize(x);
    }
    
    for(int i = 0 ; i < y; ++ i)
    {
        for(int j = 0 ; j < x ; ++ j)
        {
            Vec2 tilePosition;
            if ( i % 2 == 0 )
            {
                tilePosition.setPoint(j * 128 - (128 * DUMMY_TILE_SIZE), i * 64 - (128 * DUMMY_TILE_SIZE));
            }
            else
            {
                tilePosition.setPoint(64 + j * 128 - (128 * DUMMY_TILE_SIZE), i * 64 - (128 * DUMMY_TILE_SIZE));
            }
            
            _tileImages[i][j] = Sprite::create(file->tileInfos[i][j]);
            _tileImages[i][j]->setPosition(tilePosition);
            _tileRoot->addChild(_tileImages[i][j]);
            
//            ui::Text* indices = ui::Text::create("", "", 20);
//            indices->setPosition(Vec2(64, 64));
//            indices->setString("(" + std::to_string(j) + "," + std::to_string(i) + ")");
//            _tileImages[i][j]->addChild(indices);
        }
    }
    
    
    _selectTileRoot->setPosition(Vec2(file->tileWidth / 2, file->tileHeight / 2));
    //       3
    //     2   4
    //   1   0   5
    //     8   6
    //       7
    Vec2 positions[9]
    {
        Vec2::ZERO, // 0
        Vec2(-file->tileWidth, 0.0f), // 1
        Vec2(-file->tileWidth / 2, file->tileHeight / 2), // 2
        Vec2(0, file->tileHeight), // 3
        Vec2(file->tileWidth / 2, file->tileHeight / 2), // 4
        Vec2(file->tileWidth, 0), // 5
        Vec2(file->tileWidth / 2, -file->tileHeight / 2), // 6
        Vec2(0, -file->tileHeight), // 7
        Vec2(-file->tileWidth / 2, -file->tileHeight / 2), // 8
    };
    _attachedTileImages.resize(9);
    
    for(int i = 0 ; i < 9 ; ++ i)
    {
        _attachedTileImages[i] = Sprite::create();
        _attachedTileImages[i]->setPosition(positions[i]);
        _attachedTileImages[i]->setOpacity(128);
        _selectTileRoot->addChild(_attachedTileImages[i]);
    }
}


void GMXLayer::closeFile()
{
    int x = _file->numOfTileX + DUMMY_TILE_SIZE * 2;
    int y = _file->numOfTileY * 2 + DUMMY_TILE_SIZE * 4;

    for(int i = 0 ; i < y ; ++ i)
    {
        for(int j = 0 ; j < x ; ++ j)
        {
            _tileImages[i][j]->removeFromParent();
        }
        _tileImages[i].clear();
    }
    
    for(int i = 0 ; i < y ; ++ i)
    {
        _tileImages[i].clear();
    }
    _tileImages.clear();
    
    delete _file;
    _file = nullptr;
}


void GMXLayer::onCenterView(const cocos2d::Vec2& params)
{
    Size clipRegion = _clipNode->getClippingRegion().size;
    
    _tileRoot->setPosition(-Vec2(params.x * (_file->worldSize.width - clipRegion.width),
                                 params.y * (_file->worldSize.height - clipRegion.height)));
    
    _centerViewPosition.setPoint(clipRegion.width / 2.0f + params.x * (_file->worldSize.width - clipRegion.width),
                                 clipRegion.height / 2.0f + params.y * (_file->worldSize.height - clipRegion.height));
}


void GMXLayer::onResize()
{
    auto visibleSize = _director->getVisibleSize();
    Rect clipRect = Rect(0, 0,
                         visibleSize.width - MINIMAP_SIZE - WINDOW_PADDING * 3,
                         visibleSize.height - MENUBAR_HEIGHT - STATUSBAR_HEIGHT - WINDOW_PADDING * 2);
    
    setClippingRegion(clipRect);
}


std::pair<int,int> GMXLayer::getFocusedTileIndex(const cocos2d::Vec2& worldPos) const
{
    int centerTileIndexX = static_cast<int>((worldPos.x) / _file->tileWidth) + DUMMY_TILE_SIZE;        // not exact index!
    int centerTileIndexY = static_cast<int>((worldPos.y) / (_file->tileHeight / 2))  + DUMMY_TILE_SIZE * 2;  // not exact index!
    
    for(int i = centerTileIndexY - 2 ; i < centerTileIndexY + 2 ; ++ i)
    {
        for(int j = centerTileIndexX - 2 ; j < centerTileIndexX + 2 ; ++ j)
        {
            Vec2 pivot;
            if( i % 2 == 0 ) pivot.setPoint(j * 128 - (128 * DUMMY_TILE_SIZE), i * 64 - (128 * DUMMY_TILE_SIZE));
            else pivot.setPoint(64 + j * 128 - (128 * DUMMY_TILE_SIZE), i * 64 - (128 * DUMMY_TILE_SIZE));
            
            if(isContainPointInDiamond(pivot, Size(_file->tileWidth / 2, _file->tileHeight / 2), worldPos))
            {
                centerTileIndexX = j; // exact index!
                centerTileIndexY = i; // exact index!
                return std::make_pair(j, i);
            }
        }
    }
    
    return {0, 0};
}


bool GMXLayer::isContainPointInDiamond(const cocos2d::Vec2& diamondCenter, const cocos2d::Size& halfLen, const cocos2d::Vec2& p) const
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
    
    if (p.y - (m * p.x) - b[0] < 0.f
        && p.y - (-m * p.x) - b[1] < 0.f
        && p.y - (m * p.x) - b[2] > 0.f
        && p.y - (-m * p.x) - b[3] > 0.f)
    {
        return true;
    }
    
    return false;
}


void GMXLayer::drawSelectRegion(int x, int y)
{
    _selectRegion->clear();
    Vec2 center = _tileImages[y][x]->getPosition();
    // left to top
    _selectRegion->drawSegment(Vec2(center.x - _file->tileWidth / 2, center.y), Vec2(center.x, center.y + _file->tileHeight / 2), 1.0f, Color4F::RED);
    
    // top to right
    _selectRegion->drawSegment(Vec2(center.x, center.y + _file->tileHeight / 2), Vec2(center.x + _file->tileWidth / 2, center.y), 1.0f, Color4F::RED);
    
    // right to bottom
    _selectRegion->drawSegment(Vec2(center.x + _file->tileWidth / 2, center.y), Vec2(center.x, center.y - _file->tileHeight / 2), 1.0f, Color4F::RED);
    
    // bottom to left
    _selectRegion->drawSegment(Vec2(center.x, center.y - _file->tileHeight / 2), Vec2(center.x - _file->tileWidth / 2, center.y), 1.0f, Color4F::RED);
    
    _selectRegionPosition = center;
    _selectTileRoot->setPosition(center);
}


void GMXLayer::setSelectTileImage(int tileType)
{
    if ( _oldSelectedTileType == tileType )
        return ;
    
    _oldSelectedTileType = tileType;
    
    switch (tileType)
    {
        case TileType::DIRT:
        {
            _attachedTileImages[0]->setTexture("1_1_1234.png");
            _attachedTileImages[1]->setTexture("empty_image.png");
            _attachedTileImages[2]->setTexture("empty_image.png");
            _attachedTileImages[3]->setTexture("empty_image.png");
            _attachedTileImages[4]->setTexture("empty_image.png");
            _attachedTileImages[5]->setTexture("empty_image.png");
            _attachedTileImages[6]->setTexture("empty_image.png");
            _attachedTileImages[7]->setTexture("empty_image.png");
            _attachedTileImages[8]->setTexture("empty_image.png");
            break;
        }
        case TileType::GRASS:
        {
            _attachedTileImages[0]->setTexture("2_1_1234.png");
            _attachedTileImages[1]->setTexture("2_1_2_s.png");
            _attachedTileImages[2]->setTexture("2_1_23_s.png");
            _attachedTileImages[3]->setTexture("2_1_3_s.png");
            _attachedTileImages[4]->setTexture("2_1_34_s.png");
            _attachedTileImages[5]->setTexture("2_1_4_s.png");
            _attachedTileImages[6]->setTexture("2_1_14_s.png");
            _attachedTileImages[7]->setTexture("2_1_1_s.png");
            _attachedTileImages[8]->setTexture("2_1_12_s.png");
            break;
        }
        case TileType::WATER:
        {
            _attachedTileImages[0]->setTexture("3_1_1234.png");
            _attachedTileImages[1]->setTexture("3_1_2_s.png");
            _attachedTileImages[2]->setTexture("3_1_23_s.png");
            _attachedTileImages[3]->setTexture("3_1_3_s.png");
            _attachedTileImages[4]->setTexture("3_1_34_s.png");
            _attachedTileImages[5]->setTexture("3_1_4_s.png");
            _attachedTileImages[6]->setTexture("3_1_14_s.png");
            _attachedTileImages[7]->setTexture("3_1_1_s.png");
            _attachedTileImages[8]->setTexture("3_1_12_s.png");
            break;
        }
        case TileType::HILL:
        {
            _attachedTileImages[0]->setTexture("5_1_1234.png");
            _attachedTileImages[1]->setTexture("5_1_2_s.png");
            _attachedTileImages[2]->setTexture("5_1_23_s.png");
            _attachedTileImages[3]->setTexture("5_1_3_s.png");
            _attachedTileImages[4]->setTexture("5_1_34_s.png");
            _attachedTileImages[5]->setTexture("5_1_4_s.png");
            _attachedTileImages[6]->setTexture("5_1_14_s.png");
            _attachedTileImages[7]->setTexture("5_1_1_s.png");
            _attachedTileImages[8]->setTexture("5_1_12_s.png");
            break;
        }
        default : break;
    }
}


void GMXLayer::disableSelectRegion()
{
    _selectRegion->clear();
    for(int i = 0 ; i < 9 ; ++ i)
    {
        _attachedTileImages[i]->setTexture("empty_image.png");
    }
}


void GMXLayer::putTile(int type, int x, int y)
{
    std::string center;
    if ( type == TileType::DIRT) center = "1_" + std::to_string(random(1, 3)) + "_1234";
    else if ( type == TileType::GRASS) center = "2_" + std::to_string(random(1, 3)) + "_1234";
    else if ( type == TileType::WATER) center = "3_" + std::to_string(random(1, 3)) + "_1234";
    else if ( type == TileType::HILL) center = "5_" + std::to_string(random(1, 3)) + "_1234";
   
    _file->tileInfos[y][x] = center;
    _tileImages[y][x]->setTexture(center + ".png");
    
}









