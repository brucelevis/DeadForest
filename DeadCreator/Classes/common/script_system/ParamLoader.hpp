//
//  ParamLoader.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 10. 20..
//
//

#pragma once

#include <unordered_map>
#include <string>

#include "cocos2d.h"
#include "Singleton.hpp"

#define Prm ParamLoader::getInstance()

namespace realtrick
{
    
    class ParamLoader : public Singleton<ParamLoader>
    {
        
        friend class Singleton<ParamLoader>;
        
    public:
        
        virtual ~ParamLoader() { _container.clear(); }
        
        inline double           getValueAsDouble(const std::string& key) const;
        inline float            getValueAsFloat(const std::string& key) const;
        inline int              getValueAsInt(const std::string& key) const;
        inline std::string      getValueAsString(const std::string& key) const;
        inline bool             getValueAsBool(const std::string& key) const;
        
    private:
        
        void                        _parse();
        void                        _removeCommentFromLine(std::string& line);
		void                        _removeEndlineFromLine(std::string& line);
        ParamLoader();
        
    private:
        
        std::unordered_map<std::string, std::string>        _container;
        std::string                                         _fileName;
        std::string                                         _fileData;
        std::string::size_type                              _pointer;
        
    };
    
}

#include "ParamLoader.inl"









