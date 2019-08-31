#include <Engine/State/PauseMenueState.h>
#include <Engine/Audio/AudioSystem.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/GameLoader.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    const std::string   CPauseMenueState::STATE_NAME            = "PauseMenueState";
    const float         CPauseMenueState::FADE_IN_AMOUNT        = 0.25f;
    const float         CPauseMenueState::FADE_SCREEN_AMOUNT    = 0.99f;
    const Vec4          CPauseMenueState::FADE_FRAME_AMOUNT     = Vec4(0.05f, 0.05f, 0.40f, 0.40f);
    const FontInfo      CPauseMenueState::fontMenueSelection = FontInfo("FontMenueSelection", 36);
    const FontInfo      CPauseMenueState::fontMenueTitle = FontInfo("FontMenueTitle", 40, true);
    const FontInfo      CPauseMenueState::fontMenue = FontInfo("FontMenue", 32);

     //-----------------------------------------------------------------------------------
    CPauseMenueState::CPauseMenueState()
        : CState(CPauseMenueState::STATE_NAME)
        , m_pMenueLogo(nullptr)
        , m_pMenueLogoNode(nullptr)
        , m_pGameSound(nullptr)
        , m_pLevelNamesMtrl(nullptr)
        , m_selection(0)
        , m_minIndex(1)
        , m_bCutScene(true)
    {
        m_pMenueLogo = new CSprite();
        m_pMenueLogoNode = new CSceneNode();
        m_pMenueLogo->SetMaterialFilename("GUI/Pause/background.png");
        m_pMenueLogo->SetUpdateRectByAnimation(true);
        m_pMenueLogo->AttachNode(m_pMenueLogoNode);
        m_pMenueLogo->SetScreenSpace(true);
//        m_pMenueLogo->SetApplyFade(false);
        m_pMenueLogoNode->SetPosition(Vec3(0.5f, 0.15f, 0.0f));

        m_menueText = TextRenderObject(L"M E N U", Vec4(0.8f, 0.8f, 0.8f, 1.0f), Vec2(0.0f, 0.5f));
        m_menueText.m_bScreenSpace = true;
        m_menueText.m_alignment = TEXT_ALIGNMENT_CENTER;
        m_levelText = TextRenderObject(L"LEVEL", Vec4(0.8f, 0.8f, 0.8f, 1.0f), Vec2(0.95f, 0.5f));
        m_levelText.m_bScreenSpace = true;
        m_levelText.m_alignment = TEXT_ALIGNMENT_RIGHT;

        m_settings.resize(4);
        m_settingsValues.resize(4);

        m_settings[0] = TextRenderObject(L"SKIP CUTSCENE", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 0.3f));
        m_settings[0].m_bScreenSpace = true;
        m_settings[0].m_alignment = TEXT_ALIGNMENT_CENTER;
        m_settings[1] = TextRenderObject(L"SETTINGS", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(-0.05f, 0.1f));
        m_settings[1].m_bScreenSpace = true;
        m_settings[1].m_alignment = TEXT_ALIGNMENT_RIGHT;
        m_settings[2] = TextRenderObject(L"BACK", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(-0.05f, 0.0f));
        m_settings[2].m_bScreenSpace = true;
        m_settings[2].m_alignment = TEXT_ALIGNMENT_RIGHT;
        m_settings[3] = TextRenderObject(L"QUIT", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(-0.05f, -0.1f));
        m_settings[3].m_bScreenSpace = true;
        m_settings[3].m_alignment = TEXT_ALIGNMENT_RIGHT;

        m_settingsValues[0] = TextRenderObject(L"", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.25f, 0.3f));
        m_settingsValues[0].m_bScreenSpace = true;
        m_settingsValues[0].m_alignment = TEXT_ALIGNMENT_LEFT;
        m_settingsValues[1] = TextRenderObject(L"ADJUST GRAPHICS", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.05f, 0.1f));
        m_settingsValues[1].m_bScreenSpace = true;
        m_settingsValues[1].m_alignment = TEXT_ALIGNMENT_LEFT;
        m_settingsValues[2] = TextRenderObject(L"RETURN TO MENU", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.05f, -0.0f));
        m_settingsValues[2].m_bScreenSpace = true;
        m_settingsValues[2].m_alignment = TEXT_ALIGNMENT_LEFT;
        m_settingsValues[3] = TextRenderObject(L"RETURN TO DESKTOP", Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.05f, -0.1f));
        m_settingsValues[3].m_bScreenSpace = true;
        m_settingsValues[3].m_alignment = TEXT_ALIGNMENT_LEFT;

        gFontMgr->AddGetPtr(fontMenue.GetKey());
        gFontMgr->AddGetPtr(fontMenueSelection.GetKey());
        gFontMgr->AddGetPtr(fontMenueTitle.GetKey());
    }

    //-----------------------------------------------------------------------------------
    CPauseMenueState::~CPauseMenueState()
    {
        gMaterialMgr->Release(m_pLevelNamesMtrl);

        gFontMgr->Release(fontMenue.GetKey());
        gFontMgr->Release(fontMenueSelection.GetKey());
        gFontMgr->Release(fontMenueTitle.GetKey());

        Unload();

        ENV_DELETE(m_pMenueLogo);
        ENV_DELETE(m_pMenueLogoNode);
    }

    //-----------------------------------------------------------------------------------
    bool CPauseMenueState::Load(const std::string& strPreviousState)
    {
        m_strPreviousState = strPreviousState;

        gEngine->SetPaused(true);
		gEngine->GetAudioSystem()->PauseSound();

        SetState(PAUSEMENUE_STATE_LOAD);

        m_selection = 0;

        CSingletonPlayerMgr playerMgr;

        if (m_strPreviousState == "GameState" || 
            m_strPreviousState == "SettingsState")
        {
            LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex());
            m_levelText.SetText(levelInfo.strName + " ");
        }

        if (playerMgr->GetCurrentLevelInfo().bCutScene)
        {
            m_settings[0].SetText("SKIP CUTSCENE");

            m_minIndex = 0;
        }
        else if (playerMgr->GetCurrentLevelIndex() < playerMgr->GetCurrentChapter().levels.size() - 2 && 
                 playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex() + 1).bAccessible)
        {
            m_settings[0].SetText("LOAD NEXT LEVEL");

            m_minIndex = 0;
        }
        else
        {
            m_settings[0].SetText("");

            m_minIndex = 1;
        }

        m_selection = m_minIndex;

        gMaterialMgr->Release(m_pLevelNamesMtrl);

        std::string levelMtrl = "levelSelection.mtrl";
        std::string chapterFolder;

        chapterFolder = "GUI/" + playerMgr->GetChapterFolder();

        levelMtrl = chapterFolder + levelMtrl;

        m_pLevelNamesMtrl = gMaterialMgr->AddGetPtr(levelMtrl);


        m_spriteLevelName.SetScreenSpace(true);
        m_spriteLevelName.SetSpritePipeline(SPRITE_PIPELINE_AFTER_FADE);
        m_spriteLevelName.SetMaterialFilename(levelMtrl, true);
        m_spriteLevelName.AttachNode(&m_nodeLevelName);

        m_nodeLevelName.SetPosition(Vec3(0.6f, 0.8f, 0.0f));
        m_nodeLevelName.SetScale(Vec3(1.0f, 1.0f, 1.0f));



        int index = playerMgr->GetCurrentLevelIndex();

        int numTextureColumns = m_pLevelNamesMtrl->GetWidth() / m_pLevelNamesMtrl->GetFrameWidth();
        int columnIndex = index % numTextureColumns;
        int rowIndex = index / numTextureColumns;

        m_spriteLevelName.GetMaterial()->SetFrame(columnIndex);
        m_spriteLevelName.GetMaterial()->SetSequence(rowIndex);

        m_bCutScene = playerMgr->GetCurrentLevelInfo().bCutScene;

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CPauseMenueState::Unload()
    {
        gEngine->SetPaused(false);

        m_levelText.m_strText.clear();

        CState::Unload();
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CPauseMenueState::Update(float time, float dt, RenderQueue& renderQueue)
    {
        m_fade.fadeBarsType = FADE_BARS_FROM_TOP_AND_BOTTOM;

        UpdateInput();

        //renderQueue.m_spriteRenderObjects.push_back(m_pMenueLogo->GetRenderObject());

        float fadeScreenAmount = FADE_SCREEN_AMOUNT;

        if (m_state == PAUSEMENUE_STATE_LOAD)
        {
            m_currentStateTime -= dt;

            float alpha = 1.0f - pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_spriteLevelName.SetColor(PAUSEMENUE_MENUE_ITEM_COLOR * alpha);

            m_menueText.m_color.w = alpha;
            m_levelText.m_color.w = alpha;


            Vec3 scaling = (1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(1.0f, 1.0f, 1.0f);
            m_pMenueLogo->SetAlpha(alpha);
            m_pMenueLogoNode->SetScale(scaling);

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = alpha * FADE_FRAME_AMOUNT;
            m_fade.fadeBarsAmount = 0.0f;

            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = alpha * fadeScreenAmount;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = alpha * fadeScreenAmount;
            renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;

            if (m_currentStateTime < 0.0f)
            {
                SetState(PAUSEMENUE_STATE_MENUE);
            }
        }
        else if (m_state == PAUSEMENUE_STATE_UNLOADSETTINGS)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_spriteLevelName.SetColor(PAUSEMENUE_MENUE_ITEM_COLOR * alpha);

            m_menueText.m_color.w = alpha;
            m_levelText.m_color.w = alpha;

            Vec3 scaling = (1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(1.0f, 1.0f, 1.0f);
            m_pMenueLogo->SetAlpha(alpha);
            m_pMenueLogoNode->SetScale(scaling);

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = alpha * FADE_FRAME_AMOUNT;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = alpha * fadeScreenAmount;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = alpha * fadeScreenAmount;

            if (m_currentStateTime < 0.0f)
            {
                gEngine->SetNewState("SettingsState");
            }

        }
        else if (m_state == PAUSEMENUE_STATE_UNLOADCONTINUE)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_spriteLevelName.SetColor(PAUSEMENUE_MENUE_ITEM_COLOR * alpha);

            m_menueText.m_color.w = alpha;
            m_levelText.m_color.w = alpha;

            Vec3 scaling = (1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(1.0f, 1.0f, 1.0f);
            m_pMenueLogo->SetAlpha(alpha);
            m_pMenueLogoNode->SetScale(scaling);

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = alpha * FADE_FRAME_AMOUNT;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = alpha * fadeScreenAmount;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = alpha * fadeScreenAmount;

            if (m_currentStateTime < 0.0f)
            {
                gEngine->SetNewState("GameState");
            }

        }
        else if (m_state == PAUSEMENUE_STATE_UNLOADMENUE)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_spriteLevelName.SetColor(PAUSEMENUE_MENUE_ITEM_COLOR * alpha);

            m_menueText.m_color.w = alpha;
            m_levelText.m_color.w = alpha;

            Vec3 scaling = (1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(1.0f, 1.0f, 1.0f);
            m_pMenueLogo->SetAlpha(alpha);
            m_pMenueLogoNode->SetScale(scaling);

            m_fade.fadeBarsType = FADE_BARS_FROM_BOTTOM_TO_TOP;
            m_fade.fadeBarsAmount = 1.0f - alpha;
            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = FADE_FRAME_AMOUNT;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = fadeScreenAmount;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = fadeScreenAmount;

            if (m_currentStateTime < 0.0f)
            {
                CSingletonPlayerMgr playerMgr;
                gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env", "LevelSelectionState");
            }

        }
        else if (m_state == PAUSEMENUE_STATE_UNLOADQUIT)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR.w * alpha;
                }
                else
                {
                    m_settings[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                    m_settingsValues[i].m_color.w = PAUSEMENUE_MENUE_ITEM_COLOR.w * alpha;
                }
            }

            m_spriteLevelName.SetColor(PAUSEMENUE_MENUE_ITEM_COLOR * alpha);

            m_menueText.m_color.w = alpha;
            m_levelText.m_color.w = alpha;

            Vec3 scaling = (1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(1.0f, 1.0f, 1.0f);
            m_pMenueLogo->SetAlpha(alpha);
            m_pMenueLogoNode->SetScale(scaling);

            m_fade.fadeBarsType = FADE_BARS_FROM_BOTTOM_TO_TOP;
            m_fade.fadeBarsAmount = 1.0f - alpha;
            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = FADE_FRAME_AMOUNT;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = fadeScreenAmount;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = fadeScreenAmount;

            if (m_currentStateTime < 0.0f)
            {
                gEngine->ShutDown();
            }

        }
        else if (m_state == PAUSEMENUE_STATE_MENUE)
        {
            m_pMenueLogo->SetAlpha(1.0f);
            m_pMenueLogoNode->SetScale(Vec3(1.0f, 1.0f, 1.0f));

            for (unsigned int i = 0; i < m_settings.size(); ++i)
            {
                if (i == m_selection)
                {
                    m_settings[i].m_color = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR;
                    m_settingsValues[i].m_color = PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR;
                }
                else
                {
                    m_settings[i].m_color = PAUSEMENUE_MENUE_ITEM_COLOR;
                    m_settingsValues[i].m_color = PAUSEMENUE_MENUE_ITEM_COLOR;
                }
            }

            m_spriteLevelName.SetColor(PAUSEMENUE_MENUE_ITEM_COLOR);

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = fadeScreenAmount;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = fadeScreenAmount;

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = FADE_FRAME_AMOUNT;

        }


        for (unsigned int i = m_minIndex; i < m_settings.size(); ++i)
        {
            if (i == m_selection)
            {
                renderQueue.m_textRenderObjects[fontMenueSelection].push_back(m_settings[i]);
                renderQueue.m_textRenderObjects[fontMenueSelection].push_back(m_settingsValues[i]);
            }
            else
            {
                renderQueue.m_textRenderObjects[fontMenue].push_back(m_settings[i]);
                renderQueue.m_textRenderObjects[fontMenue].push_back(m_settingsValues[i]);
            }
        }

        renderQueue.m_textRenderObjects[fontMenueTitle].push_back(m_menueText);
        
        if (!m_bCutScene)
            renderQueue.m_spriteRenderObjects.push_back(m_spriteLevelName.GetRenderObject());

        renderQueue.m_pFadeBarsRenderObject = &m_fade;
    }

    //-----------------------------------------------------------------------------------
    void CPauseMenueState::UpdateInput()
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
            if (m_selection == PAUSEMENUE_SKIP_CUTSCENE)
            {
                CSingletonPlayerMgr playerMgr;

                if (playerMgr->GetCurrentLevelInfo().bCutScene)
                {
                    if (playerMgr->GetCurrentLevelIndex() == playerMgr->GetCurrentChapter().levels.size() - 1)
                    {
                        // Outro => Load Menue state
                        gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env", "LevelSelectionState");
                    }
                    else
                    {
//                         playerMgr->SetCurrentLevelByIndex(playerMgr->GetCurrentLevelIndex() + 1);
//                         playerMgr->GetCurrentLevelInfo().bAccessible = true;

                        // Intro => Load next level
                        playerMgr->SetLevelFinished();

                        std::string strLevelFilename = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex()).strFilename;

                        playerMgr->LoadLevel(strLevelFilename);
                    }
                }
                else
                {
                    playerMgr->SetCurrentLevelByIndex(playerMgr->GetCurrentLevelIndex() + 1);

                    // Load next level
                    std::string strLevelFilename = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex()).strFilename;

                    gEngine->GetLevelLoader().LoadLevel(strLevelFilename, "GameState");
                }
            }
            else if (m_selection == PAUSEMENUE_SETTINGS)
            {
                SetState(PAUSEMENUE_STATE_UNLOADSETTINGS);
            }
            else if (m_selection == PAUSEMENUE_QUIT)
            {
                SetState(PAUSEMENUE_STATE_UNLOADQUIT);
            }
            else if (m_selection == PAUSEMENUE_MENUE)
            {
                SetState(PAUSEMENUE_STATE_UNLOADMENUE);
            }
        }
        else if (bBack)
        {
            SetState(PAUSEMENUE_STATE_UNLOADCONTINUE);
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_UP))
        {
            if (m_selection == m_minIndex)
                m_selection = PAUSEMENUE_END - 1;
            else
                --m_selection;
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_DOWN))
        {
            if (m_selection == PAUSEMENUE_END - 1)
                m_selection = m_minIndex;
            else
                m_selection++;
        }
    }

} // env