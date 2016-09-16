//
//  Block.hpp
//  profiler_test
//
//  Created by mac on 2016. 9. 9..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#pragma once

#include <vector>
#include <chrono>
#include <string>

#define SAFE_DELETE(p) do { delete (p); (p) = nullptr; } while(0)

namespace realtrick
{
    namespace profiler
    {
        
        class Block
        {
            
        public:
            
            Block();
            virtual ~Block();
            
            static Block* create(const std::string& name);
            virtual bool init(const std::string& name);
            
            Block* getParent() const { return _parent; }
            void setParent(Block* parent) { _parent = parent; }
            
            float getCpuUsageFromParent() const;
            
            void setName(const std::string& name) { _name = name; }
            std::string getName() const { return _name; }
            
            const std::vector<Block*>& getChildren() const { return _children; }
            void addChild(Block* child) { child->setParent(this); _children.push_back(child); }
            
            long long getAvgTime() const { return _avgTime; }
            long long getMinTime() const { return _minTime; }
            long long getMaxTime() const { return _maxTime; }
            long long getTotalCall() const { return _numberOfCalls; }
            
            void writeConsole(int depth, std::string& out) const;
            
            virtual void begin();
            virtual void end();
            
            void reset();
            
        private:
            
            Block* _parent;
            std::vector<Block*> _children;
            long long _totalTime;
            long long _avgTime;
            long long _maxTime;
            long long _minTime;
            long long _numberOfCalls;
            std::chrono::high_resolution_clock::time_point _beginPoint;
            std::string _name;
            
        };
        
    }
}









