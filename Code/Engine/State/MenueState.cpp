#include <Engine/State/MenueState.h>
#include <Engine/State/StateHelper.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/PlayerMgr.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>

namespace env
{

    const Vec3 MENUE_BUTTON_SCALE = Vec3(0.9f, 0.8f, 1.0f);
    const Vec3 MENUE_BUTTON_SCALE_SELECTION = Vec3(1.05f, 0.85f, 1.0f);

    const Vec3 MENUE_BUTTON_COLOR = Vec3(104.0f / 255.0f, 156.0f / 255.0f, 185.0f / 255.0f);
    const Vec3 MENUE_BUTTON_COLOR_SELECTION = Vec3(104.0f / 255.0f, 156.0f / 255.0f, 185.0f / 255.0f);

    const Vec3 LOGO_COLOR = Vec3(73.0f / 255.0f, 189.0f / 255.0f, 222.0f / 255.0f);

    const float MENUE_BUTTON_ALPHA = 0.4f;
    const float MENUE_BUTTON_ALPHA_SELECTION = 1.0f;

    const float MENUE_SELECTION_TIME = 0.25f;
    const float TARGET_BLOOM_SCALE = 2.0f;

    const std::string CMenueState::STATE_NAME = "MenueState";
    const Vec3 MENUE_BUTTON_START_POSITION = Vec3(1128.0f, 864.0f, 90.0f);

    const FontInfo fontGameStart = FontInfo("FontMenue");

    //-----------------------------------------------------------------------------------
    CMenueState::CMenueState()
        : CState(CMenueState::STATE_NAME)
        , m_state(MENUE_STATE_PRESSSTART)
        , m_pIntroLogo(NULL)
        , m_pIntroLogoNode(NULL)
        , m_mainSelection(0)
        , m_currentSelectionTime(0.0f)
    {

        m_menueItems.resize(4);
        m_menueItems[0].first.SetMaterialFilename("GUI/Main/btnStart.png");
        m_menueItems[1].first.SetMaterialFilename("GUI/Main/btnOptions.png");
        m_menueItems[2].first.SetMaterialFilename("GUI/Main/btnCredits.png");
        m_menueItems[3].first.SetMaterialFilename("GUI/Main/btnQuit.png");

        for (int i = 0; i < 4; ++i)
        {
            m_menueItems[i].first.SetUpdateRectByAnimation(true);
            m_menueItems[i].first.AttachNode(&m_menueItems[i].second);
            m_menueItems[i].first.SetColor(Vec4(MENUE_BUTTON_COLOR, MENUE_BUTTON_ALPHA));
            m_menueItems[i].second.SetScale(MENUE_BUTTON_SCALE);
        }

        m_pIntroLogo = new CSprite();
        m_pIntroLogoNode = new CSceneNode();
        m_pIntroLogo->SetMaterialFilename("GUI/Main/logo.png");
        m_pIntroLogo->SetUpdateRectByAnimation(true);
        m_pIntroLogo->AttachNode(m_pIntroLogoNode);
        m_pIntroLogo->SetColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pIntroLogo->SetScreenSpace(true);
        m_pIntroLogo->SetSpritePipeline(SPRITE_PIPELINE_AFTER_FADE_SCREEN);
        m_pIntroLogoNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

        SetState(MENUE_STATE_PRESSSTART);
    }

    //-----------------------------------------------------------------------------------
    CMenueState::~CMenueState()
    {
        ENV_DELETE(m_pIntroLogoNode);
        ENV_DELETE(m_pIntroLogo);

        Unload();
    }

    //-----------------------------------------------------------------------------------
    inline float GetStripes(CStripeManager& stripeManager)
    {
        CSingletonPlayerMgr playerMgr;

        unsigned int chapter = playerMgr->GetCurrentChapterIndex() + 1;

        std::stringstream str;
        str << "../Stripes/tmpl_chap" << chapter << "_dest_01.stp";

        CScript* pScript = gScriptMgr->AddGetPtr(str.str());

        if (!pScript)
            return 0.0f;

        StripeParameters params;
        params.LoadTemplate(*pScript);
        params.m_numSlices = 18;
        
        gScriptMgr->Release(str.str());

        const RenderQualityParams& renderParams = gRenderSystem->GetRenderQualityParams();

        float aspectRatio = static_cast<float>(renderParams.screenHeight) /
            static_cast<float>(renderParams.screenWidth);

        params.m_aspectRatio = aspectRatio;

        return stripeManager.Generate(CDefaultStripeGenerator(params));
    }

    //-----------------------------------------------------------------------------------
    bool CMenueState::Load(const std::string& strPreviousState)
    {
        CTerrainMgrGPU* pLevelRO = gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLevelRenderObject();
        const std::vector< Matrix4x4 >& transformations = pLevelRO->GetLayerTransformations();

        m_currentSelectionTime = 0.0f;

        // START BUTTON
        //
        for (int i = 0; i < m_menueItems.size(); ++i)
        {
            //MENUE_BUTTON_START_POSITION

            Matrix4x4 mtxTranslation;
            Vec3 pos = MENUE_BUTTON_START_POSITION + Vec3(0.0f, -i * 3 * 32.0f, 0.0f);
            pos = Vec3(Vec4(pos, 1.0f) * transformations[1]);
            float rotation = pLevelRO->GetRotation(pos.z);

            m_menueItems[i].second.SetOrientation(CQuaternion(0.0f, 0.0f, rotation));
            m_menueItems[i].second.SetPosition(pos);
            m_menueItems[i].first.SetColor(Vec4(MENUE_BUTTON_COLOR, MENUE_BUTTON_ALPHA));
            m_menueItems[i].second.SetScale(MENUE_BUTTON_SCALE);
        }

        m_pMenueTransitionInSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_IN);
        m_pMenueTransitionInSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueTransitionOutSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_OUT);
        m_pMenueTransitionOutSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueSelectionSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_MENUE_SELECTION);
        m_pMenueSelectionSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);
        m_pMenueSelectionSound->SetVolume(0.05f);

        m_pMenueConfirmationSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_MENUE_CONFIRMATION);
        m_pMenueConfirmationSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);
        m_pMenueConfirmationSound->SetVolume(0.2f);

        if (strPreviousState == "LevelSelectionState")
        {
            SetState(MENUE_STATE_LOADFROMLEVELSELECTION);
        }
        else if (strPreviousState == "SettingsState")
        {
            SetState(MENUE_STATE_LOADFROMSETTINGS);
        }

        GetFadeTopColor(FADE_COLOR_TOP, FADE_COLOR_BOT);

        m_stripeDuration = GetStripes(m_stripeManager);
        m_stripeTime = m_stripeDuration;

        MENUE_STATE_DURATIONS[MENUE_STATE_FADEOUT] = m_stripeDuration;

        gFontMgr->AddGetPtr(fontGameStart.GetKey());

        m_textPressStart.m_strText = L"PRESS START";
        m_textPressStart.SetPosition(Vec2(0.0f, -0.9f));
        m_textPressStart.m_bScreenSpace = true;
        m_textPressStart.m_alignment = TEXT_ALIGNMENT_CENTER;

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMenueState::Unload()
    {
        gFontMgr->Release(fontGameStart.GetKey());

        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_TRANSITION_IN);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_TRANSITION_OUT);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_MENUE_SELECTION);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_MENUE_CONFIRMATION);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CMenueState::Update(float time, float dt, RenderQueue& renderQueue)
    {
        CSingletonPlayerMgr playerMgr;

        CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();

        Vec3 camStartPos(MENUE_BUTTON_START_POSITION.x, 1065.0f, -850.0f),
            camEndPos(1200.0f + 125, 400.0f + 370.0f - 210, -1100.0f);

        Vec3 camStartOrientation(0.0f, 0.0f, 0.0f),
            camEndOrientation(-0.1f, -0.23f, -0.2f);

        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop = FADE_COLOR_TOP;
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 0.0f;

        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot = FADE_COLOR_BOT;
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 0.0f;

        renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorTop = FADE_COLOR_TOP;
        renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorBot = FADE_COLOR_BOT;


        if (m_state == MENUE_STATE_PRESSSTART)
        {
            m_currentStateTime -= dt;

            pCam->GetAttachedNode()->SetPosition(camStartPos);
            pCam->GetAttachedNode()->SetOrientation(CQuaternion(camStartOrientation.x, camStartOrientation.y, camStartOrientation.z));

            gEngine->GetWorldMgr()->GetScenario()->SetBloomScale(0.0f);

            float alpha = std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f);
            m_fade.fadeBarsType = FADE_BARS_FROM_BOTTOM_TO_TOP;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot = Vec4(0, 0, 0, 1.0f);
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop = Vec4(0, 0, 0, 1.0f);
            renderQueue.m_pFadeBarsRenderObject = &m_fade;

            Vec4 pressStartColor(1.0f, 1.0f, 1.0f, 1.0f);

            if (m_currentStateTime >= 0.0f)
            {
                pressStartColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f - alpha);
            }
            else
            {
                static float pressStartTimer = 2.0f;
                if (pressStartTimer >= 1.0f)
                    pressStartColor = Vec4(1.0f, 1.0f, 1.0f, 0.2f + 0.8f * (pressStartTimer - 1.0f));
                else
                    pressStartColor = Vec4(1.0f, 1.0f, 1.0f, 0.2f + 0.8f * (1.0f - pressStartTimer));

                pressStartTimer -= dt;
                if (pressStartTimer < 0.0f)
                    pressStartTimer = 2.0f;
            }

            m_textPressStart.m_color = pressStartColor;

            renderQueue.m_textRenderObjects[fontGameStart].push_back(m_textPressStart);

            m_pIntroLogo->SetAlpha(1.0f - alpha);

            renderQueue.m_spriteRenderObjects.push_back(m_pIntroLogo->GetRenderObject());

            //             m_stripeManager.AddToRenderQueue(renderQueue);
            //             renderQueue.m_stripeRenderObject.m_stripesAmount = 0.0f;
            //             renderQueue.m_stripeRenderObject.m_stripeTime = m_stripeDuration;

            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_START) || 
                gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY))
            {
                m_pMenueTransitionInSound->PlaySound();

                SetState(MENUE_STATE_FADEOUT);
            }
            else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
            {
                gEngine->ShutDown();
            }
        }
        else if (m_state == MENUE_STATE_FADEOUT)
        {
            m_currentStateTime -= dt;

            float alphaIntroLogo = pow(std::max(m_currentStateTime / 0.6f, 0.0f), 3);

            m_stripeManager.AddToRenderQueue(renderQueue);

            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot = Vec4(0, 0, 0, 1.0f);
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop = Vec4(0, 0, 0, 1.0f);
            renderQueue.m_stripeRenderObject.m_stripesAmount = 1 - m_currentStateTime / m_stripeDuration;
            renderQueue.m_stripeRenderObject.m_stripeTime = m_stripeDuration;
            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;

            m_textPressStart.m_color = Vec4(alphaIntroLogo, alphaIntroLogo, alphaIntroLogo, alphaIntroLogo * alphaIntroLogo);

            renderQueue.m_textRenderObjects[fontGameStart].push_back(m_textPressStart);

            //m_pIntroLogo->SetAlpha(alphaIntroLogo);
            renderQueue.m_spriteRenderObjects.push_back(m_pIntroLogo->GetRenderObject());

            m_fade.fadeBarsType = FADE_BARS_FROM_BOTTOM_TO_TOP;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 1.0f;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 1.0f;
            renderQueue.m_pFadeBarsRenderObject = &m_fade;

            if (m_currentStateTime < 0.0f)
            {
                m_pMenueTransitionOutSound->PlaySound();
                
                SetState(MENUE_STATE_FADEIN);
            }
        }
        else if (m_state == MENUE_STATE_FADEIN)
        {
            m_currentStateTime -= dt;

            float alphaIntroLogo = pow(std::max(m_currentStateTime / 0.6f, 0.0f), 3);

            m_fade.fadeBarsType = FADE_BARS_FROM_BOTTOM_TO_TOP;
            m_fade.fadeBarsAmount = alphaIntroLogo;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop = Vec4(FADE_COLOR_TOP.x, FADE_COLOR_TOP.y, FADE_COLOR_TOP.z, alphaIntroLogo);
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = alphaIntroLogo;
            renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = alphaIntroLogo;
            Vec4 fadeFrame = (1.0f - alphaIntroLogo) * Vec4(0.1f, 0.1f, 0.1f, 0.1f) + alphaIntroLogo * Vec4(1.0f, 1.0f, 1.0f, 1.0f);
            fadeFrame = Vec4(0.1f, 0.1f, 0.1f, 0.1f);
            renderQueue.m_fadeFrameRenderObject.m_fadeFrame = fadeFrame;
            renderQueue.m_pFadeBarsRenderObject = &m_fade;
            renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;

            gEngine->GetWorldMgr()->GetScenario()->SetBloomScale(0.0f);

            if (m_currentStateTime < 0.0f)
            {
                SetState(MENUE_STATE_CAMERAMOVE);
            }
        }
        else if (m_state == MENUE_STATE_CAMERAMOVE)
        {
            m_currentStateTime -= dt;

            renderQueue.m_postProcessRenderObject.m_posterizationAmount = m_currentStateTime / GetCurrentStateDuration();


            if (m_currentStateTime >= 0.5f)
            {
                float alpha = pow(std::max((m_currentStateTime - 0.5f) / (GetCurrentStateDuration() * 0.5f), 0.0f), 3);

                pCam->GetAttachedNode()->SetPosition(camStartPos * alpha + camEndPos * (1.0f - alpha));
                Vec3 orientation = camStartOrientation * alpha + camEndOrientation * (1.0f - alpha);
                pCam->GetAttachedNode()->SetOrientation(CQuaternion(orientation.x, orientation.y, orientation.z));

                CQuaternion q2 = pCam->GetAttachedNode()->GetOrientation();
                q2.GetRotationMatrix();

                renderQueue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(0.1f, 0.1f, 0.1f, 0.1f);
                renderQueue.m_pFadeBarsRenderObject = &m_fade;
            }
            else if (m_currentStateTime >= 0.2f)
            {
                float alpha = (m_currentStateTime - 0.2f) / 0.3f;
                alpha = alpha * alpha * alpha;

                renderQueue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(0.1f, 0.1f, 0.1f, 0.1f) * alpha;
                renderQueue.m_pFadeBarsRenderObject = &m_fade;

                gEngine->GetWorldMgr()->GetScenario()->SetBloomScale((1.0f - alpha) * TARGET_BLOOM_SCALE);
            }

            if (m_currentStateTime < 0.0f)
            {
                SetState(MENUE_STATE_MENUESELECTION);

                renderQueue.m_postProcessRenderObject.m_posterizationAmount = 0.0f;

                m_currentSelectionTime = MENUE_SELECTION_TIME;
            }
        }
        else if (m_state == MENUE_STATE_LOADFROMSETTINGS)
        {
            m_currentStateTime -= dt;

            float alpha = 1.0f - pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_menueItems.size(); ++i)
            {
                if (i == m_mainSelection)
                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA_SELECTION * alpha);
                else
                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA * alpha);
            }

            if (m_currentStateTime < -0.0f)
            {
                SetState(MENUE_STATE_MENUESELECTION);

                m_currentSelectionTime = MENUE_SELECTION_TIME;
            }
        }
        else if (m_state == MENUE_STATE_LOADFROMLEVELSELECTION)
        {
            if (m_currentStateTime >= GetCurrentStateDuration())
            {
                m_pMenueTransitionInSound->PlaySound();
            }
            m_currentStateTime -= dt;

            float alpha = pow(std::max((m_currentStateTime / GetCurrentStateDuration()), 0.0f), 3);
            FadeMenuePage(alpha, renderQueue);


            pCam->GetAttachedNode()->SetPosition(camEndPos);
            pCam->GetAttachedNode()->SetOrientation(CQuaternion(camEndOrientation.x,
                                                                camEndOrientation.y,
                                                                camEndOrientation.z));

            if (m_currentStateTime < 0.0f)
            {
                SetState(MENUE_STATE_MENUESELECTION);

                m_currentSelectionTime = MENUE_SELECTION_TIME;
            }
        }
        else if (m_state == MENUE_STATE_UNLOAD)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);
            FadeMenuePage(1.0f - alpha, renderQueue);

            if (m_currentStateTime < -0.20f)
            {
                gEngine->SetNewState("LevelSelectionState");
            }
        }
        else if (m_state == MENUE_STATE_UNLOADQUIT)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max((m_currentStateTime / GetCurrentStateDuration()), 0.0f), 3);

            FadeMenuePage(1.0f - alpha, renderQueue);


            if (m_currentStateTime < 0.0f)
            {
                gEngine->ShutDown();
            }
        }
        else if (m_state == MENUE_STATE_UNLOADSETTINGS)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_menueItems.size(); ++i)
            {
                if (i == m_mainSelection)
                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA_SELECTION * alpha);
                else
                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA * alpha);
            }

            if (m_currentStateTime < -0.0f)
            {
                SetState(MENUE_STATE_MENUESELECTION);

                gEngine->SetNewState("SettingsState");
            }
        }
        else if (m_state == MENUE_STATE_UNLOADCREDITS)
        {
            m_currentStateTime -= dt;

            float alpha = pow(std::max(m_currentStateTime / GetCurrentStateDuration(), 0.0f), 3);

            for (unsigned int i = 0; i < m_menueItems.size(); ++i)
            {
                if (i == m_mainSelection)
                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA_SELECTION * alpha);
                else
                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA * alpha);
            }

            if (m_currentStateTime < -0.0f)
            {
                SetState(MENUE_STATE_MENUESELECTION);

                gEngine->SetNewState("CreditsState");
            }
        }
        else if (m_state == MENUE_STATE_MENUESELECTION)
        {
            m_currentStateTime -= dt;

            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_UP))
            {
                if (m_mainSelection == 0)
                    m_mainSelection = 3;
                else
                    --m_mainSelection;

                m_currentSelectionTime = MENUE_SELECTION_TIME;
                m_pMenueSelectionSound->PlaySound();
            }
            else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_DOWN))
            {
                m_mainSelection = (m_mainSelection + 1) % 4;

                m_currentSelectionTime = MENUE_SELECTION_TIME;
                m_pMenueSelectionSound->PlaySound();
            }

            m_currentSelectionTime = std::max(0.0f, m_currentSelectionTime - dt);
            float alphaSelectionTime = m_currentSelectionTime / MENUE_SELECTION_TIME;
            alphaSelectionTime = std::pow(alphaSelectionTime, 2.0f);


            for (int i = 0; i < m_menueItems.size(); ++i)
            {
                if (i == m_mainSelection)
                {
                    float colorAlpha = MENUE_BUTTON_ALPHA * alphaSelectionTime + MENUE_BUTTON_ALPHA_SELECTION * (1.0f - alphaSelectionTime);

                    m_menueItems[i].first.SetAlpha(colorAlpha);

                    Vec3 scaling = MENUE_BUTTON_SCALE * alphaSelectionTime + (1.0f - alphaSelectionTime) * MENUE_BUTTON_SCALE_SELECTION;
                    m_menueItems[i].second.SetScale(scaling);
                    //m_menueItems[i].first.GetMaterial()->SetSequence(1);
                }
                else
                {
                    Vec3 currentScale = m_menueItems[i].second.GetScale();

                    float scalingX = 0.0f, scalingY = 0.0f;
                    if (currentScale.x > MENUE_BUTTON_SCALE.x)
                    {
                        scalingX = (currentScale.x - 1.0f) * dt;
                        if (scalingX < (1.5f * dt))
                            scalingX = 1.5f * dt;
                    }

                    if (currentScale.y > MENUE_BUTTON_SCALE.y)
                    {
                        scalingY = (currentScale.y - 1.0f) * dt;
                        if (scalingY < (1.5f * dt))
                            scalingY = 1.5f * dt;
                    }


                    m_menueItems[i].first.SetAlpha(MENUE_BUTTON_ALPHA);
                    m_menueItems[i].second.SetScale(Vec3(currentScale.x - scalingX, currentScale.y - scalingY, 1.0f));
                    //m_menueItems[i].first.GetMaterial()->SetSequence(0);
                }

            }


            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY) || gEngine->GetInput()->IsActionJustTriggered(ACTION_START))
            {
                if (m_mainSelection == MENUE_ITEM_START)
                {
                    SetState(MENUE_STATE_UNLOAD);
                    m_pMenueTransitionOutSound->PlaySound();
                }
                else if (m_mainSelection == MENUE_ITEM_QUIT)
                {
                    SetState(MENUE_STATE_UNLOADQUIT);
                }
                else if (m_mainSelection == MENUE_ITEM_OPTIONS)
                {
                    SetState(MENUE_STATE_UNLOADSETTINGS);
                }
                else if (m_mainSelection == MENUE_ITEM_CREDITS)
                {
                    SetState(MENUE_STATE_UNLOADCREDITS);
                }
            }
        }

        for (unsigned int i = 0; i < m_menueItems.size(); ++i)
            renderQueue.m_spriteRenderObjects.push_back(m_menueItems[i].first.GetRenderObject());

        gEngine->GetAudioSystem()->PlayMusicForMenu();
    }

    //-----------------------------------------------------------------------------------
    void CMenueState::FadeMenuePage(float alpha, RenderQueue& renderQueue)
    {
        m_fade.fadeBarsType = FADE_BARS_FROM_BOTTOM_TO_TOP;
        m_fade.fadeBarsAmount = alpha;

        renderQueue.m_fadeFrameRenderObject.m_fadeFrame = alpha * Vec4(0.1f, 0.1f, 0.1f, 0.1f);
        renderQueue.m_postProcessRenderObject.m_posterizationAmount = 0.0f;
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 0.0f;
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 0.0f;
        renderQueue.m_pFadeBarsRenderObject = &m_fade;
    }

    //-----------------------------------------------------------------------------------
    void CMenueState::OnResetDevice(int screenWidth, int screenHeight)
    {

    }

} // env