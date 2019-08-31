/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#include <Common/Common.h>
#include <string>

namespace env
{
    //-----------------------------------------------------------------------------------
    inline std::string GetTruncatedString(std::string strText, unsigned int length, char fillCharacter = 32)
    {
        if (strText.length() >= length)
            strText = strText.substr(strText.length() - length, length);

        char* pFileName = new char[length + 1];
        // reset to fillCharacter
        memset(pFileName, fillCharacter, length + 1);
        // copy string
        memcpy(pFileName, strText.c_str(), strText.length());
        // null termination
        pFileName[length] = '\0';

        std::string str(pFileName);

        ENV_DELETE_ARRAY(pFileName);

        return str;
    }
} // env 
#endif // STRING_UTILITY_H