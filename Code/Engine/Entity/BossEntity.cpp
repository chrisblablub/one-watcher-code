#include <Engine/Entity/BossEntity.h>
#include <Engine/Entity/ProjectileEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CBossEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_BOSS;

    //-----------------------------------------------------------------------------------
    CBossEntity::CBossEntity()
        : m_FireDelay(3.0f)
        , m_ReloadTime(0.0f)
        , m_InitialExplosionTimer(0.5f)
        , m_ExplosionTimer(m_InitialExplosionTimer)
        , m_bGunLeftEnabled(false)
        , m_bGunRightEnabled(false)
        , m_bFinalPhase(false)
        , m_gunRightEnableTime(-1.0f)
        , m_gunLeftEnableTime(-1.0f)
        , m_gunRightEnableDuration(1.0f)
        , m_gunLeftEnableDuration(1.0f)
        , m_wakeTime(-1.0f)
        , m_wakeDuration(1.0f)
        , m_numProjectiles(0)
        , m_laserDuration(4.0f)
        , m_laserTime(-1.0f)
        , m_IsActivated(false)
        , m_bIsAwake(false)
        , m_pEye(nullptr)
        , m_pInnerEye(nullptr)
        , m_pBody(nullptr)
        , m_pGunRight(nullptr)
        , m_pGunLeft(nullptr)
        , m_pLaserEntity(nullptr)
        , m_pInnerEyeParticles(nullptr)
        , m_state(BOSS_STATE_NORMAL)
        , m_SoundWasPlayed(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Boss] Activate", BOSS_EVENT_ACTIVATE);
        RegisterEvent("[Boss] Wake", BOSS_EVENT_WAKE);
        RegisterEventParameter(BOSS_EVENT_WAKE, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100.0f);

        RegisterEvent("[Boss] SetDead", BOSS_EVENT_SET_DEAD);

        RegisterEvent("[Boss] EnableGunleft", BOSS_EVENT_ENABLE_GUN_LEFT);
        RegisterEventParameter(BOSS_EVENT_ENABLE_GUN_LEFT, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100.0f);

        RegisterEvent("[Boss] EnableGunRight", BOSS_EVENT_ENABLE_GUN_RIGHT);
        RegisterEventParameter(BOSS_EVENT_ENABLE_GUN_RIGHT, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100.0f);

        RegisterEvent("[Boss] SetFinalPhase", BOSS_EVENT_ENABLE_FINAL_PHASE);
        RegisterEventParameter(BOSS_EVENT_ENABLE_FINAL_PHASE, "Enabled", CBaseParam::PARAMETER_TYPE_BOOL, true);
        RegisterEventParameter(BOSS_EVENT_ENABLE_FINAL_PHASE, "Laser Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100.0f);
        RegisterEventParameter(BOSS_EVENT_ENABLE_FINAL_PHASE, "Num Projectiles", CBaseParam::PARAMETER_TYPE_INT, "", (int)0, (int)0, (int)100);

    }

    //-----------------------------------------------------------------------------------
    CBossEntity::~CBossEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pInnerEyeParticles);
        gEngine->GetEntityMgr()->RemoveEntity(m_pLaserEntity);
        gEngine->GetEntityMgr()->RemoveEntity(m_pGunLeft);
        gEngine->GetEntityMgr()->RemoveEntity(m_pGunRight);
        gEngine->GetEntityMgr()->RemoveEntity(m_pBody);
        gEngine->GetEntityMgr()->RemoveEntity(m_pInnerEye);
        gEngine->GetEntityMgr()->RemoveEntity(m_pEye);

        RemoveExplosionEffects();
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetupMeshes();

        InitStateMachine();
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        SetupMeshes();

        InitStateMachine();
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::InitStateMachine()
    {
        m_stateMachine.RegisterStateUpdateCallback(BOSS_STATE_NORMAL, StateMachine::StateUpdateSetupInfo(0.0f, this, &CBossEntity::NormalUpdate));
        m_stateMachine.RegisterStateEnterCallback(BOSS_STATE_HIT, StateMachine::StateEnterLeaveSetupInfo(this, &CBossEntity::HitEnter));
        m_stateMachine.RegisterStateUpdateCallback(BOSS_STATE_HIT, StateMachine::StateUpdateSetupInfo(3.0f, this, &CBossEntity::HitUpdate));
        m_stateMachine.RegisterStateUpdateCallback(BOSS_STATE_CHARACTER_JUMP, StateMachine::StateUpdateSetupInfo(5.0f, this, &CBossEntity::CharacterJumpUpdate));
        m_stateMachine.RegisterStateEnterCallback(BOSS_STATE_CHARACTER_HIT, StateMachine::StateEnterLeaveSetupInfo(this, &CBossEntity::CharacterHitEnter));
        m_stateMachine.RegisterStateUpdateCallback(BOSS_STATE_CHARACTER_HIT, StateMachine::StateUpdateSetupInfo(2.0f, this, &CBossEntity::CharacterHitUpdate));
        m_stateMachine.RegisterStateEnterCallback(BOSS_STATE_DYING, StateMachine::StateEnterLeaveSetupInfo(this, &CBossEntity::DyingEnter));
        m_stateMachine.RegisterStateUpdateCallback(BOSS_STATE_DYING, StateMachine::StateUpdateSetupInfo(5.0f, this, &CBossEntity::DyingUpdate));

        m_stateMachine.SetNextStateID(BOSS_STATE_NORMAL);
    }


    void CBossEntity::HitEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        m_pSound->PlaySound();

        m_pLaserEntity->SetDisabled(true);

        RemoveExplosionEffects();
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::HitUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        m_pLaserEntity->SetDisabled(true);

        if (m_Animations.size() == 0)
        {
            AddHitAnimation();
        }

        AddExplosionEffects(frameParams.m_dt, 3);

        m_stateMachine.SetNextStateID(BOSS_STATE_NORMAL);
    }


    //-----------------------------------------------------------------------------------
    void CBossEntity::NormalUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        if (m_bFinalPhase)
        {
            // Final phase means the watcher takes control of the laser and guns to enable them alternatingly
            m_pLaserEntity->SetDisabled(false);

            //             if (m_laserTime >= 0.0f && 
            //                 m_pGunLeft->GetProjectileRemainingLifetime() <= 0.0f &&
            //                 m_pGunLeft->GetNumProjectilesToFire() == 0 &&
            //                 m_pGunRight->GetProjectileRemainingLifetime() <= 0.0f &&
            //                 m_pGunRight->GetNumProjectilesToFire() == 0)
            //             {
            //                 m_laserTime -= frameParams.m_dt;
            // 
            //                 //m_pLaserEntity->SetDisabled(false);
            // 
            //                 if (m_laserTime < 0.0f)
            //                 {
            //                     //m_pLaserEntity->SetDisabled(true);
            // 
            //                     m_pGunLeft->FireProjectiles(m_numProjectiles);
            //                     m_pGunRight->FireProjectiles(m_numProjectiles);
            // 
            //                     m_laserTime = m_laserDuration;
            //                 }
            //             }
            //             else
            //             {
            //                 //m_pLaserEntity->SetDisabled(true);
            //             }
        }


        Vec3 targetDirection = GetNode()->GetDerivedPosition();

        float lookAtSpeed = 0.5f;


        if (m_pLaserEntity->GetLaserActive())
        {
            targetDirection -= m_pLaserEntity->GetLaserTargetPos();
            lookAtSpeed = 1.5f;
        }
        else
        {
            targetDirection -= m_pCharacter->GetNode()->GetDerivedPosition();

            lookAtSpeed = 0.5f;
        }

        CSmoothRotation::ApplyRotation(*GetNode(), glm::normalize(targetDirection), lookAtSpeed, frameParams.m_dt);

        if (!m_bIsAwake)
            m_pEye->GetNode()->SetOrientation(CQuaternion(0.0f, PI * 0.5f, 0.0f));

        if (!m_IsActivated)
            return;

        if (m_bIsAwake)
        {
            if (m_wakeTime >= 0.0f)
            {
                float alpha = Clamp(m_wakeTime / m_wakeDuration, 0.0f, 1.0f);
                float pitch = Lerp(0.0f, PI * 0.5f, alpha * alpha);

                m_pEye->GetNode()->SetOrientation(CQuaternion(0.0f, pitch, 0.0f));

                m_wakeTime -= frameParams.m_dt;
            }
            else
            {
                m_pEye->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::CharacterJumpUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        m_stateMachine.SetNextStateID(BOSS_STATE_CHARACTER_HIT);

        float lookAtSpeed = 0.5f;

        Vec3 bossPosition = GetNode()->GetDerivedPosition();
        Vec3 charPosition = m_pCharacter->GetNode()->GetDerivedPosition();
        Vec3 targetDirection = bossPosition - charPosition;

        if (!Equals(targetDirection.x, 0.0f) ||
            !Equals(targetDirection.y, 0.0f) ||
            !Equals(targetDirection.z, 0.0f))
        {
            CSmoothRotation::ApplyRotation(*GetNode(), glm::normalize(targetDirection), lookAtSpeed, frameParams.m_dt);
        }
    }

    void CBossEntity::CharacterHitEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        m_pLaserEntity->SetDisabled(true);

        //         Vec3 currentPosition = GetNode()->GetPosition();
        //         Vec3 translation1 = currentPosition + Vec3(0.0f, 0.0f, 0.0f);
        //         Vec3 translation2 = currentPosition + Vec3(0.0f, GetRandomFloat(-25.0f, 25.0f), 0.0f);
        //         Vec3 translation3 = currentPosition + Vec3(GetRandomFloat(-25.0f, 25.0f), 0.0f, GetRandomFloat(-25.0f, 25.0f));
        //         Vec3 translation4 = currentPosition;
        //         Vec3 scale = GetNode()->GetScale();
        // 
        //         float duration = 0.2f;
        //         CQuaternion rotation0 = GetNode()->GetOrientation();
        //         Vec3 rotation1(-0.7f, 0.0f, 1.0f);
        //         Vec3 rotation2(0.6f, 0.0f, 1.0f);
        //         Vec3 rotation3(0.0f, 0.5f, 1.0f);
        // 
        //         m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation1, scale, scale, rotation0, rotation1, 0.25f));
        //         m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation1, scale, scale, rotation1, rotation1, 0.05f));
        //         m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation1, scale, scale, rotation1, rotation2, 0.4f));
        //         m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation1, scale, scale, rotation2, rotation2, 0.05f));
        //         m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation1, scale, scale, rotation2, rotation3, 0.35f));
        //         m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation1, scale, scale, rotation3, rotation0, 0.1f));

        for (const auto& it : m_pInnerEyeParticles->GetEmitters())
        {
            m_innerEyeParticlesColor = it.second->GetColorEnd();
            break;
        }
    }


    //-----------------------------------------------------------------------------------
    void CBossEntity::CharacterHitUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        float interp = stateInfo.GetLinearInterpolation();

        Vec4 innerEyeParticlesColorEnd = Vec4(0.0f, 0.75f, 1.0f, 1.3f);
        Vec4 innerEyeParticlesColor = Lerp(m_innerEyeParticlesColor, innerEyeParticlesColorEnd, interp);

        for (const auto& it : m_pInnerEyeParticles->GetEmitters())
        {
            it.second->SetColor(innerEyeParticlesColor);
        }


        if (m_Animations.size() == 0)
        {
            AddHitAnimation();
        }

        m_stateMachine.SetNextStateID(BOSS_STATE_DYING);
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::DyingEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo)
    {
        if (m_pSound)
            m_pSound->PlaySound();

        m_pLaserEntity->SetDisabled(true);
        m_pGunLeft->SetDisabled(true);
        m_pGunRight->SetDisabled(true);

        RemoveExplosionEffects();
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::DyingUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo)
    {
        if (m_Animations.size() == 0)
        {
            AddHitAnimation();
        }

        AddExplosionEffects(frameParams.m_dt, 10);
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->SetVolume(0.75f);
        m_pSound->SetPeakVolumeDistance(0.0f);
        m_pSound->SetAudibleDistance(0.0f);
        m_pSound->LoadSound("Entities/Watcher.wav");
        m_pSound->SetIsAutoplay(false);
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

        CBossEntity& other = static_cast<CBossEntity&>(entity);
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::Update(float dt)
    {
        UpdateAnimations(dt);

        Vec3 gunLeftStartPos = GetNode()->GetDerivedPosition() +
            GetNode()->GetDerivedDirection() * GetNode()->GetDerivedScale() * 380.0f +
            GetNode()->GetDerivedRight() * GetNode()->GetDerivedScale() * 470.0f;

        Vec3 gunLeftEndPos = GetNode()->GetDerivedPosition() +
            GetNode()->GetDerivedDirection() * GetNode()->GetDerivedScale() * 380.0f +
            GetNode()->GetDerivedRight() * GetNode()->GetDerivedScale() * 645.0f;

        Vec3 gunRightStartPos = GetNode()->GetDerivedPosition() +
            GetNode()->GetDerivedDirection() * GetNode()->GetDerivedScale() * 380.0f -
            GetNode()->GetDerivedRight() * GetNode()->GetDerivedScale() * 470.0f;

        Vec3 gunRightEndPos = GetNode()->GetDerivedPosition() +
            GetNode()->GetDerivedDirection() * GetNode()->GetDerivedScale() * 380.0f -
            GetNode()->GetDerivedRight() * GetNode()->GetDerivedScale() * 645.0f;

        m_pGunLeft->GetNode()->SetScale(GetNode()->GetDerivedScale() * Vec3(1.5f, 1.5f, 1.5f));
        m_pGunLeft->GetNode()->SetPosition(gunLeftEndPos);

        if (m_pGunLeft->GetDisabled())
        {
            CQuaternion orientation = m_pNode->GetDerivedOrientation();
            orientation.AddYaw(-PI * 0.5f);

            m_pGunLeft->GetNode()->SetOrientation(orientation);

            Vec3 gunPos = gunLeftStartPos;

            if (m_gunLeftEnableTime >= 0.0f)
            {
                float alpha = Clamp(m_gunLeftEnableTime / m_gunLeftEnableDuration, 0.0f, 1.0f);
                alpha *= alpha;

                gunPos = Lerp(gunLeftEndPos, gunLeftStartPos, alpha);

                m_gunLeftEnableTime -= dt;

                if (m_gunLeftEnableTime < 0.0f)
                    m_pGunLeft->SetDisabled(false);
            }

            m_pGunLeft->GetNode()->SetPosition(gunPos);
        }


        m_pGunRight->GetNode()->SetScale(GetNode()->GetDerivedScale() * Vec3(1.5f, 1.5f, 1.5f));
        m_pGunRight->GetNode()->SetPosition(gunRightEndPos);

        if (m_pGunRight->GetDisabled())
        {
            CQuaternion orientation = m_pNode->GetDerivedOrientation();
            orientation.AddYaw(PI * 0.5f);

            m_pGunRight->GetNode()->SetOrientation(orientation);

            Vec3 gunPos = gunRightStartPos;

            if (m_gunRightEnableTime >= 0.0f)
            {
                float alpha = Clamp(m_gunRightEnableTime / m_gunRightEnableDuration, 0.0f, 1.0f);
                alpha *= alpha;

                gunPos = Lerp(gunRightEndPos, gunRightStartPos, alpha);

                m_gunRightEnableTime -= dt;

                if (m_gunRightEnableTime < 0.0f)
                    m_pGunRight->SetDisabled(false);
            }

            m_pGunRight->GetNode()->SetPosition(gunPos);
        }


        m_pLaserEntity->GetNode()->SetPosition(GetNode()->GetDerivedPosition());

        if (gEngine->GetWorldMgr()->GetGameMode() == false)
        {
            return;
        }

        m_stateMachine.Update(StateMachine::FrameParamsType(dt));
    }

    //-----------------------------------------------------------------------------------
    bool CBossEntity::IsDead()
    {
        return m_bIsDead;
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::UpdateAnimations(float dt)
    {
        CMeshEntity::Update(dt);

        if (m_pEye == nullptr || m_pInnerEye == nullptr)
        {
            return;
        }

        if (!m_Animations.empty() &&
            m_Animations.front().Update(dt))
        {
            m_Animations.pop();
        }
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::AddExplosionEffects(float dt, int maxNumberOfEffects)
    {
        m_ExplosionTimer = m_ExplosionTimer - dt;

        if (m_ExplosionTimer > 0 || m_ExplosionEffects.size() > maxNumberOfEffects)
        {
            return;
        }

        float scaleFactor = 3.0f;
        float shrinkFactor = 0.5f;
        const char* explosionTemplate = "EXPLOSION_T1";

        if (m_bIsDead)
        {
            scaleFactor = 7.0f;
            shrinkFactor = 1.0f;
            explosionTemplate = "EXPLOSION_T2";
        }

        m_ExplosionTimer = GetRandomFloat(0.15f, 0.35f);

        float randomX = GetRandomFloat(-700.0f * shrinkFactor, 700.0f * shrinkFactor);
        float randomY = GetRandomFloat(-600.0f * shrinkFactor, 600.0f * shrinkFactor);
        Vec3 randomVector(randomX, randomY, -600.0f);
        Vec3 explosionOrigin = GetNode()->GetDerivedPosition() + randomVector;

        CExplosionEntity* explosion = static_cast<CExplosionEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLOSION));
        explosion->LoadFromTemplate(explosionTemplate);
        explosion->GetNode()->SetPosition(explosionOrigin);
        explosion->GetNode()->SetScale(Vec3(scaleFactor, scaleFactor, scaleFactor));
        explosion->SetPickable(false);
        explosion->SetAutoDelete(false);
        explosion->SetEnabled(true);
        m_ExplosionEffects.push_back(explosion);
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::OnGameModeChanged(bool bGameMode)
    {
        m_bIsDead = false;
        m_bIsAwake = false;
        m_IsActivated = false;

        m_ExplosionTimer = m_InitialExplosionTimer;
        RemoveExplosionEffects();

        m_bGunLeftEnabled = false;
        m_bGunRightEnabled = false;

        m_gunRightEnableTime = -1.0f;
        m_gunLeftEnableTime = -1.0f;

        m_numProjectiles = 0;

        m_wakeTime = -1.0f;

        m_laserTime = -1.0f;
        m_laserDuration = 4.0f;

        m_pCharacter = gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_CHARACTER3D);

        m_stateMachine.Reset();
        m_stateMachine.SetNextStateID(BOSS_STATE_NORMAL);

        m_pInnerEyeParticles->LoadFromTemplate("WATCHER_EYE");

        if (bGameMode) {
            SetupWakeUpAnimation();
        }
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::RemoveExplosionEffects()
    {
        for (std::vector<CEntity*>::iterator it = m_ExplosionEffects.begin(); it != m_ExplosionEffects.end(); ++it)
        {
            gEngine->GetEntityMgr()->RemoveEntity(*it);
        }

        m_ExplosionEffects.clear();
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::SetupMeshes()
    {
        if (m_pEye == nullptr)
        {
            m_pEye = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEye->SetMeshFilename("watcherEye.x");
            m_pEye->SetAutoDelete(false);
            m_pEye->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEye->GetNode());
        }

        if (m_pInnerEye == nullptr)
        {
            m_pInnerEye = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pInnerEye->SetMeshFilename("watcher.x");
            m_pInnerEye->SetAutoDelete(false);
            m_pInnerEye->SetDeletable(false);
            m_pInnerEye->GetNode()->SetScale(Vec3(0.25f, 0.25f, 0.25f));
            m_pEye->GetNode()->AttachChildNode(m_pInnerEye->GetNode());
            m_pInnerEye->GetMeshInstanceContainer().GetMeshInstance(0)->GetMaterial().SetDiffuseColor(Vec3(1.0f, 0.0f, 0.0f));
        }

        if (m_pInnerEyeParticles == nullptr)
        {
            m_pInnerEyeParticles = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
            m_pInnerEyeParticles->LoadFromTemplate("WATCHER_EYE");
            m_pInnerEyeParticles->SetAutoDelete(false);
            m_pInnerEyeParticles->SetDeletable(false);
            m_pInnerEyeParticles->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -450.0f));
            m_pInnerEyeParticles->GetNode()->SetScale(Vec3(2.3f, 2.3f, 2.3f));
            m_pInnerEye->GetNode()->AttachChildNode(m_pInnerEyeParticles->GetNode());
        }

        if (m_pBody == nullptr)
        {
            m_pBody = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pBody->SetMeshFilename("watcher.x");
            m_pBody->SetAutoDelete(false);
            m_pBody->SetDeletable(false);
            m_pBody->GetNode()->SetScale(Vec3(0.9f, 0.9f, 0.9f));
            m_pBody->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 385.0f));
            m_pNode->AttachChildNode(m_pBody->GetNode());
        }

        if (m_pGunRight == nullptr)
        {
            m_pGunRight = static_cast<CProjectileGunEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PROJECTILE_GUN));
            m_pGunRight->SetAutoDelete(false);
            m_pGunRight->SetDeletable(false);
            m_pGunRight->GetNode()->SetScale(Vec3(2.35f, 2.35f, 2.35f));
            m_pGunRight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        }

        if (m_pGunLeft == nullptr)
        {
            m_pGunLeft = static_cast<CProjectileGunEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PROJECTILE_GUN));
            m_pGunLeft->SetAutoDelete(false);
            m_pGunLeft->SetDeletable(false);
            m_pGunLeft->GetNode()->SetScale(Vec3(2.35f, 2.35f, 2.35f));
            m_pGunLeft->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        }

        if (m_pLaserEntity == nullptr)
        {
            m_pLaserEntity = static_cast<CLaserRadiatorAimingEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LASERRADIATOR_AIMING));
            m_pLaserEntity->SetAutoDelete(false);
            m_pLaserEntity->SetDeletable(false);
            m_pLaserEntity->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        }
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::SetupWakeUpAnimation()
    {
        Vec3 translation(0.0f, 0.0f, 0.0f);
        Vec3 scale(1.1f, 1.1f, 1.1f);

        float duration = 1.0f;
        Vec3 rotation1(-1.0f, 0.0f, 1.0f);
        Vec3 rotation2(-0.1f, 0.0f, 1.0f);

        m_Animations = {};
        GetNode()->SetOrientation(CQuaternion(rotation1));

        m_Animations.push(CSceneNodeAnimation(m_pEye->GetNode(), translation, translation, scale, scale, rotation1, rotation2, duration));
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::AddHitAnimation()
    {
        Vec3 currentPosition = GetNode()->GetPosition();
        Vec3 translation1 = currentPosition + Vec3(0.0f, 0.0f, 0.0f);
        Vec3 translation2 = currentPosition + Vec3(0.0f, GetRandomFloat(-25.0f, 25.0f), 0.0f);
        Vec3 translation3 = currentPosition + Vec3(GetRandomFloat(-25.0f, 25.0f), 0.0f, GetRandomFloat(-25.0f, 25.0f));
        Vec3 scale = GetNode()->GetScale();

        float duration = GetRandomFloat(0.1f, 0.15f);
        CQuaternion rotation0 = GetNode()->GetOrientation();
        Vec3 rotation1(GetRandomFloat(-0.4f, 0.4f), 0.0f, 1.0f);
        Vec3 rotation2(GetRandomFloat(-0.4f, 0.4f), 0.0f, 1.0f);
        Vec3 rotation3(0.0f, GetRandomFloat(-0.4f, 0.4f), 1.0f);

        m_Animations.push(CSceneNodeAnimation(GetNode(), translation1, translation2, scale, scale, rotation0, rotation2, duration));
        m_Animations.push(CSceneNodeAnimation(GetNode(), translation2, translation3, scale, scale, rotation2, rotation3, duration));
        m_Animations.push(CSceneNodeAnimation(GetNode(), translation3, translation1, scale, scale, rotation3, rotation1, duration));
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::AddIdleAnimation()
    {
        Vec3 translation(0.0f, 0.0f, 0.0f);
        Vec3 scale(1.1f, 1.1f, 1.1f);

        float duration = 1.0f;
        Vec3 rotation1(-0.1f, 0.0f, 1.0f);
        Vec3 rotation2(0.1f, 0.0f, 1.0f);
        Vec3 rotation3(0.0f, -0.1f, 1.0f);

        m_Animations.push(CSceneNodeAnimation(m_pEye->GetNode(), translation, translation, scale, scale, rotation1, rotation2, duration));
        m_Animations.push(CSceneNodeAnimation(m_pEye->GetNode(), translation, translation, scale, scale, rotation2, rotation3, duration));
        m_Animations.push(CSceneNodeAnimation(m_pEye->GetNode(), translation, translation, scale, scale, rotation3, rotation1, duration));
    }

    //-----------------------------------------------------------------------------------
    void CBossEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case BOSS_EVENT_ENABLE_FINAL_PHASE:
        {
            if (eventParameter.IsParameterSet("Enabled"))
                eventParameter.GetParameter("Enabled", m_bFinalPhase);

            if (eventParameter.IsParameterSet("Laser Duration"))
                eventParameter.GetParameter("Laser Duration", m_laserDuration);

            if (eventParameter.IsParameterSet("Num Projectiles"))
                eventParameter.GetParameter("Num Projectiles", m_numProjectiles);

            m_laserTime = m_laserDuration;
        } break;

        case BOSS_EVENT_ACTIVATE:
            m_IsActivated = true;
            break;

        case BOSS_EVENT_WAKE:

            if (!m_bIsAwake)
            {
                if (eventParameter.IsParameterSet("Duration"))
                    eventParameter.GetParameter("Duration", m_wakeDuration);

                m_wakeTime = m_wakeDuration;

                if (m_SoundWasPlayed == false)
                {
                    m_SoundWasPlayed = true;
                    m_pSound->PlaySound();
                }
                m_bIsAwake = true;
            }

            break;

        case BOSS_EVENT_SET_DEAD:
            RemoveExplosionEffects();
            m_bIsDead = true;
            break;

        case BOSS_EVENT_ENABLE_GUN_LEFT:

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_gunLeftEnableDuration);

            m_gunLeftEnableTime = m_gunLeftEnableDuration;

            m_bGunLeftEnabled = true;

            break;

        case BOSS_EVENT_ENABLE_GUN_RIGHT:
            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_gunRightEnableDuration);

            m_gunRightEnableTime = m_gunRightEnableDuration;

            m_bGunRightEnabled = true;
            break;
        }
    }
} // env>