
#include <Engine/Core/Settings.h>

namespace env
{
    void CSettings::Load(WindowHandle hWnd)
    {
        bool bSaveScript = false;
        
        CScript* pSettings = gScriptMgr->AddGetPtr(m_strFilename);

        CScript* pResourceSettings = pSettings->GetSubscript("ResourceSettings");
        if (!pResourceSettings)
        {
            pResourceSettings = pSettings->AddSubscript("ResourceSettings");
            bSaveScript = true;
        }

        if (!pResourceSettings->Exists("ResourceRootDir"))
        {
            (*pResourceSettings)["ResourceRootDir"] = m_resourceSettings.strResourceRootDir;
            bSaveScript = true;
        }

        if (!pResourceSettings->Exists("ResourceMeshesDir"))
        {
            (*pResourceSettings)["ResourceMeshesDir"] = m_resourceSettings.strResourceMeshesDir;
            bSaveScript = true;
        }

        if (!pResourceSettings->Exists("ResourceFontsDir"))
        {
            (*pResourceSettings)["ResourceFontsDir"] = m_resourceSettings.strResourceFontsDir;
            bSaveScript = true;
        }

        if (!pResourceSettings->Exists("ResourceScriptsDir"))
        {
            (*pResourceSettings)["ResourceScriptsDir"] = m_resourceSettings.strResourceScriptsDir;
            bSaveScript = true;
        }

        if (!pResourceSettings->Exists("ResourceShadersDir"))
        {
            (*pResourceSettings)["ResourceShadersDir"] = m_resourceSettings.strResourceShadersDir;
            bSaveScript = true;
        }

        if (!pResourceSettings->Exists("ResourceTexturesDir"))
        {
            (*pResourceSettings)["ResourceTexturesDir"] = m_resourceSettings.strResourceTexturesDir;
            bSaveScript = true;
        }

        m_resourceSettings.strResourceRootDir = pResourceSettings->GetString("ResourceRootDir");
        m_resourceSettings.strResourceScriptsDir = pResourceSettings->GetString("ResourceScriptsDir");
        m_resourceSettings.strResourceTexturesDir = pResourceSettings->GetString("ResourceTexturesDir");
        m_resourceSettings.strResourceMeshesDir = pResourceSettings->GetString("ResourceMeshesDir");
        m_resourceSettings.strResourceFontsDir = pResourceSettings->GetString("ResourceFontsDir");
        m_resourceSettings.strResourceShadersDir = pResourceSettings->GetString("ResourceShadersDir");
        

        CScript* pVideoSettings = pSettings->GetSubscript("VideoSettings");
        if (!pVideoSettings)
        {
            pVideoSettings = pSettings->AddSubscript("VideoSettings");
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("ADAPTER"))
        {
            (*pVideoSettings)["ADAPTER"] = m_videoParams.Adapter;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("ZNEAR"))
        {
            (*pVideoSettings)["ZNEAR"] = m_videoParams.zNear;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("ZFAR"))
        {
            (*pVideoSettings)["ZFAR"] = m_videoParams.zFar;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("BLOOM"))
        {
            (*pVideoSettings)["BLOOM"] = m_videoParams.bloomQuality;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("VSYNC"))
        {
            (*pVideoSettings)["VSYNC"] = m_videoParams.bVSync;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("WINDOWED"))
        {
            (*pVideoSettings)["WINDOWED"] = m_videoParams.bWindowed;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("LIGHTRAYS"))
        {
            (*pVideoSettings)["LIGHTRAYS"] = m_videoParams.bLightRays;
            bSaveScript = true;
        }

        RECT r;
        
#ifdef ENV_PLATFORM_WINDOWS
        if (hWnd)
        {
            GetWindowRect(hWnd, &r);
        }
        else
            GetWindowRect(GetDesktopWindow(), &r);
#endif // ENV_PLATFORM_WINDOWS

        int screenWidth = (int)r.right - (int)r.left;
        int screenHeight = (int)r.bottom - (int)r.top;

        if (!pVideoSettings->Exists("SCREENWIDTH"))
        {
            (*pVideoSettings)["SCREENWIDTH"] = screenWidth;
            bSaveScript = true;
        }

        if (!pVideoSettings->Exists("SCREENHEIGHT"))
        {
            (*pVideoSettings)["SCREENHEIGHT"] = screenHeight;
            bSaveScript = true;
        }

        m_videoParams.Adapter = (*pVideoSettings)["ADAPTER"];
        m_videoParams.zNear = (*pVideoSettings)["ZNEAR"];
        m_videoParams.zFar = (*pVideoSettings)["ZFAR"];
        m_videoParams.bloomQuality = (*pVideoSettings)["BLOOM"];
        m_videoParams.bVSync = (*pVideoSettings)["VSYNC"];
        m_videoParams.bWindowed = (*pVideoSettings)["WINDOWED"];
        m_videoParams.screenWidth = (*pVideoSettings)["SCREENWIDTH"];
        m_videoParams.screenHeight = (*pVideoSettings)["SCREENHEIGHT"];
        m_videoParams.bLightRays = (*pVideoSettings)["LIGHTRAYS"];


        CScript* pAudioSettings = pSettings->GetSubscript("AudioSettings");
        if (!pAudioSettings)
        {
            pAudioSettings = pSettings->AddSubscript("AudioSettings");
            bSaveScript = true;
        }

        if (!pAudioSettings->Exists("SFX_VOLUME"))
        {
            (*pAudioSettings)["SFX_VOLUME"] = m_audioParams.sfxVolume;
            bSaveScript = true;
        }

        if (!pAudioSettings->Exists("MUSIC_VOLUME"))
        {
            (*pAudioSettings)["MUSIC_VOLUME"] = m_audioParams.musicVolume;
            bSaveScript = true;
        }

        m_audioParams.musicVolume = (*pAudioSettings)["MUSIC_VOLUME"];
        m_audioParams.sfxVolume = (*pAudioSettings)["SFX_VOLUME"];


        if (bSaveScript)
            pSettings->SaveScript();

        gScriptMgr->Release(m_strFilename);

    }

    void CSettings::Save()
    {
        CScript* pSettings = gScriptMgr->AddGetPtr(m_strFilename);

        CScript* pResourceSettings = pSettings->GetSubscript("ResourceSettings");
        if (!pResourceSettings)
            pResourceSettings = pSettings->AddSubscript("ResourceSettings");

        (*pResourceSettings)["ResourceRootDir"] = m_resourceSettings.strResourceRootDir;
        (*pResourceSettings)["ResourceScriptsDir"] = m_resourceSettings.strResourceScriptsDir;
        (*pResourceSettings)["ResourceTexturesDir"] = m_resourceSettings.strResourceTexturesDir;
        (*pResourceSettings)["ResourceMeshesDir"] = m_resourceSettings.strResourceMeshesDir;
        (*pResourceSettings)["ResourceFontsDir"] = m_resourceSettings.strResourceFontsDir;
        (*pResourceSettings)["ResourceShadersDir"] = m_resourceSettings.strResourceShadersDir;

        CScript* pVideoSettings = pSettings->GetSubscript("VideoSettings");
        if (!pVideoSettings)
            pVideoSettings = pSettings->AddSubscript("VideoSettings");

        (*pVideoSettings)["ADAPTER"] = m_videoParams.Adapter;
        (*pVideoSettings)["ZNEAR"] = m_videoParams.zNear;
        (*pVideoSettings)["ZFAR"] = m_videoParams.zFar;
        (*pVideoSettings)["BLOOM"] = m_videoParams.bloomQuality;
        (*pVideoSettings)["VSYNC"] = m_videoParams.bVSync;
        (*pVideoSettings)["WINDOWED"] = m_videoParams.bWindowed;
        (*pVideoSettings)["LIGHTRAYS"] = m_videoParams.bLightRays;
        (*pVideoSettings)["SCREENWIDTH"] = m_videoParams.screenWidth;
        (*pVideoSettings)["SCREENHEIGHT"] = m_videoParams.screenHeight;

        CScript* pAudioSettings = pSettings->GetSubscript("AudioSettings");
        if (!pAudioSettings)
            pAudioSettings = pSettings->AddSubscript("AudioSettings");

        (*pAudioSettings)["SFX_VOLUME"] = m_audioParams.sfxVolume;
        (*pAudioSettings)["MUSIC_VOLUME"] = m_audioParams.musicVolume;

        pSettings->SaveScript();

        gScriptMgr->Release(m_strFilename);
    }
} // env