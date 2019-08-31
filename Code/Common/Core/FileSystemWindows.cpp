#ifdef ENV_PLATFORM_WINDOWS

#include "FileSystem.h"
#include <algorithm>
#include "boost/filesystem.hpp"


namespace env
{
    //-----------------------------------------------------------------------------------
    bool GetFileExists(const std::string& strFilename)
    {
        WIN32_FIND_DATA data;
        HANDLE hFile = FindFirstFile(strFilename.c_str(), &data);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            FindClose(hFile);
            return false;
        }
        else
        {
            FindClose(hFile);
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------------------
    void GetRootDir(std::string& strDir)
    {
        char RootDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, RootDir);

        strDir = RootDir;
        strDir += "\\";
    }

    //-----------------------------------------------------------------------------------
    void GetFilenameRelativeToRoot( std::string& strFilename )
    {
        char RootDir[MAX_PATH];
        GetCurrentDirectory( MAX_PATH, RootDir );
        if( std::string::npos != strFilename.find( RootDir ))
            strFilename.erase( 0, strlen( RootDir ) + 1);
    }

    //-----------------------------------------------------------------------------------
    bool GetFileLastWriteTime(const std::string& strFilename, FILETIME* pFT)
    {
        WIN32_FIND_DATA data;
        HANDLE h = FindFirstFile(strFilename.c_str(), &data);
        if(h == INVALID_HANDLE_VALUE)
            return false;

        *pFT = data.ftLastWriteTime;

        return true;
    }

    //-----------------------------------------------------------------------------------
    void ReplaceBackwardSlashes(std::string& strFilename)
    {
        std::replace(strFilename.begin(), strFilename.end(), '\\', '/');
    }

} // env

#endif // ENV_PLATFORM_WINDOWS