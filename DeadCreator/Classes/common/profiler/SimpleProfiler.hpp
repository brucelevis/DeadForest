//
//  SimpleProfiler.hpp
//  profiler_test
//
//  Created by NamJunHyeon on 2016. 9. 8..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>
#include <chrono>


#ifndef _ENABLE_ATOMIC_ALIGNMENT_FIX
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#endif

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#define PROFILE_BEGIN_FRAME realtrick::profiler::SimpleProfiler::getInstance().beginFrame()
#define PROFILE_END_FRAME realtrick::profiler::SimpleProfiler::getInstance().endFrame()

#define PROFILE_BEGIN(name) realtrick::profiler::SimpleProfiler::getInstance().begin(name)
#define PROFILE_END(name) realtrick::profiler::SimpleProfiler::getInstance().end(name)


namespace realtrick
{
    namespace profiler
    {
        
        class Block;
        class Element;
        class NetworkWriter;
        
        class SimpleProfiler
        {
            
        public:
            
            enum class WriteType : int
            {
                PRETTY,
                JSON,
                XML,
                FLATBUFFERS
            };
            
        public:
            
            virtual ~SimpleProfiler();
            
            static SimpleProfiler& getInstance();
            
            void beginFrame();
            void endFrame();
            
            void begin(const std::string& name);
            void end(const std::string& name);
            
            void prettyWriter(std::string& out);
            void jsonWriter(std::string& out);
            void xmlWriter(std::string& out);
            
            std::pair<uint8_t*, uint32_t> flatbufferWriter();
            std::string flatbufferWriteHelper(uint8_t* pointer);
            void flatbufferWriteHelper(int depth, std::string& out, const Element* elem);
            
            void writeToNetwork(WriteType type);

            void reset();
            
        private:
            
            SimpleProfiler();
            
            void doAccept();
            
        private:
            
            std::vector<Block*> _blockStack;
            std::map<std::string, Block*> _blocks;

            Block* _mainLoopBlock;
            boost::thread* _networkThread;
            NetworkWriter* _networkWriter;
            
        };
        
    }
}







