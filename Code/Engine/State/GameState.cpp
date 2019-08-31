#include <Engine/State/GameState.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Scene/TerrainMgr.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Core/PlayerMgr.h>
#include <Graphic/Core/StripeGenerator.h>

namespace env
{
    const std::string CGameState::STATE_NAME = "GameState";
    const FontInfo fontGameHeader = FontInfo("FontGameHeader");

    const FontInfo      CGameState::fontMenue = FontInfo("FontMenue");

    //-----------------------------------------------------------------------------------
    CGameState::CGameState()
        : CState(STATE_NAME)
        , m_pCharacter(nullptr)
        , m_bNewGameMode(true)
        , m_bIsBossLevel(false) {}

    //-----------------------------------------------------------------------------------
    CGameState::~CGameState()
    {
        Unload();
    }

    //-----------------------------------------------------------------------------------
    bool CGameState::Load(const std::string& strPreviousState)
    {
        CSingletonPlayerMgr playerMgr;
        LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex());

        m_deathsPerLevel.SetPosition(Vec2(0.75f, -0.9f));
        m_deathsPerLevel.m_bScreenSpace = true;
        m_deathsPerLevel.m_alignment = TEXT_ALIGNMENT_RIGHT;
        m_deathsPerLevel.m_color = Vec4(1.0f, 1.0f, 1.0f, 0.5f);

        m_deathsTotal.SetPosition(Vec2(0.95f, -0.9f));
        m_deathsTotal.m_bScreenSpace = true;
        m_deathsTotal.m_alignment = TEXT_ALIGNMENT_RIGHT;
        m_deathsTotal.m_color = Vec4(1.0f, 1.0f, 1.0f, 0.5f);

        //         m_deathsSplitter.SetPosition(Vec2(0.8f, -0.9f));
        //         m_deathsSplitter.m_bScreenSpace = true;
        //         m_deathsSplitter.m_alignment = TEXT_ALIGNMENT_CENTER;
        //         m_deathsSplitter.SetText("|");

        m_bIsBossLevel =
            levelInfo.strFilename == "../Resources/Levels/Chapter1/chapter1_level20.env" ||
            levelInfo.strFilename == "../Resources/Levels/Chapter2/chapter2_level20.env" ||
            levelInfo.strFilename == "../Resources/Levels/Chapter3/chapter3_level20.env" ||
            levelInfo.strFilename == "../Resources/Levels/Chapter4/chapter4_level06.env";

        m_bIsCutScene = levelInfo.bCutScene;

        if (strPreviousState == "PauseMenueState")
        {
            gEngine->GetAudioSystem()->ContinueSound();
        }

        bool isMenuMusicPlaying = gEngine->GetAudioSystem()->GetCurrentMusicType() == MUSIC_TYPE::MUSIC_TYPE_MENU;
        bool isBossMusicPlaying = gEngine->GetAudioSystem()->GetCurrentMusicType() == MUSIC_TYPE::MUSIC_TYPE_BOSS;

        if (isMenuMusicPlaying || isBossMusicPlaying != m_bIsBossLevel || m_bIsCutScene)
        {
            gEngine->GetAudioSystem()->StopMusic();
        }

        gFontMgr->AddGetPtr(fontGameHeader.GetKey());
        gFontMgr->AddGetPtr(fontMenue.GetKey());

        m_bKilled = false;

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CGameState::Unload()
    {
        CState::Unload();

        gFontMgr->Release(fontMenue.GetKey());
        gFontMgr->Release(fontGameHeader.GetKey());

        m_pCharacter = nullptr;

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CGameState::Update(float time, float dt, RenderQueue& renderQueue)
    {
        m_deathsPerLevel.m_color.a = 0.0f;
        m_deathsTotal.m_color.a = 0.0f;

        if (m_pCharacter == nullptr)
            m_pCharacter = static_cast<CCharacter3DEntity*>(gEngine->GetEntityMgr()->GetCharacter());

        ProcessInput(dt, renderQueue);

        float m_levelTime = gEngine->GetWorldMgr()->GetLevelTime();

        if (m_pCharacter == nullptr)
            return;

        if (m_pCharacter->GetKilled() &&
            m_pCharacter->GetCurrentKillTime() < 0.0f)
        {
            // If the character has been killed and the killTimer has elapsed,
            // restart the session
            gEngine->GetWorldMgr()->SetGameMode(true);

            m_bKilled = true;

            return;
        }

        if (m_pCharacter->GetKilled() && m_pCharacter->GetCurrentKillTime() > 0.0f)
        {
            float a = m_pCharacter->GetCurrentKillTime() / m_pCharacter->GetKillTime();

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 0.5f * (1.0f - a);
        }
        else if (m_levelTime <= 0.2f)
        {
            float a = m_levelTime / 0.2f;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 0.5f * (1.0f - a);
        }
        

        if (m_bIsBossLevel || m_bIsCutScene)
        {
            //gEngine->GetAudioSystem()->PlayMusicForBoss();
        }
        else
        {
            CSingletonPlayerMgr playerMgr;
            int chapter = playerMgr->GetCurrentChapterIndex();
            gEngine->GetAudioSystem()->PlayMusicForChapter(chapter);
        }


        std::stringstream numKillsPerLevel, numKillsTotal;

        CSingletonPlayerMgr playerMgr;

        if (!playerMgr->GetCurrentLevelInfo().bCutScene)
        {
            int killsPerLevel = playerMgr->GetCurrentLevelInfo().numKills;
            int killsTotal = playerMgr->GetNumKills();

            if (m_bKilled && m_levelTime < 0.75f)
            {
                killsPerLevel--;
                killsTotal--;
            }

            numKillsPerLevel << killsPerLevel;
            numKillsTotal << killsTotal;

            m_deathsPerLevel.SetText(numKillsPerLevel.str());
            m_deathsTotal.SetText(numKillsTotal.str());


            if (m_levelTime <= 0.5f)
            {
                m_deathsPerLevel.m_color.a = Clamp(m_levelTime / 0.5f, 0.0f, 1.0f);
                m_deathsTotal.m_color.a = Clamp(m_levelTime / 0.5f, 0.0f, 1.0f);
            }
            else if (m_levelTime <= 2.5f)
            {
                m_deathsPerLevel.m_color.a = 1.0f;
                m_deathsTotal.m_color.a = 1.0f;
            }
            else if (m_levelTime <= 3.0f)
            {
                m_deathsPerLevel.m_color.a = 1.0f - Clamp((m_levelTime - 2.5f) / 0.5f, 0.0f, 1.0f);
                m_deathsTotal.m_color.a = 1.0f - Clamp((m_levelTime - 2.5f) / 0.5f, 0.0f, 1.0f);
            }
            else
            {
                m_bKilled = false;
            }

            renderQueue.m_textRenderObjects[fontMenue].push_back(m_deathsPerLevel);
            renderQueue.m_textRenderObjects[fontMenue].push_back(m_deathsTotal);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGameState::ProcessInput(float dt, RenderQueue& renderQueue)
    {
        if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK) ||
            gEngine->GetInput()->IsActionJustTriggered(ACTION_START))
        {
            if (!gEngine->GetEditor())
            {
                gEngine->SetNewState("PauseMenueState");
            }
            else
            {
                gEngine->SetNewState("EditorState");
            }
        }

        static bool bSFX = true;
        static bool bMusic = true;
        static bool bShowFPS = false;
        static bool bRenderMeshes = true;
        static bool bRenderLevelTerrain = true;
        static bool bRenderLevelBorder = true;
        static bool bRenderParticles = true;
        static bool bRenderLighting = true;
        static bool bBakedLighting = false;
        static bool bPaused = false;

#ifndef ENV_FINAL

        if (gEngine->GetInput()->IsKeyJustDown('P'))
        {
            bPaused = !bPaused;
        }

        if (gEngine->GetInput()->IsKeyJustDown('C'))
        {
            gEngine->ShutDown();
        }

        gEngine->SetPaused(bPaused);

        if (gEngine->GetInput()->IsKeyDown(VK_CONTROL))
        {
            if (gEngine->GetInput()->IsKeyJustDown(VK_F11))
            {
                bSFX = !bSFX;

                if (bSFX)
                    gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_EFFECTS, false);
                else
                    gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_EFFECTS, true);
            }
            else if (gEngine->GetInput()->IsKeyJustDown('F'))
            {
                bShowFPS = !bShowFPS;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('R'))
            {
                gEngine->SetReloadResources(!gEngine->GetReloadResources());
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_F5))
            {
                bRenderMeshes = !bRenderMeshes;
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_F6))
            {
                bRenderLevelTerrain = !bRenderLevelTerrain;
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_F7))
            {
                bRenderLevelBorder = !bRenderLevelBorder;
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_F8))
            {
                bRenderParticles = !bRenderParticles;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('L'))
            {
                bRenderLighting = !bRenderLighting;
            }
        }


        if (gEngine->GetInput()->IsKeyJustDown(VK_F12))
        {
            bMusic = !bMusic;

            if (bMusic)
                gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_MUSIC, false);
            else
                gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_MUSIC, true);
        }

        if (bShowFPS)
        {
            std::stringstream str;
            str << "FPS: " << (int)gEngine->GetFramerate() << std::endl;
            renderQueue.m_textRenderObjects[fontGameHeader].push_back(TextRenderObject(str.str(), Vec4(0.0f, 1.0f, 0.0f, 1.0f), Vec2(0.05f, 0.1f)));
        }

        if (bBakedLighting)
        {
            std::stringstream str;
            str << "Lighting: Baked" << std::endl;
            renderQueue.m_textRenderObjects[fontGameHeader].push_back(TextRenderObject(str.str(), Vec4(0.0f, 1.0f, 0.0f, 1.0f), Vec2(0.05f, 0.2f)));
        }
        else
        {
            std::stringstream str;
            str << "Lighting: Dynamic" << std::endl;
            renderQueue.m_textRenderObjects[fontGameHeader].push_back(TextRenderObject(str.str(), Vec4(0.0f, 1.0f, 0.0f, 1.0f), Vec2(0.05f, 0.2f)));
        }

#endif // ENV_FINAL

        renderQueue.bRenderMeshes = bRenderMeshes;
        renderQueue.bRenderLevelTerrain = bRenderLevelTerrain;
        renderQueue.bRenderLevelBorder = bRenderLevelBorder;
        renderQueue.bRenderParticles = bRenderParticles;
        renderQueue.lightingMode = RenderQueue::LIGHTING_MODE_FINAL;
    }


} // env