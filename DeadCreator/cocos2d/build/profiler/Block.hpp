//
//  Block.hpp
//  profiler_test
//
//  Created by NamJunHyeon on 2016. 9. 9..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#pragma once

#include <vector>
#include <chrono>
#include <string>

#include "profiling_schema_generated.h"
using namespace flatbuffers;

#define SAFE_DELETE(p) do { delete (p); (p) = nullptr; } while(0)

namespace realtrick
{
    namespace profiler
    {
        
        class Block
        {
         
            friend class SimpleProfiler;
            
        public:
            
            Block();
            virtual ~Block();
            
            static Block* create(const std::string& name);
            virtual bool init(const std::string& name);
            
            Block* getParent() const { return _parent; }
            void setParent(Block* parent) { _parent = parent; }
            
            float getUsageFromParent() const;
            
            void setName(const std::string& name) { _name = name; }
            std::string getName() const { return _name; }
            
            const std::vector<Block*>& getChildren() const { return _children; }
            void addChild(Block* child) { child->setParent(this); _children.push_back(child); }
            
            int32_t getAvgTime() const { return _avgTime; }
            int32_t getMinTime() const { return _minTime; }
            int32_t getMaxTime() const { return _maxTime; }
            int32_t getTotalCall() const { return _numberOfCalls; }
            
            void prettyWrite(int depth, std::string& out) const;
            Offset<Vector<Offset<Element>>> getChildrenFlatbuffers(FlatBufferBuilder& builder) const;
            
            virtual void begin();
            virtual void end();
            
            void reset();
            
        private:
            
            Block* _parent;
            std::vector<Block*> _children;
            int32_t _lastDuration;
            int32_t _totalTime;
            int32_t _avgTime;
            int32_t _maxTime;
            int32_t _minTime;
            int32_t _numberOfCalls;
            std::chrono::high_resolution_clock::time_point _beginPoint;
            std::string _name;
            
        };
        
    }
}









