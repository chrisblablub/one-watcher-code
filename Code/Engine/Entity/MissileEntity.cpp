
#include <Engine/Entity/MissileEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CMissileEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MISSILE;

    //-----------------------------------------------------------------------------------
    CMissileEntity::CMissileEntity()
        : m_state(BOMB_STATE_INACTIVE)
        , m_pExplosion(NULL)
        , m_currentLifeTime(0.0f)
        , m_velocity(Vec3(0.0f, 0.0f, 0.0f))
        , m_lifeTime(1.0f)
        , m_targetingSpeed(1.0f)
        , m_bTargeting(true)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CMissileEntity::~CMissileEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pExplosion);
    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Missile.x");

        LoadTemplate("../Meshes/Templates/tmpl_missile_01.mesh");

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-10.0f, 25.0f, 0.0f));
        p.AddPoint(Vec3(10.0f, 25.0f, 0.0f));
        p.AddPoint(Vec3(10.0f, -20.0f, 0.0f));
        p.AddPoint(Vec3(-10.0f, -20.0f, 0.0f));
        AddPolygon(p);

        m_pExplosion = static_cast<CExplosionEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLOSION));
        m_pExplosion->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_pExplosion->GetNode()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
        m_pExplosion->SetRenderEditor(false);
        m_pExplosion->SetRenderGame(false);
        m_pExplosion->SetPickable(false);
        m_pExplosion->SetAutoDelete(false);

        //m_pNode->AttachChildNode(m_pExplosion->GetNode());

        SetEnabled(false);
    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        LoadTemplate("../Meshes/Templates/tmpl_missile_01.mesh");

        RemoveCircles();
        RemovePolygons();

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-10.0f, 25.0f, 0.0f));
        p.AddPoint(Vec3(10.0f, 25.0f, 0.0f));
        p.AddPoint(Vec3(10.0f, -20.0f, 0.0f));
        p.AddPoint(Vec3(-10.0f, -20.0f, 0.0f));
        AddPolygon(p);


        m_pExplosion->GetNode()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

        SetMeshFilename("Missile.x");
    }

    void CMissileEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());

        }

        m_pSound->LoadSound("Entities/Missile.wav");
    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::UpdateBombLoading(float dt)
    {
        if (m_bEnabled == false)
        {
            return;
        }

        CEntity* pTarget = gEngine->GetEntityMgr()->GetCharacter();

        if (pTarget == NULL)
        {
            return;
        }

        if (CGeometryCollidable::HasCollision(*pTarget))
        {
            CCharacter3DEntity* pChar = static_cast<CCharacter3DEntity*>(pTarget);

            pChar->SetKilled(true);

            m_bGhost = true;
            SetState(CMissileEntity::BOMB_STATE_EXPLODING);
            return;
        }

        std::list < CEntity* > platforms;
        gEngine->GetEntityMgr()->GetEntities(platforms, ENTITY_TYPEID_PLATFORMBASE);

        CPlatformEntity* pCurrentPlatform;
        CSceneNode* pNode = nullptr;

        for (std::list< CEntity* >::iterator it = platforms.begin(); it != platforms.end(); ++it)
        {
            pCurrentPlatform = static_cast<CPlatformEntity*>(*it);

            if (CGeometryCollidable::HasCollision(*pCurrentPlatform) == false)
            {
                continue;
            }

            m_bGhost = true;
            SetState(CMissileEntity::BOMB_STATE_EXPLODING);
            return;
        }

        CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();
        float factor = 10.0f / glm::length(m_currentVelocity * dt);
        if (pLevel->IsRayIntersecting(m_pNode->GetDerivedPosition(), m_pNode->GetDerivedPosition() + m_currentVelocity * dt * factor, NULL))
        {
            m_bGhost = true;
            SetState(CMissileEntity::BOMB_STATE_EXPLODING);
            return;
        }

        if (m_bTargeting)
        {
            Vec3 targetDirection(pTarget->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());

            float targetRoll = ComputeRoll(targetDirection);

            CQuaternion quat = m_pNode->GetOrientation();

            float rollDiff = 0.0f;
            if (ComputeRollRotationDirection(&rollDiff, targetRoll, quat.GetRoll()))
                rollDiff = rollDiff * dt * m_targetingSpeed;
            else
                rollDiff = -rollDiff * dt * m_targetingSpeed;

            quat.AddRoll(rollDiff);
            m_pNode->SetOrientation(quat);

            Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), rollDiff, Vec3(0.0f, 0.0f, 1.0f)));
            m_currentVelocity = Vec3(Vec4(m_currentVelocity, 0.0f) * mtxRot);
        }

        m_pNode->Translate(m_currentVelocity * dt);

        ApplyTransform(m_pNode->GetDerivedRST());
        m_pExplosion->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::UpdateBombExploding(float dt)
    {
        m_bGhost = true;

        if (m_stateTimer.GetTime() < 0.3f)
        {
            float alpha = m_stateTimer.GetTime() / 0.3f;


            Vec3 scaling(1.0f, 1.0f, 1.0f);
            scaling *= (1.0f - alpha);

            m_pNode->SetScale(0.6f * scaling);
            ApplyTransform(m_pNode->GetDerivedRST(), 0.6f * scaling.x);
        }
        else
        {
            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });
            ApplyTransform(m_pNode->GetDerivedRST(), 0.0f);
        }

    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::SetState(BOMB_STATE state)
    {
        m_stateTimer.Reset();
        m_state = state;

        if (m_state == BOMB_STATE_EXPLODING)
        {
            m_pExplosion->SetEnabled(true);

        }
    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CMissileEntity::Update);

        CMeshEntity::Update(dt);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
            return;


        float soundVolume = m_bEnabled && m_state != BOMB_STATE_EXPLODING ? 0.15f : 0.0f;
        m_pSound->SetVolume(soundVolume);


        if (!m_bEnabled)
            return;

        m_currentLifeTime += dt;

        m_stateTimer.Update(dt);


        switch (m_state)
        {
        case BOMB_STATE_LOADING:
        {
            UpdateBombLoading(dt);
        }
        break;

        case BOMB_STATE_EXPLODING:
            UpdateBombExploding(dt);
            break;

        default:
            break;
        }

    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::SetEnabled(bool bEnable)
    {
        m_bEnabled = bEnable;

        m_stateTimer.Reset();

        if (!bEnable)
        {
            m_pExplosion->SetEnabled(false);
            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(true); });
        }

        m_pNode->SetScale(Vec3(0.6f, 0.6f, 0.6f));

        GetMeshInstanceContainer().CallForEach([&](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInEditor(false); });
        GetMeshInstanceContainer().CallForEach([&](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(bEnable); });

        m_bGhost = !bEnable;

        m_currentVelocity = m_velocity;

        m_currentLifeTime = 0.0f;

        //SetState(BOMB_STATE_LOADING);
    }

    //-----------------------------------------------------------------------------------
    void CMissileEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        SetEnabled(false);
    }








    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CMissileGunEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MISSILEGUN;

    //-----------------------------------------------------------------------------------
    CMissileGunEntity::CMissileGunEntity()
        : m_fireAccum(0.0f)
        , m_fireDelay(0.0f)
        , m_fireDelayMin(3.5f)
        , m_lifeTimeMin(1.0f)
        , m_velocityMin(700.0f)
        , m_fireDelayMax(3.5f)
        , m_lifeTimeMax(1.0f)
        , m_velocityMax(700.0f)
        , m_missileTargetingSpeed(2.0f)
        , m_visibilityRadius(800.0f)
        , m_bMissilesTargeting(false)
        , m_bVisibility(false)
        , m_pCharacter(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CMissileGunEntity::~CMissileGunEntity()
    {
        for (int i = 0; i < m_bombs.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_bombs[i]->GetName());
        }
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Streamer01.x");

        UpdateBombs();
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        for (int i = 0; i < m_bombs.size(); ++i)
        {
            m_bombs[i]->SetRotationSpeedZ(0.0f);
            m_bombs[i]->SetMeshFilename("Missile.x");
        }

        UpdateBombs();
    }

    void CMissileGunEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->SetIsAutoplay(false);
        m_pSound->SetPeakVolumeDistance(300.0f);
        m_pSound->SetAudibleDistance(800.0f);
        m_pSound->SetVolume(0.4f);
        m_pSound->LoadSound("Entities/MissileGun.wav");
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

        CMissileGunEntity& other = static_cast<CMissileGunEntity&>(entity);

        SetFireDelayMin(other.GetFireDelayMin());
        SetFireDelayMax(other.GetFireDelayMax());
        SetVelocityMin(other.GetVelocityMin());
        SetVelocityMax(other.GetVelocityMax());
        SetLifetimeMin(other.GetLifetimeMin());
        SetLifetimeMax(other.GetLifetimeMax());
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CMissileGunEntity::Update);

        CMeshEntity::Update(dt);

        if (!GetGameMode())
            return;

        m_fireAccum += dt;


        m_currentRotatationTime -= dt;

        if (m_currentRotatationTime < 0.0f)
        {
            SetRotationSpeedY(0.0f);
        }

        for (int i = 0; i < m_currentNumActiveBombs; ++i)
        {
            CMissileEntity* p = m_bombs[i];
            if (p->GetCurrentLifetime() >= (p->GetLifeTime() + EXPLOSION_TIME))
            {
                p->SetEnabled(false);

                // Disable stream
                m_bombs[i] = m_bombs[m_currentNumActiveBombs - 1];
                m_bombs[m_currentNumActiveBombs - 1] = p;

                --m_currentNumActiveBombs;
            }
            else if (p->GetCurrentLifetime() >= p->GetLifeTime() && m_bombs[i]->GetState() != CMissileEntity::BOMB_STATE_EXPLODING)
            {
                m_bombs[i]->SetState(CMissileEntity::BOMB_STATE_EXPLODING);
            }
        }

        if (m_pCharacter)
        {
            float distance = glm::length(m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());
            if (distance > m_visibilityRadius)
            {
                return;
            }

            if (m_bVisibility && gEngine->GetWorldMgr()->GetLevelMgr()->IsRayIntersecting(m_pNode->GetDerivedPosition(), m_pCharacter->GetNode()->GetDerivedPosition(), NULL))
            {
                return;
            }
        }


        if (m_fireAccum >= m_fireDelay)
        {
            if (m_currentNumActiveBombs < m_bombs.size())
            {
                // Activate bomb
                m_bombs[m_currentNumActiveBombs]->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
                m_bombs[m_currentNumActiveBombs]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
                Vec3 vel = m_pNode->GetDerivedUp();
                vel *= GetRandomFloat(m_velocityMin, m_velocityMax);

                m_bombs[m_currentNumActiveBombs]->SetVelcotiy(vel);
                m_bombs[m_currentNumActiveBombs]->SetLifetime(GetRandomFloat(m_lifeTimeMin, m_lifeTimeMax));
                m_bombs[m_currentNumActiveBombs]->SetEnabled(true);
                m_bombs[m_currentNumActiveBombs]->SetState(CMissileEntity::BOMB_STATE_LOADING);


                ++m_currentNumActiveBombs;

                SetRotationSpeedY(5.0f);

                m_currentRotatationTime = 0.25f;

                m_fireAccum = 0.0f;

                m_fireDelay = GetRandomFloat(m_fireDelayMin, m_fireDelayMax);
                m_pSound->PlaySound();
            }

        }

    }

    //-----------------------------------------------------------------------------------
    void CMissileGunEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentNumActiveBombs = 0;

        m_fireAccum = 0.0f;

        m_currentRotatationTime = -1.0f;

        m_fireDelay = GetRandomFloat(m_fireDelayMin, m_fireDelayMax);

        m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();

        SetRotationSpeedY(0.0f);
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunEntity::UpdateBombs()
    {
        int numBombs = std::max((int)(1 + (m_lifeTimeMax + EXPLOSION_TIME) / m_fireDelayMin), 3);

        int curNumBombs = m_bombs.size();

        for (int i = m_bombs.size() - 1; i >= numBombs; --i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_bombs[i]->GetName());
        }

        m_bombs.resize(numBombs);

        for (int i = curNumBombs; i < numBombs; ++i)
        {
            m_bombs[i] = static_cast<CMissileEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MISSILE));
            m_bombs[i]->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
            m_bombs[i]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
            m_bombs[i]->SetAutoDelete(false);
            m_bombs[i]->SetPickable(false);
            m_bombs[i]->SetEnabled(false);
        }

        for (int i = 0; i < m_bombs.size(); ++i)
        {
            m_bombs[i]->SetTargeting(m_bMissilesTargeting);
            m_bombs[i]->SetTargetingSpeed(m_missileTargetingSpeed);
        }

        m_currentNumActiveBombs = 0;
    }


    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CMissileGunAimingEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MISSILEGUNAIMING;

    //-----------------------------------------------------------------------------------
    CMissileGunAimingEntity::CMissileGunAimingEntity()
        : m_offsetRadius(100.0f)
        , m_targetingSpeed(8.0f)
        , m_bCurrentlyTargeting(true)
        , m_prevPosition(0.0f, 0.0f, 0.0)
        , m_pCharacter(nullptr)

    {
        m_bMissilesTargeting = true;

        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CMissileGunAimingEntity::~CMissileGunAimingEntity()
    {

    }

    //-----------------------------------------------------------------------------------
    void CMissileGunAimingEntity::InitializeAtCreation()
    {
        CMissileGunEntity::InitializeAtCreation();

        GetMeshInstanceNode(0)->SetPosition(Vec3(0.0f, m_offsetRadius, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunAimingEntity::InitializeAtLoad()
    {
        CMissileGunEntity::InitializeAtLoad();
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunAimingEntity::SetOffsetRadius(float radius)
    {
        m_offsetRadius = radius;
        GetMeshInstanceNode(0)->SetPosition(Vec3(0.0f, m_offsetRadius, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunAimingEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CMissileGunAimingEntity::Update);

        if (!GetGameMode())
            return;

        Vec3 translation = m_pNode->GetDerivedPosition() - m_prevPosition;
        m_prevPosition = m_pNode->GetDerivedPosition();

        for (int i = 0; i < m_currentNumActiveBombs; ++i)
        {
            CMissileEntity* p = m_bombs[i];
            if (p->GetCurrentLifetime() >= (p->GetLifeTime() + EXPLOSION_TIME))
            {
                p->SetEnabled(false);

                // Disable stream
                m_bombs[i] = m_bombs[m_currentNumActiveBombs - 1];
                m_bombs[m_currentNumActiveBombs - 1] = p;

                --m_currentNumActiveBombs;
            }
            else if (p->GetCurrentLifetime() >= p->GetLifeTime() && m_bombs[i]->GetState() != CMissileEntity::BOMB_STATE_EXPLODING)
            {
                m_bombs[i]->SetState(CMissileEntity::BOMB_STATE_EXPLODING);
            }
            else if (p->GetCurrentLifetime() <= 0.3f)
            {
                p->GetNode()->Translate(translation);
            }
        }

        m_currentRotatationTime -= dt;

        if (m_currentRotatationTime < 0.0f)
        {
            SetRotationSpeedY(0.0f);
        }
        else
            return;

        if (m_pCharacter)
        {
            float distance = glm::distance(m_pCharacter->GetNode()->GetDerivedPosition(), m_pNode->GetDerivedPosition());
            if (distance > m_visibilityRadius)
            {
                return;
            }

            if (m_bVisibility && gEngine->GetWorldMgr()->GetLevelMgr()->IsRayIntersecting(m_pNode->GetDerivedPosition(), m_pCharacter->GetNode()->GetDerivedPosition(), NULL))
            {
                return;
            }
        }

        if (m_bCurrentlyTargeting)
        {
            // Rollt to target
            if (!m_pCharacter)
                return;

            Vec3 characterRay = m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition();
            Vec3 missileGunRay = m_pNode->GetUp();

            // float characterDistance = glm::length(characterRay);

            //if (characterDistance < 600.0f)
            {
                characterRay = glm::normalize(characterRay);
                missileGunRay = glm::normalize(missileGunRay);

                float angle = std::acos(glm::dot(characterRay, Vec3(0.0f, 1.0f, 0.0)));
                if (characterRay.x >= 0.0f)
                    angle = -angle;


                CQuaternion q = m_pNode->GetOrientation();

                float roll = 0.0f;
                if (ComputeRollRotationDirection(&roll, angle, q.GetRoll()))
                {
                    //if (roll < 0.1f)
                    //	roll = 0.1f;
                    q.AddRoll(roll * dt * m_targetingSpeed);
                }
                else
                {
                    //if (roll < 0.1f)
                    //	roll = 0.1f;
                    q.AddRoll(-roll * dt * m_targetingSpeed);
                }


                m_pNode->SetOrientation(q);

                if (roll < 0.15f && m_fireAccum > m_fireDelay)
                    m_bCurrentlyTargeting = false;
            }


            //m_currentTargetingTime -= dt;
            //m_fireAccum = -1.0f;
            m_fireAccum += dt;

        }
        else
        {
            //if (m_fireAccum < 0)
            {
                // Shoot

                if (m_currentNumActiveBombs < m_bombs.size())
                {
                    // Activate bomb
                    m_bombs[m_currentNumActiveBombs]->GetNode()->SetPosition(GetMeshInstanceNode(0)->GetDerivedPosition());
                    m_bombs[m_currentNumActiveBombs]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
                    Vec3 vel = m_pNode->GetDerivedUp();
                    vel *= GetRandomFloat(m_velocityMin, m_velocityMax);

                    m_bombs[m_currentNumActiveBombs]->SetVelcotiy(vel);
                    m_bombs[m_currentNumActiveBombs]->SetLifetime(GetRandomFloat(m_lifeTimeMin, m_lifeTimeMax));
                    m_bombs[m_currentNumActiveBombs]->SetEnabled(true);
                    m_bombs[m_currentNumActiveBombs]->SetState(CMissileEntity::BOMB_STATE_LOADING);


                    ++m_currentNumActiveBombs;

                    SetRotationSpeedY(5.0f);

                    m_currentRotatationTime = 0.25f;

                    m_fireAccum = 0.0f;

                    m_fireDelay = GetRandomFloat(m_fireDelayMin, m_fireDelayMax);

                    m_bCurrentlyTargeting = true;
                    m_pSound->PlaySound();

                }
            }
            //else //if (m_fireAccum >= m_fireDelay)
            {
                // Wait delay
                //m_currentTargetingTime = 1.0f;

                //m_bTargeting = true;

                //m_initialRoll = m_pNode->GetOrientation().GetRoll();

            }
            /*else
            {
            m_fireAccum += dt;
            }*/
        }
    }

    //-----------------------------------------------------------------------------------
    void CMissileGunAimingEntity::OnGameModeChanged(bool bGameMode)
    {
        CMissileGunEntity::OnGameModeChanged(bGameMode);

        //m_currentTargetingTime = 0.5f;

        m_prevPosition = m_pNode->GetDerivedPosition();

        m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();

        m_bCurrentlyTargeting = true;
    }
} // env