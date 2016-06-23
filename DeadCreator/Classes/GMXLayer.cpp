//
//  GMXLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include <map>
#include <stack>

#include "GMXLayer.hpp"
#include "GMXLayerManager.hpp"
#include "PaletteWindow.hpp"
#include "MinimapLayer.hpp"
#include "SizeProtocol.h"
#include "HistoryQueue.hpp"
#include "HistoryModifyTile.hpp"
using namespace cocos2d;

#include "CCImGui.h"


int GMXLayer::indexToNumber(int x, int y) const
{
    return x + (_file->numOfTileX + DUMMY_TILE_SIZE * 2) * y;
}


std::pair<int, int> GMXLayer::numberToIndex(int number) const
{
    int width = _file->numOfTileX + DUMMY_TILE_SIZE * 2;
    return std::make_pair(number % width, number / width);
}


std::vector<std::pair<int, int>> GMXLayer::getNeighborTiles(int x, int y) const
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


std::string GMXLayer::getTileHeader(int type)
{
    std::string ret;
    if ( type == TileType::DIRT ) ret += "1_";
    if ( type == TileType::GRASS ) ret += "2_";
    if ( type == TileType::WATER ) ret += "3_";
    if ( type == TileType::HILL ) ret += "5_";
    ret += std::to_string(random(1,3)) + "_";
    
    return ret;
}

std::string GMXLayer::getTileHeader(const std::string& number)
{
    std::string ret;
    ret += number[0];
    ret += "_" + std::to_string(random(1,3)) + "_";
    return ret;
}


std::string GMXLayer::getTileTail(const std::string& number)
{
    return number.substr(4, number.size() - 4);
}


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
    _tileRoot->addChild(_selectRegion , 2);
    
    _selectTileRoot = Node::create();
    _tileRoot->addChild(_selectTileRoot, 1);
    
    _centerViewPosition = Vec2(_clipNode->getClippingRegion().size / 2);
    
    _historyQueue = HistoryQueue::create();
    addChild(_historyQueue);
    
    Dispatch.registerNode(MessageNodeType::GMX_LAYER, this);
    
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
            
            auto stuff = PhysicsBody::createCircle(15.0f);
            stuff->setEnabled(true);
   
            _tileImages[i][j] = Sprite::create(file->tileInfos[i][j] + ".png");
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
    HistoryModifyTile* history = new HistoryModifyTile();
    
    std::stack<Tiling> s;
    Tiling init(x, y, getTileHeader(type) + "1234");
    s.push(init);
    while( !s.empty() )
    {
        Tiling temp = s.top();
        s.pop();
        
        if ( temp.tileNumber[0] != '1')
        {
            // 추가하는 로직
            if ( getTileTail(temp.tileNumber) == "1234" )
            {
                auto nei = getNeighborTiles(temp.x, temp.y);
                s.push(Tiling(nei[0].first, nei[0].second, getTileHeader(temp.tileNumber) + "2"));
                s.push(Tiling(nei[1].first, nei[1].second, getTileHeader(temp.tileNumber) + "23"));
                s.push(Tiling(nei[2].first, nei[2].second, getTileHeader(temp.tileNumber) + "3"));
                s.push(Tiling(nei[3].first, nei[3].second, getTileHeader(temp.tileNumber) + "34"));
                s.push(Tiling(nei[4].first, nei[4].second, getTileHeader(temp.tileNumber) + "4"));
                s.push(Tiling(nei[5].first, nei[5].second, getTileHeader(temp.tileNumber) + "14"));
                s.push(Tiling(nei[6].first, nei[6].second, getTileHeader(temp.tileNumber) + "1"));
                s.push(Tiling(nei[7].first, nei[7].second, getTileHeader(temp.tileNumber) + "12"));
                
                // save history
                std::string prev = _file->tileInfos[temp.y][temp.x];
                
                _file->tileInfos[temp.y][temp.x] = temp.tileNumber;
                _tileImages[temp.y][temp.x]->setTexture(temp.tileNumber + ".png");
                
                history->modifiedDatas.push_back(ModifiedData(temp.x, temp.y, prev, temp.tileNumber));
            }
            else
            {
                if ( _file->tileInfos[temp.y][temp.x][0] == temp.tileNumber[0] )
                {
                    // 같은 타일의 경우 합친후 적용한다.
                    std::string originalTail = getTileTail(_file->tileInfos[temp.y][temp.x]);
                    std::string tail = getTileTail(temp.tileNumber);
                    
                    originalTail += tail;
                    std::sort(originalTail.begin(), originalTail.end());
                    originalTail.erase(unique(originalTail.begin(), originalTail.end()), originalTail.end());
                    
                    // save history
                    std::string prev = _file->tileInfos[temp.y][temp.x];
                    
                    std::string finalTile = getTileHeader(_file->tileInfos[temp.y][temp.x]) + originalTail;
                    _file->tileInfos[temp.y][temp.x] = finalTile;
                    _tileImages[temp.y][temp.x]->setTexture(finalTile + ".png");
                    
                    history->modifiedDatas.push_back(ModifiedData(temp.x, temp.y, prev, finalTile));
                }
                else if ( _file->tileInfos[temp.y][temp.x][0] == '1' )
                {
                    // 흙 타일일 경우에는 바로 적용한다.
                    
                    // save history
                    std::string prev = _file->tileInfos[temp.y][temp.x];
                    history->modifiedDatas.push_back(ModifiedData(temp.x, temp.y, prev, temp.tileNumber));
                    
                    _file->tileInfos[temp.y][temp.x] = temp.tileNumber;
                    _tileImages[temp.y][temp.x]->setTexture(temp.tileNumber + ".png");
                }
                else
                {
                    // 다른 타일일 경우
                    
                    // save history
                    std::string prev = _file->tileInfos[temp.y][temp.x];
                    history->modifiedDatas.push_back(ModifiedData(temp.x, temp.y, prev, temp.tileNumber));
                    
                    _file->tileInfos[temp.y][temp.x] = temp.tileNumber;
                    _tileImages[temp.y][temp.x]->setTexture(temp.tileNumber + ".png");
                    
                    auto nei = getNeighborTiles(temp.x, temp.y);
                    std::string tail = getTileTail(temp.tileNumber);
                    if ( tail == "2")
                    {
                        s.push(Tiling(nei[0].first, nei[0].second, getTileHeader(TileType::DIRT) + "2"));
                        s.push(Tiling(nei[1].first, nei[1].second, getTileHeader(TileType::DIRT) + "23"));
                        s.push(Tiling(nei[7].first, nei[7].second, getTileHeader(TileType::DIRT) + "12"));
                    }
                    else if ( tail == "23")
                    {
                        s.push(Tiling(nei[1].first, nei[1].second, getTileHeader(TileType::DIRT) + "23"));
                    }
                    else if ( tail == "3")
                    {
                        s.push(Tiling(nei[1].first, nei[1].second, getTileHeader(TileType::DIRT) + "23"));
                        s.push(Tiling(nei[2].first, nei[2].second, getTileHeader(TileType::DIRT) + "3"));
                        s.push(Tiling(nei[3].first, nei[3].second, getTileHeader(TileType::DIRT) + "34"));
                    }
                    else if ( tail == "34")
                    {
                        s.push(Tiling(nei[3].first, nei[3].second, getTileHeader(TileType::DIRT) + "34"));
                    }
                    else if ( tail == "4")
                    {
                        s.push(Tiling(nei[3].first, nei[3].second, getTileHeader(TileType::DIRT) + "34"));
                        s.push(Tiling(nei[4].first, nei[4].second, getTileHeader(TileType::DIRT) + "4"));
                        s.push(Tiling(nei[5].first, nei[5].second, getTileHeader(TileType::DIRT) + "14"));
                    }
                    else if ( tail == "14")
                    {
                        s.push(Tiling(nei[5].first, nei[5].second, getTileHeader(TileType::DIRT) + "14"));
                    }
                    else if ( tail == "1")
                    {
                        s.push(Tiling(nei[5].first, nei[5].second, getTileHeader(TileType::DIRT) + "14"));
                        s.push(Tiling(nei[6].first, nei[6].second, getTileHeader(TileType::DIRT) + "1"));
                        s.push(Tiling(nei[7].first, nei[7].second, getTileHeader(TileType::DIRT) + "12"));
                    }
                    else if ( tail == "12")
                    {
                        s.push(Tiling(nei[7].first, nei[7].second, getTileHeader(TileType::DIRT) + "12"));
                    }
                }
            }
        }
        else // if ( temp.tileNumber[0] == '1' )
        {
            // 지우는 로직
            if ( getTileTail(temp.tileNumber) == "1234" )
            {
                auto nei = getNeighborTiles(temp.x, temp.y);
                s.push(Tiling(nei[0].first, nei[0].second, getTileHeader(temp.tileNumber) + "2"));
                s.push(Tiling(nei[1].first, nei[1].second, getTileHeader(temp.tileNumber) + "23"));
                s.push(Tiling(nei[2].first, nei[2].second, getTileHeader(temp.tileNumber) + "3"));
                s.push(Tiling(nei[3].first, nei[3].second, getTileHeader(temp.tileNumber) + "34"));
                s.push(Tiling(nei[4].first, nei[4].second, getTileHeader(temp.tileNumber) + "4"));
                s.push(Tiling(nei[5].first, nei[5].second, getTileHeader(temp.tileNumber) + "14"));
                s.push(Tiling(nei[6].first, nei[6].second, getTileHeader(temp.tileNumber) + "1"));
                s.push(Tiling(nei[7].first, nei[7].second, getTileHeader(temp.tileNumber) + "12"));
                
                // save history
                std::string prev = _file->tileInfos[temp.y][temp.x];
                history->modifiedDatas.push_back(ModifiedData(temp.x, temp.y, prev, temp.tileNumber));
                
                _file->tileInfos[temp.y][temp.x] = temp.tileNumber;
                _tileImages[temp.y][temp.x]->setTexture(temp.tileNumber + ".png");
            }
            else
            {
                if ( _file->tileInfos[temp.y][temp.x][0] == '1') continue;
                
                // save history
                std::string prev = _file->tileInfos[temp.y][temp.x];
                
                std::string originalTail = getTileTail(_file->tileInfos[temp.y][temp.x]);
                std::string tail = getTileTail(temp.tileNumber);
                
                for(auto& d : tail)
                {
                    originalTail.erase(std::remove(originalTail.begin(), originalTail.end(), d), originalTail.end());
                }
                
                std::string finalTile;
                if ( originalTail.empty() ) finalTile = getTileHeader(TileType::DIRT) + "1234";
                else finalTile = getTileHeader(_file->tileInfos[temp.y][temp.x]) + originalTail;
                
                _file->tileInfos[temp.y][temp.x] = finalTile;
                _tileImages[temp.y][temp.x]->setTexture(finalTile + ".png");
                
                history->modifiedDatas.push_back(ModifiedData(temp.x, temp.y, prev, finalTile));
            }
        }
    }
    
    _historyQueue->push(history);
}


bool GMXLayer::handleMessage(const Telegram& msg)
{
    if ( msg.msg == MessageType::REDO_TILE )
    {
        auto modifiedTileDatas = *reinterpret_cast<std::vector<ModifiedData>*>(msg.extraInfo);
        for(auto &d : modifiedTileDatas)
        {
            _file->tileInfos[d.y][d.x] = d.curr;
            _tileImages[d.y][d.x]->setTexture(d.curr + ".png");
        }
        
        return true;
    }
    
    else if ( msg.msg == MessageType::UNDO_TILE )
    {
        auto modifiedTileDatas = *reinterpret_cast<std::vector<ModifiedData>*>(msg.extraInfo);
        for(auto &d : modifiedTileDatas)
        {
            _file->tileInfos[d.y][d.x] = d.prev;
            _tileImages[d.y][d.x]->setTexture(d.prev + ".png");
        }
        
        return true;
    }
    
    return false;
}


void GMXLayer::redo()
{
    if ( _historyQueue->isRedo() )
    {
        _historyQueue->redo();
    }
}

void GMXLayer::undo()
{
    if ( _historyQueue->isUndo() )
    {
        _historyQueue->undo();
    }
}


bool GMXLayer::isRedo() const
{
    return _historyQueue->isRedo();
}


bool GMXLayer::isUndo() const
{
    return _historyQueue->isUndo();
}


bool GMXLayer::isChanged() const
{
    return _historyQueue->isUndo();
}









