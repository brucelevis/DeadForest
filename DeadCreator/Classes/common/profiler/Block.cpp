//
//  Block.cpp
//  profiler_test
//
//  Created by mac on 2016. 9. 9..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#include <iostream>
using namespace std;

#include "Block.hpp"
using namespace realtrick::profiler;


Block::Block() :
_parent(nullptr),
_children(),
_totalTime(0),
_avgTime(0),
_maxTime(0),
_minTime(1000000000),
_numberOfCalls(0),
_beginPoint(),
_name("")
{
}


Block::~Block()
{
    for ( auto& child : _children )
    {
        SAFE_DELETE(child);
    }
    _children.clear();
}


Block* Block::create(const std::string& name)
{
    auto ret = new (std::nothrow) Block();
    if ( ret && ret->init(name) )
    {
        return ret;
    }
    SAFE_DELETE(ret);
    return nullptr;
}


bool Block::init(const std::string& name)
{
    _name = name;
    
    return true;
}


float Block::getCpuUsageFromParent() const
{
    if ( _parent )
    {
        auto entryTime = static_cast<float>(_parent->getAvgTime());
        if ( entryTime != 0.0f )
            return (_avgTime / entryTime) * 100.0f;
    }
    return 100.0f;
}


void Block::begin()
{
    _beginPoint = chrono::high_resolution_clock::now();
}


void Block::end()
{
    _numberOfCalls ++;
    
    auto curr = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(curr - _beginPoint).count();
    _totalTime += duration;
    _avgTime = _totalTime  / _numberOfCalls;
    _minTime = std::min(_minTime, duration);
    _maxTime = std::max(_maxTime, duration);
}


void Block::writeConsole(int depth) const
{
    std::string name;
    for( int i = 0 ; i < depth ; ++ i )
        name.push_back(' ');
    name.insert(depth, _name);
    printf(" %-40s%-10s%-15s%-15s%-15s%-10s\n",
           name.c_str(),
           std::to_string(_numberOfCalls).c_str(),
           std::to_string(_avgTime).c_str(),
           std::to_string(_minTime).c_str(),
           std::to_string(_maxTime).c_str(),
           std::to_string(getCpuUsageFromParent()).c_str());
    for ( const auto& child : _children )
    {
        child->writeConsole(depth + 1);
    }
}


void Block::reset()
{
    _totalTime = 0;
    _numberOfCalls = 0;
    _avgTime = 0;
    _maxTime = 0;
    _minTime = 1000000000;
    _beginPoint = chrono::high_resolution_clock::now();
    
    for( const auto& child : _children )
        child->reset();
}









