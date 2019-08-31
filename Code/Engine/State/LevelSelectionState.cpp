#include <Engine/State/LevelSelectionState.h>
#include <Engine/State/StateHelper.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/GameLoader.h>
#include <Graphic/Core/Material.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    const Vec3 LEVEL_TITLE_SCALE = Vec3(0.9f, 0.8f, 1.0f);
    const Vec3 LEVEL_TITLE_SCALE_SELECTION = Vec3(1.05f, 0.85f, 1.0f);
    const Vec3 LEVEL_TITLE_COLOR = Vec3(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
    const Vec3 LEVEL_TITLE_COLOR_SELECTION = Vec3(104.0f / 255.0f, 156.0f / 255.0f, 185.0f / 255.0f);
    const Vec3 LEVEL_TITLE_COLOR_FINISHED = Vec3(104.0f / 255.0f, 156.0f / 255.0f, 185.0f / 255.0f);
    const Vec3 LEVEL_TITLE_COLOR_FINISHED_SELECTION = Vec3(104.0f / 255.0f, 156.0f / 255.0f, 185.0f / 255.0f);
    const Vec3 CHAPTER_TITLE_COLOR = Vec3(78.0f / 255.0f, 120 / 255.0f, 130.0f / 255.0f);

    const Vec4 CHAPTER_COMPLETE_FRAME = Vec4(0.1f, 0.1f, 0.3f, 0.3f);

    const float LEVEL_TITLE_ALPHA = 0.7f;
    const float LEVEL_TITLE_ALPHA_SELECTION = 1.0f;

    const std::string LEVEL_MATERIAL = "levelSelection.mtrl";
    const std::string CLevelSelectionState::STATE_NAME = "LevelSelectionState";

    //-----------------------------------------------------------------------------------
    CLevelSelectionState::CLevelSelectionState()
        : CState(CLevelSelectionState::STATE_NAME)
        , m_pPart1LevelSelectionMtrl(0)
        , m_pTitleNode(0)
        , m_pTitleSprite(0)
        , m_pChapterNode(0)
        , m_pChapterSprite(0)
        , m_pCompleteNode(0)
        , m_pCompleteSprite(0)
        , m_pTimeBackground(0)
        , m_pTimeBackgroundNode(0)
        , m_pTimeBackgroundGold(0)
        , m_pTimeBackgroundSilver(0)
        , m_pTimeBackgroundBronze(0)
        , m_pKillsBackground(0)
        , m_pKillsBackgroundNode(0)
        , m_currentSelection(0)
        , m_state(STATE_LOAD)
        , m_loadTime(0.4f)
        , m_unloadTime(0.4f)
        , m_currentSelectionTime(0.25f)
        , m_bChangeChapterLeft(false)
        , m_bLoadFromChapter(false)
        , m_bPlayChapterFinish(false)
    {

    }

    //-----------------------------------------------------------------------------------
    CLevelSelectionState::~CLevelSelectionState()
    {
        Unload();
    }


    const Vec3 startPosition(1132.0f, 1350.0f + 572.0f, 80.0f);

    //-----------------------------------------------------------------------------------
    bool CLevelSelectionState::Load(const std::string& strPreviousState)
    {
        std::string levelMtrl = LEVEL_MATERIAL;
        std::string chapterFolder;

        CSingletonPlayerMgr playerMgr;

        m_firstLevelIndex = playerMgr->GetCurrentChapter().firstLevelIndex;

        chapterFolder = "GUI/" + playerMgr->GetChapterFolder();

        levelMtrl = chapterFolder + levelMtrl;

        m_pPart1LevelSelectionMtrl = gMaterialMgr->AddGetPtr(levelMtrl);

        int numTextureColumns = m_pPart1LevelSelectionMtrl->GetWidth() / m_pPart1LevelSelectionMtrl->GetFrameWidth();

        m_part1LevelSelections.resize(playerMgr->GetChapter(playerMgr->GetCurrentChapterIndex()).GetNumLevelsInMenue());

        CTerrainMgrGPU* pLevelRO = gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLevelRenderObject();
        const std::vector< Matrix4x4 >& transformations = pLevelRO->GetLayerTransformations();

        for (int i = 0; i < m_part1LevelSelections.size(); ++i)
        {
            int columnIndex = i % numTextureColumns;
            int rowIndex = i / numTextureColumns;

            pairSpriteNode& pairSN = m_part1LevelSelections[i];

            const LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), i);

            if (!levelInfo.bSelectable)
                continue;

            if (levelInfo.bAccessible)
            {
                pairSN.first.SetMaterialFilename(levelMtrl, true);
                pairSN.first.GetMaterial()->SetFrame(columnIndex);
                pairSN.first.GetMaterial()->SetSequence(rowIndex);

                if (levelInfo.bFinished)
                {
                    pairSN.first.SetColor(LEVEL_TITLE_COLOR_FINISHED);
                    pairSN.first.SetAlpha(LEVEL_TITLE_ALPHA);
                }
                else
                {
                    pairSN.first.SetColor(LEVEL_TITLE_COLOR);
                    pairSN.first.SetAlpha(LEVEL_TITLE_ALPHA);
                }

            }
            else
            {
                pairSN.first.SetMaterialFilename("GUI/" + playerMgr->GetChapterFolder() + "locked.png");

                pairSN.first.SetColor(LEVEL_TITLE_COLOR);
                pairSN.first.SetAlpha(LEVEL_TITLE_ALPHA);
            }

            pairSN.first.AttachNode(&pairSN.second);
            pairSN.second.SetScale(LEVEL_TITLE_SCALE);

            if (i % 2 == 0)
            {
                Vec3 pos = startPosition + Vec3(0.0f, i * (m_pPart1LevelSelectionMtrl->GetFrameHeight() + 32.0f), 0.0f);
                pos = Vec3(Vec4(pos, 1.0f) * transformations[1]);
                float rotation = pLevelRO->GetRotation(pos.z);


                pairSN.second.SetPosition(pos);
                pairSN.second.SetOrientation(CQuaternion(0.0f, 0.0f, rotation));
            }
            else
            {
                Vec3 pos = startPosition + Vec3(0.0f, i * (m_pPart1LevelSelectionMtrl->GetFrameHeight() + 32.0f), 0.0f);
                pos = Vec3(Vec4(pos, 1.0f) * transformations[1]);
                float rotation = pLevelRO->GetRotation(pos.z);

                pairSN.second.SetPosition(pos);
                pairSN.second.SetOrientation(CQuaternion(0.0f, 0.0f, rotation));

            }
        }


        m_pTitleNode = new CSceneNode();
        m_pTitleSprite = new CSprite();
        m_pTitleSprite->SetMaterialFilename(levelMtrl, true);
        m_pTitleSprite->SetColor(LEVEL_TITLE_COLOR_SELECTION);
        m_pTitleSprite->GetMaterial()->SetFrame(0);
        m_pTitleSprite->GetMaterial()->SetSequence(11);
        m_pTitleSprite->SetUpdateRectByAnimation(true);
        m_pTitleSprite->AttachNode(m_pTitleNode);

        Vec3 pos = startPosition + Vec3(0.0f, -220.0f, 0.0f);
        pos = Vec3(Vec4(pos, 1.0f) * transformations[1]);
        float rotation = pLevelRO->GetRotation(pos.z);

        m_pTitleNode->SetPosition(pos);
        m_pTitleNode->SetScale(Vec3(0.9f, 0.9f, 1.0f));
        m_pTitleNode->SetOrientation(CQuaternion(0.0f, 0.0f, rotation));



        m_pCompleteNode = new CSceneNode();
        m_pCompleteSprite = new CSprite();
        m_pCompleteSprite->SetMaterialFilename(levelMtrl, true);
        m_pCompleteSprite->GetMaterial()->SetFrame(1);
        m_pCompleteSprite->GetMaterial()->SetSequence(10);
        m_pCompleteSprite->SetUpdateRectByAnimation(true);
        m_pCompleteSprite->SetColor(Vec3(0, 0, 0));
        m_pCompleteSprite->SetAlpha(0.7f);
		m_pCompleteSprite->AttachNode(m_pCompleteNode);

        if (playerMgr->GetCurrentChapter().bFinished)
        {
            m_pCompleteSprite->SetColor(LEVEL_TITLE_COLOR_SELECTION);
            m_pCompleteSprite->SetAlpha(LEVEL_TITLE_ALPHA_SELECTION);
        }
        else
        {
            m_pCompleteSprite->SetColor(LEVEL_TITLE_COLOR);
            m_pCompleteSprite->SetAlpha(LEVEL_TITLE_ALPHA);
        }

        pos = startPosition + Vec3(0.0f, playerMgr->GetCurrentChapter().GetNumLevelsInMenue() * (m_pPart1LevelSelectionMtrl->GetFrameHeight() + 32.0f) + 13.0f, 0.0f);
        pos = Vec3(Vec4(pos, 1.0f) * transformations[1]);
        rotation = pLevelRO->GetRotation(pos.z);

        m_pCompleteNode->SetPosition(pos);
        m_pCompleteNode->SetScale(Vec3(1.0f, 1.0f, 1.0f));
        m_pCompleteNode->SetOrientation(CQuaternion(0.0f, 0.0f, rotation));



        m_pChapterNode = new CSceneNode();
        m_pChapterSprite = new CSprite();
        m_pChapterSprite->SetMaterialFilename(levelMtrl, true);
        m_pChapterSprite->SetColor(LEVEL_TITLE_COLOR_SELECTION);
        m_pChapterSprite->GetMaterial()->SetFrame(1);
        m_pChapterSprite->GetMaterial()->SetSequence(11);
        m_pChapterSprite->SetUpdateRectByAnimation(true);
        m_pChapterSprite->AttachNode(m_pChapterNode);

        pos = startPosition + Vec3(0.0f, -160.0f, 0.0f);
        pos = Vec3(Vec4(pos, 1.0f) * transformations[1]);
        rotation = pLevelRO->GetRotation(pos.z);

        m_pChapterNode->SetPosition(pos);
        m_pChapterNode->SetOrientation(CQuaternion(0.0f, 0.0f, rotation));
        m_pChapterNode->SetScale(Vec3(0.9f, 0.9f, 1.0f));


        m_camStartPos = m_part1LevelSelections[m_firstLevelIndex].second.GetDerivedPosition();
        m_camStartPos.x = startPosition.x;
        m_camStartPos.z = -1000.0f;
        m_camStartPos.y -= 300.0f;

        m_camEndPos = m_part1LevelSelections[m_part1LevelSelections.size() - 1].second.GetDerivedPosition();
        m_camEndPos.x = startPosition.x;
        m_camEndPos.z = -1000.0f;
        m_camEndPos.y -= 70.0f;

        m_currentSelection = playerMgr->GetCurrentLevelIndex(); //playerMgr->GetChapter(playerMgr->GetCurrentChapterIndex()).firstLevelIndex;

        m_currentStateTime = m_loadTime;

        m_state = STATE_LOAD;

        m_pTimeBackground = new CSprite();
        m_pTimeBackgroundNode = new CSceneNode();
        m_pTimeBackground->SetMaterialFilename("GUI/Game/time_background.png");
        m_pTimeBackground->SetUpdateRectByAnimation(true);
        m_pTimeBackground->AttachNode(m_pTimeBackgroundNode);
        m_pTimeBackground->SetScreenSpace(true);
        m_pTimeBackgroundNode->SetPosition(Vec3(0.43f, 0.05f, 0.0f));

        m_pTimeBackgroundGold = new CSprite();
        m_pTimeBackgroundGold->SetMaterialFilename("GUI/Game/time_background_gold.png");
        m_pTimeBackgroundGold->SetUpdateRectByAnimation(true);
        m_pTimeBackgroundGold->AttachNode(m_pTimeBackgroundNode);
        m_pTimeBackgroundGold->SetScreenSpace(true);

        m_pTimeBackgroundSilver = new CSprite();
        m_pTimeBackgroundSilver->SetMaterialFilename("GUI/Game/time_background_silver.png");
        m_pTimeBackgroundSilver->SetUpdateRectByAnimation(true);
        m_pTimeBackgroundSilver->AttachNode(m_pTimeBackgroundNode);
        m_pTimeBackgroundSilver->SetScreenSpace(true);

        m_pTimeBackgroundBronze = new CSprite();
        m_pTimeBackgroundBronze->SetMaterialFilename("GUI/Game/time_background_bronze.png");
        m_pTimeBackgroundBronze->SetUpdateRectByAnimation(true);
        m_pTimeBackgroundBronze->AttachNode(m_pTimeBackgroundNode);
        m_pTimeBackgroundBronze->SetScreenSpace(true);

        m_pKillsBackground = new CSprite();
        m_pKillsBackgroundNode = new CSceneNode();
        m_pKillsBackground->SetMaterialFilename("GUI/Game/kills_background.png");
        m_pKillsBackground->SetUpdateRectByAnimation(true);
        m_pKillsBackground->AttachNode(m_pKillsBackgroundNode);
        m_pKillsBackground->SetScreenSpace(true);
        m_pKillsBackgroundNode->SetPosition(Vec3(0.57f, 0.05f, 0.0f));



        m_chapter = playerMgr->GetCurrentChapterIndex();

        m_pMenueSelectionSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_MENUE_SELECTION);
        m_pMenueSelectionSound->SetVolume(0.05f);
        m_pMenueSelectionSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueTransitionInSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_IN);
        m_pMenueTransitionInSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueTransitionOutSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_OUT);
        m_pMenueTransitionOutSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueConfirmationSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_MENUE_CONFIRMATION);
        m_pMenueConfirmationSound->SetVolume(0.2f);
        m_pMenueConfirmationSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueUnlockLevelSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_MENUE_UNLOCK_LEVEL);
        m_pMenueUnlockLevelSound->SetVolume(2.0f);
        m_pMenueUnlockLevelSound->SetSpeed(3.0f);
        m_pMenueUnlockLevelSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueUnlockChapterSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_MENUE_UNLOCK_CHAPTER);
        m_pMenueUnlockChapterSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

		if (strPreviousState == "PauseMenueState")
		{
			gEngine->GetAudioSystem()->ContinueSound();
		}

        if (gEngine->GetAudioSystem()->GetCurrentMusicType() != MUSIC_TYPE::MUSIC_TYPE_MENU)
        {
            gEngine->GetAudioSystem()->StopMusic();
        }

        m_pMenueTransitionInSound->PlaySound();

        GetFadeTopColor(FADE_COLOR_TOP, FADE_COLOR_BOT);

        m_stateMachine.RegisterStateUpdateCallback(STATE_LOAD, StateMachine::StateUpdateSetupInfo(m_loadTime, this, &CLevelSelectionState::LoadMenuUpdate));
        m_stateMachine.RegisterStateLeaveCallback(STATE_LOAD, StateMachine::StateEnterLeaveSetupInfo(this, &CLevelSelectionState::LoadMenuLeave));

        m_stateMachine.RegisterStateUpdateCallback(STATE_MENUE, StateMachine::StateUpdateSetupInfo(this, &CLevelSelectionState::MenuUpdate));

        m_stateMachine.RegisterStateUpdateCallback(STATE_UNLOAD, StateMachine::StateUpdateSetupInfo(m_unloadTime, this, &CLevelSelectionState::UnloadMenuUpdate));
        m_stateMachine.RegisterStateLeaveCallback(STATE_UNLOAD, StateMachine::StateEnterLeaveSetupInfo(this, &CLevelSelectionState::UnloadMenuLeave));

        m_stateMachine.RegisterStateUpdateCallback(STATE_UNLOADCHAPTER, StateMachine::StateUpdateSetupInfo(m_unloadTime, this, &CLevelSelectionState::UnloadChapterUpdate));

        m_stateMachine.RegisterStateUpdateCallback(STATE_LOADCHAPTER, StateMachine::StateUpdateSetupInfo(m_loadTime, this, &CLevelSelectionState::LoadChapterUpdate));
        m_stateMachine.RegisterStateLeaveCallback(STATE_LOADCHAPTER, StateMachine::StateEnterLeaveSetupInfo(this, &CLevelSelectionState::LoadChapterLeave));

        m_stateMachine.RegisterStateUpdateCallback(STATE_LOADLEVEL, StateMachine::StateUpdateSetupInfo(m_loadTime, this, &CLevelSelectionState::LoadLevelUpdate));
        m_stateMachine.RegisterStateLeaveCallback(STATE_LOADLEVEL, StateMachine::StateEnterLeaveSetupInfo(this, &CLevelSelectionState::LoadLevelLeave));

        m_stateMachine.RegisterStateUpdateCallback(STATE_CHAPTER_COMPLETE_CAM_TRANS, StateMachine::StateUpdateSetupInfo(4.0f, this, &CLevelSelectionState::ChapterCompleteCamTransitionUpdate));
        m_stateMachine.RegisterStateUpdateCallback(STATE_CHAPTER_COMPLETE_CAM_SHAKE, StateMachine::StateUpdateSetupInfo(0.4f, this, &CLevelSelectionState::ChapterCompleteCamShakeUpdate));

        m_stateMachine.RegisterStateUpdateCallback(STATE_CHAPTER_COMPLETE_CHANGE, StateMachine::StateUpdateSetupInfo(2.5f, this, &CLevelSelectionState::ChapterCompleteChangeUpdate));
        m_stateMachine.RegisterStateLeaveCallback(STATE_CHAPTER_COMPLETE_CHANGE, StateMachine::StateEnterLeaveSetupInfo(this, &CLevelSelectionState::ChapterCompleteChangeLeave));

        m_stateMachine.SetStateID(STATE_LOAD);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CLevelSelectionState::Unload()
    {
        m_bPlayChapterFinish = false;

        ENV_DELETE(m_pTimeBackground);
        ENV_DELETE(m_pTimeBackgroundNode);
        ENV_DELETE(m_pTimeBackgroundGold);
        ENV_DELETE(m_pTimeBackgroundSilver);
        ENV_DELETE(m_pTimeBackgroundBronze);
        ENV_DELETE(m_pKillsBackground);
        ENV_DELETE(m_pKillsBackgroundNode);
        ENV_DELETE(m_pCompleteNode);
        ENV_DELETE(m_pCompleteSprite);
        ENV_DELETE(m_pChapterNode);
        ENV_DELETE(m_pChapterSprite);
        ENV_DELETE(m_pTitleNode);
        ENV_DELETE(m_pTitleSprite);

        m_part1LevelSelections.clear();

        if (m_pPart1LevelSelectionMtrl)
            gMaterialMgr->Release(m_pPart1LevelSelectionMtrl->GetName());

        m_pPart1LevelSelectionMtrl = NULL;

        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_MENUE_SELECTION);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_TRANSITION_IN);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_TRANSITION_OUT);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_MENUE_CONFIRMATION);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::FadeMenuePage(float alpha, FADE_BARS_TYPE fadeType, RenderQueue& queue)
    {
        m_fade.fadeBarsType = fadeType;
        m_fade.fadeBarsAmount = alpha;
        queue.m_fadeFrameRenderObject.m_fadeFrame = alpha * Vec4(0.1f, 0.1f, 0.1f, 0.1f);

        queue.m_postProcessRenderObject.m_posterizationAmount = alpha;
        queue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 0.0f;
        queue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 0.0f;
        queue.m_pFadeBarsRenderObject = &m_fade;
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::LoadMenuLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        if (m_bPlayChapterFinish)
        {
            m_pMenueSelectionSound->PlaySound(20);

            m_stateMachine.SetNextStateID(STATE_CHAPTER_COMPLETE_CAM_TRANS);
        }
        else
        {
            m_stateMachine.SetNextStateID(STATE_MENUE);
        }
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::LoadMenuUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = stateInfo.GetExpInterpolationInverse(3.0f);

        if (m_bLoadFromChapter)
        {
            if (m_bChangeChapterLeft)
                FadeMenuePage(alpha, FADE_BARS_FROM_LEFT_TO_RIGHT, frameParams.m_renderQueue);
            else
                FadeMenuePage(alpha, FADE_BARS_FROM_RIGHT_TO_LEFT, frameParams.m_renderQueue);
        }
        else
            FadeMenuePage(alpha, FADE_BARS_FROM_TOP_TO_BOTTOM, frameParams.m_renderQueue);

        m_currentSelectionTime = 0.25f;

        Vec3 firstPos = m_part1LevelSelections[m_firstLevelIndex].first.GetNode()->GetPosition();

        float startPitch = atan((m_camStartPos.y - firstPos.y) / abs((firstPos.z - m_camStartPos.z)));

        CSceneNode* pCameraNode = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();
        pCameraNode->SetPosition(m_camStartPos);
        pCameraNode->SetOrientation(CQuaternion(0.0f, startPitch, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::UnloadMenuUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        m_bChangeChapterLeft = false;
        m_bLoadFromChapter = false;
        float alpha = stateInfo.GetExpInterpolationInverse(3.0f);

        FadeMenuePage(1.0f - alpha, FADE_BARS_FROM_TOP_TO_BOTTOM, frameParams.m_renderQueue);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::UnloadMenuLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        gEngine->SetNewState("MenueState");

        m_stateMachine.SetNextStateID(STATE_LOAD);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::MenuUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        STATE state = STATE_MENUE;

        ProcessInput(state);

        UpdateLabels(frameParams.m_dt);

        CSingletonPlayerMgr playerMgr;
        CSceneNode* pCameraNode = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();

        float alpha = (float)m_currentSelection / (float)(playerMgr->GetChapter(m_chapter).GetNumLevelsInMenue() - 1);

        Vec3 camCurrentPos = pCameraNode->GetDerivedPosition();
        Vec3 camTargetPos = Lerp(m_camStartPos, m_camEndPos, alpha);
        Vec3 camTargetDir = Vec3(camTargetPos - camCurrentPos);

        if (glm::length(camTargetDir) > 0.0001f)
            camTargetDir = glm::normalize(camTargetDir);

        float camTargetDist = glm::length(camCurrentPos - camTargetPos);

        camCurrentPos += camTargetDir * camTargetDist * frameParams.m_dt * 4.0f;

        const Vec3& firstPos = m_part1LevelSelections[m_firstLevelIndex].first.GetNode()->GetDerivedPosition();
        const Vec3& lastPos = m_part1LevelSelections[m_part1LevelSelections.size() - 1].first.GetNode()->GetDerivedPosition();
        float startPitch = atan((m_camStartPos.y - firstPos.y) / abs((startPosition.z - m_camStartPos.z)));
        float endPitch = atan((m_camEndPos.y - lastPos.y) / abs((startPosition.z - m_camEndPos.z)));
        float continuousAlpha = (camCurrentPos.y - m_camStartPos.y) / (m_camEndPos.y - m_camStartPos.y);

        float pitch = Lerp(startPitch, endPitch, continuousAlpha);

        pCameraNode->SetPosition(camCurrentPos);
        pCameraNode->SetOrientation(CQuaternion(0.0f, pitch, 0.0f));

        m_stateMachine.SetNextStateID(state);
        //m_stateMachine.SetNextStateID(STATE_CHAPTER_COMPLETE_CAM_TRANS);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::UnloadChapterUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = stateInfo.GetExpInterpolationInverse(3.0f);

        if (m_bChangeChapterLeft)
            FadeMenuePage(1.0f - alpha, FADE_BARS_FROM_RIGHT_TO_LEFT, frameParams.m_renderQueue);
        else
            FadeMenuePage(1.0f - alpha, FADE_BARS_FROM_LEFT_TO_RIGHT, frameParams.m_renderQueue);

        m_stateMachine.SetNextStateID(STATE_LOADCHAPTER);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::LoadChapterUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        if (m_bChangeChapterLeft)
            m_fade.fadeBarsType = FADE_BARS_FROM_RIGHT_TO_LEFT;
        else
            m_fade.fadeBarsType = FADE_BARS_FROM_LEFT_TO_RIGHT;

        frameParams.m_renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 0.0f;
        frameParams.m_renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 0.0f;
        frameParams.m_renderQueue.m_pFadeBarsRenderObject = &m_fade;
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::LoadChapterLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        m_currentStateTime = m_loadTime;

        CSingletonPlayerMgr playerMgr;

        std::string strMenueLevel = "../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env";

        gEngine->GetLevelLoader().LoadLevel(strMenueLevel, CLevelSelectionState::STATE_NAME);

        m_stateMachine.SetNextStateID(STATE_LOAD);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::LoadLevelUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = stateInfo.GetExpInterpolationInverse(3.0f);

        FadeMenuePage(1.0f - alpha, FADE_BARS_FROM_BOTTOM_TO_TOP, frameParams.m_renderQueue);

        frameParams.m_renderQueue.m_pFadeBarsRenderObject = &m_fade;
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::LoadLevelLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        m_state = STATE_LOAD;
        m_currentStateTime = m_loadTime;

        CSingletonPlayerMgr playerMgr;
        playerMgr->SetCurrentLevelByIndex(m_currentSelection);

        const LevelInfo& info = playerMgr->GetLevelinfo(m_chapter, m_currentSelection);
        gEngine->GetLevelLoader().LoadLevel(info.strFilename, "GameState");

        m_stateMachine.SetNextStateID(STATE_LOAD);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::ChapterCompleteCamShakeUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = 1.0f - stateInfo.GetExpInterpolationInverse(3.0f);
        float scaling = alpha * 1.0f + (1.0f - alpha) * 2.0f;
        m_pCompleteNode->SetScale(Vec3(scaling, 1.0f, 1.0f));
        m_pCompleteSprite->SetColor(LEVEL_TITLE_COLOR_FINISHED);
        m_pCompleteSprite->SetAlpha(alpha * 1.15f);

        Vec3 v = GetRandomVec(v);

        CSceneNode* pCameraNode = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();
        Vec3 camPos = pCameraNode->GetDerivedPosition();
        //camPos += v * 8.0f;
        camPos.z = Lerp(-1000.0f, -850.0f, alpha);
        pCameraNode->SetPosition(camPos);

        if (m_pMenueUnlockChapterSound->IsPlaying() == false)
        {
            m_pMenueUnlockChapterSound->PlaySound();
        }

        frameParams.m_renderQueue.m_fadeFrameRenderObject.m_fadeFrame = CHAPTER_COMPLETE_FRAME;

        m_stateMachine.SetNextStateID(STATE_CHAPTER_COMPLETE_CHANGE);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::ChapterCompleteChangeUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = stateInfo.GetLinearInterpolation();

        CSceneNode* pCameraNode = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();
        Vec3 camPos = pCameraNode->GetDerivedPosition();
        camPos.z = Lerp(-850.0f, -700.0f, alpha);
        pCameraNode->SetPosition(camPos);

        frameParams.m_renderQueue.m_fadeFrameRenderObject.m_fadeFrame = CHAPTER_COMPLETE_FRAME;
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::ChapterCompleteChangeLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        CSingletonPlayerMgr playerMgr;

        playerMgr->SetCurrentChapterByIndex((m_chapter + 1) % playerMgr->GetNumChapters());
        playerMgr->SetCurrentLevelByIndex(0);

        m_bChangeChapterLeft = false;

        m_pMenueTransitionOutSound->PlaySound();

        frameParams.m_renderQueue.m_fadeFrameRenderObject.m_fadeFrame = CHAPTER_COMPLETE_FRAME;

        m_stateMachine.SetNextStateID(STATE_UNLOADCHAPTER);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::ChapterCompleteCamTransitionUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        CSingletonPlayerMgr playerMgr;
        m_currentSelection = playerMgr->GetChapter(m_chapter).levels.size() - 1;

        CSceneNode* pCameraNode = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();

        Vec3 camPos = pCameraNode->GetDerivedPosition();

        float alpha = stateInfo.GetLinearInterpolation();


        const Vec3& firstPos = m_part1LevelSelections[m_firstLevelIndex].first.GetNode()->GetDerivedPosition();
        const Vec3& lastPos = m_pCompleteNode->GetDerivedPosition();
        float startPitch = atan((m_camStartPos.y - firstPos.y) / abs((firstPos.z - m_camStartPos.z)));
        float endPitch = 0.0f; //atan((m_camEndPos.y - lastPos.y) / abs((lastPos.z - m_camEndPos.z)));

        camPos = Lerp(m_camStartPos, Vec3(lastPos.x, lastPos.y, m_camEndPos.z), alpha);

        pCameraNode->SetPosition(camPos);
        pCameraNode->SetOrientation(CQuaternion(0.0f, alpha * endPitch + (1.0f - alpha) * startPitch, 0.0f));

        for (int i = 0; i < m_part1LevelSelections.size(); ++i)
        {
            float limit = stateInfo.GetElapsedTime();
            float alphaSel = 4.0f * (float)i / (m_part1LevelSelections.size());

            if (alphaSel < limit)
            {
                float alphaColor = std::min((limit - alphaSel) / 0.15f, 1.0f);
                alphaColor *= alphaColor;
                alphaColor = alphaColor * 1.15f;

                m_part1LevelSelections[i].first.SetColor(LEVEL_TITLE_COLOR_FINISHED * alphaColor);
                m_part1LevelSelections[i].first.SetAlpha(std::max(alphaColor, 0.1f));
            }
        }

        if (m_pMenueUnlockLevelSound->IsPlaying() == false)
        {
            m_pMenueUnlockLevelSound->PlaySound();
        }

        m_pCompleteSprite->SetColor(Vec4(0.0f, 0.0f, 0.0f, 0.0f));

        m_stateMachine.SetNextStateID(STATE_CHAPTER_COMPLETE_CAM_SHAKE);

        frameParams.m_renderQueue.m_fadeFrameRenderObject.m_fadeFrame = Lerp(Vec4(0.0f, 0.0f, 0.0f, 0.0f), CHAPTER_COMPLETE_FRAME, alpha);
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::Update(float time, float dt, RenderQueue& renderQueue)
    {
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop = FADE_COLOR_TOP;
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 0;

        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot = FADE_COLOR_BOT;
        renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 0;

        renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorTop = FADE_COLOR_TOP;
        renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorBot = FADE_COLOR_BOT;

        CSceneNode* pCameraNode = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();

        m_stateMachine.Update(StateMachine::FrameParamsType(dt, renderQueue));

        for (int i = 0; i < m_part1LevelSelections.size(); ++i)
        {
            if (m_part1LevelSelections[i].first.GetMaterial())
                renderQueue.m_spriteRenderObjects.push_back(m_part1LevelSelections[i].first.GetRenderObject());
        }

        renderQueue.m_spriteRenderObjects.push_back(m_pTitleSprite->GetRenderObject());
        renderQueue.m_spriteRenderObjects.push_back(m_pChapterSprite->GetRenderObject());
        renderQueue.m_spriteRenderObjects.push_back(m_pCompleteSprite->GetRenderObject());

        gEngine->GetAudioSystem()->PlayMusicForMenu();
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::UpdateLabels(float dt)
    {
        m_currentSelectionTime = std::max(0.0f, m_currentSelectionTime - dt);
        float alphaSelectionTime = m_currentSelectionTime / 0.25f;
        alphaSelectionTime = std::pow(alphaSelectionTime, 2.0f);

        CSingletonPlayerMgr playerMgr;

        unsigned int i = m_firstLevelIndex;

        for (int j = m_firstLevelIndex; j < m_part1LevelSelections.size(); ++j)
        {
            if (!playerMgr->GetLevelinfo(m_chapter, j).bSelectable)
                continue;

            //             if (playerMgr->GetNumLevels(m_chapter) <= i)
            //                 continue;

            if (i >= m_part1LevelSelections.size())
                continue;

            if (i == m_currentSelection)
            {
                LevelInfo& levelInfo = playerMgr->GetLevelinfo(m_chapter, j);

                Vec3 startColor = LEVEL_TITLE_COLOR;
                if (levelInfo.bFinished)
                    startColor = LEVEL_TITLE_COLOR_SELECTION;

                Vec3 endColor = LEVEL_TITLE_COLOR_FINISHED;
                if (levelInfo.bFinished)
                    endColor = LEVEL_TITLE_COLOR_FINISHED_SELECTION;

                Vec3 color = startColor * alphaSelectionTime + endColor * (1.0f - alphaSelectionTime);
                float colorAlpha = LEVEL_TITLE_ALPHA * alphaSelectionTime + LEVEL_TITLE_ALPHA_SELECTION * (1.0f - alphaSelectionTime);

                m_part1LevelSelections[i].first.SetColor(Vec4(color, colorAlpha));

                Vec3 scaling = LEVEL_TITLE_SCALE * alphaSelectionTime + (1.0f - alphaSelectionTime) * LEVEL_TITLE_SCALE_SELECTION;
                m_part1LevelSelections[i].first.GetNode()->SetScale(scaling);


                const Vec3& pos = m_part1LevelSelections[i].first.GetNode()->GetPosition();
                m_part1LevelSelections[i].first.GetNode()->SetPosition(Vec3(pos.x, pos.y, startPosition.z - (1.0f - alphaSelectionTime) * 1.0f));
            }
            else if (i == (m_currentSelection - 1) || i == (m_currentSelection + 1))
            {
                //                 if (!playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), i).bSelectable ||
                //                     !m_part1LevelSelections[i].first.GetNode())
                //                 if (i >= m_part1LevelSelections.size())
                //                     continue;

                m_part1LevelSelections[i].first.SetAlpha(LEVEL_TITLE_ALPHA);

                Vec3 currentScale = m_part1LevelSelections[i].first.GetNode()->GetScale();

                float scalingX = 0.0f, scalingY = 0.0f;
                if (currentScale.x > LEVEL_TITLE_SCALE.x)
                {
                    scalingX = (currentScale.x - 1.0f) * dt;
                    if (scalingX < (1.5f * dt))
                        scalingX = 1.5f * dt;
                }

                if (currentScale.y > LEVEL_TITLE_SCALE.y)
                {
                    scalingY = (currentScale.y - 1.0f) * dt;
                    if (scalingY < (1.5f * dt))
                        scalingY = 1.5f * dt;
                }


                m_part1LevelSelections[i].first.GetNode()->SetScale(Vec3(currentScale.x - scalingX, currentScale.y - scalingY, 1.0f));

                const Vec3& pos = m_part1LevelSelections[i].first.GetNode()->GetPosition();
                if (pos.z < startPosition.z)
                {
                    float translation = (80.0f - pos.x) * dt * 0.15f;
                    if (translation > -(4.5f * dt))
                        translation = -4.5f * dt;

                    m_part1LevelSelections[i].first.GetNode()->SetPosition(Vec3(pos.x, pos.y, startPosition.z - translation));
                }

                LevelInfo& levelInfo = playerMgr->GetLevelinfo(m_chapter, j);

                if (levelInfo.bFinished)
                {
                    m_part1LevelSelections[i].first.SetColor(Vec4(LEVEL_TITLE_COLOR_FINISHED, LEVEL_TITLE_ALPHA));
                }
                else
                {
                    m_part1LevelSelections[i].first.SetColor(Vec4(LEVEL_TITLE_COLOR, LEVEL_TITLE_ALPHA));
                }
            }
            else
            {
                //                 if (i >= m_part1LevelSelections.size())
                //                     continue;

                LevelInfo& levelInfo = playerMgr->GetLevelinfo(m_chapter, j);
                if (levelInfo.bFinished)
                {
                    m_part1LevelSelections[i].first.SetColor(LEVEL_TITLE_COLOR_FINISHED);
                }
                else
                {
                    m_part1LevelSelections[i].first.SetColor(LEVEL_TITLE_COLOR);
                }

                m_part1LevelSelections[i].first.SetAlpha(LEVEL_TITLE_ALPHA);
                m_part1LevelSelections[i].first.GetNode()->SetScale(LEVEL_TITLE_SCALE);
            }

            ++i;
        }
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::ProcessInput(STATE& nextState)
    {
        CSingletonPlayerMgr playerMgr;

        if (gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY) || gEngine->GetInput()->IsActionJustTriggered(ACTION_START))
        {
            if (playerMgr->GetLevelinfo(m_chapter, m_currentSelection).bAccessible)
            {
                nextState = STATE_LOADLEVEL;
                m_currentStateTime = m_unloadTime;

                m_pMenueTransitionOutSound->PlaySound();
            }
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_BACK))
        {
            nextState = STATE_UNLOAD;

            m_pMenueTransitionOutSound->PlaySound();
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_UP))
        {
            m_pMenueSelectionSound->PlaySound();
            m_currentSelection = playerMgr->GetChapter(m_chapter).GetNextHigherSelectableLevelIndex(m_currentSelection);

            m_currentSelectionTime = 0.25f;
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_DOWN))
        {
            m_pMenueSelectionSound->PlaySound();

            m_currentSelection = playerMgr->GetChapter(m_chapter).GetNextLowerSelectableLevelIndex(m_currentSelection);

            m_currentSelectionTime = 0.25f;
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_RIGHT))
        {
            int curChapter = (static_cast<int>(m_chapter) + 1) % static_cast<int>(playerMgr->GetNumChapters());

            if (playerMgr->GetChapter(curChapter).bAccessible)
            {
                m_bChangeChapterLeft = false;
                m_bLoadFromChapter = true;
                nextState = STATE_UNLOADCHAPTER;
                m_currentStateTime = m_unloadTime;

                playerMgr->SetCurrentChapterByIndex(curChapter);
                playerMgr->SetCurrentLevelByIndex(0);

                m_pMenueTransitionOutSound->PlaySound();
            }
        }
        else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_LEFT))
        {
            int curChapter = static_cast<int>(m_chapter);

            if (curChapter == 0)
                curChapter = playerMgr->GetNumChapters() - 1;
            else
                curChapter = curChapter - 1;

            if (playerMgr->GetChapter(curChapter).bAccessible)
            {
                m_bChangeChapterLeft = true;
                m_bLoadFromChapter = true;
                nextState = STATE_UNLOADCHAPTER;
                m_currentStateTime = m_unloadTime;

                playerMgr->SetCurrentChapterByIndex(curChapter);
                playerMgr->SetCurrentLevelByIndex(0);

                m_pMenueTransitionOutSound->PlaySound();
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CLevelSelectionState::OnResetDevice(int screenWidth, int screenHeight)
    {

    }

} // env