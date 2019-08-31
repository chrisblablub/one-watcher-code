#include <Engine/Entity/LaserRadiatorEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Common/Math/MathUtility.h>

namespace env
{
    ENTITY_TYPEID CLaserRadiatorEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_LASERRADIATOR;

    //-----------------------------------------------------------------------------------
    CLaserRadiatorEntity::CLaserRadiatorEntity()
        : m_laserActiveTime(2.0f)
        , m_laserPausingTime(1.0f)
        , m_laserLoadingTime(0.5f)
        , m_laserMaxDistance(500.0f)
        , m_laserDelay(0.0f)
        , m_time(0.0f)
        , m_laserOffsetRadius(0.0f)
        , m_currentLaserState(LASER_STATE_PAUSED)
        , m_numLasers(1)
        , m_attachedMeshInstIndex(0)
        , m_bDisabled(false)
        , m_bLaserTerrainCollisions(true)
        , m_bLaserPlatformCollisions(true)
        , m_laserColor(1.0f, 0.0f, 0.0f)
    {
        RegisterEvent("[Laser Radiator] Enable", LASERRADIATOR_EVENT_ENABLE);
        RegisterEvent("[Laser Radiator] Disable", LASERRADIATOR_EVENT_DISABLE);

        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CLaserRadiatorEntity::~CLaserRadiatorEntity()
    {
        for (int i = 0; i < m_nodes.size(); ++i)
        {
            if (m_nodes[i])
                gEngine->GetSceneMgr()->RemoveNode(m_nodes[i]->GetName());
        }

        for (int i = 0; i < m_particles.size(); ++i)
        {
            if (m_particles[i])
                gEngine->GetEntityMgr()->RemoveEntity(m_particles[i]->GetName());
        }
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->SetAudibleDistance(400.0f);
        m_pSound->SetPeakVolumeDistance(150.0f);
        m_pSound->SetVolume(0);
        m_pSound->LoadSound("Entities/LaserRadiator.wav");
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CLaserRadiatorEntity::Update);

        CMeshEntity::Update(dt);

        m_bGhost = true;
        if (m_bDisabled)
            return;

        m_time += dt;

        if (m_time <= m_laserDelay)
            return;

        m_currentLaserTime -= dt;

        if (m_currentLaserState == LASER_STATE_PAUSED)
        {
            //if(m_numLasers == 1)
            //	SetRotationSpeedX(0.0f);

            if (m_currentLaserTime <= 0.0f)
            {
                m_currentLaserState = LASER_STATE_LOADING;
                m_currentLaserTime = m_laserLoadingTime;
            }
            else
            {
                m_pSound->SetVolume(0);
                return;
            }
        }

        float soundVolume = 0;

        if (m_currentLaserState == LASER_STATE_LOADING)
        {
            //if(m_numLasers == 1)
            //	SetRotationSpeedX(1.5f);

            if (m_laserLoadingTime > 0)
            {
                soundVolume = (m_laserLoadingTime - m_currentLaserTime) / m_laserLoadingTime;
            }

            if (m_currentLaserTime <= 0.0f)
            {
                m_currentLaserState = LASER_STATE_ACTIVE;
                m_currentLaserTime = m_laserActiveTime;
            }
        }

        if (m_currentLaserState == LASER_STATE_ACTIVE)
        {
            //if(m_numLasers == 1)
            //	SetRotationSpeedX(4.5f);
            soundVolume = 1.0f;

            if (m_currentLaserTime < 0.3f)
            {
                soundVolume = m_currentLaserTime / 0.3f;
            }

            if (m_currentLaserTime < 0.0f)
            {
                m_currentLaserState = LASER_STATE_PAUSED;
                m_currentLaserTime = m_laserPausingTime;
            }
        }

        m_pSound->SetVolume(soundVolume * 0.5f);

        float minSoundDistanceToPlayer = std::numeric_limits<float>::max();

        CCharacter3DEntity* pCharacter = static_cast<CCharacter3DEntity*>(gEngine->GetEntityMgr()->GetCharacter());
        Vec3 charPosition;

        if (pCharacter != nullptr)
        {
            charPosition = pCharacter->GetNode()->GetDerivedPosition() - GetNode()->GetDerivedPosition();
        }

        for (int i = 0; i < m_lasers.size(); ++i)
        {
            CSprite& laserSprite = m_lasers[i];
            CSceneNode& laserTransform = *m_nodes[i];

            Matrix4x4 mtxRotation = glm::transpose(glm::rotate(Matrix4x4(1.0f), (float)i / m_lasers.size() * 2 * PI, Vec3(0.0f, 0.0f, 1.0f)));

            CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

            float laserDistance = m_laserMaxDistance;
            Vec3 startLaserPosition = m_nodes[i]->GetDerivedPosition();
            Vec3 endLaserPosition = Vec3(laserDistance + m_laserOffsetRadius, 0.0f, 0.0f);
            Vec3 intersection(0.0f, 0.0f, 0.0f);

            endLaserPosition = Vec3(Vec4(endLaserPosition, 1.0f) * mtxRotation * GetMeshInstanceNode(m_attachedMeshInstIndex)->GetDerivedRST());
            Vec3 intersectLevel;


            bool bUseProjectedCollision = false;
            float intersectDistance = 0;
            if (m_bLaserTerrainCollisions)
            {
                if (pLevel->IsRayIntersecting(startLaserPosition, endLaserPosition, &intersection, NULL))
                {
                    laserDistance = glm::length(intersection - startLaserPosition);
                }
            }
            else if (glm::intersectRayPlane(startLaserPosition, endLaserPosition, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), intersectDistance))
            {
                intersection = endLaserPosition;

                Vec3 laserRay = glm::normalize(endLaserPosition - startLaserPosition);

                if (glm::dot(laserRay, Vec3(0.0f, 0.0f, -1.0f)) >= 0.0f)
                {
                    // scale radiator ray
                    laserDistance = std::min(glm::length(intersectLevel - startLaserPosition), glm::length(endLaserPosition - startLaserPosition));
                    if (glm::length(intersectLevel - startLaserPosition) <= glm::length(endLaserPosition - startLaserPosition))
                        bUseProjectedCollision = true;
                }
                else
                {
                    laserDistance = glm::length(endLaserPosition - startLaserPosition);
                }
            }

            if (m_bLaserPlatformCollisions)
            {
                std::list < CEntity* > platforms;
                gEngine->GetEntityMgr()->GetEntities(platforms, ENTITY_TYPEID_PLATFORMBASE);

                CPlatformEntity* pCurrentPlatform;
                float intersectionDistance = 0;
                CSceneNode* pNode = nullptr;

                Vec3 direction = glm::normalize(endLaserPosition - startLaserPosition);

                for (std::list< CEntity* >::iterator it = platforms.begin(); it != platforms.end(); ++it)
                {
                    pCurrentPlatform = static_cast<CPlatformEntity*>(*it);

                    if (pCurrentPlatform->HasCollisionWithRay(startLaserPosition, direction, intersectionDistance, &pNode) == false)
                    {
                        continue;
                    }

                    if (intersectionDistance < laserDistance)
                    {
                        laserDistance = intersectionDistance;
                    }
                }
            }

            m_particles[i]->SetEmitNewParticles(false);
            m_particles[i]->SetRender(false);

            if (m_currentLaserState == LASER_STATE_ACTIVE)
            {
                if (laserDistance < m_laserMaxDistance || !m_bLaserTerrainCollisions)
                {
                    m_particles[i]->SetEmitNewParticles(true);
                    m_particles[i]->GetNode()->SetPosition(Vec3(intersection.x, intersection.y, startLaserPosition.z - 5.0f));
                }
            }

            Vec3 direction = glm::normalize(endLaserPosition - startLaserPosition);
            float soundDistance = ComputePointOnLineProjection(charPosition, Vec3(0, 0, 0), direction);
            soundDistance = Clamp(soundDistance, 0.0f, laserDistance);

            Vec3 soundPosition = soundDistance * direction;
            float soundDistanceToPlayer = glm::length(soundPosition - charPosition);

            if (minSoundDistanceToPlayer > soundDistanceToPlayer)
            {
                minSoundDistanceToPlayer = soundDistanceToPlayer;
                const Matrix4x4 inverseRotation = glm::transpose(GetNode()->GetOrientation().GetRotationMatrix());
                m_pSound->GetNode()->SetPosition(Vec4(soundPosition, 1.0f) * inverseRotation);
            }

            Matrix4x4 mtxScaling = glm::scale(Matrix4x4(1.0f), Vec3(laserDistance / LASER_TEXTURE_WIDTH, 1.0f, 1.0f));

            TextureRect rect;
            laserSprite.GetAbsoluteRect(rect);
            laserSprite.SetCenter(Vec3(0.0f, rect.bottom * 0.5f, 0.0f));


            laserTransform.SetOrientation(CQuaternion(0.0f, 0.0f, (float)i / m_lasers.size() * 2 * PI));

            if (m_currentLaserState == LASER_STATE_LOADING)
            {
                float alpha = 1.0f - m_currentLaserTime / m_laserLoadingTime;

                if (alpha < 0.2f)
                    laserTransform.SetScale(Vec3((alpha / 0.2f) * laserDistance / LASER_TEXTURE_WIDTH, 1.0f, 1.0f));
                else
                    laserTransform.SetScale(Vec3(laserDistance / LASER_TEXTURE_WIDTH, 1.0f, 1.0f));

                laserSprite.SetColor(Vec4(m_laserColor.x, m_laserColor.y, m_laserColor.z, alpha * 0.3f));

                m_bGhost = true;
            }
            else
            {
                m_bGhost = false;

                laserTransform.SetScale(Vec3(laserDistance / LASER_TEXTURE_WIDTH, 1.0f, 1.0f));

                laserSprite.SetColor(Vec4(m_laserColor.x, m_laserColor.y, m_laserColor.z, 1.0f));

                if (pCharacter)
                {
                    //RemovePolygons();
                    CPolygon poly;
                    if (!bUseProjectedCollision)
                    {
                        poly.SetNumPoints(2);
                        poly.AddPoint(Vec3(0.0f, 0.0f, 0.0f));
                        poly.AddPoint(Vec3(LASER_TEXTURE_WIDTH, 0.0f, 0.0f));
                        ReplacePolygon(i, poly);
                        ApplyTransform(i, mtxScaling * mtxRotation * GetMeshInstanceNode(m_attachedMeshInstIndex)->GetDerivedRST());
                    }
                    else
                    {
                        poly.SetNumPoints(2);
                        poly.AddPoint(intersectLevel + Vec3(-10.0f, 0.0f, 0.0f));
                        poly.AddPoint(intersectLevel + Vec3(10.0f, 0.0f, 0.0f));
                        ReplacePolygon(i, poly);
                        //collidable.ApplyTransform(mtxScaling * mtxRotation * m_pMeshNode->GetDerivedRST());

                        //intersectLevel
                    }

                    //collidable.AddPolygon(poly);
                    //collidable.ApplyTransform(mtxScaling * mtxRotation * m_pMeshNode->GetDerivedRST());

                    //if(collidable.HasCollision(*pCharacter))
                    {
                        //gEngine->GetWorldMgr()->SetPlayerKilled(true);
                    }
                }
            }


        }
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CMeshEntity::AddToRenderQueue(dt, queue);

        if (m_currentLaserState == LASER_STATE_PAUSED || m_bDisabled)
            return;

        for (int i = 0; i < m_lasers.size(); ++i)
        {
            queue.m_spriteRenderObjects.push_back(m_lasers[i].GetRenderObject());
        }
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("LaserRadiator01.x");
        SetLaserCount(m_numLasers);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        SetLaserCount(m_numLasers);

        RemovePolygons();

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentLaserTime = m_laserPausingTime;
        m_currentLaserState = LASER_STATE_PAUSED;

        m_time = 0.0f;

        GetMeshInstanceNode(m_attachedMeshInstIndex)->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));

        m_bGhost = false;
        m_bDisabled = false;
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::SetLaserCount(unsigned int numLasers)
    {
        m_numLasers = numLasers;

        for (int i = 0; i < m_nodes.size(); ++i)
        {
            if (m_nodes[i])
                gEngine->GetSceneMgr()->RemoveNode(m_nodes[i]->GetName());
        }

        if (m_particles.size() != numLasers)
        {
            for (int i = 0; i < m_particles.size(); ++i)
            {
                if (m_particles[i])
                {
                    gEngine->GetEntityMgr()->RemoveEntity(m_particles[i]->GetName());
                    m_particles[i] = nullptr;
                }
            }
        }

        m_lasers.resize(numLasers);
        m_nodes.resize(numLasers);
        m_particles.resize(numLasers);

        CallForEachNode([](auto& pNode) { pNode->SetOrientation(CQuaternion()); });

        SetRotationSpeedX(0.0f);

        for (int i = 0; i < m_lasers.size(); ++i)
        {
            Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), i * (2.0f * PI) / (float)m_lasers.size(), Vec3(0.0f, 0.0f, 1.0f)));

            Vec3 laserPos(m_laserOffsetRadius, 0.0f, 0.0f);
            laserPos = Vec3(Vec4(laserPos, 1.0f) * mtxRot);

            m_nodes[i] = gEngine->GetSceneMgr()->CreateSceneNode();
            m_nodes[i]->SetPosition(laserPos);
            
            m_lasers[i].AttachNode(m_nodes[i]);
            m_lasers[i].SetMaterialFilename("Sprites/laser.mtrl");
        }

        for (int i = 0; i < m_particles.size(); ++i)
        {
            if (m_particles[i] == nullptr)
            {
                m_particles[i] = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
                m_particles[i]->SetAutoDelete(false);
                m_particles[i]->SetDeletable(false);
                m_particles[i]->SetRender(false);
                m_particles[i]->LoadFromTemplate("LASER_HITPOINT");
                m_particles[i]->SetEmitNewParticles(false);
                m_particles[i]->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -5.0f));
            }
        }

        SetAttachedMeshInstanceIndex(m_attachedMeshInstIndex);
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case LASERRADIATOR_EVENT_ENABLE:
            m_bDisabled = false;
            break;

        case LASERRADIATOR_EVENT_DISABLE:
            m_bDisabled = true;
            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        if (m_attachedMeshInstIndex >= GetMeshInstanceContainer().GetNumMeshInstances())
            SetAttachedMeshInstanceIndex(0);
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorEntity::SetAttachedMeshInstanceIndex(unsigned int index)
    {
        m_attachedMeshInstIndex = index;

        for (auto& pNode : m_nodes)
        {
            GetMeshInstanceNode(m_attachedMeshInstIndex)->AttachChildNode(pNode);
        }
    }

    ENTITY_TYPEID CLaserRadiatorAimingEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_LASERRADIATOR_AIMING;

    //-----------------------------------------------------------------------------------
	CLaserRadiatorAimingEntity::CLaserRadiatorAimingEntity()
		: m_radius(300.0f)
		, m_growDuration(0.2f)
		, m_shrinkDuration(0.2f)
		, m_pCharacter(nullptr)
		, m_pLaserNode(nullptr)
		, m_pLaserParticlesHitpoint(nullptr)
		, m_bDisabled(false)
		, m_bLaserActive(false)
		, m_offset(200.0f)
		, m_pSound(nullptr)
    {
        SetMetaTypeID(CLaserRadiatorAimingEntity::ENTITY_META_TYPEID);

        RegisterEvent("[LaserRadiator] SetRadius", LASERRADIATOR_AIMING_EVENT_SET_RADIUS);
        RegisterEventParameter(LASERRADIATOR_AIMING_EVENT_SET_RADIUS, "Radius", CBaseParam::PARAMETER_TYPE_FLOAT, 300.0f, 0.0f, 100000.0f);

        RegisterEvent("[LaserRadiator] SetDisabled", LASERRADIATOR_AIMING_EVENT_SET_DISABLED);
        RegisterEventParameter(LASERRADIATOR_AIMING_EVENT_SET_DISABLED, "Disabled", CBaseParam::PARAMETER_TYPE_BOOL, true);

    }

	//-----------------------------------------------------------------------------------
	void CLaserRadiatorAimingEntity::SetupSounds()
	{
		if (m_pSound == nullptr)
		{
			m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
			m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			m_pSound->SetDeletable(false);
			m_pSound->SetAutoDelete(false);
			m_pLaserParticlesHitpoint->GetNode()->AttachChildNode(m_pSound->GetNode());
		}

		m_pSound->SetAudibleDistance(700.0f);
		m_pSound->SetPeakVolumeDistance(150.0f);
		m_pSound->SetVolume(1.0f);
		m_pSound->LoadSound("Entities/LaserRadiator.wav");
	}

    //-----------------------------------------------------------------------------------
    CLaserRadiatorAimingEntity::~CLaserRadiatorAimingEntity()
    {
        gEngine->GetSceneMgr()->RemoveNode(m_pLaserNode);
        gEngine->GetEntityMgr()->RemoveEntity(m_pLaserParticlesHitpoint);
		gEngine->GetEntityMgr()->RemoveEntity(m_pSound);
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorAimingEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

        SetGhost(true);

#ifndef ENV_FINAL
        for (auto it = m_targets.begin(); it != m_targets.end();)
        {
            if (!gEngine->GetEntityMgr()->IsValid(*it))
            {
                if (m_pCurrentTarget == *it)
                    m_pCurrentTarget = nullptr;

                it = m_targets.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (!gEngine->GetEntityMgr()->IsValid(m_pCharacter))
            m_pCharacter = nullptr;
#endif

        m_bLaserActive = false;

        if (!m_pCharacter || m_targets.empty() || m_bDisabled)
        {
            m_pLaserParticlesHitpoint->SetRenderParticles(false);

            return;
        }

        float targetDistance = FLT_MAX;

        CEntity* pNearestTarget = nullptr;

        Vec3 charPos = m_pCharacter->GetNode()->GetDerivedPosition();
        Vec3 targetWaypointPos0;
        Vec3 targetWaypointPos1;
        Vec3 targetWaypointDir;
        Vec3 targetProjCharPos;

        float targetSpeed = 0.0f;

        for (const auto& pTarget : m_targets)
        {
            CPathEntity* pTargetPath = dynamic_cast<CPathEntity*>(pTarget);

            if (!pTargetPath)
                continue;

            if (pTargetPath->GetNumWaypoints() != 2)
                continue;

            Vec3 waypointPos0 = pTargetPath->GetWaypoint(0)->GetNode()->GetDerivedPosition();
            Vec3 waypointPos1 = pTargetPath->GetWaypoint(1)->GetNode()->GetDerivedPosition();
            Vec3 waypointDir = waypointPos1 - waypointPos0;

            Vec3 projCharPos;
            float t = ComputePointOnLineProjection(charPos, waypointPos0, waypointDir);

            if (t < 0.0f || t > 1.0f)
                continue;

            projCharPos = waypointPos0 + waypointDir * t;

            float dist = glm::length(projCharPos - charPos);

            if (dist < targetDistance &&
                dist < m_radius)
            {
                targetDistance = dist;
                pNearestTarget = pTarget;

                targetWaypointPos0 = waypointPos0;
                targetWaypointPos1 = waypointPos1;
                targetProjCharPos = projCharPos;
                targetWaypointDir = glm::normalize(waypointDir);

                if (t < 0.5f)
                    targetSpeed = pTargetPath->GetWaypoint(0)->GetSpeed();
                else
                    targetSpeed = pTargetPath->GetWaypoint(1)->GetSpeed();
            }
        }

        float minTargetDistance = m_offset;

        if (m_shrinkTime < 0.0f && m_growTime < 0.0f && pNearestTarget != m_pCurrentTarget)
        {
            if (m_pCurrentTarget == nullptr)
            {
                m_targetVelocity = Vec3(0.0f);

                // New target => grow
                m_growTime = m_growDuration;
                m_shrinkTime = -1.0f;

                m_pCurrentTarget = pNearestTarget;

                // Find target start position
                float distWaypoint0 = glm::length(targetWaypointPos0 - targetProjCharPos);
                float distWaypoint1 = glm::length(targetWaypointPos1 - targetProjCharPos);

                if (distWaypoint0 > (minTargetDistance * 0.4f) &&
                    distWaypoint1 < (minTargetDistance * 0.4f))
                {
                    m_targetPos = targetProjCharPos - targetWaypointDir * std::min(minTargetDistance, distWaypoint0);
                }
                else if (distWaypoint0 < (minTargetDistance * 0.4f) &&
                         distWaypoint1 >(minTargetDistance * 0.4f))
                {
                    m_targetPos = targetProjCharPos + targetWaypointDir * std::min(minTargetDistance, distWaypoint1);
                }
                else
                {
                    // Both waypoints have min distance; choose the one which is in the opposite direction of the characters movement
                    Vec2 charVel = static_cast<CCharacter3DEntity*>(m_pCharacter)->GetVelocityFloatingPoint();
                    
                    if (glm::dot(charVel, Vec2(targetWaypointDir)) > 0.0f)
                    {
                        m_targetPos = targetProjCharPos - targetWaypointDir * std::min(minTargetDistance, distWaypoint0);// targetWaypointPos0;
                    }
                    else
                    {
                        m_targetPos = targetProjCharPos + targetWaypointDir * std::min(minTargetDistance, distWaypoint1); //targetWaypointPos1;
                    }
                }




//                 else if (distWaypoint0 > minTargetDistance)
//                 {
//                     m_targetPos = targetProjCharPos - targetWaypointDir * minTargetDistance; // targetWaypointPos0;
//                 }
//                 else if (distWaypoint1 > minTargetDistance)
//                 {
//                     m_targetPos = targetProjCharPos + targetWaypointDir * minTargetDistance; // targetWaypointPos1;
//                 }
//                 else
//                 {
//                     if (distWaypoint0 > distWaypoint1)
//                         m_targetPos = targetWaypointPos0;
//                     else
//                         m_targetPos = targetWaypointPos1;
//                 }
            }
            else
            {
                // Switch target or no target => shrink
                m_pLaserParticlesHitpoint->SetRenderParticles(false);

                m_growTime = -1.0f;
                m_shrinkTime = m_shrinkDuration;
            }
        }
        
        float scaling = 1.0f;
        if (m_growTime >= 0.0f)
        {
            scaling = 1.0f - Clamp(m_growTime / m_growDuration, 0.0f, 1.0f);
			m_pSound->SetVolume(scaling);
            m_growTime -= dt;
        }
        else  if (m_shrinkTime >= 0.0f)
        {
            scaling = Clamp(m_shrinkTime / m_shrinkDuration, 0.0f, 1.0f);
			m_pSound->SetVolume(scaling);
            m_shrinkTime -= dt;

            if (m_shrinkTime < 0.0f)
            {
                scaling = 1.0f;
                m_pCurrentTarget = nullptr;
            }
        }
        else
        {
            // Not shrinking, not growing => switch target if necessary
            m_pCurrentTarget = pNearestTarget;

            if (m_pCurrentTarget)
            {
                 Vec3 targetDir = glm::normalize(targetProjCharPos - m_targetPos);

                if (glm::dot(m_targetVelocity, targetDir) < 0.0f)
                {
                    // switch target
                    m_pLaserParticlesHitpoint->SetRenderParticles(false);

                    m_growTime = -1.0f;
                    m_shrinkTime = m_shrinkDuration;
                }
                else
                {
                    m_targetVelocity = targetDir * dt * targetSpeed;

                    m_targetPos += targetDir * dt * targetSpeed;

                    m_pLaserParticlesHitpoint->SetRenderParticles(true);
                }
            }
        }

        if (!m_pCurrentTarget)
            return;

        Vec3 targetDir = glm::normalize(m_targetPos - m_pNode->GetDerivedPosition());

        float laserDistance = glm::length(m_targetPos - m_pNode->GetDerivedPosition());

        if (abs(targetDir.x) < abs(targetDir.y))
        {
            CQuaternion rotationTowardsDirection(glm::normalize(targetDir), Vec3(-1.0f, 0.0f, 0.0f));
            m_pNode->SetOrientation(rotationTowardsDirection, true);
        }
        else
        {
            CQuaternion rotationTowardsDirection(glm::normalize(targetDir), Vec3(0.0f, -1.0f, 0.0f));
            m_pNode->SetOrientation(rotationTowardsDirection, true);
        }

        m_pLaserNode->SetScale(Vec3(scaling * laserDistance / LASER_TEXTURE_WIDTH, 2.0f, 1.0f));
        m_pLaserNode->SetOrientation(CQuaternion(-PI * 0.5f, 0.0f, 0.0f));

        TextureRect rect;
        m_laser.GetAbsoluteRect(rect);
        m_laser.SetCenter(Vec3(0.0f, rect.bottom * 0.5f, 0.0f));

        RemoveCircles();
        SetGhost(false);

       if (m_growTime < 0.0f && m_shrinkTime < 0.0f)
        {
            CCircle c;
            c.SetRadius(8);
            AddCircle(c);
        }

        m_pLaserParticlesHitpoint->GetNode()->SetPosition(m_targetPos + Vec3(0.0f, 0.0f, -5.0f));

        ApplyTransform(math::translation(m_targetPos), 1.0f);

        m_bLaserActive = true;
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorAimingEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CSpriteEntity::AddToRenderQueue(dt, queue);

        if (!GetGhost())
            queue.m_spriteRenderObjects.push_back(m_laser.GetRenderObject());
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorAimingEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        m_pLaserNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pNode->AttachChildNode(m_pLaserNode);

        m_laser.AttachNode(m_pLaserNode);
        m_laser.SetMaterialFilename("Sprites/laser.mtrl");

        InitParticles();
		SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorAimingEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        m_laser.AttachNode(m_pLaserNode);
        m_laser.SetMaterialFilename("Sprites/laser.mtrl");
        m_laser.SetColor(Vec4(1.0f, 0.25f, 0.0f, 1.4f));

        InitParticles();
		SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CLaserRadiatorAimingEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_pLaserParticlesHitpoint->SetRenderParticles(false);

        m_pCharacter = gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_CHARACTER3D);
        m_pCurrentTarget = nullptr;

        m_growTime = -1.0f;
        m_shrinkTime = -1.0f;

        m_bLaserActive = false;
    }

    void CLaserRadiatorAimingEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case LASERRADIATOR_AIMING_EVENT_SET_RADIUS:
            if (eventParameter.IsParameterSet("Radius"))
                eventParameter.GetParameter("Radius", m_radius);
            break;

        case LASERRADIATOR_AIMING_EVENT_SET_DISABLED:
            if (eventParameter.IsParameterSet("Disabled"))
                eventParameter.GetParameter("Disabled", m_bDisabled);
            break;

        default:
            CSpriteEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    void CLaserRadiatorAimingEntity::InitParticles()
    {
        if (m_pLaserParticlesHitpoint == nullptr)
        {
            m_pLaserParticlesHitpoint = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
            m_pLaserParticlesHitpoint->SetAutoDelete(false);
            m_pLaserParticlesHitpoint->SetDeletable(false);
        }

        m_pLaserParticlesHitpoint->LoadFromTemplate("LASER_HITPOINT");
        m_pLaserParticlesHitpoint->SetEmitNewParticles(false);
    }

} // env