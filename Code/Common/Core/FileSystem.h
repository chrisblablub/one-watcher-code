/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <Common/Common.h>
#include <Common/Core/Macros.h>
#include <Common/Core/Log.h>
#include <Common/Core/ResMgrBase.h>

namespace env
{
    bool GetFileExists(const std::string& strFilename);

    bool GetFileExtension(const std::string& strFilename, std::string& strExtension);

    void GetRootDir(std::string& strDir);

    void GetFilenameRelativeToRoot(std::string& strFilename);

    void GetPathFromFilename(const std::string& strFilename, std::string& strPath);

    void GetNameFromFilename(const std::string& strFilename, std::string& strName);

    bool GetFileLastWriteTime(const std::string& strFilename, FILETIME* pFT);

    void ReplaceBackwardSlashes(std::string& strFilename);

   // bool GetFileLastWriteTime(const std::string& strFilename, FILETIME* pFT);
} // env 
#endif // FILE_SYSTEM_H