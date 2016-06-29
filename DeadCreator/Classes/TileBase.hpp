//
//  TileBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#ifndef TileBase_hpp
#define TileBase_hpp

#include <string>

#include "cocos2d.h"

enum TileType
{
    DIRT,
    GRASS,
    WATER,
    HILL
};

class TileBase
{
    
public:
    
    TileBase(const std::string& number, const cocos2d::Vec2& pos) :
    _type(TileType::DIRT),
    _number(number),
    _position(pos)
    {}
    
    TileBase(const TileBase& rhs)
    {
        copyFrom(rhs);
    }
    
    TileBase& operator=(const TileBase& rhs)
    {
        if ( &rhs == this)
            return *this;
        copyFrom(rhs);
        return *this;
    }
    
    void copyFrom(const TileBase& rhs)
    {
        _type = rhs._type;
        _number = rhs._number;
        _position = rhs._position;
    }
    
    virtual ~TileBase() = default;
    
    TileType getType() const { return _type; }
    
    std::string getNumber() const { return _number; }
    
    void setNumber(const std::string& number)
    {
        _number = number;
        if ( _number[0] == '1') _type = TileType::DIRT;
        else if ( _number[0] == '2' ) _type = TileType::GRASS;
        else if ( _number[0] == '3' ) _type = TileType::WATER;
        else if ( _number[0] == '5' ) _type = TileType::HILL;
    }
    
    void setPosition(const cocos2d::Vec2& pos) { _position = pos; }
    
    cocos2d::Vec2 getPosition() const { return _position; }
    
private:
    
    TileType _type;
    
    std::string _number;
    
    cocos2d::Vec2 _position;
    
};


#endif /* TileBase_hpp */
