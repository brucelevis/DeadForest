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
            
            std::chrono::duration<long long, std::milli>::rep getInterval() const;
            void setInterval(const std::chrono::duration<long long, std::micro>::rep& interval) { _interval = interval; }
            
            float getCpuUsageFromParent() const;
            
            void setName(const std::string& name) { _name = name; }
            std::string getName() const { return _name; }
            
            const std::vector<Block*>& getChildren() const { return _children; }
            void addChild(Block* child) { child->setParent(this); _children.push_back(child); }
            
            void writeConsole(int depth) const;
            
            virtual void begin();
            virtual void end();
            
        private:
            
            Block* _parent;
            std::vector<Block*> _children;
            std::chrono::duration<long long, std::milli>::rep _interval;
            std::chrono::high_resolution_clock::time_point _beginPoint;
            std::string _name;
            
        };
        
    }
}









