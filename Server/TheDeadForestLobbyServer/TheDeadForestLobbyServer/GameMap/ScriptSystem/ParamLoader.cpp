
#include "ParamLoader.hpp"

namespace realtrick
{
    
    //
    // Constructor
    //
    ParamLoader::ParamLoader()
    {
        
        _fileName = "Params.ini";

		_container.clear();
		_file.open(_fileName);
		if (_file.is_open())
		{
			_parse();
		}
		else
		{
			throw std::runtime_error("<ParamLoader::ParamLoader> " + _fileName + " is not exist.");
		}

		_file.close();

    }
    
    void ParamLoader::_parse()
    {
		while (!_file.eof())
		{
			std::string line;
			std::getline(_file, line);
			_removeCommentFromLine(line);
			if (line.empty())
			{
				continue;
			}

			std::string::size_type begIdx;
			std::string::size_type endIdx;
			const std::string delims(" \\;=,");

			// find key
			begIdx = line.find_first_not_of(delims);
			if (begIdx != std::string::npos)
			{
				endIdx = line.find_first_of(delims, begIdx);
				if (endIdx == std::string::npos)
				{
					endIdx = line.size();
				}
			}
			std::string key = line.substr(begIdx, endIdx);

			// find value
			begIdx = line.find_first_not_of(delims, endIdx);
			if (begIdx != std::string::npos)
			{
				endIdx = line.find_first_of(delims, begIdx);
				if (endIdx == std::string::npos)
				{
					endIdx = line.size();
				}
			}
			std::string value = line.substr(begIdx, endIdx);

			// insert key, value
			_container.insert(std::make_pair(key, value));

		}
        
    }
    
    
    //
    // RemoveCommentFromLine
    //
    void ParamLoader::_removeCommentFromLine(std::string& line)
    {
        std::string::size_type idx = line.find("--");
        if(idx != std::string::npos)
        {
            line = line.substr(0, idx);
        }
    }
    
    
    
}





