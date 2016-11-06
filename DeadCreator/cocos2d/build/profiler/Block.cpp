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
_interval(),
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


std::chrono::duration<long long, micro>::rep Block::getInterval() const
{
    return _interval;
}


float Block::getCpuUsageFromParent() const
{
    if ( _parent ) return (_interval / static_cast<float>(_parent->getInterval())) * 100.0f;
    return 100.0f;
}


void Block::begin()
{
    _beginPoint = chrono::high_resolution_clock::now();
}


void Block::end()
{
    auto curr = chrono::high_resolution_clock::now();
    _interval = chrono::duration_cast<chrono::milliseconds>(curr - _beginPoint).count();
}


void Block::writeConsole(int depth) const
{
    std::string name;
    for( int i = 0 ; i < depth ; ++ i )
        name.push_back(' ');
    name.insert(depth, _name);
    printf("%-26s%-26s%-26s\n",name.c_str(), std::to_string(_interval).c_str(), std::to_string(getCpuUsageFromParent()).c_str());
    for ( const auto& child : _children )
    {
        child->writeConsole(depth + 1);
    }
}







