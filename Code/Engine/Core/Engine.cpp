#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/ParticleSystemMgr.h>
#include <Engine/Core/GameLoader.h>
#include <Engine/State/GameState.h>
#include <Engine/State/EditorState.h>
#include <Engine/State/MenueState.h>
#include <Engine/State/LevelSelectionState.h>
#include <Engine/State/PauseMenueState.h>
#include <Engine/State/SettingsState.h>
#include <Engine/State/CreditsState.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Audio/AudioSystem.h>
#include <Engine/Core/WndProc.h>
#include <Graphic/RenderSystems/DeferredRenderSystem.h>
#include <Graphic/Renderer/SpriteRenderer.h>
#include <Graphic/Core/RenderWindow.h>
#include <Common/Core/FilePackageManager.h>

namespace env
{
    struct CEngine::ResMgrContainerImpl
    {
        std::unique_ptr<CTextureMgr> m_pTextureMgr;
        std::unique_ptr<CMaterialMgr> m_pMaterialMgr;
        std::unique_ptr<CMeshMgr> m_pMeshMgr;
        std::unique_ptr<CFontMgr> m_pFontMgr;
        std::unique_ptr<CShaderMgr> m_pShaderMgr;
        std::unique_ptr<CConstantBufferGPUMgr> m_pConstantBufferMgr;
    };

    CEngine* gEngine = nullptr;

    //-----------------------------------------------------------------------------------
    CEngine::CEngine()
        : m_frequency(0)
        , m_frequencyLastCount(0)
        , m_frequencyCurCount(0)
        , m_frequencyLastFrameCount(0)
        , m_time(0)
        , m_framerate(0)
        , m_numFrames(0)
        , m_numFramesRunning(0)
        , m_timeElapsed(0.0f)
        , m_pStateCurrent(nullptr)
        , m_bChangeState(false)
        , m_bLoadingStarts(false)
        , m_bLoadingFinishes(false)
        , m_bShutDown(false)
        , m_bPaused(false)
        , m_bReloadResources(true)
        , m_bResetDeviceQueued(false)
    {
        gEngine = this;
        std::srand(time(nullptr));
    }

    //-----------------------------------------------------------------------------------
    CEngine::~CEngine()
    {
        CSingletonPlayerMgr playerMgr;
        playerMgr->Save();

        m_pWorldMgr->Unload();

        if (m_pAudioSystem)
        {
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_00);
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_01);
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_02);
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_03);
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_04);
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_05);
            m_pAudioSystem->Release(AUDIO_MUSIC_TRACK_06);

            m_pAudioSystem->Release(AUDIO_EFFECT_TRANSITION_IN);
            m_pAudioSystem->Release(AUDIO_EFFECT_MENUE_SELECTION);
            m_pAudioSystem->Release(AUDIO_EFFECT_MENUE_CONFIRMATION);

            m_pAudioSystem->ReleaseAll();
        }

        ENV_DELETE(gTexturePackageMgr);
        ENV_DELETE(gSoundPackageMgr);

        m_states.clear();

        ENV_LOG_INFO("Engine destructed.");

#ifdef ENV_GRAPHIC_API_D3D11
                CoUninitialize();
#endif // ENV_GRAPHIC_API_D3D11
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::InitResourceManagers()
    {
        ENV_LOG_INFO("INITIALIZING RESOURCE MANAGERS");

        m_pScriptMgr = std::make_unique<CScriptMgr>();
        m_pSettings = std::make_unique<CSettings>(m_EngineSetup.strAppAbsRootPath + m_EngineSetup.strSettingsPath + m_EngineSetup.strSettingsFilename);

        gScriptMgr = m_pScriptMgr.get();

        if (!m_pSettings)
            return false;

        m_pSettings->Load();

        if (m_EngineSetup.bEditor)
            m_pSettings->m_videoParams.bWindowed = true;

        gScriptMgr->SetResourceRoot(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceScriptsDir);

        m_pResMgrContainer = std::make_unique<ResMgrContainerImpl>();
        m_pResMgrContainer->m_pTextureMgr = std::make_unique<CTextureMgr>(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceTexturesDir);
        m_pResMgrContainer->m_pMaterialMgr = std::make_unique<CMaterialMgr>(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceTexturesDir);
        m_pResMgrContainer->m_pMeshMgr = std::make_unique<CMeshMgr>(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceMeshesDir);
        m_pResMgrContainer->m_pFontMgr = std::make_unique<CFontMgr>(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceFontsDir);
        m_pResMgrContainer->m_pShaderMgr = std::make_unique<CShaderMgr>(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceShadersDir + "D3D11/");
        m_pResMgrContainer->m_pConstantBufferMgr = std::make_unique<CConstantBufferGPUMgr>(m_pSettings->m_resourceSettings.strResourceRootDir + m_pSettings->m_resourceSettings.strResourceShadersDir + "D3D11/");

        gTextureMgr = m_pResMgrContainer->m_pTextureMgr.get();
        gMaterialMgr = m_pResMgrContainer->m_pMaterialMgr.get();
        gMeshMgr = m_pResMgrContainer->m_pMeshMgr.get();
        gFontMgr = m_pResMgrContainer->m_pFontMgr.get();
        gShaderMgr = m_pResMgrContainer->m_pShaderMgr.get();
        gConstBufMgr = m_pResMgrContainer->m_pConstantBufferMgr.get();

        m_pSettings->SetFilename(m_EngineSetup.strSettingsFilename);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::InitSystems()
    {
        ENV_LOG_INFO("INITIALIZING SYSTEMS");

        //MessageBox(NULL, "Attach debugger...", "...", MB_OK);

        gSoundPackageMgr = new CFilePackageManager();
        gTexturePackageMgr = new CFilePackageManager();

#ifndef ENV_FINAL
#ifdef ENV_CREATE_FILE_PACKAGES
        gSoundPackageMgr->CreateFilePackage("../Resources/Audio", "Audio");
        gTexturePackageMgr->CreateFilePackage("../Resources/Textures", "Texture");
#endif // CREATE_FILE_PACKAGES
#endif // ENV_FINAL

#ifdef ENV_USE_FILE_PACKAGES
        gSoundPackageMgr->LoadPackage(AUDIO_PACKAGE_NAME);
        gTexturePackageMgr->LoadPackage(TEXTURE_PACKAGE_NAME);
#endif // ENV_USE_FILE_PACKAGES

        RenderSystemParams renderSysParams;

        m_pInput = std::make_unique<CInput>();
        m_pSceneMgr = std::make_unique<CSceneMgr>();
        m_pEntityMgr = std::make_unique<CEntityMgr>();
        m_pParticleSystemMgr = std::make_unique<CParticleSystemMgr>();
        m_pWorldMgr = std::make_unique<CWorldMgr>();

#ifdef ENV_PLATFORM_WINDOWS


        renderSysParams.m_windowHandle = m_EngineSetup.hWnd;

        if (!renderSysParams.m_windowHandle)
        {
            m_pRenderWindow = std::make_unique<CRenderWindow>();

            if (!m_pRenderWindow->Create(m_EngineSetup.hInstance,
                                         m_pSettings->m_videoParams.screenWidth,
                                         m_pSettings->m_videoParams.screenHeight,
                                         m_pSettings->m_videoParams.bWindowed,
                                         WndMsgProc))
            {
                return false;
            }
        }

        if (m_pRenderWindow)
            renderSysParams.m_windowHandle = m_pRenderWindow->GetHandle();

        m_pInput->SetHwnd(renderSysParams.m_windowHandle);

        m_pRenderSystem = std::make_unique<CRenderSystem>();
#endif // ENV_PLATFORM_WINDOWS


        if (m_EngineSetup.bEditor)
            m_pSettings->m_videoParams.bVSync = false;

        if (!m_pRenderSystem->Initialize(m_pSettings->m_videoParams, renderSysParams))
            return false;

        gTextureMgr->SetAllocPolicy(CTextureManaged::TextureAllocationPolicy(m_pRenderSystem->GetRenderDevice()));
        gShaderMgr->SetAllocPolicy(CShader::ShaderAllocationPolicy(m_pRenderSystem->GetRenderDevice()));
        gConstBufMgr->SetAllocPolicy(CConstantBufferGPU::ConstantBufferAllocationPolicy(m_pRenderSystem->GetRenderDevice()));

        if (!m_pRenderSystem->InitializeRenderers())
            return false;

        m_pParticleSystemMgr->Initialize();

        m_pLevelLoader = std::make_unique<CLevelLoader>();
        m_pLevelLoader->Initialize();

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::InitPerformanceCounter()
    {
        ENV_LOG_INFO("INITIALIZING PERFORMANCE COUNTER");

#ifdef ENV_PLATFORM_WINDOWS

        if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency))
        {
            ENV_LOG_ERROR("Could not query the performance frequency.");
            return false;
        }

        if (!QueryPerformanceCounter((LARGE_INTEGER*)&m_frequencyCurCount))
        {
            ENV_LOG_ERROR("Could not query the performance counter.");
            return false;
        }

        m_frequencyLastCount = m_frequencyCurCount;
        m_frequencyLastFrameCount = m_frequencyCurCount;

        m_prevTickCount = GetTickCount();

#endif // ENV_PLATFORM_WINDOWS

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::InitStates()
    {
        ENV_LOG_INFO("INITIALIZING STATES");

        AddState<CGameState>();
        AddState<CEditorState>();
        AddState<CMenueState>();
        AddState<CLevelSelectionState>();
        AddState<CPauseMenueState>();
        AddState<CSettingsState>();
        AddState<CCreditsState>();

        ENV_LOG_INFO("INITIALIZING STATES SUCCEEDED");

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::InitAudio()
    {
        m_pAudioSystem = std::make_unique<CAudioSystem>("../Resources/Audio/");

        if (!m_pAudioSystem->Initialize())
            return false;

        m_pAudioSystem->AddChannelGroup(AUDIO_CHANNELGROUP_EFFECTS, "Effects");
        m_pAudioSystem->AddChannelGroup(AUDIO_CHANNELGROUP_MUSIC, "Music");
        m_pAudioSystem->AddChannelGroup(AUDIO_CHANNELGROUP_AMBIENT, "Ambient");
        m_pAudioSystem->AddChannelGroup(AUDIO_CHANNELGROUP_VOICE, "Voice");
		m_pAudioSystem->AddChannelGroup(AUDIO_CHANNELGROUP_EFFECTS_PREVIEW, "EffectsPreview");
		m_pAudioSystem->AddChannelGroup(AUDIO_CHANNELGROUP_MUSIC_PREVIEW, "MusicPreview");

        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_00);
        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_01)->SetVolume(1.3f);
        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_02);
        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_03)->SetVolume(1.3f);
        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_04)->SetVolume(1.3f);
        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_05)->SetVolume(1.3f);
        m_pAudioSystem->AddGetPtr(AUDIO_MUSIC_TRACK_06)->SetVolume(1.3f);

        m_pAudioSystem->AddGetPtr(AUDIO_EFFECT_TRANSITION_IN);
        m_pAudioSystem->AddGetPtr(AUDIO_EFFECT_MENUE_SELECTION);
        m_pAudioSystem->AddGetPtr(AUDIO_EFFECT_MENUE_CONFIRMATION);

//         if (m_EngineSetup.bMusic == false || m_EngineSetup.bEditor)
//         {
//             m_pAudioSystem->SetMute(AUDIO_CHANNELGROUP_MUSIC, true);
//         }

        m_pAudioSystem->SetAudioParameters(m_pSettings->m_audioParams);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::Init(EngineSetup& setup)
    {
        //        MessageBox(NULL, "Attach Debugger...", "", MB_OK);

        m_EngineSetup = setup;

        if (m_EngineSetup.strGameName.empty())
            m_EngineSetup.strGameName = "Envisioned Engine";

        m_pLog = std::make_unique<CLog>(false, "Log.txt");

        gLog = m_pLog.get();

#ifdef ENV_GRAPHIC_API_D3D11
        CoInitialize(NULL);
#endif // ENV_GRAPHIC_API_D3D11

        ENV_LOG_INFO("INITIALIZING ENGINE");

        if (!InitResourceManagers())
        {
            ENV_LOG_ERROR("INITIALIZING RESOURCE MANAGERS FAILED");
            return false;
        }

        if (!InitSystems())
        {
            ENV_LOG_ERROR("INITIALIZING SYSTEMS FAILED");
            return false;
        }

        if (!InitAudio())
        {
            ENV_LOG_ERROR("INITIALIZING AUDIO FAILED");
            return false;
        }

        if (!InitPerformanceCounter())
        {
            ENV_LOG_ERROR("INITIALIZING PERFORMANCE COUNTER FAILED");
            return false;
        }

        if (!InitStates())
        {
            ENV_LOG_ERROR("INITIALIZING STATES FAILED");
            return false;
        }

        CSingletonPlayerMgr playerMgr;
        playerMgr->Load();

        OnResetDevice();

        ENV_PLATFORM_WINDOWS_ONLY(if (m_pRenderWindow) m_pRenderWindow->Show(););

        if (!m_EngineSetup.bEditor)
        {
            m_pLevelLoader->LoadLevel("../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env", "MenueState");
            //m_pLevelLoader->LoadLevel("../Resources/Levels/Chapter1/menue.env", "MenueState");
            //m_pLevelLoader->LoadLevel("../Resources/Levels/Chapter1/chapter1_level02.env", "GameState");
            //SetNewState("GameState");
        }

        ENV_LOG_INFO("INITIALIZING ENGINE SUCCEEDED");

        return true;
    }

    //-----------------------------------------------------------------------------------
    int CEngine::Run()
    {
        m_bShutDown = false;

        ENV_PLATFORM_WINDOWS_ONLY(MSG msg = { 0 };);

        ENV_REL_CODE(CProfilingMgr& profilingMgr = CProfilingMgr::GetInstance());

        while (true)
        {
#ifdef ENV_PLATFORM_WINDOWS
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    //m_bShutDown = true;
                    //break;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

#endif // ENV_PLATFORM_WINDOWS

            if (m_bShutDown)
                break;

            // Enter the Game-Loop 
            UpdateProfiler();

            ENV_REL_CODE(profilingMgr.BeginProfilingFrame());

            if (!GameLoop())
            {
                break;
            }

            ENV_REL_CODE(profilingMgr.EndProfilingFrame(m_timeElapsed));
            TriggerEndOfFrameCallback();
        }

        return 0;
    }

    //-----------------------------------------------------------------------------------
    void CEngine::UpdateFramerate()
    {
        ENV_PLATFORM_WINDOWS_ONLY(QueryPerformanceCounter((LARGE_INTEGER*)&m_frequencyCurCount););

        if (m_frequencyCurCount - m_frequencyLastCount > m_frequency)
        {
            m_frequencyLastCount = m_frequencyCurCount;
            m_framerate = m_numFrames;
            m_numFrames = 0;

            if (0 == m_framerate)
            {
                m_framerate = 1;
            }
        }
        else
        {
            m_numFrames++;
        }

        const unsigned int NUM_AVG_FRAME_TIMES = 5;

        if (m_prevTimeElapsed.size() < NUM_AVG_FRAME_TIMES)
        {
            m_prevTimeElapsed.push_back((double)(m_frequencyCurCount - m_frequencyLastFrameCount) / (double)m_frequency);
        }
        else
        {
            for (int i = 1; i < m_prevTimeElapsed.size(); ++i)
            {
                m_prevTimeElapsed[i - 1] = m_prevTimeElapsed[i];
            }

            m_prevTimeElapsed[m_prevTimeElapsed.size() - 1] = (double)(m_frequencyCurCount - m_frequencyLastFrameCount) / (double)m_frequency;
        }

        double timeElapsed = 0.0;

        for (int i = 0; i < m_prevTimeElapsed.size(); ++i)
        {
            timeElapsed += m_prevTimeElapsed[i];
        }

        m_timeElapsed = static_cast<float>(timeElapsed / static_cast<double>(m_prevTimeElapsed.size()));

        if (m_EngineSetup.bEditor && m_timeElapsed >= (1.0f / 30.0f))
        {
            m_timeElapsed = (1.0f / 30.0f);
        }

        static bool bFirstFrame = true;

        if (bFirstFrame)
        {
            m_timeElapsed = 0.0f;
            bFirstFrame = false;
        }

        m_time += m_timeElapsed;

        m_frequencyLastFrameCount = m_frequencyCurCount;
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::UpdateGameLoader(RenderQueue& renderQueue)
    {
        LOADER_STATE loaderState = m_pLevelLoader->Update(m_timeElapsed, renderQueue);

        //        ChangeState(m_pLevelLoader->GetNewState());

        //         if (loaderState == LOADER_STATE_LOADING)
        //         {
        //             m_numFramesRunning = 0;
        // 
        //             m_pRenderSystem->RenderLoadingScreen(renderQueue);
        // 
        //             ChangeState(std::string());
        // 
        //             return true;
        //         }
        //         else if (loaderState == LOADER_STATE_FIRSTFRAME)
        //         {
        //             m_pWorldMgr->InitializeAtLoad();
        // 
        //             gEngine->GetWorldMgr()->Update(0.0f, 0.0f, renderQueue);
        // 
        //             m_pRenderSystem->OnLevelChanged();
        // 
        //             m_time = 0.0f;
        //             m_timeElapsed = 0.0f;
        // 
        //             return true;
        //         }
        //         else if (loaderState == LOADER_STATE_SECONDFRAME)
        //         {
        //             m_time = 0.0f;
        //             m_timeElapsed = 0.0f;
        //             m_prevTimeElapsed.clear();
        //             ENV_PLATFORM_WINDOWS_ONLY(QueryPerformanceCounter((LARGE_INTEGER*)&m_frequencyLastCount););
        // 
        //             ChangeState(m_pLevelLoader->GetNewState());
        //         }

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CEngine::UpdateProfiler()
    {
#ifdef ENV_PLATFORM_WINDOWS
        CProfilingMgr& profilingMgr = CProfilingMgr::GetInstance();

        if (m_pInput->IsKeyJustDown(VK_NUMPAD1))
        {
            ENV_REL_CODE(profilingMgr.SetFrameProfilingGroup("Engine"));
        }
        else if (m_pInput->IsKeyJustDown(VK_NUMPAD2))
        {
            ENV_REL_CODE(profilingMgr.SetFrameProfilingGroup("Entity"));
        }
        else if (m_pInput->IsKeyJustDown(VK_NUMPAD3))
        {
            ENV_REL_CODE(profilingMgr.SetFrameProfilingGroup("SceneNodes"));
        }
        else if (m_pInput->IsKeyJustDown(VK_NUMPAD4))
        {
            ENV_REL_CODE(profilingMgr.SetFrameProfilingGroup("PropTrans"));
        }
        else if (m_pInput->IsKeyJustDown(VK_NUMPAD5))
        {
            ENV_REL_CODE(profilingMgr.SetFrameProfilingGroup("Rendering"));
        }
        else if (m_pInput->IsKeyJustDown(VK_NUMPAD6))
        {
            ENV_REL_CODE(profilingMgr.SetFrameProfilingGroup("RenderPasses"));
        }
#endif // ENV_PLATFORM_WINDOWS
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::GameLoop()
    {
        RenderQueue renderQueue;

        ENV_SCOPED_CPU_PROFILING(Engine, GameLoop);

        {
            UpdateFramerate();

            {
                ENV_SCOPED_CPU_PROFILING(Engine, DeviceLost);

                if (IsDeviceLost())
                    return true;

                if (m_bResetDeviceQueued)
                {
                     OnLostDevice();

                     RenderQualityParams renderQualityParams = m_pRenderSystem->GetRenderQualityParams();

                    // Find closest resolution for m_width and m_height
//                     if (!m_EngineSetup.bEditor)
//                         m_pRenderSystem->GetRenderDevice()->FindClosestMatchingResolution(m_width, m_height, m_width, m_height);

                    bool bResolutionChanged = renderQualityParams.screenWidth != m_width ||
                                              renderQualityParams.screenHeight != m_height ||
                                              renderQualityParams.bWindowed == m_pRenderSystem->GetRenderDevice()->GetFullscreen();


                    renderQualityParams.screenWidth = m_width;
                    renderQualityParams.screenHeight = m_height;
                    renderQualityParams.bWindowed = !m_pRenderSystem->GetRenderDevice()->GetFullscreen();
                    
                     m_pRenderSystem->ResetDevice(renderQualityParams);
                    
                     OnResetDevice();

                    if (renderQualityParams.bWindowed && bResolutionChanged && !m_EngineSetup.bEditor)
                    {
                        m_pRenderWindow->CenterWindow(renderQualityParams.screenWidth, renderQualityParams.screenHeight);
                    }
                }

                m_bResetDeviceQueued = false;
            }

            {
                ENV_SCOPED_CPU_PROFILING(Engine, Audio);

                m_pAudioSystem->Update(m_timeElapsed);
            }

            {
                ENV_SCOPED_CPU_PROFILING(Engine, GameLoader);

                if (UpdateGameLoader(renderQueue))
                    return true;
            }

            renderQueue.timeElapsedFrame = m_timeElapsed;
            renderQueue.timeElapsed = m_time;

            if (m_numFramesRunning < 5)
            {
                m_timeElapsed = 0.0f;
                m_time = 0.0f;
            }

            ++m_numFramesRunning;

            // Update Input
            {
                ENV_SCOPED_CPU_PROFILING(Engine, Input);

                m_pInput->BeforeFrame();
                m_pInput->UpdateObservers();
            }

            // Update resources
            {
                ENV_SCOPED_CPU_PROFILING(Engine, UpdateResources);

#ifndef ENV_FINAL
                m_bReloadResources = false;

                if (m_bReloadResources || (m_pInput->IsKeyDown(VK_CONTROL) && m_pInput->IsKeyJustDown('U')))
                {
                    gTextureMgr->UpdateResources();

                    gMeshMgr->UpdateResources();
                    GetSceneMgr()->UpdateMeshInstanceContainers();

                    gScriptMgr->UpdateResources();

                    gMaterialMgr->UpdateResources();

                    gShaderMgr->UpdateResources();

                    m_pAudioSystem->UpdateResources();
                }
#endif // ENV_FINAL

                gMaterialMgr->Update(m_timeElapsed);


            }

            if (m_bChangeState)
            {
                ChangeState(m_strNewState);
                m_strNewState.clear();
                m_bChangeState = false;
            }


            if (m_pStateCurrent)
            {
                ENV_SCOPED_CPU_PROFILING(Engine, UpdateState);

                m_pStateCurrent->Update(m_time, m_timeElapsed, renderQueue);
            }

            if (m_bPaused)
                m_timeElapsed = 0.0f;

            // Update Worldmgr
            {
                ENV_SCOPED_CPU_PROFILING(Engine, UpdateWorldMgr);

                m_pWorldMgr->Update(m_time, m_timeElapsed, renderQueue);
            }

            //#pragma message("--> REMOVE THAT CLEAR!")
            //		m_pRenderSystem->Clear(VECTOR4(0,0,0, 1));

            // BegiNScene
            {
                ENV_SCOPED_CPU_PROFILING(Engine, BeginScene);

                m_pRenderSystem->BeginScene();
            }

            // Render
            {
                ENV_SCOPED_CPU_PROFILING(Engine, Render);

                m_pRenderSystem->SetRenderQueue(&renderQueue);
                m_pRenderSystem->PreRender();
                m_pRenderSystem->Render(m_time, m_timeElapsed);

            }

            // EndScene
            {
                ENV_SCOPED_CPU_PROFILING(Engine, EndScene);

                m_pRenderSystem->EndScene();
            }


            m_pInput->AfterFrame();

        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CEngine::OnLostDevice()
    {
        m_pWorldMgr->OnLostDevice();

        if (m_pStateCurrent)
            m_pStateCurrent->OnLostDevice();

        if (m_pRenderSystem)
            m_pRenderSystem->OnLostDevice();

        if (gShaderMgr)
            gShaderMgr->OnLostDevice();

        if (gFontMgr)
            gFontMgr->OnLostDevice();

        if (!m_pRenderSystem->GetRenderQualityParams().bWindowed &&
            !gEngine->GetEditor() &&
            gEngine->GetCurrentState() &&
            gEngine->GetCurrentState()->GetStateName() == CGameState::STATE_NAME)
        {
            gEngine->ChangeState(CPauseMenueState::STATE_NAME);
        }

        ENV_LOG_INFO("CEngine::OnLostDevice SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    void CEngine::OnResetDevice()
    {
        ENV_LOG_INFO("CEngine::OnResetDevice");

        m_pWorldMgr->OnResetDevice();

        if (m_pRenderSystem)
            m_pRenderSystem->OnResetDevice();

        ENV_PLATFORM_WINDOWS_ONLY(if (m_pRenderWindow) m_pRenderWindow->Show(););

        if (gShaderMgr)
            gShaderMgr->OnResetDevice();

        if (gFontMgr)
            gFontMgr->OnResetDevice();


        bool bWindowed = m_pRenderSystem->GetRenderDevice()->GetWindowed();

        //SetCursor(NULL);
        ShowCursor(bWindowed);

        ENV_LOG_INFO("CEngine::OnResetDevice SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    bool CEngine::IsDeviceLost()
    {
        if (!m_pRenderSystem)
            return true;

        ERROR_TYPE error = m_pRenderSystem->GetRenderDevice()->TestCooperativeLevel();

        if (error == ERROR_TYPE_DEVICELOST)
        {
            ENV_PLATFORM_WINDOWS_ONLY(Sleep(20););
            return true;
        }
        else if (error == ERROR_TYPE_DRIVERINTERNALERROR)
        {
            ENV_LOG_ERROR("Internal driver error.");
            return false;

        }
        else if (error == ERROR_TYPE_DEVICENOTRESET)
        {
            ResetDevice();
            return false;
        }
        else
            return false;
    }

    //-----------------------------------------------------------------------------------
    void CEngine::ResetDevice(RenderQualityParams renderQualityParams)
    {
        m_pRenderSystem->GetRenderDevice()->SetFullscreen(!renderQualityParams.bWindowed);
        m_pRenderSystem->GetRenderDevice()->ResizeTarget(renderQualityParams.screenWidth, renderQualityParams.screenHeight);
        m_pRenderSystem->GetRenderDevice()->SetVSync(renderQualityParams.bVSync);
        m_pRenderSystem->GetRenderQualityParams().bWindowed = renderQualityParams.bWindowed;
        m_pRenderSystem->GetRenderQualityParams().bVSync = renderQualityParams.bVSync;
        m_pRenderSystem->GetRenderQualityParams().bLightRays = renderQualityParams.bLightRays;
        m_pRenderSystem->GetRenderQualityParams().bloomQuality = renderQualityParams.bloomQuality;

        if (m_EngineSetup.bEditor)
        {
            SetResetDeviceQueued(renderQualityParams.screenWidth, renderQualityParams.screenHeight);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEngine::ResetDevice()
    {
        ResetDevice(m_pRenderSystem->GetRenderQualityParams());
    }

    //-----------------------------------------------------------------------------------
    void CEngine::RemoveState(const std::string& strName)
    {
        if (m_pStateCurrent && m_pStateCurrent->GetStateName() == strName)
            m_pStateCurrent = nullptr;

        const auto& it = m_states.find(strName);
        if (it == m_states.end())
            return;

        m_states.erase(it);
        m_states[strName].reset();
    }

    //-----------------------------------------------------------------------------------
    void CEngine::ChangeState(const std::string& strName)
    {
        std::string strCurrentState;
        if (m_pStateCurrent)
        {
            strCurrentState = m_pStateCurrent->GetStateName();
            ENV_LOG_INFO("UNLOADING STATE: " + m_pStateCurrent->GetStateName());
            m_pStateCurrent->Unload();
        }

        if (strName.empty())
        {
            // Do not update/render a state
            m_pStateCurrent = nullptr;
        }
        else
        {
            m_pStateCurrent = m_states[strName].get();

            ENV_LOG_INFO("LOADING STATE: " + m_pStateCurrent->GetStateName());
            m_pStateCurrent->Load(strCurrentState);

            UpdateFramerate();

            m_prevTimeElapsed.clear();

            m_timeElapsed = 0.0f;
            m_time = 0.0f;
        }
    }

    //-----------------------------------------------------------------------------------
    void CEngine::SetNewState(const std::string& strName)
    {
        m_strNewState = strName;
        m_bChangeState = true;
    }

    //-----------------------------------------------------------------------------------
    CState* CEngine::GetState(const std::string& strName)
    {
        if (m_states.find(strName) == m_states.end())
            return nullptr;
        else
            return m_states[strName].get();
    }


    void CEngine::TriggerEndOfFrameCallback()
    {
        if (m_EndOfFrameCallback)
        {
            m_EndOfFrameCallback();
        }
    }

    void CEngine::TriggerDeathCallback()
    {
        if (m_DeathCallback)
        {
            m_DeathCallback();
        }
    }
    void CEngine::TriggerChapterProgressCallback(const int chapter, const int numberOfLevelBested, bool bossFinished)
    {
        if (m_ChapterProgressCallback)
        {
            m_ChapterProgressCallback(chapter, numberOfLevelBested, bossFinished);
        }
    }

} // env