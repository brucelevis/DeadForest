//
//  FileSystem.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#include "FileSystem.hpp"

std::string FileSystem::getInitialPath()
{
    return boost::filesystem::initial_path().string();
}

bool FileSystem::isExist(const std::string& path)
{
    return boost::filesystem::exists(boost::filesystem::path(path));
}

bool FileSystem::remove(const std::string& path)
{
    boost::filesystem::path p(path);
    if ( !isExist(p.native()) ) return false;
    return boost::filesystem::remove(p);
}

std::vector<std::string> FileSystem::getFilesInPath(const std::string& path, bool leaf)
{
    std::vector<std::string> ret;
    boost::filesystem::path p(path);
    if ( !isExist(p.native()) ) return ret;
    
    for ( boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator(p))
    {
        if ( boost::filesystem::is_directory(x.path()) ) continue;
        if ( leaf )
            ret.push_back(x.path().leaf().native());
        else
            ret.push_back(x.path().native());
    }
    return ret;
}

std::vector<std::string> FileSystem::getDirectoryInPath(const std::string& path, bool leaf)
{
    std::vector<std::string> ret;
    boost::filesystem::path p(path);
    if ( !isExist(p.native()) ) return ret;
        
    for ( boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator(p))
    {
        if ( !boost::filesystem::is_directory(x.path()) ) continue;
        if ( leaf)
            ret.push_back(x.path().leaf().native());
        else
            ret.push_back(x.path().native());
    }
    return ret;
}

std::string FileSystem::getLeafName(const std::string& path)
{
    boost::filesystem::path p(path);
//    if ( !isExist(p.native()) ) return "#NOT EXIST";
    return p.leaf().native();
}

std::string FileSystem::getParentPath(const std::string& path)
{
    boost::filesystem::path p(path);
    if ( !isExist(p.native()) ) return "#NOT EXIST";
    if ( p.has_parent_path() )
    {
        return p.parent_path().native();
    }
    return "#NOT EXIST";
}

bool FileSystem::createDirectory(const std::string& path)
{
    if ( isExist(path) ) return false;
    return boost::filesystem::create_directories(path);
}


