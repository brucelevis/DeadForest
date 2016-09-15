//
//  SimpleProfiler.hpp
//  profiler_test
//
//  Created by mac on 2016. 9. 8..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>
#include <chrono>

#define PROFILE_BEGIN_FRAME realtrick::profiler::SimpleProfiler::getInstance().beginFrame()
#define PROFILE_END_FRAME realtrick::profiler::SimpleProfiler::getInstance().endFrame()

#define PROFILE_BEGIN(name) realtrick::profiler::SimpleProfiler::getInstance().begin(name)
#define PROFILE_END(name) realtrick::profiler::SimpleProfiler::getInstance().end(name)

namespace realtrick
{
    namespace profiler
    {
        
        class Block;
        
        class SimpleProfiler
        {
            
        public:
            
            virtual ~SimpleProfiler();
            
            static SimpleProfiler& getInstance();
            
            void beginFrame();
            void endFrame();
            
            void begin(const std::string& name);
            void end(const std::string& name);
            
            void writeConsole(long long ms);
            void reset();
            
        private:
            
            SimpleProfiler();
            
        private:
            
            std::vector<Block*> _blockStack;
            std::map<std::string, Block*> _blocks;

            Block* _mainLoopBlock;
            std::chrono::high_resolution_clock::time_point _timePoint;
            
        };
        
    }
}







