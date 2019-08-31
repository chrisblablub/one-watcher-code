#include <Engine/Entity/CheckpointEntity.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/CameraEntity.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/GameLoader.h>
#include <Engine/State/GameState.h>
#include <Engine/State/LevelSelectionState.h>
#include <Graphic/RenderSystems/RenderSystem.h>


namespace env
{
    ENTITY_TYPEID CCheckpointEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_CHECKPOINT;

    CCheckpointEntity::CCheckpointEntity()
        : m_openingTime(0.65f),
        m_closingTime(0.8f),
        m_closingPauseTime(1.0f),
        m_openingZoomFactor(0.85f),
        m_closingZoomFactor(0.7f),
        m_currentStateTime(0.0f),
        m_openingPauseTime(0.1f),
        m_yaw(0.1f),
        m_pitch(-0.1f),
        m_roll(0.1f),
        m_levelBronzeTime(200.0f),
        m_levelSilverTime(7.5f),
        m_levelGoldTime(5.0f),
        m_bFinal(false),
        m_bStart(false),
        m_bCheckpointFired(false),
        m_bFinishChapter(false),
        m_pFade(NULL),
        m_pReward(0),
        m_pRewardNode(0),
        m_pRewardGold(0),
        m_pRewardSilver(0),
        m_pRewardBronze(0),
        m_pTimeBackground(0),
        m_pTimeBackgroundNode(0),
        m_pTimeBackgroundGold(0),
        m_pTimeBackgroundGoldNode(0),
        m_pTimeBackgroundSilver(0),
        m_pTimeBackgroundSilverNode(0),
        m_pTimeBackgroundBronze(0),
        m_pTimeBackgroundBronzeNode(0),
        m_pKillsBackground(0),
        m_pKillsBackgroundNode(0),
        m_pArrowLeft(0),
        m_pArrowLeftNode(0),
        m_pArrowRight(0),
        m_pArrowRightNode(0),
        m_pEscape(0),
        m_pEscapeNode(0),
        m_state(CHECKPOINT_STATE_UNDEFINED),
        m_checkpointAction(CHECKPOINT_ACTION_WAITFORINPUT),
        m_bTriggerCheckpointAction(false),
        m_pCharacter(0)
    {
        SetMetaTypeID(CCheckpointEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Checkpoint] Change Level", CHECKPOINT_EVENT_CHANGELEVEL);

        m_bGhost = false;
    }

    CCheckpointEntity::~CCheckpointEntity()
    {
        ENV_DELETE(m_pReward);
        ENV_DELETE(m_pRewardNode);
        ENV_DELETE(m_pRewardGold);
        ENV_DELETE(m_pRewardSilver);
        ENV_DELETE(m_pRewardBronze);
        ENV_DELETE(m_pTimeBackground);
        ENV_DELETE(m_pTimeBackgroundNode);
        ENV_DELETE(m_pTimeBackgroundGold);
        ENV_DELETE(m_pTimeBackgroundGoldNode);
        ENV_DELETE(m_pTimeBackgroundSilverNode);
        ENV_DELETE(m_pTimeBackgroundSilver);
        ENV_DELETE(m_pTimeBackgroundBronze);
        ENV_DELETE(m_pTimeBackgroundBronzeNode);
        ENV_DELETE(m_pKillsBackground);
        ENV_DELETE(m_pKillsBackgroundNode);
        ENV_DELETE(m_pArrowLeft);
        ENV_DELETE(m_pArrowLeftNode);
        ENV_DELETE(m_pArrowRight);
        ENV_DELETE(m_pArrowRightNode);
        ENV_DELETE(m_pEscape);
        ENV_DELETE(m_pEscapeNode);

        gEngine->GetAudioSystem()->Release("CharacterFinish.wav");
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_TRANSITION_IN);
        gEngine->GetAudioSystem()->Release(AUDIO_EFFECT_TRANSITION_OUT);
    }

    void CCheckpointEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetTextPosition(Vec2(0.0, 32.0));
        SetMaterialFilename("Sprites/checkpoint_finish.dds");

        SetFireOnce(true);

        m_bGhost = false;

        RemovePolygons();

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-32.0f, -32.0f, 0.0f));
        p.AddPoint(Vec3(-32.0f,  32.0f, 0.0f));
        p.AddPoint(Vec3( 32.0f,  32.0f, 0.0f));
        p.AddPoint(Vec3( 32.0f, -32.0f, 0.0f));
        AddPolygon(p);

        m_pMenueTransitionInSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_IN);
        m_pMenueTransitionInSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueTransitionOutSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_OUT);
        m_pMenueTransitionOutSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);


        //InitSprites(m_bFinal);
    }

    void CCheckpointEntity::InitSprites(bool bCreate)
    {
        ENV_DELETE(m_pReward);
        ENV_DELETE(m_pRewardNode);
        ENV_DELETE(m_pRewardGold);
        ENV_DELETE(m_pRewardSilver);
        ENV_DELETE(m_pRewardBronze);
        ENV_DELETE(m_pTimeBackground);
        ENV_DELETE(m_pTimeBackgroundNode);
        ENV_DELETE(m_pTimeBackgroundGold);
        ENV_DELETE(m_pTimeBackgroundGoldNode);
        ENV_DELETE(m_pTimeBackgroundSilverNode);
        ENV_DELETE(m_pTimeBackgroundSilver);
        ENV_DELETE(m_pTimeBackgroundBronze);
        ENV_DELETE(m_pTimeBackgroundBronzeNode);
        ENV_DELETE(m_pKillsBackground);
        ENV_DELETE(m_pKillsBackgroundNode);
        ENV_DELETE(m_pArrowLeft);
        ENV_DELETE(m_pArrowLeftNode);
        ENV_DELETE(m_pArrowRight);
        ENV_DELETE(m_pArrowRightNode);
        ENV_DELETE(m_pEscape);
        ENV_DELETE(m_pEscapeNode);

        if (!bCreate)
            return;

        m_pReward = new CSprite();
        m_pRewardNode = new CSceneNode();
        m_pReward->SetMaterialFilename("GUI/Game/reward.png");
        m_pReward->SetUpdateRectByAnimation(true);
        m_pReward->AttachNode(m_pRewardNode);
        m_pReward->SetScreenSpace(true);
        m_pRewardNode->SetPosition(Vec3(0.5f, 0.5f, 0.0f));

        m_pRewardGold = new CSprite();
        m_pRewardGold->SetMaterialFilename("GUI/Game/reward_gold.png");
        m_pRewardGold->SetUpdateRectByAnimation(true);
        m_pRewardGold->AttachNode(m_pRewardNode);
        m_pRewardGold->SetScreenSpace(true);

        m_pRewardSilver = new CSprite();
        m_pRewardSilver->SetMaterialFilename("GUI/Game/reward_silver.png");
        m_pRewardSilver->SetUpdateRectByAnimation(true);
        m_pRewardSilver->AttachNode(m_pRewardNode);
        m_pRewardSilver->SetScreenSpace(true);

        m_pRewardBronze = new CSprite();
        m_pRewardBronze->SetMaterialFilename("GUI/Game/reward_bronze.png");
        m_pRewardBronze->SetUpdateRectByAnimation(true);
        m_pRewardBronze->AttachNode(m_pRewardNode);
        m_pRewardBronze->SetScreenSpace(true);

        m_pTimeBackground = new CSprite();
        m_pTimeBackgroundNode = new CSceneNode();
        m_pTimeBackground->SetMaterialFilename("GUI/Game/time_background.png");
        m_pTimeBackground->SetUpdateRectByAnimation(true);
        m_pTimeBackground->AttachNode(m_pTimeBackgroundNode);
        m_pTimeBackground->SetScreenSpace(true);
        m_pTimeBackgroundNode->SetPosition(Vec3(0.9f, 0.055f, 0.0f));

        m_pTimeBackgroundGold = new CSprite();
        m_pTimeBackgroundGoldNode = new CSceneNode();
        m_pTimeBackgroundGold->SetMaterialFilename("GUI/Game/time_background_gold.png");
        m_pTimeBackgroundGold->SetUpdateRectByAnimation(true);
        m_pTimeBackgroundGold->AttachNode(m_pTimeBackgroundGoldNode);
        m_pTimeBackgroundGold->SetScreenSpace(true);
        m_pTimeBackgroundGoldNode->SetPosition(Vec3(0.9f, 0.055f, 0.0f));

        m_pTimeBackgroundSilver = new CSprite();
        m_pTimeBackgroundSilverNode = new CSceneNode();
        m_pTimeBackgroundSilver->SetMaterialFilename("GUI/Game/time_background_silver.png");
        m_pTimeBackgroundSilver->SetUpdateRectByAnimation(true);
        m_pTimeBackgroundSilver->AttachNode(m_pTimeBackgroundSilverNode);
        m_pTimeBackgroundSilver->SetScreenSpace(true);
        m_pTimeBackgroundSilverNode->SetPosition(Vec3(0.9f, 0.055f, 0.0f));

        m_pTimeBackgroundBronze = new CSprite();
        m_pTimeBackgroundBronzeNode = new CSceneNode();
        m_pTimeBackgroundBronze->SetMaterialFilename("GUI/Game/time_background_bronze.png");
        m_pTimeBackgroundBronze->SetUpdateRectByAnimation(true);
        m_pTimeBackgroundBronze->AttachNode(m_pTimeBackgroundBronzeNode);
        m_pTimeBackgroundBronze->SetScreenSpace(true);
        m_pTimeBackgroundBronzeNode->SetPosition(Vec3(0.9f, 0.055f, 0.0f));

        m_pKillsBackground = new CSprite();
        m_pKillsBackgroundNode = new CSceneNode();
        m_pKillsBackground->SetMaterialFilename("GUI/Game/kills_background.png");
        m_pKillsBackground->SetUpdateRectByAnimation(true);
        m_pKillsBackground->AttachNode(m_pKillsBackgroundNode);
        m_pKillsBackground->SetScreenSpace(true);
        m_pKillsBackgroundNode->SetPosition(Vec3(0.75f, 0.055f, 0.0f));

        m_pArrowLeft = new CSprite();
        m_pArrowLeftNode = new CSceneNode();
        m_pArrowLeft->SetMaterialFilename("GUI/arrow_left.png");
        m_pArrowLeft->SetUpdateRectByAnimation(true);
        m_pArrowLeft->AttachNode(m_pArrowLeftNode);
        m_pArrowLeft->SetScreenSpace(true);
        m_pArrowLeftNode->SetPosition(Vec3(0.05f, 0.9f, 0.0f));

        m_pArrowRight = new CSprite();
        m_pArrowRightNode = new CSceneNode();
        m_pArrowRight->SetMaterialFilename("GUI/arrow_right.png");
        m_pArrowRight->SetUpdateRectByAnimation(true);
        m_pArrowRight->AttachNode(m_pArrowRightNode);
        m_pArrowRight->SetScreenSpace(true);
        m_pArrowRightNode->SetPosition(Vec3(0.95f, 0.9f, 0.0f));

        m_pEscape = new CSprite();
        m_pEscapeNode = new CSceneNode();
        m_pEscape->SetMaterialFilename("GUI/escape.png");
        m_pEscape->SetUpdateRectByAnimation(true);
        m_pEscape->AttachNode(m_pEscapeNode);
        m_pEscape->SetScreenSpace(true);
        m_pEscapeNode->SetPosition(Vec3(0.5f, 0.9f, 0.0f));
    }

    void CCheckpointEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        SetText(GetName());
        
        m_levelBronzeTime = 200.0f;


        //m_bJustCreated = true;

        SetFireOnce(true);

        if(m_pFade)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pFade->GetName());
            m_pFade = NULL;
        }

        InitSprites(m_bFinal);

        if(m_bFinal)
        {

            /*m_logo.SetMaterialFilename("Sprites/checkpoint_status.png");
            m_logo.SetScreenSpace(false);
            m_logo.AttachNode(&m_logoNode);
            m_logoNode.SetPosition(m_pNode->GetDerivedPosition());
            m_logoNode.SetScale(VECTOR3(0.8f, 0.8f, 0.8f));


            m_pReward = new CSprite();
            m_pRewardNode = new CSceneNode();
            m_pReward->SetMaterialFilename("GUI/Game/reward.png");
            m_pReward->SetUpdateRectByAnimation(true);
            m_pReward->AttachNode(m_pRewardNode);
            m_pReward->SetScreenSpace(true);
            m_pRewardNode->SetPosition(VECTOR3(0.5f, 0.5f, 0.0f));
        
            m_pRewardGold = new CSprite();
            m_pRewardGold->SetMaterialFilename("GUI/Game/reward_gold.png");
            m_pRewardGold->SetUpdateRectByAnimation(true);
            m_pRewardGold->AttachNode(m_pRewardNode);
            m_pRewardGold->SetScreenSpace(true);

            m_pRewardSilver = new CSprite();
            m_pRewardSilver->SetMaterialFilename("GUI/Game/reward_silver.png");
            m_pRewardSilver->SetUpdateRectByAnimation(true);
            m_pRewardSilver->AttachNode(m_pRewardNode);
            m_pRewardSilver->SetScreenSpace(true);

            m_pRewardBronze = new CSprite();
            m_pRewardBronze->SetMaterialFilename("GUI/Game/reward_bronze.png");
            m_pRewardBronze->SetUpdateRectByAnimation(true);
            m_pRewardBronze->AttachNode(m_pRewardNode);
            m_pRewardBronze->SetScreenSpace(true);

            m_pTimeBackground = new CSprite();
            m_pTimeBackgroundNode = new CSceneNode();
            m_pTimeBackground->SetMaterialFilename("GUI/Game/time_background.png");
            m_pTimeBackground->SetUpdateRectByAnimation(true);
            m_pTimeBackground->AttachNode(m_pTimeBackgroundNode);
            m_pTimeBackground->SetScreenSpace(true);
            m_pTimeBackgroundNode->SetPosition(VECTOR3(0.9f, 0.055f, 0.0f));

            m_pTimeBackgroundGold = new CSprite();
            m_pTimeBackgroundGoldNode = new CSceneNode();
            m_pTimeBackgroundGold->SetMaterialFilename("GUI/Game/time_background_gold.png");
            m_pTimeBackgroundGold->SetUpdateRectByAnimation(true);
            m_pTimeBackgroundGold->AttachNode(m_pTimeBackgroundGoldNode);
            m_pTimeBackgroundGold->SetScreenSpace(true);
            m_pTimeBackgroundGoldNode->SetPosition(VECTOR3(0.9f, 0.055f, 0.0f));

            m_pTimeBackgroundSilver = new CSprite();
            m_pTimeBackgroundSilverNode = new CSceneNode();
            m_pTimeBackgroundSilver->SetMaterialFilename("GUI/Game/time_background_silver.png");
            m_pTimeBackgroundSilver->SetUpdateRectByAnimation(true);
            m_pTimeBackgroundSilver->AttachNode(m_pTimeBackgroundSilverNode);
            m_pTimeBackgroundSilver->SetScreenSpace(true);
            m_pTimeBackgroundSilverNode->SetPosition(VECTOR3(0.9f, 0.055f, 0.0f));

            m_pTimeBackgroundBronze = new CSprite();
            m_pTimeBackgroundBronzeNode = new CSceneNode();
            m_pTimeBackgroundBronze->SetMaterialFilename("GUI/Game/time_background_bronze.png");
            m_pTimeBackgroundBronze->SetUpdateRectByAnimation(true);
            m_pTimeBackgroundBronze->AttachNode(m_pTimeBackgroundBronzeNode);
            m_pTimeBackgroundBronze->SetScreenSpace(true);
            m_pTimeBackgroundBronzeNode->SetPosition(VECTOR3(0.9f, 0.055f, 0.0f));

            m_pKillsBackground = new CSprite();
            m_pKillsBackgroundNode = new CSceneNode();
            m_pKillsBackground->SetMaterialFilename("GUI/Game/kills_background.png");
            m_pKillsBackground->SetUpdateRectByAnimation(true);
            m_pKillsBackground->AttachNode(m_pKillsBackgroundNode);
            m_pKillsBackground->SetScreenSpace(true);
            m_pKillsBackgroundNode->SetPosition(VECTOR3(0.75f, 0.055f, 0.0f));
            
            m_pArrowLeft = new CSprite();
            m_pArrowLeftNode = new CSceneNode();
            m_pArrowLeft->SetMaterialFilename("GUI/arrow_left.png");
            m_pArrowLeft->SetUpdateRectByAnimation(true);
            m_pArrowLeft->AttachNode(m_pArrowLeftNode);
            m_pArrowLeft->SetScreenSpace(true);
            m_pArrowLeftNode->SetPosition(VECTOR3(0.06f, 0.9f, 0.0f));

            m_pArrowRight = new CSprite();
            m_pArrowRightNode = new CSceneNode();
            m_pArrowRight->SetMaterialFilename("GUI/arrow_right.png");
            m_pArrowRight->SetUpdateRectByAnimation(true);
            m_pArrowRight->AttachNode(m_pArrowRightNode);
            m_pArrowRight->SetScreenSpace(true);
            m_pArrowRightNode->SetPosition(VECTOR3(0.94f, 0.9f, 0.0f));

            m_pEscape = new CSprite();
            m_pEscapeNode = new CSceneNode();
            m_pEscape->SetMaterialFilename("GUI/escape.png");
            m_pEscape->SetUpdateRectByAnimation(true);
            m_pEscape->AttachNode(m_pEscapeNode);
            m_pEscape->SetScreenSpace(true);
            m_pEscapeNode->SetPosition(VECTOR3(0.5f, 0.9f, 0.0f));*/



        }
        else
        {
            SetResetTrigger(false);
        }

        m_openingTime = 0.65f;

        m_bGhost = false;

        RemovePolygons();

//        m_sprite.SetApplyFade(false);

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-32.0f, -32.0f, 0.0f));
        p.AddPoint(Vec3(-32.0f,  32.0f, 0.0f));
        p.AddPoint(Vec3( 32.0f,  32.0f, 0.0f));
        p.AddPoint(Vec3( 32.0f, -32.0f, 0.0f));
        AddPolygon(p);

        m_pMenueTransitionInSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_IN);
        m_pMenueTransitionInSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);

        m_pMenueTransitionOutSound = gEngine->GetAudioSystem()->AddGetPtr(AUDIO_EFFECT_TRANSITION_OUT);
        m_pMenueTransitionOutSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);


        m_pCharacterFinishSound = gEngine->GetAudioSystem()->AddGetPtr("CharacterFinish.wav");
        m_pCharacterFinishSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);
    }

    void CCheckpointEntity::OnTriggerEntered(CEntity& entity)
    {
        if(!gEngine->GetEditor())
        {
            FireCheckpoint();
        }
    }

    void CCheckpointEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        if(eventId == CHECKPOINT_EVENT_CHANGELEVEL && !gEngine->GetEditor())
            LoadLevel();
        else
            CSpriteEntity::OnFired(eventId);
    }

    void CCheckpointEntity::FireCheckpoint()
    {
        if(!gEngine->GetWorldMgr()->GetGameMode() || (gEngine->GetEditor()))
            return;

        m_fade = FadeBarsRenderObject();

        if(m_bStart)
        {
            m_pMenueTransitionInSound->PlaySound();

            m_state = CHECKPOINT_STATE_STARTER;
            //m_state = CHECKPOINT_STATE_OPENING;
            m_currentStateTime = 0.0f;

            CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
            m_camStartPos = pCam->GetAttachedNode()->GetDerivedPosition();

            
        }
        else if(m_bFinal)
        {
            m_levelFinishTime = gEngine->GetWorldMgr()->GetLevelTime();

            m_pCharacterFinishSound->PlaySound();

            m_state = CHECKPOINT_STATE_FINISHER;

            if (m_pCharacter)
                m_pCharacter->OnInput(CCharacter3DEntity::CHARACTER_EVENT_ENABLE_FREEZE, EventParameter());

            m_currentStateTime = 0.0f;

            CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
            m_camStartPos = pCam->GetAttachedNode()->GetDerivedPosition();

            Vec3 camTargetPos = m_pNode->GetDerivedPosition();
            m_camFinalPos = Vec3(camTargetPos.x, camTargetPos.y, m_camStartPos.z);
        }

        //m_bJustCreated = false;
        m_bCheckpointFired = true;
    }

    void CCheckpointEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if(m_bCheckpointFired)
        {
            switch(m_state)
            {
            case CHECKPOINT_STATE_STARTER:
                {
                    float leftBar = 0.1f, rightBar = 0.1f,
                        topBar = 0.1f,
                        bottomBar = 0.1f;


                    
                    //.fadeBarsType = FADE_BARS_FROM_RIGHT_TO_LEFT;
                    if(m_currentStateTime < 0.6f)
                    {
                        if (m_currentStateTime <= 0.0f)
                            m_pMenueTransitionInSound->PlaySound();

                        float alpha = m_currentStateTime / 0.6f;
                        alpha = 1.0f - alpha;
                        alpha = alpha * alpha * alpha;
                        alpha = 1.0f - alpha;
//					
                        m_fade.fadeBarsAmount = 1.0f - alpha;
                        queue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(leftBar, rightBar, topBar, bottomBar);

                        queue.m_fadeScreenRenderObject.m_fadeScreenColorTop.a = 1.0f - alpha;
                        queue.m_fadeScreenRenderObject.m_fadeScreenColorBot.a = 1.0f - alpha;
                        queue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
                        queue.m_pFadeBarsRenderObject = &m_fade;
                    }
                    
                    if(m_currentStateTime >= 0.4f && m_currentStateTime < 0.7f)
                    {
                        float alpha = 1.0f - (m_currentStateTime - 0.4f) / 0.3f;
                        queue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(alpha * leftBar, alpha * rightBar, alpha * topBar, alpha * bottomBar);
                        m_fade.fadeBarsAmount = 0.0f;
                        queue.m_postProcessRenderObject.m_posterizationAmount = alpha;
                        queue.m_pFadeBarsRenderObject = &m_fade;
                    }

                }
                break;
            case CHECKPOINT_STATE_FINISHER:
                {
                    gEngine->GetWorldMgr()->SetPaused();

                    m_fade.fadeBarsType = FADE_BARS_FROM_TOP_TO_BOTTOM;
                    
                    float leftBar = 0.05f, 
                        rightBar = 0.05f,
                        topBar = 0.50f,
                        bottomBar = 0.50f;

                    if(m_currentStateTime <= 0.3f)
                    {
                        // Center cam to checkpoint, fade frame border in
                        //
                        float alpha = m_currentStateTime / 0.3f;
                        alpha = 1.0f - alpha;
                        alpha = alpha * alpha * alpha;
                        alpha = 1.0f - alpha;

                        //m_fade.fadeFrameAmount = alpha;

                        queue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(alpha * leftBar, alpha * rightBar, alpha * topBar, alpha * bottomBar);
                        queue.m_postProcessRenderObject.m_posterizationAmount = alpha;
                        queue.m_pFadeBarsRenderObject = &m_fade;

                        CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
                        //VECTOR3 camEndPos = m_camStartPos + VECTOR3(0.0f, 150.0f, 0.0f);

                        Vec3 currentCamPos = alpha * m_camFinalPos + (1.0f - alpha) * m_camStartPos;
                        pCam->GetAttachedNode()->SetPosition(currentCamPos);
                        
                        //m_levelFinishedNode.SetScale((1.0f - alpha) * VECTOR3(6.5f, 6.5f, 1.0f) + alpha * VECTOR3(1.0f, 1.0f, 1.0f));
                        //m_levelFinishedNode.SetOrientation(CQuaternion(0.0f, 0.0f, GetRandomFloat(-0.05f, 0.05f)));
                        //m_levelFinished.SetColor(VECTOR4(1.0f, 1.0f, 1.0f, alpha));
                        //queue.spriteGuiData.push_back(&m_levelFinished);

                        //m_levelNameNode.SetScale((1.0f - alpha) * VECTOR3(6.5f, 6.5f, 1.0f) + alpha * VECTOR3(1.0f, 1.0f, 1.0f));
                        //m_levelNameNode.SetOrientation(CQuaternion(0.0f, 0.0f, GetRandomFloat(-0.05f, 0.05f)));
                        //m_levelName.SetColor(VECTOR4(1.0f, 1.0f, 1.0f, alpha));
                        //queue.spriteGuiData.push_back(&m_levelName);
                        
                    }
                    else if(m_currentStateTime <= 1.5f)
                    {
                        if (m_currentStateTime > 0.5f)
                        {
                            m_currentStateTime = 1.5f;
                            m_pMenueTransitionOutSound->PlaySound();
                        }
                        // Just wait
                        //
                        queue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(leftBar, rightBar, topBar, bottomBar);
                        queue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
                        queue.m_pFadeBarsRenderObject = &m_fade;

                        CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
                        pCam->GetAttachedNode()->SetPosition(m_camFinalPos);

                        //queue.spriteGuiData.push_back(&m_levelFinished);
                        //queue.spriteGuiData.push_back(&m_levelName);

                        //m_currentStateTime = 1.4f;
                    }
                    else if(m_currentStateTime <= 2.0f)
                    {
                        // Fade out screen, hide character, hide checkpoint
                        //
//                         if (m_pCharacter)
//                             m_pCharacter->SetVisible(false);

                        float alpha = (m_currentStateTime - 1.5f) / 0.5f;
                        alpha = 1.0f - alpha;
                        alpha = alpha * alpha * alpha;
                        alpha = 1.0f - alpha;

                        //m_fade.fadeFrameAmount = 1.0f;
                        CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
                        pCam->GetAttachedNode()->SetPosition(m_camFinalPos);


                        float startYaw = 1.0f, endYaw = 0.0f;
                        float roll = alpha * endYaw + (1.0 - alpha) * startYaw;
                        m_pNode->SetScale(Vec3(1.0f, 1.0f, 1.0f) * roll);

                        queue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(leftBar, rightBar, topBar, bottomBar);
                        m_fade.fadeBarsType = FADE_BARS_FROM_LEFT_TO_RIGHT;
                        m_fade.fadeBarsAmount = alpha;
                        queue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
                        queue.m_pFadeBarsRenderObject = &m_fade;

                        //m_levelFinishedNode.SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));
                        //m_levelNameNode.SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));


                        //m_levelFinished.SetColor(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f - alpha));
                        //m_levelName.SetColor(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f - alpha));
                        
                    }
                    else
                    {
                        std::stringstream strLevelTime;
                        strLevelTime.precision(5);
                        strLevelTime << m_levelFinishTime << "s";

                        std::stringstream strLevelTimeDifference;
                        strLevelTimeDifference.precision(5);

                        CSingletonPlayerMgr playerMgr;

                        LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex());

                        //bool bShowTimeDifference = levelInfo.bestTime > 0.0f;

                        if (levelInfo.bestTime > m_levelFinishTime)
                        {
                            strLevelTime << " | -" << (levelInfo.bestTime - m_levelFinishTime) << "s";
                        }
                        else
                        {
                            strLevelTime << " | +" << (m_levelFinishTime - levelInfo.bestTime) << "s";
                        }

                        m_bTriggerCheckpointAction = true; 

                        m_checkpointAction = CHECKPOINT_ACTION_LOADLEVEL;

                        if (!m_bTriggerCheckpointAction)
                        {
                            /*if (m_levelFinishTime > m_levelBronzeTime)
                            {
                                VECTOR2 textFinishTimePos(0.5f, 0.46f);
                                VECTOR2 textFinishTimeDifferencePos(0.5f, 0.54f);
                                VECTOR2 textReplayPos(0.13f, 0.9f);
                                VECTOR2 textNextPos(0.87f, 0.9f);

                                if (!bShowTimeDifference)
                                    textFinishTimePos.y = 0.5f;

                                // No medal won
                                if (m_currentStateTime <= 2.25f)
                                {
                                    // Fade time in
                                    float alpha = (m_currentStateTime - 2.0f) / 0.25f;

                                    Text textTime(strLevelTime.str(), VECTOR4(0.75f, 0.85f, 1.0f, alpha), textFinishTimePos);
                                    queue.text["FontRewardTime"].push_back(textTime);

                                    if (bShowTimeDifference)
                                    {
                                        Text textTimeDifference(strLevelTimeDifference.str(), VECTOR4(0.75f, 0.85f, 1.0f, alpha), textFinishTimeDifferencePos);
                                        queue.text["FontRewardDifferenceTime"].push_back(textTimeDifference);
                                    }

                                    Text textNext("REPLAY", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textReplayPos);
                                    queue.text["FontMenue"].push_back(textNext);

                                    Text textReplay("NEXT", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textNextPos);
                                    queue.text["FontMenue"].push_back(textReplay);

                                    m_pArrowLeft->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pArrowRight->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pEscape->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    queue.spriteGuiData.push_back(m_pArrowLeft);
                                    queue.spriteGuiData.push_back(m_pArrowRight);
                                    //queue.spriteGuiData.push_back(m_pEscape);

                                }
                                else if (m_currentStateTime <= 3.25f)
                                {
                                    // Show time
                                    Text textTime(strLevelTime.str(), VECTOR4(0.75f, 0.85f, 1.0f, 1.0f), textFinishTimePos);
                                    queue.text["FontRewardTime"].push_back(textTime);

                                    if (bShowTimeDifference)
                                    {
                                        Text textTimeDifference(strLevelTimeDifference.str(), VECTOR4(0.75f, 0.85f, 1.0f, 1.0f), textFinishTimeDifferencePos);
                                        queue.text["FontRewardDifferenceTime"].push_back(textTimeDifference);
                                    }

                                    Text textNext("REPLAY", VECTOR4(0.8f, 0.8f, 0.8f, 1), textReplayPos);
                                    queue.text["FontMenue"].push_back(textNext);

                                    Text textReplay("NEXT", VECTOR4(0.8f, 0.8f, 0.8f, 1), textNextPos);
                                    queue.text["FontMenue"].push_back(textReplay);

                                    m_currentStateTime = 3.0f;

                                    if (gEngine->GetInput()->IsActionJustTriggered(ACTION_RIGHT))
                                    {
                                        m_currentStateTime = 3.251f;

                                        m_checkpointAction = CHECKPOINT_ACTION_LOADLEVEL;
                                    }
                                    else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_LEFT))
                                    {
                                        m_currentStateTime = 3.251f;

                                        m_checkpointAction = CHECKPOINT_ACTION_REPLAYLEVEL;
                                    }

                                    m_pArrowLeft->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, 1));
                                    m_pArrowRight->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, 1));
                                    m_pEscape->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, 1));
                                    queue.spriteGuiData.push_back(m_pArrowLeft);
                                    queue.spriteGuiData.push_back(m_pArrowRight);
                                    //queue.spriteGuiData.push_back(m_pEscape);
                                }
                                else if (m_currentStateTime <= 3.5f)
                                {
                                    // Fade time out
                                    float alpha = 1.0f - (m_currentStateTime - 3.25f) / 0.25f;

                                    Text textTime(strLevelTime.str(), VECTOR4(0.75f, 0.85f, 1.0f, alpha), textFinishTimePos);
                                    queue.text["FontRewardTime"].push_back(textTime);

                                    if (bShowTimeDifference)
                                    {
                                        Text textTimeDifference(strLevelTimeDifference.str(), VECTOR4(0.75f, 0.85f, 1.0f, alpha), textFinishTimeDifferencePos);
                                        queue.text["FontRewardDifferenceTime"].push_back(textTimeDifference);
                                    }

                                    Text textNext("REPLAY", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textReplayPos);
                                    queue.text["FontMenue"].push_back(textNext);

                                    Text textReplay("NEXT", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textNextPos);
                                    queue.text["FontMenue"].push_back(textReplay);

                                    m_pArrowLeft->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pArrowRight->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pEscape->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    queue.spriteGuiData.push_back(m_pArrowLeft);
                                    queue.spriteGuiData.push_back(m_pArrowRight);
                                    //queue.spriteGuiData.push_back(m_pEscape);
                                }
                                else
                                    m_bTriggerCheckpointAction = true;

                            }
                            else*/
                            /*{
                                // Show won medal on black screen: bronze, silver or gold

                                VECTOR2 medalPos(0.5f, 0.25f);
                                VECTOR2 textFinishTimePos(0.5f, 0.75f);
                                VECTOR2 textFinishTimeDifferencePos(0.5f, 0.8f);
                                VECTOR2 textReplayPos(0.13f, 0.9f);
                                VECTOR2 textNextPos(0.87f, 0.9f);

                                std::string strMedalName;
                                VECTOR3 medalColor(1.0f, 1.0f, 1.0f),
                                    baseMedalColor(0.3f, 0.3f, 0.3f);

                                CSprite* pWonReward = 0;
                                if (m_levelFinishTime <= m_levelGoldTime)
                                {
                                    // Gold
                                    strMedalName = "G O L D ";
                                    medalColor = VECTOR3(1.0f, 0.75f, 0.0f);
                                    levelInfo.medal = std::min(levelInfo.medal, 0);
                                    pWonReward = m_pRewardGold;
                                }
                                else if (m_levelFinishTime <= m_levelSilverTime)
                                {
                                    // Silver
                                    strMedalName = "S I L V E R ";
                                    medalColor = VECTOR3(1.0f, 1.0f, 1.0f);
                                    levelInfo.medal = std::min(levelInfo.medal, 1);
                                    pWonReward = m_pRewardSilver;
                                }
                                else if (m_levelFinishTime <= m_levelBronzeTime)
                                {
                                    strMedalName = "B R O N Z E ";
                                    medalColor = VECTOR3(1.0f, 0.5f, 0.0f);
                                    levelInfo.medal = std::min(levelInfo.medal, 2);
                                    pWonReward = m_pRewardBronze;
                                }
                                else
                                    medalColor = VECTOR3(0.8f, 0.8f, 0.8f);


                                if (m_currentStateTime <= 2.25f)
                                {
                                    // Fade & scale base medal in
                                    float alpha = (m_currentStateTime - 2.0f) / 0.25f;
                                    alpha = 1.0f - alpha;
                                    alpha = alpha * alpha * alpha;
                                    alpha = 1.0f - alpha;
                                    VECTOR3 scale = VECTOR3(1.0f, 1.0f, 1.0f) * alpha + VECTOR3(2.5f, 2.5f, 2.5f) * (1.0f - alpha);
                                    m_pRewardNode->SetScale(scale);
                                    m_pReward->SetColor(baseMedalColor);
                                    m_pReward->SetAlpha(alpha * 1.0f);

                                    queue.m_spriteRenderObjects.push_back(m_pReward);
                                }
                                else if (m_currentStateTime <= 2.75f)
                                {
                                    // Show base medal
                                    m_pRewardNode->SetScale(VECTOR3(1.0f, 1.0f, 1.0f));
                                    m_pReward->SetAlpha(1.0f);

                                    queue.m_spriteRenderObjects.push_back(m_pReward);
                                }
                                else if (m_currentStateTime <= 3.5f)
                                {
                                    // Fade to medal color & fade text in
                                    float alpha = (m_currentStateTime - 2.75f) / 0.75f;
                                    VECTOR3 color = baseMedalColor * (1.0f - alpha) + medalColor * alpha;
                                    //m_pReward->SetColor(color);
                                    m_pReward->SetAlpha(1.0f - alpha);
                                    if (pWonReward)
                                    {
                                        pWonReward->SetAlpha(alpha);
                                        queue.m_spriteRenderObjects.push_back(pWonReward);
                                    }

                                    TextRenderObject textTime(strLevelTime.str(), VECTOR4(medalColor.x, medalColor.y, medalColor.z, alpha), textFinishTimePos);
                                    queue.m_textRenderObjects["FontRewardTime"].push_back(textTime);

                                    if (bShowTimeDifference)
                                    {
                                        TextRenderObject textTimeDifference(strLevelTimeDifference.str(), VECTOR4(medalColor.x, medalColor.y, medalColor.z, alpha), textFinishTimeDifferencePos);
                                        queue.m_textRenderObjects["FontRewardDifferenceTime"].push_back(textTimeDifference);
                                    }

                                    TextRenderObject textNext("REPLAY", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textReplayPos);
                                    queue.m_textRenderObjects["FontMenue"].push_back(textNext);

                                    TextRenderObject textReplay("NEXT", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textNextPos);
                                    queue.m_textRenderObjects["FontMenue"].push_back(textReplay);

                                    m_pArrowLeft->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pArrowRight->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pEscape->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    queue.m_spriteRenderObjects.push_back(m_pArrowLeft);
                                    queue.m_spriteRenderObjects.push_back(m_pArrowRight);
                                    //queue.spriteGuiData.push_back(m_pEscape);

                                    queue.m_spriteRenderObjects.push_back(m_pReward);
                                    
                                }
                                else if (m_currentStateTime <= 4.5f)
                                {
                                    // Show medal & text
                                    //m_pReward->SetColor(medalColor);
                                    //m_pReward->SetAlpha(1.0f);
                                    if (pWonReward)
                                    {
                                        pWonReward->SetAlpha(1.0f);
                                        queue.m_spriteRenderObjects.push_back(pWonReward);
                                    }

                                    TextRenderObject textTime(strLevelTime.str(), VECTOR4(medalColor.x, medalColor.y, medalColor.z, 1), textFinishTimePos);
                                    queue.m_textRenderObjects["FontRewardTime"].push_back(textTime);

                                    if (bShowTimeDifference)
                                    {
                                        TextRenderObject textTimeDifference(strLevelTimeDifference.str(), VECTOR4(medalColor.x, medalColor.y, medalColor.z, 1), textFinishTimeDifferencePos);
                                        queue.m_textRenderObjects["FontRewardDifferenceTime"].push_back(textTimeDifference);
                                    }


                                    // Wait for user input to change the level
                                    m_currentStateTime = 3.6f;

                                    if (gEngine->GetInput()->IsActionJustTriggered(ACTION_RIGHT))
                                    {
                                        m_currentStateTime = 4.501f;

                                        m_checkpointAction = CHECKPOINT_ACTION_LOADLEVEL;
                                    }
                                    else if (gEngine->GetInput()->IsActionJustTriggered(ACTION_LEFT))
                                    {
                                        m_currentStateTime = 4.501f;

                                        m_checkpointAction = CHECKPOINT_ACTION_REPLAYLEVEL;
                                    }
    
                                    TextRenderObject textNext("REPLAY", VECTOR4(0.8f, 0.8f, 0.8f, 1), textReplayPos);
                                    queue.m_textRenderObjects["FontMenue"].push_back(textNext);

                                    TextRenderObject textReplay("NEXT", VECTOR4(0.8f, 0.8f, 0.8f, 1), textNextPos);
                                    queue.m_textRenderObjects["FontMenue"].push_back(textReplay);

                
                                
                                    m_pArrowLeft->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, 1));
                                    m_pArrowRight->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, 1));
                                    m_pEscape->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, 1));
                                    queue.m_spriteRenderObjects.push_back(m_pArrowLeft);
                                    queue.m_spriteRenderObjects.push_back(m_pArrowRight);
                                    //queue.spriteGuiData.push_back(m_pEscape);


                                }
                                else if (m_currentStateTime <= 4.75f)
                                {
                                    // Fade out rewards
                                    float alpha = (m_currentStateTime - 4.5f) / 0.25f;
                                    alpha = 1.0f - alpha;
                                    alpha = alpha * alpha * alpha;

                                    m_pRewardNode->SetScale(VECTOR3(1.0f, 1.0f, 1.0f) * alpha);

                                    if (pWonReward)
                                    {
                                        pWonReward->SetAlpha(alpha);
                                        queue.m_spriteRenderObjects.push_back(pWonReward);
                                    }
                                    

                                    TextRenderObject textTime(strLevelTime.str(), VECTOR4(medalColor.x, medalColor.y, medalColor.z, alpha), textFinishTimePos);
                                    queue.m_textRenderObjects["FontRewardTime"].push_back(textTime);

                                    if (bShowTimeDifference)
                                    {
                                        TextRenderObject textTimeDifference(strLevelTimeDifference.str(), VECTOR4(medalColor.x, medalColor.y, medalColor.z, alpha), textFinishTimeDifferencePos);
                                        queue.m_textRenderObjects["FontRewardDifferenceTime"].push_back(textTimeDifference);
                                    }

                                    TextRenderObject textNext("REPLAY", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textReplayPos);
                                    queue.m_textRenderObjects["FontMenue"].push_back(textNext);

                                    TextRenderObject textReplay("NEXT", VECTOR4(0.8f, 0.8f, 0.8f, alpha), textNextPos);
                                    queue.m_textRenderObjects["FontMenue"].push_back(textReplay);

                                    m_pArrowLeft->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pArrowRight->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    m_pEscape->SetColor(VECTOR4(0.75f, 0.85f, 1.0f, alpha));
                                    queue.m_spriteRenderObjects.push_back(m_pArrowLeft);
                                    queue.m_spriteRenderObjects.push_back(m_pArrowRight);
                                    //queue.spriteGuiData.push_back(m_pEscape);
                                    
                                }
                                else
                                {
                                    m_bTriggerCheckpointAction = true;
                                }
                            }*/
                        }
                        else
                        {
                            if (m_levelFinishTime < levelInfo.bestTime)
                                levelInfo.bestTime = m_levelFinishTime;

                            CSingletonPlayerMgr playerMgr;

                            LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), playerMgr->GetCurrentLevelIndex());
                            levelInfo.bFinished = true;
                            levelInfo.bAccessible = true;

                            if (m_levelFinishTime > levelInfo.bestTime)
                                levelInfo.bestTime = m_levelFinishTime;

                            // Load next level
                            if (m_checkpointAction == CHECKPOINT_ACTION_LOADLEVEL)
                                LoadLevel();
                            else if (m_checkpointAction == CHECKPOINT_ACTION_REPLAYLEVEL)
                            {
                                CGameState* pGameState = static_cast<CGameState*>(gEngine->GetState("GameState"));
                                //gEngine->SetNewState("GameState");
                            }
                            else if (m_checkpointAction == CHECKPOINT_ACTION_LOADMENUE)
                            {
                                gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env", "LevelSelectionState");
                            }

                            m_checkpointAction = CHECKPOINT_ACTION_WAITFORINPUT;
                        }	

                        CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
                        pCam->GetAttachedNode()->SetPosition(m_camFinalPos);

                        queue.m_fadeFrameRenderObject.m_fadeFrame = Vec4(0.1f, 0.1f, 0.75f, 0.1f);
                        m_fade.fadeBarsType = FADE_BARS_FROM_LEFT_TO_RIGHT;
                        m_fade.fadeBarsAmount = 1;
                        queue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;
                        queue.m_pFadeBarsRenderObject = &m_fade;
                    }
                }

                break;

                default:
                    break;
            }
        }
        else if (m_bFinal)
        {
            /*if (GetRenderGame())
            {
                double levelTime = (double)gEngine->GetWorldMgr()->GetLevelTime();

                CSingletonPlayerMgr playerMgr;

                LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapter(), playerMgr->GetCurrentLevel());

                std::stringstream strLevelTime;
                strLevelTime.precision(5);

                strLevelTime << levelTime;


                std::stringstream strKills;
                strKills << levelInfo.numKills;


                TextRenderObject textTime(strLevelTime.str(), VECTOR4(0.75f, 0.85f, 1.0f, 1.0f), VECTOR2(m_pTimeBackgroundNode->GetPosition().x, m_pTimeBackgroundNode->GetPosition().y));
                queue.m_textRenderObjects["FontGameHeader"].push_back(textTime);

                TextRenderObject textKills(strKills.str(), VECTOR4(0.75f, 0.85f, 1.0f, 1.0f), VECTOR2(m_pKillsBackgroundNode->GetPosition().x, m_pKillsBackgroundNode->GetPosition().y));
                queue.m_textRenderObjects["FontGameHeader"].push_back(textKills);


                if (levelTime <= m_levelGoldTime)
                {
                    queue.m_spriteRenderObjects.push_back(m_pTimeBackgroundGold);
                }
                else if (levelTime <= m_levelSilverTime)
                {
                    queue.m_spriteRenderObjects.push_back(m_pTimeBackgroundSilver);
                }
                else if (levelTime <= m_levelBronzeTime)
                {
                    queue.m_spriteRenderObjects.push_back(m_pTimeBackgroundBronze);
                }
                else
                {
                    queue.m_spriteRenderObjects.push_back(m_pTimeBackground);
                }

                queue.m_spriteRenderObjects.push_back(m_pKillsBackground);
            }*/
            
        }
    }


    void CCheckpointEntity::LoadLevel()
    {
        CSingletonPlayerMgr playerMgr;
    
        if(m_bFinishChapter)
        {
            playerMgr->SetCurrentChapterFinished(true);
            CLevelSelectionState* pLevelSelectionState = static_cast<CLevelSelectionState*>(gEngine->GetState("LevelSelectionState"));
            pLevelSelectionState->SetPlayChapterComplete(true);
            gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env", "LevelSelectionState");
        }
        else if(!m_strNextLevel.empty())
        {
            std::string strNextLevel = "../Resources/Levels/" + m_strNextLevel;

//             WIN32_FIND_DATA data;
//             HANDLE hFile = FindFirstFile(strNextLevel.c_str(), &data);
// 
//             if(hFile != INVALID_HANDLE_VALUE)
//             {
//                 FindClose(hFile);
//                 CGameState* pGameState = static_cast<CGameState*>(gEngine->GetState(CGameState::STATE_NAME));
//                 playerMgr->SetCurrentLevel(playerMgr->GetCurrentLevel() + 1);
//                 playerMgr->GetLevelinfo(playerMgr->GetCurrentChapter(), playerMgr->GetCurrentLevel()).bAccessible = true;
//                 gEngine->GetLevelLoader().LoadLevel(strNextLevel/*playerMgr->GetLevelinfo(playerMgr->GetCurrentChapter(), 1).strFilename*/, "GameState");
//             }
//             else
//             {
//                 FindClose(hFile);
//                 gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + playerMgr->GetChapterFolder() + "menue.env", "LevelSelectionState");
//             }
        }

        playerMgr->Save();
    }

    void CCheckpointEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CCheckpointEntity::Update);

        CSpriteEntity::Update(dt);

        m_currentStateTime += gEngine->GetTimeElapsed();
    }

    void CCheckpointEntity::SetFinal(bool bFinal)
    {
        m_bFinal = bFinal;

        if (m_bFinal)
        {
            SetResetTrigger(true);
            SetMaterialFilename("Sprites/checkpoint_finish.dds");

            InitSprites(true);
        }
        else
        {
            SetResetTrigger(false);
            SetMaterialFilename("Sprites/checkpoint.png");

            InitSprites(false);
        }
    }

    bool CCheckpointEntity::GetFinal()
    {
        return m_bFinal;
    }

    void CCheckpointEntity::SetStart(bool bStart)
    {
        m_bStart = bStart;

        if(m_bStart)
            SetRenderGame(false);
        else
            SetRenderGame(true);
    }

    bool CCheckpointEntity::GetStart()
    {
        return m_bStart;
    }

    void CCheckpointEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_pCharacter = static_cast< CCharacter3DEntity* >(gEngine->GetEntityMgr()->GetFirstEntityOfType(CCharacter3DEntity::ENTITY_META_TYPEID));

        CCamera* pCamera = gEngine->GetWorldMgr()->GetActiveCamera();
        pCamera->GetAttachedNode()->SetOrientation(CQuaternion());
        m_currentStateTime = 0.0f;

        m_checkpointAction = CHECKPOINT_ACTION_WAITFORINPUT;
        m_bTriggerCheckpointAction = false;

        m_state = CHECKPOINT_STATE_OPENED;

        m_pNode->SetScale(Vec3(1.0f, 1.0f, 1.0f));

        m_sprite.SetColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));

        m_levelFinishTime = 0.0f;

        gEngine->GetWorldMgr()->GetScenario()->SetSepia(0.0f);

        m_bCheckpointFired = false;
    }









    //-----------------------------------------------------------------------------------
    const float CHECKPOINT_TIME_FADEIN = 0.3f;
    const float CHECKPOINT_BARS_LEFT = 0.1f;
    const float CHECKPOINT_BARS_RIGHT = 0.1f;
    const float CHECKPOINT_BARS_TOP = 0.1f;
    const float CHECKPOINT_BARS_BOTTOM = 0.1f;
    const float CHECKPOINT_ROTATION_SPEED = 3.0f;

    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CCheckpointOriginEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_CHECKPOINT_ORIGIN;

    //-----------------------------------------------------------------------------------
    CCheckpointOriginEntity::CCheckpointOriginEntity()
        : m_pCharacter(nullptr)
        , m_bCheckpointFired(false)
        , m_bDisabled(false)
        , m_bDisabledInitial(false)
    {
        SetMetaTypeID(CCheckpointOriginEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Checkpoint Origin] SetDisabled", CHECKPOINT_EVENT_SET_DISABLED);
        RegisterEventParameter(CHECKPOINT_EVENT_SET_DISABLED, "SetDisabled", CBaseParam::PARAMETER_TYPE_BOOL, true);

        SetGhost(false);
    }
    
    //-----------------------------------------------------------------------------------
    CCheckpointOriginEntity::~CCheckpointOriginEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        InitStateMachine();

        SetTextPosition(Vec2(0.0, 32.0));
        SetRenderGame(false);
        SetFireOnce(true);
        SetGhost(false);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        InitStateMachine();

        m_bDisabled = m_bDisabledInitial;

        SetGhost(m_bDisabled);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::InitStateMachine()
    {
        m_stateMachineBars.SetLooped(false);
        m_stateMachineBars.RegisterStateUpdateCallback(0, StateMachineOrigin::StateUpdateSetupInfo(CHECKPOINT_TIME_FADEIN, this, &CCheckpointOriginEntity::FadeInUpdate));
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::OnTriggerEntered(CEntity& e)
    {
        if (!gEngine->GetWorldMgr()->GetGameMode())
            return;

//        m_fade = FadeBarsRenderObject();

        m_bCheckpointFired = true;
    }
    
    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }
    
    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (m_bCheckpointFired)
        {
            m_stateMachineBars.Update(StateMachineOrigin::FrameParamsType(dt, queue));
        }
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::FadeInUpdate(const StateMachineOrigin::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = stateInfo.GetExpInterpolationInverse(3.0f);

//         m_fade.fadeBarsType = FADE_BARS_FROM_RIGHT_TO_LEFT;
//         m_fade.fadeBarsAmount = alpha;
//         m_fade.fadeFrameAmount = Vec4(CHECKPOINT_BARS_LEFT, 
//                                       CHECKPOINT_BARS_RIGHT, 
//                                       CHECKPOINT_BARS_TOP, 
//                                       CHECKPOINT_BARS_BOTTOM) * alpha;

//        frameParams.m_renderQueue.m_fadeScreenRenderObject.m_fadeScreenAmount = alpha;
        frameParams.m_renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;
//        frameParams.m_renderQueue.m_pFadeBarsRenderObject = &m_fade;

        if (Equals(alpha, 0.0f))
            m_bCheckpointFired = false;
    }
    
    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_pCharacter = static_cast<CCharacter3DEntity*>(gEngine->GetEntityMgr()->GetCharacter());

        CCamera* pCamera = gEngine->GetWorldMgr()->GetActiveCamera();
        pCamera->GetAttachedNode()->SetOrientation(CQuaternion());

        m_stateMachineBars.Reset();

        m_pNode->SetScale(Vec3(1.0f, 1.0f, 1.0f));

        gEngine->GetWorldMgr()->GetScenario()->SetSepia(0.0f);

        m_bCheckpointFired = false;

        if (!bGameMode)
        {
            m_bDisabled = m_bDisabledInitial;
        }
        else
        {
            if (!m_bDisabled)
                m_pCharacter->SetPosition(m_pNode->GetDerivedPosition(), true);
        }

        SetGhost(m_bDisabled);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointOriginEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case CHECKPOINT_EVENT_SET_DISABLED:

            if (eventParameter.IsParameterSet("SetDisabled"))
                eventParameter.GetParameter("SetDisabled", m_bDisabled);

            SetGhost(m_bDisabled);
            break;

        default:
            CSpriteEntity::OnInput(eventId, eventParameter);
            break;
        }
    }







    const FontInfo  CCheckpointDestinatonEntity::fontMenue = FontInfo("FontMenue");


    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CCheckpointDestinatonEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_CHECKPOINT_DESTINATION;

    //-----------------------------------------------------------------------------------
    CCheckpointDestinatonEntity::CCheckpointDestinatonEntity()
        : m_pLogoBackground(nullptr)
        , m_pCharacter(nullptr)
        , m_pPlayerCamera(nullptr)
        , m_pLevelNamesMtrl(nullptr)
        , m_pLevelUnlocked(nullptr)
        , m_pBossUnlocked(nullptr)
        , m_bCheckpointFired(false)
        , m_bFinishChapter(false)
        , m_startRotationRoll(0.0f)
        , m_targetRotationRoll(0.0f)
        , m_rotationSpeed(0.0f)
    {
        SetMetaTypeID(CCheckpointDestinatonEntity::ENTITY_META_TYPEID);

        RegisterEvent("Checkpoint: Change Level", CHECKPOINT_EVENT_CHANGELEVEL);

        SetGhost(false);

        gFontMgr->AddGetPtr(fontMenue.GetKey());

        m_pLevelFinishedSound = gEngine->GetAudioSystem()->AddGetPtr("LevelTransition.wav");
        m_pLevelFinishedSound->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);
        m_pLevelFinishedSound->SetVolume(1.5f);
    }

    //-----------------------------------------------------------------------------------
    CCheckpointDestinatonEntity::~CCheckpointDestinatonEntity()
    {
        gEngine->GetAudioSystem()->Release("MenueSelection.wav");

        gMaterialMgr->Release(m_pBossUnlocked);
        gMaterialMgr->Release(m_pLevelUnlocked);
        gMaterialMgr->Release(m_pLevelNamesMtrl);

        gFontMgr->Release(fontMenue.GetKey());
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        InitStateMachine();

        InitSprites();

        SetTextPosition(Vec2(0.0, 32.0));
        SetMaterialFilename("Sprites/checkpointLogo.png");
        SetFireOnce(true);
        SetGhost(false);


//        m_pLogoBackground = static_cast<CSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SPRITE));
//        m_pLogoBackground->SetMaterialFilename("Sprites/checkpointLogoBackground.png");
//        m_pLogoBackground->SetAutoDelete(false);
//        m_pLogoBackground->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 150.0f));
//        m_pNode->AttachChildNode(m_pLogoBackground->GetNode());
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        InitStateMachine();

        InitSprites();

        SetGhost(false);
    }


    void CCheckpointDestinatonEntity::InitSprites()
    {
        std::string levelMtrl = "levelSelection.mtrl";
        std::string chapterFolder;

        CSingletonPlayerMgr playerMgr;
        chapterFolder = "GUI/" + playerMgr->GetChapterFolder();

        levelMtrl = chapterFolder + levelMtrl;

        m_pLevelNamesMtrl = gMaterialMgr->AddGetPtr(levelMtrl);
        m_pBossUnlocked = gMaterialMgr->AddGetPtr("Sprites/checkpointBossUnlocked.png");
        m_pLevelUnlocked = gMaterialMgr->AddGetPtr("Sprites/checkpointLevelUnlocked.png");

        m_spriteComplete.SetScreenSpace(true);
        m_spriteComplete.SetSpritePipeline(SPRITE_PIPELINE_AFTER_FADE);
        m_spriteComplete.SetMaterialFilename("Sprites/checkpointComplete.png");
        m_spriteComplete.AttachNode(&m_nodeComplete);

        m_nodeComplete.SetPosition(Vec3(0.0f, 1080.0f  * 0.5f * 0.0f, 0.0f));
        m_nodeComplete.SetScale(Vec3(1.0f, 1.0f, 1.0f));

        m_spriteLevelUnlocked.SetScreenSpace(true);
        m_spriteLevelUnlocked.SetSpritePipeline(SPRITE_PIPELINE_AFTER_FADE);
        m_spriteLevelUnlocked.AttachNode(&m_nodeLevelUnlocked);

        m_nodeLevelUnlocked.SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_nodeLevelUnlocked.SetScale(Vec3(1.0f, 1.0f, 1.0f));

        m_spriteLevelUnlockedName.SetScreenSpace(true);
        m_spriteLevelUnlockedName.SetSpritePipeline(SPRITE_PIPELINE_AFTER_FADE);
        m_spriteLevelUnlockedName.AttachNode(&m_nodeLevelUnlockedName);

        m_nodeLevelUnlockedName.SetPosition(Vec3(0.03f, 0.15f, 0.0f));
        m_nodeLevelUnlockedName.SetScale(Vec3(1.0f, 1.0f, 1.0f));
        

    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::InitStateMachine()
    {
        m_stateMachineBars.RegisterStateUpdateCallback(0, StateMachine::StateUpdateSetupInfo(0.4f, this, &CCheckpointDestinatonEntity::FadeBordersAndPositionCamUpdate));

        m_stateMachineBars.RegisterStateEnterCallback(1, StateMachine::StateEnterLeaveSetupInfo(this, &CCheckpointDestinatonEntity::FadeCharacterAndLogoEnter));
        m_stateMachineBars.RegisterStateUpdateCallback(1, StateMachine::StateUpdateSetupInfo(2.0f, this, &CCheckpointDestinatonEntity::FadeCharacterAndLogoUpdate));
        
        m_stateMachineBars.RegisterStateUpdateCallback(2, StateMachine::StateUpdateSetupInfo(0.01f, this, &CCheckpointDestinatonEntity::FadeBarsAndScreenUpdate));
        
        m_stateMachineBars.RegisterStateEnterCallback(3, StateMachine::StateEnterLeaveSetupInfo(this, &CCheckpointDestinatonEntity::LevelCompleteEnter));
        m_stateMachineBars.RegisterStateUpdateCallback(3, StateMachine::StateUpdateSetupInfo(2.0f, this, &CCheckpointDestinatonEntity::LevelCompleteUpdate));

        m_stateMachineBars.RegisterStateEnterCallback(4, StateMachine::StateEnterLeaveSetupInfo(this, &CCheckpointDestinatonEntity::LevelUnlockedEnter));
        m_stateMachineBars.RegisterStateUpdateCallback(4, StateMachine::StateUpdateSetupInfo(2.3f, this, &CCheckpointDestinatonEntity::LevelUnlockedUpdate));

        m_stateMachineBars.RegisterStateLeaveCallback(5, StateMachine::StateEnterLeaveSetupInfo(this, &CCheckpointDestinatonEntity::FadeBarsAndScreenLeave));
        
        m_stateMachineBars.SetNextStateID(0);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::OnTriggerEntered(CEntity& e)
    {
        if (!gEngine->GetWorldMgr()->GetGameMode())
            return;

//        m_fade = FadeBarsRenderObject();
        m_pCharacter->OnLevelFinished();

        CCamera* pCam  = gEngine->GetWorldMgr()->GetActiveCamera();
        m_camStartPos  = pCam->GetAttachedNode()->GetDerivedPosition();
        m_camTargetPos = m_pNode->GetDerivedPosition();
        m_camTargetPos.z = m_camStartPos.z;

        m_bCheckpointFired = true;

        m_charPos = m_pCharacter->GetNode()->GetDerivedPosition();

        CSingletonPlayerMgr playerMgr;
        playerMgr->SetLevelFinished(m_unlockedLevels);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

        if (m_stateMachineBars.GetStateID() < STATE_FADE_CHARACTER)
        {
            CQuaternion orientation = m_pNode->GetOrientation();
            orientation.AddRoll(dt * CHECKPOINT_ROTATION_SPEED);
            m_pNode->SetOrientation(orientation);
        }
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        if (m_bCheckpointFired)
        {
            m_stateMachineBars.Update(StateMachine::FrameParamsType(dt, queue));
        }

        CSpriteEntity::AddToRenderQueue(dt, queue);

//         queue.m_spriteRenderObjects.push_back(m_spriteLevelFinished.GetRenderObject());
//         queue.m_spriteRenderObjects.push_back(m_spriteComplete.GetRenderObject());
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::FadeBordersAndPositionCamUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float alpha = stateInfo.GetExpInterpolation(3.0f);

        frameParams.m_renderQueue.m_postProcessRenderObject.m_posterizationAmount = alpha;

        if (m_pPlayerCamera)
        {
            Vec3 currentCamPos = alpha * m_camTargetPos + (1.0f - alpha) * m_camStartPos;
            m_pPlayerCamera->GetNode()->SetPosition(currentCamPos);
            m_pPlayerCamera->SetRespectLevelBoundaries(false);
        }
        
        Vec3 pos = Lerp(m_charPos, m_pNode->GetDerivedPosition(), 1.0f - stateInfo.GetExpInterpolationInverse(1.5f));
        m_pCharacter->SetPosition(pos);

        gEngine->GetWorldMgr()->SetTimeMultiplier(0.25f);

        m_stateMachineBars.SetNextStateID(1);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::FadeBordersAndPositionCamLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        frameParams.m_renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;

        m_stateMachineBars.SetNextStateID(1);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::FadeCharacterAndLogoEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        // x³ integrated over [0, 1]
        const float integratedCubicAlphaParam = 0.25f;

        // angular speed in rad / sec
        const float angularSpeed = CHECKPOINT_ROTATION_SPEED;

        float currentRoll = m_pNode->GetOrientation().GetRoll();

        if (currentRoll < 0.0f)
        {
            currentRoll = 2 * PI + currentRoll;
        }

        float integratedAngularSpeed = angularSpeed * integratedCubicAlphaParam;
        float totalRoll = integratedAngularSpeed * stateInfo.GetTotalTime();
        float totalRollAligned = 2.0f * PI / 6.0f;

        // Start + target roll without alignment
        m_startRotationRoll = currentRoll;
        m_targetRotationRoll = m_startRotationRoll + totalRoll;

        // Align target roll to the one watcher hexagon logo
        m_targetRotationRoll = totalRollAligned * (int)(m_targetRotationRoll / totalRollAligned);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::FadeCharacterAndLogoUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        gEngine->GetWorldMgr()->SetTimeMultiplier(1.0f);


        CQuaternion orientation = m_pNode->GetOrientation();

        frameParams.m_renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;

        orientation.SetRoll(Lerp(m_targetRotationRoll, m_startRotationRoll, stateInfo.GetExpInterpolationInverse(3.0f)));

        m_pNode->SetOrientation(orientation);

        if (stateInfo.GetElapsedTime() > 1.5f && !m_pLevelFinishedSound->IsPlaying())
            m_pLevelFinishedSound->PlaySound();

        m_stateMachineBars.SetNextStateID(2);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::FadeBarsAndScreenUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        CCamera* pCam = gEngine->GetWorldMgr()->GetActiveCamera();
        pCam->GetAttachedNode()->SetPosition(m_camTargetPos);

        frameParams.m_renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;

        CQuaternion orientation = m_pNode->GetOrientation();
        orientation.SetRoll(m_targetRotationRoll);
        m_pNode->SetOrientation(orientation);

        m_stateMachineBars.SetNextStateID(3);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::FadeBarsAndScreenLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        frameParams.m_renderQueue.m_postProcessRenderObject.m_posterizationAmount = 1.0f;

        m_stateMachineBars.SetIdle(true);

        LoadLevel();
    }


    void CCheckpointDestinatonEntity::LevelCompleteEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::LevelCompleteUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float fadeOutThreshold = stateInfo.GetTotalTime() - 0.2f;

        if (stateInfo.GetElapsedTime() < 0.2f)
        {
            // Fade in sprite
            float alpha = Clamp(stateInfo.GetElapsedTime() / 0.2f, 0.0f, 1.0f);

            m_nodeComplete.SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.1f, 1.1f, 1.1f), alpha));

            m_spriteComplete.SetColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
        else if (stateInfo.GetElapsedTime() > fadeOutThreshold)
        {
            // Fade out sprite
            float alpha = 1.0f - Clamp((stateInfo.GetElapsedTime() - fadeOutThreshold) / 0.2f, 0.0f, 1.0f);

            m_nodeComplete.SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.2f, 1.2f, 1.2f), alpha));
            m_spriteComplete.SetColor(Vec3(1.0f, 1.0f, 1.0f) * alpha);
       }
        else
        {
            // Show sprite
            float alpha = (stateInfo.GetElapsedTime() - 0.2f) / (stateInfo.GetTotalTime() - 0.4f);

            m_nodeComplete.SetScale(Lerp(Vec3(1.1f, 1.1f, 1.1f), Vec3(1.2f, 1.2f, 1.2f), alpha));
            m_spriteComplete.SetColor(Vec3(1.0f, 1.0f, 1.0f) * alpha);
        }

        frameParams.m_renderQueue.m_spriteRenderObjects.push_back(m_spriteComplete.GetRenderObject());

        if (m_unlockedLevels.empty())
            m_stateMachineBars.SetNextStateID(5);
        else
            m_stateMachineBars.SetNextStateID(4);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::LevelUnlockedEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        int index = m_unlockedLevels.front();

        int numTextureColumns = m_pLevelNamesMtrl->GetWidth() / m_pLevelNamesMtrl->GetFrameWidth();
        int columnIndex = index % numTextureColumns;
        int rowIndex = index / numTextureColumns;

        m_spriteLevelUnlockedName.SetMaterialFilename(m_pLevelNamesMtrl->GetName(), true);
        m_spriteLevelUnlockedName.GetMaterial()->SetFrame(columnIndex);
        m_spriteLevelUnlockedName.GetMaterial()->SetSequence(rowIndex);


        CSingletonPlayerMgr playerMgr;

        if (playerMgr->GetLevelinfo(playerMgr->GetCurrentChapterIndex(), index).bBoss)
        {
            m_spriteLevelUnlocked.SetMaterialFilename(m_pBossUnlocked->GetName());
        }
        else
        {
            m_spriteLevelUnlocked.SetMaterialFilename(m_pLevelUnlocked->GetName());
        }

        m_unlockedLevels.pop();

        if (!m_pLevelFinishedSound->IsPlaying())
            m_pLevelFinishedSound->PlaySound();

        m_stateMachineBars.ResetCurrentState();
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::LevelUnlockedUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        if (stateInfo.GetElapsedTime() < 0.3f)
        {
            m_stateMachineBars.SetNextStateID(4);
            return;
        }

        float elapsedTime = stateInfo.GetElapsedTime();
        float totalTime = stateInfo.GetTotalTime();

        elapsedTime -= 0.3f;
        totalTime -= 0.3f;

        float fadeOutThreshold = totalTime - 0.2f;

        if (elapsedTime < 0.2f)
        {
            // Fade in sprite
            float alpha = Clamp(elapsedTime / 0.2f, 0.0f, 1.0f);
            alpha = 1.0f - alpha;
            alpha *= alpha;
            alpha = 1.0f - alpha;

            m_nodeLevelUnlocked.SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.1f, 1.1f, 1.1f), alpha));
            m_nodeLevelUnlockedName.SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.1f, 1.1f, 1.1f), alpha));

            m_spriteLevelUnlocked.SetColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
            m_spriteLevelUnlockedName.SetColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
        else if (elapsedTime > fadeOutThreshold)
        {
            // Fade out sprite
            float alpha = 1.0f - Clamp((elapsedTime - fadeOutThreshold) / 0.2f, 0.0f, 1.0f);
            alpha *= alpha;

            m_nodeLevelUnlocked.SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.2f, 1.2f, 1.2f), alpha));
            m_nodeLevelUnlockedName.SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.2f, 1.2f, 1.2f), alpha));
            m_spriteLevelUnlocked.SetColor(Vec3(1.0f, 1.0f, 1.0f) * alpha);
            m_spriteLevelUnlockedName.SetColor(Vec3(1.0f, 1.0f, 1.0f) * alpha);
        }
        else
        {
            // Show sprite
            float alpha = (elapsedTime - 0.2f) / (totalTime - 0.4f);


            m_nodeLevelUnlocked.SetScale(Lerp(Vec3(1.1f, 1.1f, 1.1f), Vec3(1.2f, 1.2f, 1.2f), alpha));
            m_nodeLevelUnlockedName.SetScale(Lerp(Vec3(1.1f, 1.1f, 1.1f), Vec3(1.2f, 1.2f, 1.2f), alpha));
            m_spriteLevelUnlocked.SetColor(Vec3(1.0f, 1.0f, 1.0f) * alpha);
            m_spriteLevelUnlockedName.SetColor(Vec3(1.0f, 1.0f, 1.0f) * alpha);
        }

        frameParams.m_renderQueue.m_spriteRenderObjects.push_back(m_spriteLevelUnlocked.GetRenderObject());
        frameParams.m_renderQueue.m_spriteRenderObjects.push_back(m_spriteLevelUnlockedName.GetRenderObject());

        if (m_unlockedLevels.empty())
            m_stateMachineBars.SetNextStateID(5);
        else
            m_stateMachineBars.SetNextStateID(4);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::LoadLevel()
    {
        CSingletonPlayerMgr playerMgr;

        if (m_strNextLevel.empty())
            playerMgr->LoadLevel("");
        else
            playerMgr->LoadLevel("../Resources/Levels/" + m_strNextLevel);
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_pCharacter = static_cast<CCharacter3DEntity*>(gEngine->GetEntityMgr()->GetCharacter());

        m_stateMachineBars.Reset();

        m_pNode->SetScale(Vec3(0.8f, 0.8f, 0.8f));
        m_pPlayerCamera = static_cast<CPlayerCameraEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_PLAYERCAMERA));

        if (m_pPlayerCamera)
            m_pPlayerCamera->GetNode()->SetOrientation(CQuaternion());

        gEngine->GetWorldMgr()->GetScenario()->SetSepia(0.0f);

        m_bCheckpointFired = false;
        m_startRotationRoll = 0.0f;
        m_targetRotationRoll = 0.0f;
        m_rotationSpeed = 0.0f;
    }

    //-----------------------------------------------------------------------------------
    void CCheckpointDestinatonEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        if (eventId == CHECKPOINT_EVENT_CHANGELEVEL && !gEngine->GetEditor())
        {
            CSingletonPlayerMgr playerMgr;
            playerMgr->SetLevelFinished(m_unlockedLevels);

            LoadLevel();
        }
        else
            CSpriteEntity::OnFired(eventId);
    }

} // env
