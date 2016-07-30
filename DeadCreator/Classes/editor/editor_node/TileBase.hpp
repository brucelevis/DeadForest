//
//  TileBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 28..
//
//

#pragma once

#include <string>

#include "cocos2d.h"
#include "TileHelperFunctions.hpp"
#include "StringHelper.hpp"

#define DUMMY_TILE_SIZE 4

namespace realtrick
{
    
    enum class TileInputState : int
    {
        VALID,
        INVALID,
        RIGHT_UP,
        LEFT_DOWN,
        RIGHT_DOWN,
        LEFT_UP,
    };
        
        class TileBase
        {
            
        public:
            
            TileBase() = default;
            
            TileBase(int xx, int yy, const std::string& number, const cocos2d::Vec2& pos) :
            _xIndex(xx),
            _yIndex(yy),
            _number(number),
            _position(pos)
            {
                if ( number[0] == '1') _type = TileType::DIRT;
                else if ( number[0] == '2' ) _type = TileType::GRASS;
                else if ( number[0] == '3' ) _type = TileType::WATER;
                else if ( number[0] == '5' ) _type = TileType::HILL;
            }
            
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
                _xIndex = rhs._xIndex;
                _yIndex = rhs._yIndex;
            }
            
            virtual ~TileBase() = default;
            
            TileType getTileType() const { return _type; }
            
            std::string getNumber() const { return _number; }
            
            void setNumber(const std::string& number)
            {
                _number = number;
                if ( _number[0] == '1') _type = TileType::DIRT;
                else if ( _number[0] == '2' ) _type = TileType::GRASS;
                else if ( _number[0] == '3' ) _type = TileType::WATER;
                else if ( _number[0] == '5' ) _type = TileType::HILL;
            }
            
            std::string getTileHeader() const
            {
                std::string ret;
                if ( _type == TileType::DIRT ) ret += "1_";
                if ( _type == TileType::GRASS ) ret += "2_";
                if ( _type == TileType::WATER ) ret += "3_";
                if ( _type == TileType::HILL ) ret += "5_";
                ret += _to_string(cocos2d::random(1,3)) + "_";
                
                return ret;
            }
            
            std::string getTileTail() const
            {
                return _number.substr(4, _number.size() - 4);
            }
            
            static std::string getTileHeader(TileType type)
            {
                std::string ret;
                if ( type == TileType::DIRT ) ret += "1_";
                if ( type == TileType::GRASS ) ret += "2_";
                if ( type == TileType::WATER ) ret += "3_";
                if ( type == TileType::HILL ) ret += "5_";
                ret += _to_string(cocos2d::random(1,3)) + "_";
                
                return ret;
            }
            
            static std::string getTileHeader(const std::string& number)
            {
                std::string ret;
                ret += number[0];
                ret += "_" + _to_string(cocos2d::random(1,3)) + "_";
                return ret;
            }
            
            static std::string getTileTail(const std::string& number)
            {
                return number.substr(4, number.size() - 4);
            }
            
            void setPosition(const cocos2d::Vec2& pos) { _position = pos; }
            cocos2d::Vec2 getPosition() const { return _position; }
            
            void setIndex(int x, int y) { _xIndex = x; _yIndex = y; }
            std::pair<int, int> getIndex() const { return { _xIndex, _yIndex }; }
            
            int getIndexX() const { return _xIndex; }
            int getIndexY() const { return _yIndex; }
            
            TileInputState getInputState() const { return _inputState; }
            void setInputState(TileInputState state) { _inputState = state; }
            
        private:
            
            int _xIndex;
            int _yIndex;
            
            TileType _type;
            std::string _number;
            cocos2d::Vec2 _position;
            
        private:
            
            TileInputState _inputState = TileInputState::VALID;
            
        };
        
}
    
    
    
    
    
    
    
    
    
