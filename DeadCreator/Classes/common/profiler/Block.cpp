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


void Block::writeConsole(int depth, std::string& out) const
{
    std::string name;
    for( int i = 0 ; i < depth ; ++ i ) name.push_back(' ');
    name.insert(depth, _name);
    for(int i = _name.size() + depth; i < 29; ++ i) name.push_back(' ');
    name.push_back('|');
    
    std::string calls(to_string(_numberOfCalls));
    for(int i = calls.size() ; i < 9; ++ i) calls.push_back(' ');
    calls.push_back('|');
    
    std::string avgTime(to_string(_avgTime));
    for(int i = avgTime.size() ; i < 14; ++ i) avgTime.push_back(' ');
    avgTime.push_back('|');
    
    std::string minTime(to_string(_minTime));
    for(int i = minTime.size() ; i < 14; ++ i) minTime.push_back(' ');
    minTime.push_back('|');
    
    std::string maxTime(to_string(_maxTime));
    for(int i = maxTime.size() ; i < 14; ++ i) maxTime.push_back(' ');
    maxTime.push_back('|');
    
    std::string cpuUsage(to_string(getCpuUsageFromParent()));
    for(int i = cpuUsage.size() ; i < 14; ++ i) cpuUsage.push_back(' ');
    cpuUsage.push_back('|');
    
    if ( calls.front() == '0' ) return ;
    
    out += '|';
    out += (name + calls + avgTime + minTime + maxTime + cpuUsage);
    out += '\n';
    
    for ( const auto& child : _children )
        child->writeConsole(depth + 1, out);
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









