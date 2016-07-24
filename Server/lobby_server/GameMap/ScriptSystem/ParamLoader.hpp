#pragma once

#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>

#include "Singleton.h"

namespace realtrick
{
    
#define Prm ParamLoader::getInstance()
    
    class ParamLoader : public Singleton<ParamLoader>
    {
        
    public:
        
        virtual ~ParamLoader() { 
			if (_file.is_open())
				_file.close();
			_container.clear();
		}
        
        inline double           getValueAsDouble(const std::string& key) const;
        
        inline float            getValueAsFloat(const std::string& key) const;
        
        inline int              getValueAsInt(const std::string& key) const;
        
        inline std::string      getValueAsString(const std::string& key) const;
        
        inline bool             getValueAsBool(const std::string& key) const;
        
        friend Singleton<ParamLoader>;
        
    private:

		std::ifstream										_file;
        
        std::unordered_map<std::string, std::string>        _container;
        
        std::string                                         _fileName;

        std::string                                         _fileData;
        
    private:
        
        void                        _parse();
        
        void                        _removeCommentFromLine(std::string& line);
        
        ParamLoader();
        
    };
    
}

#include "ParamLoader.inl"





