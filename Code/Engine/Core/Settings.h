/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Engine/EngineCommon.h>
#include <Graphic/Core/RenderQualityParams.h>

namespace env
{
    struct ResourceSettings
    {
        ResourceSettings()
            : strResourceRootDir("ResourceRootDir")
            , strResourceMeshesDir("ResourceMeshesDir")
            , strResourceFontsDir("ResourceFontsDir")
            , strResourceScriptsDir("ResourceScriptsDir")
            , strResourceShadersDir("ResourceShadersDir")
            , strResourceTexturesDir("ResourceTexturesDir") {}

        std::string strResourceRootDir;
        std::string strResourceMeshesDir;
        std::string strResourceFontsDir;
        std::string strResourceScriptsDir;
        std::string strResourceShadersDir;
        std::string strResourceTexturesDir;
    };

    struct AudioParameters
    {
        AudioParameters()
            : sfxVolume(50)
            , musicVolume(10) {}

        int sfxVolume;
        int musicVolume;
    };


    class CSettings
    {
    public:
        CSettings(const std::string& strFilename)
            : m_strFilename(strFilename) {}

        void Load(WindowHandle hWnd = 0);
        void Save();

        ResourceSettings m_resourceSettings;
        RenderQualityParams m_videoParams;
        AudioParameters m_audioParams;

        void SetFilename(const std::string& strFilename) { m_strFilename = strFilename; }

    private:
        std::string m_strFilename;
    };

} // env

#endif // SETTINGS_H
