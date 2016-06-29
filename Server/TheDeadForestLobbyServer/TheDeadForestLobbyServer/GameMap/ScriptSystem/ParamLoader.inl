#include "ParamLoader.hpp"

namespace realtrick
{
    
    inline double ParamLoader::getValueAsDouble(const std::string& key) const
    {
        try {
            return atof(_container.at(key).c_str());
        } catch (std::exception& e) {
            std::cout << "[\"" << key.c_str() << "\"] " << e.what() << std::endl;
            return 0.0;
        }
    }
    
    inline float ParamLoader::getValueAsFloat(const std::string& key) const
    {
        try {
            return (float)atof(_container.at(key).c_str());
        } catch (std::exception& e) {
			std::cout << "[\"" << key.c_str() << "\"] " << e.what() << std::endl;
            return 0.0f;
        }
    }
    
    inline int ParamLoader::getValueAsInt(const std::string& key) const
    {
        try {
            return atoi(_container.at(key).c_str());
        } catch (std::exception& e) {
			std::cout << "[\"" << key.c_str() << "\"] " << e.what() << std::endl;
            return 0;
        }
    }
    
    inline std::string ParamLoader::getValueAsString(const std::string& key) const
    {
        try {
            return _container.at(key);
        } catch (std::exception& e) {
			std::cout << "[\"" << key.c_str() << "\"] " << e.what() << std::endl;
            return 0;
        }
    }
    
    inline bool ParamLoader::getValueAsBool(const std::string& key) const
    {
        try {
            return _container.at(key) == "true" ? true : false;
        } catch (std::exception& e) {
            std::cout<<"[\"" << key.c_str() << "\"] " << e.what() << std::endl;
            return false;
        }
    }
    
}





