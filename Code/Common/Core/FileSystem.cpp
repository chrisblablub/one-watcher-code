#include "FileSystem.h"
#include <algorithm>

namespace env
{
    //-----------------------------------------------------------------------------------
    bool GetFileExtension(const std::string& strFilename, std::string& strExtension)
    {
        std::string::size_type pos = strFilename.find_last_of('.');

        if (pos >= strFilename.npos)
            return false;

        strExtension = strFilename.substr(pos, strFilename.npos - pos);
        return true;
    }

    //-----------------------------------------------------------------------------------
    void GetPathFromFilename( const std::string& strFilename, std::string& strPath )
    {
        strPath = strFilename;

        size_t pos;
        pos = 1;
        while( std::string::npos != strPath.find( "\\", pos + 2 ) 
            || std::string::npos != strPath.find( "/", pos + 2 ))
        {
            if( (pos = strPath.find( "\\", pos + 2 )) == std::string::npos )
                pos = strPath.find( "/", pos + 2 );
        }

        if( pos == 1 )
            strPath = "";
        else if( std::string::npos != pos )
            strPath.erase( pos + 1, strlen( strPath.c_str() ) - (pos + 1) );
    }

    //-----------------------------------------------------------------------------------
    void GetNameFromFilename(const std::string& strFilename, std::string& strName)
    {
        strName = strFilename;

        size_t pos = strName.find_last_of("\\");

        if (pos == std::string::npos)
            pos = strName.find_last_of("/");

        if (std::string::npos != pos && pos != 1)
            strName.erase(0, pos + 1);
    }

} // env