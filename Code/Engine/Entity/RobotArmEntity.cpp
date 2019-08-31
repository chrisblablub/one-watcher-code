#include <Engine/Entity/RobotArmEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/CameraEntity.h>
#include <Engine/Scene/Light.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CRobotArmEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ROBOTARM;

    CRobotArmEntity::CRobotArmEntity()
        : m_pLowerArm(nullptr)
        , m_pTarget(nullptr)
        , m_pCharacter(nullptr)
        , m_pPlayerCamera(nullptr)
        , m_aimPauseTime(0.5f)
        , m_hitTime(0.4f)
        , m_hitPauseTime(0.3f)
        , m_reverseHitTime(0.5f)
        , m_reverseHitPauseTime(0.6f)
        , m_delay(0.0f)
        , m_directionFactor(1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CRobotArmEntity::~CRobotArmEntity()
    {
        if (m_pLowerArm)
            gEngine->GetEntityMgr()->RemoveEntity(m_pLowerArm->GetName());

        if (m_pTarget)
            gEngine->GetEntityMgr()->RemoveEntity(m_pTarget->GetName());

        for (int i = 0; i < m_entities.size(); ++i)
            if (m_entities[i])
                gEngine->GetEntityMgr()->RemoveEntity(m_entities[i]->GetName());
    }


    void CRobotArmEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        m_lowerArmLength = 160.0f;
        m_upperArmLength = 160.0f;

        CMeshEntity::SetMeshFilename("robotUpperArmT1.x");

        CMeshEntity* pLowerArm = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
        pLowerArm->SetMeshFilename("robotLowerArmT1.x");
        m_pLowerArm = pLowerArm;

        m_pLowerArm->SetAutoDelete(false);
        m_pLowerArm->SetPickable(false);
        m_pLowerArm->GetNode()->SetPosition(Vec3(m_upperArmLength, 0.0f, 0.0f));

        CMeshEntity* pEntity01 = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SAWBLADE));
        pEntity01->SetAutoDelete(false);
        pEntity01->GetNode()->SetPosition(Vec3(m_lowerArmLength, 0.0f, 0.0f));
        pEntity01->GetNode()->SetScale(Vec3(0.8f, 0.8f, 0.8f));


        m_entities.resize(1);
        m_entities[0] = pEntity01;

        CSpriteEntity* pTarget = static_cast<CSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SPRITE));
        pTarget->SetRenderGame(false);
        m_pTarget = pTarget;
        m_pTarget->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + Vec3(100.0f, 0.0f, 0.0f));
        m_pTarget->SetAutoDelete(false);
        m_pTarget->GetNode()->AttachChildNode(m_pSound->GetNode());



        m_pNode->AttachChildNode(m_pLowerArm->GetNode());
        //m_pUpperArm->GetNode()->AttachChildNode(m_pLowerArm->GetNode());

        for (int i = 0; i < m_entities.size(); ++i)
            m_pLowerArm->GetNode()->AttachChildNode(m_entities[i]->GetNode());

        SetState(ROBOT_ARM_STATE_AIM);
    }

    void CRobotArmEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
        }

        m_pSound->SetIsAutoplay(false);
        m_pSound->SetPeakVolumeDistance(300.0f);
        m_pSound->SetAudibleDistance(900.0f);
        m_pSound->SetVolume(1.5f);
        m_pSound->LoadSound("Entities/RobotArm.wav");
    }

    void CRobotArmEntity::SetState(ROBOT_ARM_STATE state)
    {
        m_state = state;
        m_stateTimer.Reset();
    }

    float CRobotArmEntity::ComputeRootAngle()
    {
        m_attackPos = m_pTarget->GetNode()->GetDerivedPosition();

        Vec3 charPos(m_attackPos);

        Vec3 dir(charPos - m_pNode->GetDerivedPosition());

        dir = glm::normalize(dir);

        Vec2 vXAxis(1.0f, 0.0f);
        Vec2 vVelo(dir);
        vVelo = glm::normalize(vVelo);

        float rootRollAngle = acos(glm::dot(vVelo, vXAxis));
        if (vVelo.y < 0.0f)
            rootRollAngle = -rootRollAngle;

        return rootRollAngle;
    }

    float CRobotArmEntity::UpdateRootAngle(float rootAngle, float rotationSpeed)
    {
        CQuaternion rootOrientation = m_pNode->GetOrientation();

        return rootOrientation.GetRoll();
    }

    float CRobotArmEntity::ComputeUpperArmAngle()
    {
        float distance = glm::length(m_attackPos - m_pNode->GetDerivedPosition());

        float x1 = (m_upperArmLength * m_upperArmLength - m_lowerArmLength * m_lowerArmLength +
                    distance * distance) / (2 * distance);

        float finalUpperArmRollAngle = acos(x1 / m_upperArmLength);

        return finalUpperArmRollAngle;
    }

    float CRobotArmEntity::ComputeLowerArmAngle(float upperArmRollAngle)
    {
        m_attackPos = m_pTarget->GetNode()->GetDerivedPosition();
        float sumRootUpperArmAngles = upperArmRollAngle;
        Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), sumRootUpperArmAngles, Vec3(0.0f, 0.0f, 1.0f)));
        mtxRot = glm::inverse(mtxRot);
        mtxRot = glm::transpose(mtxRot);


        Vec3 jointDir(1.0f, 0.0f, 0.0f);
        jointDir = Vec3(Vec4(jointDir, 0.0f) * mtxRot);

        Vec3 jointPos(m_pNode->GetDerivedPosition() + jointDir * m_upperArmLength);

        Vec2 vXAxis(1.0f, 0.0f);
        Vec2 vVelo(m_attackPos - jointPos);
        vVelo = glm::normalize(vVelo);

        float targetLowerArmRollAngle = acos(glm::dot(vVelo, vXAxis));
        if (vVelo.y < 0.0f)
            targetLowerArmRollAngle = -targetLowerArmRollAngle;

        targetLowerArmRollAngle = -upperArmRollAngle + targetLowerArmRollAngle;

        return targetLowerArmRollAngle;
    }

    float CRobotArmEntity::UpdateLowerArmAngle(float lowerArmAngle, float rotationSpeed)
    {
        CQuaternion lowerArmOrientation = m_pLowerArm->GetNode()->GetOrientation();
        float diffLowerArm;
        bool bRotateLeft = ComputeRollRotationDirection(&diffLowerArm, lowerArmAngle, lowerArmOrientation.GetRoll());
        lowerArmOrientation.SetYaw(0.0f);
        lowerArmOrientation.SetPitch(0.0f);

        if (bRotateLeft)
            lowerArmOrientation.AddRoll(std::min(rotationSpeed, diffLowerArm));
        else
            lowerArmOrientation.AddRoll(-std::min(rotationSpeed, diffLowerArm));


        m_pLowerArm->GetNode()->SetOrientation(lowerArmOrientation);

        return lowerArmOrientation.GetRoll();
    }

    void CRobotArmEntity::UpdateAim(float dt)
    {
        m_attackPos = m_pTarget->GetNode()->GetDerivedPosition();
        Vec3 targetPos(m_attackPos);

        Vec3 dir(targetPos - m_pNode->GetDerivedPosition());
        float distance = glm::length(dir);
        float distanceThreshold = m_upperArmLength + m_lowerArmLength;
        if (distance >= distanceThreshold)
            return;

        m_attackPos = targetPos;

        float rootRollAngle = m_directionFactor * ComputeRootAngle();
        float upperArmRollAngle = m_directionFactor * PI * 0.60f;

        m_pNode->SetOrientation(CQuaternion(0.0f, 0.0f, rootRollAngle + upperArmRollAngle));

        float targetLowerArmAngle = ComputeLowerArmAngle(rootRollAngle + upperArmRollAngle);
        UpdateLowerArmAngle(targetLowerArmAngle, 100);

        if (m_stateTimer.GetTime() > m_aimPauseTime)
        {
            SetState(ROBOT_ARM_STATE_HIT);
        }

        m_upperArmAimRoll = upperArmRollAngle;
    }


    void CRobotArmEntity::UpdateHit(float dt)
    {
        m_attackPos = m_pTarget->GetNode()->GetDerivedPosition();

        //CQuaternion rootOrientation = m_pNode->GetOrientation();
        CQuaternion upperArmOrientation = m_pNode->GetOrientation();
        float r = ComputeRootAngle();
        if (m_stateTimer.GetTime() >= m_hitTime + m_hitPauseTime)
        {
            SetState(ROBOT_ARM_STATE_REVERSEHIT);
            //SetState(ROBOT_ARM_STATE_IDLE);


            m_upperArmHitRoll = m_directionFactor * ComputeUpperArmAngle();

            return;
        }
        else if (m_stateTimer.GetTime() >= m_hitTime)
        {
            m_upperArmHitRoll = ComputeUpperArmAngle();

            upperArmOrientation.SetRoll(r + m_directionFactor * ComputeUpperArmAngle());

            m_pNode->SetOrientation(upperArmOrientation);

            float targetLowerArmAngle = ComputeLowerArmAngle(m_pNode->GetOrientation().GetRoll());
            UpdateLowerArmAngle(targetLowerArmAngle, 100);

            if (m_pCharacter)
            {
                float distance = glm::length(m_attackPos - m_pCharacter->GetNode()->GetDerivedPosition());

                if (distance < 600.0f)
                {
                    if (m_pPlayerCamera && GetGameMode())
                        m_pPlayerCamera->Shake(0.05f, (1.0f - distance / 600.0f) * Vec2(-900.0f, 900.0f));
                }

                m_pSound->PlaySound();
            }


            return;
        }


        float finalUpperArmRollAngle = m_directionFactor * ComputeUpperArmAngle();
        float alpha = m_stateTimer.GetTime() / m_hitTime;

        alpha = std::pow(alpha, 2.5f);


        upperArmOrientation.SetYaw(0.0f);
        upperArmOrientation.SetPitch(0.0f);


        upperArmOrientation.SetRoll(r + (1.0f - alpha) * m_upperArmAimRoll + alpha * finalUpperArmRollAngle);

        m_pNode->SetOrientation(upperArmOrientation);


        float targetLowerArmAngle = ComputeLowerArmAngle(m_pNode->GetOrientation().GetRoll());
        UpdateLowerArmAngle(targetLowerArmAngle, 100);
    }

    void CRobotArmEntity::UpdateReverseHit(float dt)
    {
        m_attackPos = m_pTarget->GetNode()->GetDerivedPosition();

        if (m_stateTimer.GetTime() >= m_reverseHitTime + m_reverseHitPauseTime)
        {
            SetState(ROBOT_ARM_STATE_HIT);

            m_upperArmHitRoll = ComputeUpperArmAngle();

            return;
        }
        else if (m_stateTimer.GetTime() >= m_reverseHitTime)
        {

            return;
        }

        CQuaternion upperArmOrientation = m_pNode->GetOrientation();

        float alpha = m_stateTimer.GetTime() / m_reverseHitTime;

        alpha = 1.0f - alpha;
        alpha = std::pow(alpha, 2.0f);
        alpha = 1.0f - alpha;

        float r = ComputeRootAngle();
        upperArmOrientation.SetRoll(r + (1.0f - alpha) * m_upperArmHitRoll + alpha * m_upperArmAimRoll);



        m_pNode->SetOrientation(upperArmOrientation);

        float lowerArmAngle = ComputeLowerArmAngle(upperArmOrientation.GetRoll());
        UpdateLowerArmAngle(lowerArmAngle, 100.0f);


    }

    void CRobotArmEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
        m_pTarget->GetNode()->AttachChildNode(m_pSound->GetNode());
    }

    void CRobotArmEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        SetState(ROBOT_ARM_STATE_IDLE);

        if (!m_pTarget)
            return;

        m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();
        m_pPlayerCamera = static_cast<CCameraEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_PLAYERCAMERA));

        m_attackPos = m_pTarget->GetNode()->GetDerivedPosition();

        float rootRollAngle = ComputeRootAngle();
        float upperArmRollAngle = PI * 0.60f;

        m_pNode->SetOrientation(CQuaternion(0.0f, 0.0f, rootRollAngle + upperArmRollAngle));

        float targetLowerArmAngle = ComputeLowerArmAngle(rootRollAngle + upperArmRollAngle);
        UpdateLowerArmAngle(targetLowerArmAngle, 100);
    }

    void CRobotArmEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CRobotArmEntity::Update);

        CMeshEntity::Update(dt);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this &&
            gEngine->GetInput()->IsKeyJustDown('G'))
        {
            SetMirror(m_directionFactor);
        }

        if (!m_pTarget)
            return;

        m_stateTimer.Update(dt);

        switch (m_state)
        {
        case ROBOT_ARM_STATE_IDLE:
        {
            if (m_stateTimer.GetTime() > m_delay)
            {
                SetState(ROBOT_ARM_STATE_AIM);
            }
        }
        break;
        case ROBOT_ARM_STATE_AIM:
            UpdateAim(dt);
            break;

        case ROBOT_ARM_STATE_HIT:
            UpdateHit(dt);
            break;

        case ROBOT_ARM_STATE_REVERSEHIT:
            UpdateReverseHit(dt);
            break;
        default:
            break;
        }
    }




    ENTITY_TYPEID CRobotArmT2Entity::ENTITY_META_TYPEID = ENTITY_TYPEID_ROBOTARMT2;

    CRobotArmT2Entity::CRobotArmT2Entity()
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_aimPauseTime = 0.3f;
        m_hitTime = 0.15f;
        m_hitPauseTime = 0.3f;
    }

    CRobotArmT2Entity::~CRobotArmT2Entity()
    {
    }

    void CRobotArmT2Entity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        m_lowerArmLength = 350.0f;
        m_upperArmLength = 250.0f;

        //CMeshEntity* pUpperArm = static_cast< CMeshEntity* >(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
        //pUpperArm->SetMeshFilename("upperArmBig.x");
        //m_pUpperArm = pUpperArm;

        SetMeshFilename("robotUpperArmT2.x");
        //pUpperArm->SetAutoDelete(false);
        //pUpperArm->SetPickable(false);
        //pUpperArm->GetNode()->SetPosition(VECTOR3(0.0f, 0.0f, 0.0f));


        CMeshEntity* pLowerArm = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
        pLowerArm->SetMeshFilename("robotLowerArmT2.x");
        m_pLowerArm = pLowerArm;

        m_pLowerArm->SetAutoDelete(false);
        m_pLowerArm->SetPickable(false);
        m_pLowerArm->GetNode()->SetPosition(Vec3(m_upperArmLength, 0.0f, 0.0f));

        CMeshEntity* pEntity01 = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SAWBLADE));
        pEntity01->SetAutoDelete(false);
        pEntity01->GetNode()->SetPosition(Vec3(m_lowerArmLength, 0.0f, 0.0f));
        pEntity01->GetNode()->SetScale(Vec3(1.65f, 1.65f, 1.65f));

        CMeshEntity* pEntity02 = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SAWBLADE));
        pEntity02->SetAutoDelete(false);
        pEntity02->GetNode()->SetPosition(Vec3(m_lowerArmLength * 0.75f, 65.0f, 0.0f));
        pEntity02->GetNode()->SetScale(Vec3(1.65f, 1.65f, 1.65f));

        CMeshEntity* pEntity03 = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SAWBLADE));
        pEntity03->SetAutoDelete(false);
        pEntity03->GetNode()->SetPosition(Vec3(m_lowerArmLength * 0.75f, -65.0f, 0.0f));
        pEntity03->GetNode()->SetScale(Vec3(1.65f, 1.65f, 1.65f));




        m_entities.resize(3);
        m_entities[0] = pEntity01;
        m_entities[1] = pEntity02;
        m_entities[2] = pEntity03;

        CSpriteEntity* pTarget = static_cast<CSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SPRITE));
        pTarget->SetRenderGame(false);
        m_pTarget = pTarget;
        m_pTarget->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + Vec3(100.0f, 0.0f, 0.0f));
        m_pTarget->SetAutoDelete(false);
        m_pTarget->GetNode()->AttachChildNode(m_pSound->GetNode());



        //m_pNode->AttachChildNode(m_pUpperArm->GetNode());
        m_pNode->AttachChildNode(m_pLowerArm->GetNode());

        for (int i = 0; i < m_entities.size(); ++i)
            m_pLowerArm->GetNode()->AttachChildNode(m_entities[i]->GetNode());

        SetState(ROBOT_ARM_STATE_AIM);
    }

    void CRobotArmT2Entity::Update(float dt)
    {
        m_lowerArmLength = 350.0f * m_pNode->GetDerivedScale().x;
        m_upperArmLength = 250.0f * m_pNode->GetDerivedScale().x;

        CRobotArmEntity::Update(dt);
    }














    ENTITY_TYPEID CRobotArmT3Entity::ENTITY_META_TYPEID = ENTITY_TYPEID_ROBOTARMT3;

    CRobotArmT3Entity::CRobotArmT3Entity()
        : m_pLight(NULL)
    {
        SetMetaTypeID(CRobotArmT3Entity::ENTITY_META_TYPEID);

    }

    CRobotArmT3Entity::~CRobotArmT3Entity()
    {
        if (m_pLight)
            gEngine->GetEntityMgr()->RemoveEntity(m_pLight->GetName());
    }

    void CRobotArmT3Entity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        m_lowerArmLength = 350.0f;
        m_upperArmLength = 290.0f;

        //CMeshEntity* pUpperArm = static_cast< CMeshEntity* >(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
        //pUpperArm->SetMeshFilename("upperArmBig.x");
        //m_pUpperArm = pUpperArm;

        SetMeshFilename("robotUpperArmT3.x");
        //pUpperArm->SetAutoDelete(false);
        //pUpperArm->SetPickable(false);
        //pUpperArm->GetNode()->SetPosition(VECTOR3(0.0f, 0.0f, 0.0f));


        CMeshEntity* pLowerArm = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
        pLowerArm->SetMeshFilename("robotLowerArmT3.x");
        m_pLowerArm = pLowerArm;

        m_pLowerArm->SetAutoDelete(false);
        m_pLowerArm->SetPickable(false);
        m_pLowerArm->GetNode()->SetPosition(Vec3(m_upperArmLength, 0.0f, 0.0f));



        CMeshEntity* pEntity01 = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SAWBLADE));

        pEntity01->SetAutoDelete(false);
        pEntity01->GetNode()->SetPosition(Vec3(m_lowerArmLength, 0.0f, 0.0f));
        pEntity01->GetNode()->SetScale(Vec3(1.0f, 1.0f, 1.0f));


        m_entities.resize(1);
        m_entities[0] = pEntity01;

        CSpriteEntity* pTarget = static_cast<CSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SPRITE));
        pTarget->SetRenderGame(false);
        m_pTarget = pTarget;
        m_pTarget->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + Vec3(100.0f, 0.0f, 0.0f));
        m_pTarget->SetAutoDelete(false);
        m_pTarget->GetNode()->AttachChildNode(m_pSound->GetNode());

        m_pLight = static_cast<CLightSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHTSPRITE));
        m_pLight->GetLight()->SetInitialEnergy(7.0f);
        m_pLight->GetLight()->SetRadius(600.0f);
        m_pLight->GetLight()->SetInitialDiffuseColor(Vec4(1.0f, 0.2f, 0.0f, 1.0f));
        m_pLight->GetLight()->SetInitialSpecularColor(Vec4(1.0f, 0.2f, 0.0f, 1.0f));
        m_pLight->GetLight()->SetSpecular(true);
        m_pLight->SetAutoDelete(false);
        m_pLight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));

        pEntity01->GetNode()->AttachChildNode(m_pLight->GetNode());

        //m_pNode->AttachChildNode(m_pUpperArm->GetNode());
        m_pNode->AttachChildNode(m_pLowerArm->GetNode());

        for (int i = 0; i < m_entities.size(); ++i)
            m_pLowerArm->GetNode()->AttachChildNode(m_entities[i]->GetNode());

        SetState(ROBOT_ARM_STATE_AIM);
    }

    void CRobotArmT3Entity::Update(float dt)
    {
        m_lowerArmLength = 350.0f * m_pNode->GetDerivedScale().x;
        m_upperArmLength = 290.0f * m_pNode->GetDerivedScale().x;

        if (gEngine->GetEntityMgr()->GetActiveEntity())
            return;

        CRobotArmEntity::Update(dt);
    }
} // 