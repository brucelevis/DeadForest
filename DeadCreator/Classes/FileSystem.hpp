//
//  FileSystem.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#ifndef FileSystem_hpp
#define FileSystem_hpp

#include <vector>

#include <boost/filesystem.hpp>

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
  
    static bool isDirectory(const std::string& path);
    
    static bool isFile(const std::string& path);
    
};

#endif /* FileSystem_hpp */
