#include <Engine/State/CreditsState.h>
#include <Engine/Audio/AudioSystem.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/GameLoader.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/RenderDevice.h>

namespace env
{
    const std::string   CCreditsState::STATE_NAME = "CreditsState";
    const float         CCreditsState::FADE_IN_AMOUNT = 0.25f;
    const float         CCreditsState::FADE_SCREEN_AMOUNT = 1.0f;
    const Vec4          CCreditsState::FADE_FRAME_AMOUNT = Vec4(0.05f, 0.05f, 0.40f, 0.40f);
    const FontInfo      CCreditsState::fontMenue = FontInfo("FontMenue");
    const FontInfo      CCreditsState::fontMenueTitle = FontInfo("FontMenueTitle", 40, true);

    //-----------------------------------------------------------------------------------
    CCreditsState::CCreditsState()
        : CState(CSettingsState::STATE_NAME)
        , m_selection(0)
        , m_pGameSound(nullptr)
    {

        Vec2 settingsFirstPosition = Vec2(0.0f, 0.0f + 5.5f * 0.1f);

        m_title.m_strText = L"C R E D I T S";
        m_title.SetPosition(Vec2(0.0f, 0.7f));
        m_title.m_color = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
        m_title.m_bScreenSpace = true;
        m_title.m_alignment = TEXT_ALIGNMENT_CENTER;


        std::vector<std::string> creditsStr;
        creditsStr.push_back("DEVELOPER");
        creditsStr.push_back("CHRI STI AN HAMM");
        creditsStr.push_back(" ");
        creditsStr.push_back("ASSOCI ATE DEVELOPER");
        creditsStr.push_back("MI CHAI L  OSTROWSKI");
        creditsStr.push_back(" ");
        creditsStr.push_back("NARRATOR");
        creditsStr.push_back("LAURA  K  WELSH");
        creditsStr.push_back(" ");
        creditsStr.push_back("SOUNDTRACK");
        creditsStr.push_back("I K- SOUND");

        m_credits.resize(creditsStr.size());

        for (int i = 0; i < m_credits.size(); ++i) {
            m_credits[i].SetPosition(settingsFirstPosition - (float)i * Vec2(0.0f, 0.1f));
            m_credits[i].m_bScreenSpace = true;
            m_credits[i].m_alignment = TEXT_ALIGNMENT_CENTER;
            m_credits[i].SetText(creditsStr[i]);
        }

        m_spriteFMod.SetMaterialFilename("Sprites/fmod.png");
        m_spriteFMod.SetScreenSpace(true);
        m_spriteFMod.AttachNode(&m_nodeFMod);
        m_spriteFMod.SetSpritePipeline(SPRITE_PIPELINE_AFTER_FADE);

        m_nodeFMod.SetPosition(Vec3(0.0f, -0.7f, 0.0f));
        m_nodeFMod.SetScale(Vec3(0.6f, 0.6f, 1.0f));
    }

    //-----------------------------------------------------------------------------------
    CCreditsState::~CCreditsState()
    {
        Unload();
    }

    //-----------------------------------------------------------------------------------
    bool CCreditsState::Load(const std::string& strPreviousState)
    {
        m_strPreviousState = strPreviousState;

        gEngine->SetPaused(true);

        SetState(SETTINGS_STATE_LOAD);

        m_selection = 0;

        gFontMgr->AddGetPtr(fontMenue.GetKey());
        gFontMgr->AddGetPtr(fontMenueTitle.GetKey());

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CCreditsState::Unload()
    {
        gFontMgr->Release(fontMenueTitle.GetKey());
        gFontMgr->Release(fontMenue.GetKey());

        gEngine->SetPaused(false);

        for (unsigned int i = 0; i < m_credits.size(); ++i)
        {
            m_credits[i].m_color.w = SETTINGS_MENUE_ITEM_COLOR.w;
        }

        CState::Unload();
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CCreditsState::Update(float time, float dt, RenderQueue& renderQueue)
    {
        m_fade.fadeBarsType = FADE_BARS_FROM_TOP_AND_BOTTOM;

        UpdateInput();

        if (m_state == SETTINGS_STATE_LOAD)
        {
            m_currentStateTime -= dt;

            float alpha = 1.0f - pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_credits.size(); ++i)
            {
                m_credits[i].m_color.w = SETTINGS_MENUE_ITEM_SELECTION_COLOR.w * alpha;
            }

            m_title.m_color.w = alpha;

            Vec3 scaling = (1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(1.0f, 1.0f, 1.0f);

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

            for (unsigned int i = 0; i < m_credits.size(); ++i)
            {
                m_credits[i].m_color.w = SETTINGS_MENUE_ITEM_SELECTION_COLOR.w * alpha;
            }

            m_title.m_color.w = alpha;

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
            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = FADE_SCREEN_AMOUNT;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = FADE_SCREEN_AMOUNT;

            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = FADE_FRAME_AMOUNT;

            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
            {
                SetState(SETTINGS_STATE_UNLOAD);
            }
        }

        for (int i = 0; i < m_credits.size(); ++i) {
            renderQueue.m_textRenderObjects[fontMenue].push_back(m_credits[i]);
        }

        renderQueue.m_textRenderObjects[fontMenueTitle].push_back(m_title);
        renderQueue.m_pFadeBarsRenderObject = &m_fade;
        renderQueue.m_spriteRenderObjects.push_back(m_spriteFMod.GetRenderObject());
    }

    //-----------------------------------------------------------------------------------
    void CCreditsState::UpdateInput()
    {
        if (gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY) || gEngine->GetInput()->IsActionJustTriggered(ACTION_START))
        {
            SetState(SETTINGS_STATE_UNLOAD);
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
        {
            SetState(SETTINGS_STATE_UNLOAD);
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_UP))
        {

        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_DOWN))
        {

        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_LEFT))
        {

        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_RIGHT))
        {

        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_START))
        {

        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
        {

        }


    }
} // env