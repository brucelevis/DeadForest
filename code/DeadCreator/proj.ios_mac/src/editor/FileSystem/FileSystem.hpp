//
//  FileSystem.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#pragma once

#include <vector>

#include <boost/filesystem.hpp>

namespace realtrick
{
    
    class FileSystem
    {
        
    public:
        
        static std::string getInitialPath();
        
        static bool isExist(const std::string& path);
        
        static bool remove(const std::string& path);
        
        static std::vector<std::string> getFilesInPath(const std::string& path, bool leaf = false);
        
        static std::vector<std::string> getDirectoryInPath(const std::string& path, bool leaf = false);
        
        static std::string getLeafName(const std::string& path);
        
        static std::string getParentPath(const std::string& path);
        
        static std::vector<std::string> getFilesAndDirectoriesInPath(const std::string& path, bool leaf = false);
        
        static bool createDirectory(const std::string& path);
        
    };
    
}









