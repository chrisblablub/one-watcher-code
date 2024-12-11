#include <Engine/State/SettingsState.h>
#include <Engine/Audio/AudioSystem.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/GameLoader.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/RenderDevice.h>

namespace env
{
    const std::string   CSettingsState::STATE_NAME          = "SettingsState";
    const float         CSettingsState::FADE_IN_AMOUNT      = 0.25f;
    const float         CSettingsState::FADE_SCREEN_AMOUNT  = 0.98f;
    const Vec4          CSettingsState::FADE_FRAME_AMOUNT   = Vec4(0.05f, 0.05f, 0.40f, 0.40f);

    const FontInfo fontInfoMenueSelection = FontInfo("FontMenueSelection", 36);
    const FontInfo fontMenue              = FontInfo("FontMenue");
    const FontInfo fontMenueTitle         = FontInfo("FontMenueTitle", 40, true);

    //-----------------------------------------------------------------------------------
    CSettingsState::CSettingsState()
        : CState(CSettingsState::STATE_NAME)
        , m_selection(0)
        , m_pGameSound(nullptr)
        , m_pSFXSound(nullptr)
		, m_pMusic(nullptr)
    {

        Vec2 settingsFirstPosition          = Vec2(-0.05f, 0.0f + 3.0f * 0.1f);
        Vec2 settingsValuesFirstPosition    = Vec2( 0.05f, 0.0f + 3.0f * 0.1f);

        m_settings.resize(SETTINGS_END);
        m_settingsValues.resize(SETTINGS_END);

        for (unsigned int i = 0; i < m_settings.size(); ++i)
        {
            m_settings[i].SetPosition(settingsFirstPosition - (float)i * Vec2(0.0f, 0.1f));
            m_settings[i].m_bScreenSpace = true;
            m_settings[i].m_alignment = TEXT_ALIGNMENT_RIGHT;

            m_settingsValues[i].SetPosition(settingsValuesFirstPosition - (float)i * Vec2(0.0f, 0.1f));
            m_settingsValues[i].m_bScreenSpace = true;
            m_settingsValues[i].m_alignment = TEXT_ALIGNMENT_LEFT;

        }

        m_settingsText.m_strText = L"S E T T I N G S";
        m_settingsText.SetPosition(Vec2(0.0f, 0.5f));
        m_settingsText.m_color = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
        m_settingsText.m_bScreenSpace = true;
        m_settingsText.m_alignment = TEXT_ALIGNMENT_CENTER;
    }

    //-----------------------------------------------------------------------------------
    CSettingsState::~CSettingsState()
    {
        Unload();
    }

    //-----------------------------------------------------------------------------------
    bool CSettingsState::Load(const std::string& strPreviousState)
    {
        m_strPreviousState = strPreviousState;

        gEngine->SetPaused(true);

        SetState(SETTINGS_STATE_LOAD);

        m_selection = 0;

        m_videoParams = gRenderSystem->GetRenderQualityParams();
        m_audioParams = gEngine->GetAudioSystem()->GetAudioParameters();

        m_audioParams.musicVolume *= 5;

        vecDisplayModes displayModes;
        gRenderSystem->GetRenderDevice()->EnumerateDisplayModes(displayModes);

        int currentRes = -1;

        // search the index of the current resolution to check if it's valid
        for (unsigned int i = 0; i < displayModes.size(); ++i)
        {
            if (displayModes[i].Width == m_videoParams.screenWidth &&
                displayModes[i].Height == m_videoParams.screenHeight)
            {
                currentRes = i;
                break;
            }
        }

        // default to the native desktop (monitor) resolution, if the current res seems to be invalid
        if (currentRes < 0)
        {
            RECT r;
            ENV_PLATFORM_WINDOWS_ONLY(GetWindowRect(GetDesktopWindow(), &r););

            int screenWidth = (int)r.right - (int)r.left;
            int screenHeight = (int)r.bottom - (int)r.top;

            currentRes = 0;

            for (unsigned int i = 0; i < displayModes.size(); ++i)
            {
                if (displayModes[i].Width == screenWidth &&
                    displayModes[i].Height == screenHeight)
                {
                    currentRes = i;
                    break;
                }
            }
        }

        if (displayModes.size() > 0) // TODO: Investigate ESC -> ENTER crash
        {
            m_videoParams.screenWidth = displayModes[currentRes].Width;
            m_videoParams.screenHeight = displayModes[currentRes].Height;
        }

        m_bResolutionChanged = false;

        gFontMgr->AddGetPtr(fontInfoMenueSelection.GetKey());
        gFontMgr->AddGetPtr(fontMenue.GetKey());
        gFontMgr->AddGetPtr(fontMenueTitle.GetKey());

        m_pSFXSound = gEngine->GetAudioSystem()->AddGetPtr("Entities/LaserRadiator.wav");
		m_pMusic = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_MUSIC_TRACK_02);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CSettingsState::Unload()
    {
        ApplyAudioSettings();

        gEngine->GetSettings()->Save();

        if (m_pSFXSound)
            m_pSFXSound->StopSound();

		if (m_pMusic)
			m_pMusic->StopSound();

        gEngine->GetAudioSystem()->Release(m_pSFXSound);
		gEngine->GetAudioSystem()->Release(m_pMusic);

        m_pSFXSound = nullptr;
		m_pMusic = nullptr;

        gFontMgr->Release(fontMenueTitle.GetKey());
        gFontMgr->Release(fontMenue.GetKey());
        gFontMgr->Release(fontInfoMenueSelection.GetKey());

        m_bResolutionChanged = false;

        gEngine->SetPaused(false);

        for (unsigned int i = 0; i < m_settings.size(); ++i)
        {
            m_settings[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w;
            m_settingsValues[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w;
        }

        CState::Unload();
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CSettingsState::Update(float time, float dt, RenderQueue& renderQueue)
    {
        m_fade.fadeBarsType = FADE_BARS_FROM_TOP_AND_BOTTOM;

        UpdateInput();

        std::string strResolution = "RESOLUTI ON";
        std::string strFullScreen = "FULLSCREEN";
        std::string strBloom = "BLOOM";
        std::string strVSync = "VSYNC";
        std::string strLightRays = "LI GHT RAYS";
        std::string strAudioSFX = "SFX VOLUME";
        std::string strAudioMusic = "MUSI C VOLUME";

        std::stringstream strResolutionValue;
        std::stringstream strFullScreenValue;
        std::stringstream strBloomValue;
        std::stringstream strVSyncValue;
        std::stringstream strLightRaysValue;
        std::stringstream strAudioSFXValue;
        std::stringstream strAudioMusicValue;

        strAudioSFXValue.precision(2);

        strResolutionValue  << m_videoParams.screenWidth << " x " << m_videoParams.screenHeight;
        strFullScreenValue  << (m_videoParams.bWindowed ? "OFF" : "ON");

        if (m_videoParams.bloomQuality == 1)
            strBloomValue << "HI GH";
        else if (m_videoParams.bloomQuality == 2)
            strBloomValue << "MEDI UM";
        else if (m_videoParams.bloomQuality == 3)
            strBloomValue << "LOW";
        else
            strBloomValue << "OFF";

        strVSyncValue       << (m_videoParams.bVSync ? "ON" : "OFF");
        strAudioSFXValue    << m_audioParams.sfxVolume << "%";
        strAudioMusicValue  << m_audioParams.musicVolume << "%";
        strLightRaysValue   << (m_videoParams.bLightRays ? "ON" : "OFF");

        m_settings[SETTINGS_RESOLUTION].SetText(strResolution);
        m_settings[SETTINGS_FULLSCREEN].SetText(strFullScreen);
        m_settings[SETTINGS_BLOOM].SetText(strBloom);
        m_settings[SETTINGS_VSYNC].SetText(strVSync);
        m_settings[SETTINGS_LIGHTRAYS].SetText(strLightRays);
        m_settings[SETTINGS_AUDIO_SFX].SetText(strAudioSFX);
        m_settings[SETTINGS_AUDIO_MUSIC].SetText(strAudioMusic);
        m_settings[SETTINGS_APPLY].SetText("APPLY");

        m_settingsValues[SETTINGS_RESOLUTION].SetText(strResolutionValue.str());
        m_settingsValues[SETTINGS_FULLSCREEN].SetText(strFullScreenValue.str());
        m_settingsValues[SETTINGS_BLOOM].SetText(strBloomValue.str());
        m_settingsValues[SETTINGS_VSYNC].SetText(strVSyncValue.str());
        m_settingsValues[SETTINGS_LIGHTRAYS].SetText(strLightRaysValue.str());
        m_settingsValues[SETTINGS_AUDIO_SFX].SetText(strAudioSFXValue.str());
        m_settingsValues[SETTINGS_AUDIO_MUSIC].SetText(strAudioMusicValue.str());


        if (m_state == SETTINGS_STATE_LOAD)
        {
            m_currentStateTime -= dt;

            float alpha = 1.0f - pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = SETTINGS_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = SETTINGS_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_settingsText.m_color.w = alpha;

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = alpha * FADE_FRAME_AMOUNT;
            m_fade.fadeBarsAmount = 0.0f;

            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = alpha * FADE_SCREEN_AMOUNT;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = alpha * FADE_SCREEN_AMOUNT;
            renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;

            if (m_currentStateTime < 0.0f)
            {
                SetState(SETTINGS_STATE_MENUE);
            }
        }
        else if (m_state == SETTINGS_STATE_UNLOAD)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = SETTINGS_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = SETTINGS_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_settingsText.m_color.w = alpha;

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = alpha * FADE_FRAME_AMOUNT;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = alpha * FADE_SCREEN_AMOUNT;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = alpha * FADE_SCREEN_AMOUNT;

            if (m_currentStateTime < 0.0f)
            {
                if (m_strPreviousState == "MenueState")
                {
                    gEngine->SetNewState("MenueState");
                }
                else if (m_strPreviousState == "PauseMenueState")
                {
                    gEngine->SetNewState("PauseMenueState");
                }
            }

        }
        else if (m_state == SETTINGS_STATE_MENUE)
        {
            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color = SETTINGS_MENUE_ITEM_SELECTION_COLOR;
                    m_settingsValues[i].m_color = SETTINGS_MENUE_ITEM_SELECTION_COLOR;
                }
                else
                {
                    m_settings[i].m_color = SETTINGS_MENUE_ITEM_COLOR;
                    m_settingsValues[i].m_color = SETTINGS_MENUE_ITEM_COLOR;
                }
            }

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = FADE_SCREEN_AMOUNT;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = FADE_SCREEN_AMOUNT;

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = FADE_FRAME_AMOUNT;

            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
            {
                SetState(SETTINGS_STATE_UNLOAD);
            }
        }


        for (unsigned int i = 0; i < m_settings.size(); ++i)
        {
            if (i == m_selection)
            {
                renderQueue.m_textRenderObjects[fontInfoMenueSelection].push_back(m_settings[i]);
                renderQueue.m_textRenderObjects[fontInfoMenueSelection].push_back(m_settingsValues[i]);
            }
            else
            {
                renderQueue.m_textRenderObjects[fontMenue].push_back(m_settings[i]);
                renderQueue.m_textRenderObjects[fontMenue].push_back(m_settingsValues[i]);
            }
        }

        renderQueue.m_textRenderObjects[fontMenueTitle].push_back(m_settingsText);
        renderQueue.m_pFadeBarsRenderObject = &m_fade;
    }

    void CSettingsState::ApplyAudioSettings()
    {
        gEngine->GetSettings()->m_audioParams = m_audioParams;
        gEngine->GetSettings()->m_audioParams.musicVolume /= 5;
        gEngine->GetAudioSystem()->SetAudioParameters(gEngine->GetSettings()->m_audioParams);
    }

    //-----------------------------------------------------------------------------------
    void CSettingsState::UpdateInput()
    {
        bool bConfirmed = false;
        bool bBack = false;

        if (gEngine->GetInput()->GetKeyboardInput() &&
            gEngine->GetInput()->IsActionJustTriggered(ACTION_START))
        {
            bConfirmed = true;
        }
        else if (!gEngine->GetInput()->GetKeyboardInput() &&
                 gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY))
        {
            bConfirmed = true;
        }

        if (gEngine->GetInput()->GetKeyboardInput() &&
            gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
        {
            bBack = true;
        }
        else if (!gEngine->GetInput()->GetKeyboardInput() &&
            (gEngine->GetInput()->IsActionJustTriggered(ACTION_START) ||
             gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK)))
        {
            bBack = true;
        }

        if (bConfirmed)
        {
            if (m_selection == SETTINGS_APPLY)
            {

                ApplyAudioSettings();
                gEngine->GetSettings()->m_videoParams = m_videoParams;
                gEngine->GetSettings()->Save();


                // Commit settings
                gEngine->ResetDevice(m_videoParams);
                //gRenderSystem->ResetDevice(m_videoParams);
                SetState(SETTINGS_STATE_UNLOAD);
            }
        }
        else if (bBack)
        {
            SetState(SETTINGS_STATE_UNLOAD);
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_UP))
        {
            if (m_selection == 0)
                m_selection = SETTINGS_END - 1;
            else
                --m_selection;
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_DOWN))
        {
            m_selection = (m_selection + 1) % SETTINGS_END;
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_LEFT))
        {
            if (m_selection == SETTINGS_RESOLUTION)
            {
                vecDisplayModes displayModes;
                gRenderSystem->GetRenderDevice()->EnumerateDisplayModes(displayModes);

                int currentRes = -1;

                // search the index of the current resolution to check if it's valid
                for (unsigned int i = 0; i < displayModes.size(); ++i)
                {
                    if (displayModes[i].Width == m_videoParams.screenWidth &&
                        displayModes[i].Height == m_videoParams.screenHeight)
                    {
                        currentRes = i;
                        break;
                    }
                }

                if (currentRes > 0)
                    --currentRes;

                // default to the native desktop (monitor) resolution, if the current res seems to be invalid
                if (currentRes < 0)
                {
                    RECT r;
                    ENV_PLATFORM_WINDOWS_ONLY(GetWindowRect(GetDesktopWindow(), &r););

                    int screenWidth = (int)r.right - (int)r.left;
                    int screenHeight = (int)r.bottom - (int)r.top;

                    currentRes = 0;

                    for (unsigned int i = 0; i < displayModes.size(); ++i)
                    {
                        if (displayModes[i].Width == screenWidth &&
                            displayModes[i].Height == screenHeight)
                        {
                            currentRes = i;
                            break;
                        }
                    }
                }

                m_videoParams.screenWidth = displayModes[currentRes].Width;
                m_videoParams.screenHeight = displayModes[currentRes].Height;
                m_bResolutionChanged = true;
            }
            else if (m_selection == SETTINGS_FULLSCREEN)
            {
                m_videoParams.bWindowed = true;
            }
            else if (m_selection == SETTINGS_BLOOM)
            {
                m_videoParams.bloomQuality++;

                if (m_videoParams.bloomQuality > 3)
                    m_videoParams.bloomQuality = 0;
            }
            else if (m_selection == SETTINGS_VSYNC)
            {
                m_videoParams.bVSync = true;
            }
            else if (m_selection == SETTINGS_LIGHTRAYS)
            {
                m_videoParams.bLightRays = false;
            }
            else if (m_selection == SETTINGS_AUDIO_SFX)
            {
                m_audioParams.sfxVolume -= 5;

                if (m_audioParams.sfxVolume < 0)
                    m_audioParams.sfxVolume = 0;
            }
            else if (m_selection == SETTINGS_AUDIO_MUSIC)
            {
                m_audioParams.musicVolume -= 5;

                if (m_audioParams.musicVolume < 0)
                    m_audioParams.musicVolume = 0;
            }
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_RIGHT))
        {
            if (m_selection == SETTINGS_RESOLUTION)
            {
                vecDisplayModes displayModes;
                gRenderSystem->GetRenderDevice()->EnumerateDisplayModes(displayModes);

                int currentRes = -1;

                // search the index of the current resolution to check if it's valid
                for (unsigned int i = displayModes.size() - 1; i > 0; --i)
                {
                    if (displayModes[i].Width == m_videoParams.screenWidth &&
                        displayModes[i].Height == m_videoParams.screenHeight)
                    {
                        currentRes = i;
                        break;
                    }
                }

                if (currentRes < (displayModes.size() - 1))
                    ++currentRes;

                // default to the native desktop (monitor) resolution, if the current res seems to be invalid
                if (currentRes < 0)
                {
                    RECT r;
                    ENV_PLATFORM_WINDOWS_ONLY(GetWindowRect(GetDesktopWindow(), &r););

                    int screenWidth = (int)r.right - (int)r.left;
                    int screenHeight = (int)r.bottom - (int)r.top;

                    currentRes = 0;

                    for (unsigned int i = 0; i < displayModes.size(); ++i)
                    {
                        if (displayModes[i].Width == screenWidth &&
                            displayModes[i].Height == screenHeight)
                        {
                            currentRes = i;
                            break;
                        }
                    }
                }

                m_videoParams.screenWidth = displayModes[currentRes].Width;
                m_videoParams.screenHeight = displayModes[currentRes].Height;
                m_bResolutionChanged = true;
            }
            else if (m_selection == SETTINGS_FULLSCREEN)
            {
                m_videoParams.bWindowed = false;
            }
            else if (m_selection == SETTINGS_BLOOM)
            {
                m_videoParams.bloomQuality--;

                if (m_videoParams.bloomQuality < 0)
                    m_videoParams.bloomQuality = 3;
            }
            else if (m_selection == SETTINGS_VSYNC)
            {
                m_videoParams.bVSync = false;
            }
            else if (m_selection == SETTINGS_LIGHTRAYS)
            {
                m_videoParams.bLightRays = true;
            }
            else if (m_selection == SETTINGS_AUDIO_SFX)
            {
                m_audioParams.sfxVolume += 5;

                if (m_audioParams.sfxVolume > 100)
                    m_audioParams.sfxVolume = 100;
            }
            else if (m_selection == SETTINGS_AUDIO_MUSIC)
            {
                m_audioParams.musicVolume += 5;

                if (m_audioParams.musicVolume > 100)
                    m_audioParams.musicVolume = 100;
            }
        }

        static bool bSoundPlaying = false;

        if (m_selection == SETTINGS_AUDIO_SFX || 
            m_selection == SETTINGS_AUDIO_MUSIC)
        {
            if (!bSoundPlaying)
            {
                m_pSFXSound->SetVolume(1.0f);
                m_pSFXSound->PlaySound(AUDIO_CHANNELGROUP_EFFECTS_PREVIEW, -1);

				bool isMusicPaused = false;
#ifdef ENV_FINAL
				gEngine->GetAudioSystem()->GetChannelGroup(AUDIO_CHANNELGROUP_MUSIC)->getPaused(&isMusicPaused);
#endif
				if (isMusicPaused)
				{
					m_pMusic->SetVolume(1.0f);
					m_pMusic->PlaySound(AUDIO_CHANNELGROUP_MUSIC_PREVIEW, -1);
				}

                bSoundPlaying = true;
            }
        }
        else
        {
            m_pSFXSound->StopSound();
			m_pMusic->StopSound();

            bSoundPlaying = false;
        }

        AudioParameters params = m_audioParams;
        params.musicVolume /= 5;
        gEngine->GetAudioSystem()->SetAudioParameters(params);
    }
} // env