//
//  SimpleProfiler.cpp
//  profiler_test
//
//  Created by mac on 2016. 9. 8..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#include <exception>
using namespace std;
using namespace std::chrono;

#include "SimpleProfiler.hpp"
#include "Block.hpp"
using namespace realtrick::profiler;

#include "tinyxml2.h"
#include "json/rapidjson.h"
#include "flatbuffers.h"
#include "profiling_schema_generated.h"


SimpleProfiler::SimpleProfiler() :
_blockStack(),
_blocks(),
_mainLoopBlock(nullptr)
{
}


SimpleProfiler::~SimpleProfiler()
{
}


SimpleProfiler& SimpleProfiler::getInstance()
{
    static SimpleProfiler instance;
    return instance;
}


void SimpleProfiler::beginFrame()
{
    if ( !_mainLoopBlock )
        _mainLoopBlock = Block::create("main_loop");
    
    _blockStack.push_back(_mainLoopBlock);
    _mainLoopBlock->begin();
}


void SimpleProfiler::endFrame()
{
    if ( !_mainLoopBlock )
        throw std::runtime_error("main_loop block is not exist so can not call endFrame().");
    
    if ( !_blockStack.empty() && _blockStack.back()->getName() != "main_loop" )
        throw std::runtime_error("main_loop is not top block you must call " + _blockStack.back()->getName() + "'s end().");
    
    _blockStack.pop_back();
    _mainLoopBlock->end();
}


void SimpleProfiler::begin(const std::string& name)
{
    if ( _blockStack.front()->getName() != "main_loop" )
        throw std::runtime_error("main_loop block is not exist. please call 'PROFILE_BEGIN_FRAME'");
    
    if ( !_blockStack.empty() && _blockStack.back()->getName() == name )
        throw std::runtime_error(_blockStack.back()->getName() + " is already begin.");
    
    if ( _blocks.count(name) == 0 )
    {
        auto block = Block::create(name);
        _blocks.insert( {name, block } );
        _blockStack.back()->addChild(block);
    }
    
    auto cachedBlock = _blocks.at(name);
    _blockStack.push_back(cachedBlock);
    cachedBlock->begin();
}


void SimpleProfiler::end(const std::string& name)
{
    if ( _blockStack.empty() )
        throw std::runtime_error("block stack is empty so can not call end().");
    
    if ( _blockStack.back()->getName() != name )
        throw std::runtime_error(name + " is not top block you must call " + _blockStack.back()->getName() + "'s end().");
    
    if ( _blocks.count(name) == 0)
        throw std::runtime_error(name + " is not exist so can not call end().");
    
    _blockStack.pop_back();
    _blocks.at(name)->end();
}


void SimpleProfiler::reset()
{
    if ( _mainLoopBlock ) _mainLoopBlock->reset();
}


std::string SimpleProfiler::prettyWriter()
{
    if ( _mainLoopBlock )
    {
        std::string out;
        out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
        out += "|name                         | calls   | avg time  | min time  | max time  | usage(%)   |\n";
        out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
        _mainLoopBlock->prettyWrite(0, out);
        out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
        
        return out;
    }
    return "##main_loop is not exist!";
}


std::string SimpleProfiler::jsonWriter()
{
    return "";
}


std::string SimpleProfiler::xmlWriter()
{
    //    tinyxml2::XMLDocument doc;
    return "";
}


uint8_t* SimpleProfiler::flatbufferWriter()
{
    flatbuffers::FlatBufferBuilder builder;
    auto obj = realtrick::profiler::CreateData(builder);
    builder.Finish(obj);
    return builder.GetBufferPointer();
}









