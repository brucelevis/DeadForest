//
//  SimpleProfiler.cpp
//  profiler_test
//
//  Created by mac on 2016. 9. 8..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#include <thread>
#include <functional>
#include <exception>
#include <iostream>
using namespace std;
using namespace std::chrono;

#include "SimpleProfiler.hpp"
#include "Block.hpp"
using namespace realtrick::profiler;

#include "tinyxml2.h"
#include "json/rapidjson.h"
#include "flatbuffers.h"
#include "profiling_schema_generated.h"
using namespace flatbuffers;


SimpleProfiler::SimpleProfiler() :
_blockStack(),
_blocks(),
_mainLoopBlock(nullptr),
_socket(_io),
_acceptor(_io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4242))
{
    doAccept();
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


void SimpleProfiler::prettyWriter(std::string& out)
{
    CCASSERT(_mainLoopBlock, "main loop block is not exist");
    
    if ( _mainLoopBlock )
    {
        out.clear();
        
        out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
        out += "|name                         | calls   | avg time  | min time  | max time  | usage(%)   |\n";
        out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
        _mainLoopBlock->prettyWrite(0, out);
        out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
    }
    
}


void SimpleProfiler::jsonWriter(std::string& out)
{
}


void SimpleProfiler::xmlWriter(std::string& out)
{
    //    tinyxml2::XMLDocument doc;
}


std::pair<uint8_t*, uint32_t> SimpleProfiler::flatbufferWriter()
{
    FlatBufferBuilder builder;
    auto obj = realtrick::profiler::CreateData(builder,
                                               CreateElement(builder,
                                                             builder.CreateString(_mainLoopBlock->getName()),
                                                             _mainLoopBlock->getTotalCall(),
                                                             _mainLoopBlock->getAvgTime(),
                                                             _mainLoopBlock->getMinTime(),
                                                             _mainLoopBlock->getMaxTime(),
                                                             _mainLoopBlock->getUsageFromParent(),
                                                             _mainLoopBlock->getChildrenFlatbuffers(builder)));
    builder.Finish(obj);
    return std::make_pair(builder.GetBufferPointer(), builder.GetSize());
}


std::string SimpleProfiler::flatbufferWriteHelper(uint8_t* pointer)
{
    auto obj = GetData(pointer);
    auto mainLoop = obj->main_loop();
    
    std::string out;
    out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
    out += "|name                         | calls   | avg time  | min time  | max time  | usage(%)   |\n";
    out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
    flatbufferWriteHelper(0, out, mainLoop);
    out += "+-----------------------------+---------+-----------+-----------+-----------+------------+\n";
    
    return out;
}


void SimpleProfiler::flatbufferWriteHelper(int depth, std::string& out, const Element* elem)
{
    std::string name;
    for( int i = 0 ; i < depth ; ++ i ) name.push_back(' ');
    name.insert(depth, elem->name()->str());
    for(auto i = elem->name()->str().size() + depth; i < 29; ++ i) name.push_back(' ');
    name.push_back('|');
    
    std::string calls(to_string(elem->calls()));
    for(auto i = calls.size() ; i < 9; ++ i) calls.push_back(' ');
    calls.push_back('|');
    
    std::string avgTime(to_string(elem->avg_time()));
    for(auto i = avgTime.size() ; i < 11; ++ i) avgTime.push_back(' ');
    avgTime.push_back('|');
    
    std::string minTime(to_string(elem->min_time()));
    for(auto i = minTime.size() ; i < 11; ++ i) minTime.push_back(' ');
    minTime.push_back('|');
    
    std::string maxTime(to_string(elem->max_time()));
    for(auto i = maxTime.size() ; i < 11; ++ i) maxTime.push_back(' ');
    maxTime.push_back('|');
    
    std::string usage(to_string(elem->usage()));
    for(auto i = usage.size() ; i < 12; ++ i) usage.push_back(' ');
    usage.push_back('|');
    
    if ( calls.front() == '0' ) return ;
    
    out += '|';
    out += (name + calls + avgTime + minTime + maxTime + usage);
    out += '\n';
    
    for ( auto iter = elem->children()->begin() ; iter != elem->children()->end(); ++ iter)
        flatbufferWriteHelper(depth + 1, out, *iter);
}


void SimpleProfiler::doAccept()
{
    std::cout << "ready to accept" <<std::endl;
    
//    _acceptor.async_accept(_socket, [this](const boost::system::error_code& ec){
    
//        if ( !ec )
//        {
//            std::cout << "client connected." << std::endl;
//            auto session = make_shared<Session>(move(_socket));
//            session->start();
//            _sessions.push_back(session);
//            
//            cout << "sesison size: " << _sessions.size() << endl;
//        }
        
//        doAccept();
        
//    });

}


