
#ifdef ENV_PLATFORM_ORBIS

#include "FileSystem.h"
#include <algorithm>

namespace env
{
    //-----------------------------------------------------------------------------------
    bool GetFileExists(const std::string& strFilename)
    {
        return false;
    }

    //-----------------------------------------------------------------------------------
    void GetRootDir(std::string& strDir)
    {
    }

    //-----------------------------------------------------------------------------------
    void GetFilenameRelativeToRoot( std::string& strFilename )
    {
    }

    //-----------------------------------------------------------------------------------
    bool GetFileLastWriteTime(const std::string& strFilename, FILETIME* pFT) 
    { 
        return false; 
    }

} // env


#endif // ENV_PLATFORM_ORBIS