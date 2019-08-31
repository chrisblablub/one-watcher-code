/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <Engine/EngineConfig.h>
#include <Engine/Core/State.h>
#include <Engine/Core/Settings.h>

namespace env
{
    struct EngineSetup
    {
        EngineSetup()
            : hInstance(0)
            , hWnd(0)
            , bEditor(false)
            , bMusic(true)
            , strSettingsFilename("Settings.txt")
            , strSettingsPath("../Resources/Scripts/") {}

        HINSTANCE	hInstance;
        WindowHandle hWnd;

        std::string strGameName;
        std::string strAppAbsRootPath;
        std::string strSettingsFilename;
        std::string strSettingsPath;

        bool bEditor;
        bool bMusic;
    };

    struct RenderQueue;
    struct RenderQualityParams;

    class CLevelLoader;
    class IStateMgr;
    class CInput;
    class CWorldMgr;
    class CRenderSystem;
    class CParticleSystemMgr;
    class CEntityMgr;
    class CSceneMgr;
    class CScript;
    class CAudioSystem;
    class CRenderWindow;

    class CEngine final
    {
        friend class CState;
    public:
        CEngine();
        CEngine(const CEngine&) = delete;
        CEngine& operator=(const CEngine&) = delete;
        ~CEngine();

        template<class State> CState* AddState();

        void RemoveState(const std::string& strName);

        CState*	GetCurrentState() { return m_pStateCurrent; }
        CState* GetState(const std::string& strName);

        void ShutDown() { m_bShutDown = true; }
        bool IsShuttingDown() const { return m_bShutDown; }

        bool Init(EngineSetup& Setup);
        int	Run();

        bool GameLoop();
        void ResetDevice();
        void ResetDevice(RenderQualityParams videoParams);

        void SetNewState(const std::string& strName);
        const std::string& GetNewState() const { return m_strNewState; }

        inline CSettings* GetSettings() const { return m_pSettings.get(); }
        inline CInput* GetInput() const { return m_pInput.get(); }
        inline CWorldMgr* GetWorldMgr() const { return m_pWorldMgr.get(); }
        inline CParticleSystemMgr* GetParticleSystemMgr() const { return m_pParticleSystemMgr.get(); }
        inline CEntityMgr* GetEntityMgr() const { return m_pEntityMgr.get(); }
        inline CSceneMgr* GetSceneMgr() const { return m_pSceneMgr.get(); }
        inline CAudioSystem* GetAudioSystem() const { return m_pAudioSystem.get(); }
        inline CLevelLoader& GetLevelLoader() const { return *m_pLevelLoader; }
        inline CRenderWindow* GetRenderWindow() const { return m_pRenderWindow.get(); }

        inline void SetPaused(bool bPaused) { m_bPaused = bPaused; }
        inline bool GetPaused() const { return m_bPaused; }

        inline void SetResetDeviceQueued(int w, int h) { m_width = w; m_height = h; m_bResetDeviceQueued = true; }

        inline void SetReloadResources(bool bReload) { m_bReloadResources = bReload; }
        inline bool GetReloadResources() const { return m_bReloadResources; }

        inline long GetFramerate() const { return m_framerate; }
        inline bool GetEditor() { return m_EngineSetup.bEditor; }
        inline float GetTimeElapsed() const { return m_timeElapsed; }
        inline void Pause(bool bPause = true) { m_bPaused = bPause; }
        void ChangeState(const std::string& strName);
    private:

        bool InitResourceManagers();
        bool InitSystems();
        bool InitPerformanceCounter();
        bool InitStates();
        bool InitAudio();

        void OnLostDevice();
        void OnResetDevice();
        bool IsDeviceLost();

        void UpdateFramerate();
        void UpdateProfiler();
        bool UpdateGameLoader(RenderQueue& renderQueue);



    private:

        CState*	m_pStateCurrent;

        std::string m_strNewState;

        bool m_bChangeState;
        bool m_bLoadingStarts;
        bool m_bLoadingFinishes;
        bool m_bShutDown;
        bool m_bPaused;
        bool m_bReloadResources;
        bool m_bResetDeviceQueued;

        int m_numFramesRunning;

        long m_framerate;
        long m_numFrames;

        envS64 m_frequency;
        envS64 m_frequencyCurCount;
        envS64 m_frequencyLastCount;
        envS64 m_frequencyLastFrameCount;

        unsigned long   m_prevTickCount;

        float   m_timeElapsed;
        float   m_time;

        std::vector<double> m_prevTimeElapsed;

        int m_width;
        int m_height;

        struct ResMgrContainerImpl;

        std::map<std::string, std::unique_ptr<CState>> m_states;

        std::unique_ptr<CLog> m_pLog;
        std::unique_ptr<CScriptMgr> m_pScriptMgr;
        std::unique_ptr<CSettings> m_pSettings;
        std::unique_ptr<ResMgrContainerImpl> m_pResMgrContainer;
        std::unique_ptr<CAudioSystem> m_pAudioSystem;
        std::unique_ptr<CInput> m_pInput;
        std::unique_ptr<CSceneMgr> m_pSceneMgr;
        std::unique_ptr<CEntityMgr> m_pEntityMgr;
        std::unique_ptr<CParticleSystemMgr> m_pParticleSystemMgr;
        std::unique_ptr<CRenderSystem> m_pRenderSystem;
        std::unique_ptr<CRenderWindow> m_pRenderWindow;
        std::unique_ptr<CWorldMgr> m_pWorldMgr;
        std::unique_ptr<CLevelLoader> m_pLevelLoader;



        EngineSetup m_EngineSetup;

        std::function<void()> m_EndOfFrameCallback;
        std::function<void()> m_DeathCallback;
        std::function<void(int, int, bool)> m_ChapterProgressCallback;
    public:
        void SetOnDeathCallback(void(*callback)()) { m_DeathCallback = callback; }
        void SetOnChapterProgressCallback(void(*callback)(int, int, bool)) { m_ChapterProgressCallback = callback; }
        void SetEndOfFrameCallback(void(*callback)()) { m_EndOfFrameCallback = callback; }

        void TriggerEndOfFrameCallback();
        void TriggerDeathCallback();
        void TriggerChapterProgressCallback(const int chapter, const int numberOfLevelBested, bool bossFinished);
    };

    //-----------------------------------------------------------------------------------
    template<class State> CState* CEngine::AddState()
    {
        m_states[State::STATE_NAME] = std::make_unique<State>();

        return m_states[State::STATE_NAME].get();
    }

    extern CEngine* gEngine;



} // env

#endif // ENGINE_H 