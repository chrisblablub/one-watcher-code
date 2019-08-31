#include <Engine/Entity/HunterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Graphic/Core/Mesh.h>

namespace env
{
    ENTITY_TYPEID CHunterBaseEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_HUNTER;

    //-----------------------------------------------------------------------------------
    CHunterBaseEntity::CHunterBaseEntity()
        : m_currentState(HUNTER_STATE_PAUSED)
        , m_pCharacter(nullptr)
        , m_pTarget(nullptr)
        , m_targetVelocity(600.0f)
        , m_acceleration(1500.0f)
        , m_startPos(0.0f, 0.0f, 0.0f)
        , m_bTargetBraking(true)
        , m_bCurrentTargetBraking(false)
        , m_attackDir(0.0f, 0.0f, 0.0f)
        , m_targetPos(0.0f, 0.0f, 0.0f)
        , m_radius(1600.0f)
        , m_aiming(0.2f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CHunterBaseEntity::~CHunterBaseEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    bool CHunterBaseEntity::HasCollision(CEntity& e)
    {
        if (e.GetMetaTypeId() == ENTITY_TYPEID_CHARACTER && CGeometryCollidable::HasCollision(e))
        {
            //gEngine->GetWorldMgr()->SetPlayerKilled(true);
            return true;
        }


        return false;
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("SawbladeNew01.x");

        m_pNode->SetScale(Vec3(0.4f, 0.4f, 0.4f));

        CCircle c;
        c.SetRadius(15.0f);
        AddCircle(c);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_bGhost = false;

    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->LoadSound("Entities/Hunter.wav");

        m_pSound->SetVolume(0.7f);
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_pCharacter = static_cast<CCharacter2DEntity*>(gEngine->GetEntityMgr()->GetCharacter());

        m_pNode->SetPosition(m_startPos);

        ApplyTransform(m_pNode->GetDerivedRST(), 1.0f);

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(true); });

        m_bCurrentTargetBraking = false;

        m_hunterPhysics.Reset();
        m_stateTimer.Reset();

        m_bGhost = false;

        if (!bGameMode)
            SetState(HUNTER_STATE_INACTIVE);
        else
        {
            if (m_pTarget)
                SetState(HUNTER_STATE_FLYINGTOTARGET);
            else
                SetState(HUNTER_STATE_SEARCHING);
        }
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CHunterBaseEntity::Update);

        CMeshEntity::Update(dt);

        if (!m_pCharacter)
            return;

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this || !GetGameMode())
        {
            m_startPos = m_pNode->GetDerivedPosition();
            return;
        }

        m_stateTimer.Update(dt);

        if (m_currentState == HUNTER_STATE_PAUSED)
        {
            UpdatePaused(dt);
        }
        else if (m_currentState == HUNTER_STATE_SEARCHING)
        {
            UpdateSearching(dt);
        }
        else if (m_currentState == HUNTER_STATE_ATTACKINGTARGET)
        {
            UpdateAttackingTarget(dt);
        }
        else if (m_currentState == HUNTER_STATE_FLYINGTOTARGET)
        {
            UpdateFlyingToTarget(dt);
        }
        else if (m_currentState == HUNTER_STATE_INACTIVE)
        {
            UpdateInactive(dt);
        }

        ApplyTransform(m_pNode->GetDerivedRST(), 1.0f);

        float velo = glm::length(m_hunterPhysics.GetVelocity());
        SetRotationSpeedZ(2.5f + abs(velo / 75.0f));
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::UpdatePaused(float dt)
    {
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::UpdateInactive(float dt)
    {
        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::UpdateFlyingToTarget(float dt)
    {
        //ENV_LOG_INFO("UpdateFlyingToTarget");

        if (m_pTarget)
            m_targetPos = m_pTarget->GetNode()->GetDerivedPosition();


        float targetDistance = glm::length(m_targetPos - m_pNode->GetDerivedPosition());
        if (targetDistance < 5.0f)
        {
            m_hunterPhysics.SetGravity(false, Vec2(0.0f, 0.0f));
            m_hunterPhysics.SetVelocity(Vec2(0.0f, 0.0f));

            SetState(HUNTER_STATE_SEARCHING);
        }
        else
        {
            Vec3 targetDir = glm::normalize(m_targetPos - m_pNode->GetDerivedPosition());

            if (targetDistance > 35.0f)
            {
                m_hunterPhysics.SetGravity(true, Vec2(targetDir) * m_acceleration);
                if (m_hunterPhysics.GetVelocityScalar() > m_targetVelocity)
                    m_hunterPhysics.SetGravity(false, Vec2(0.0f, 0.0f));
            }
            else
            {
                if (!m_bCurrentTargetBraking)
                {
                    float velo = m_hunterPhysics.GetVelocityScalar();
                    float a = 0.5 * velo * velo / 35.0f;

                    m_hunterPhysics.SetGravity(true, -Vec2(targetDir) * a);
                }
                m_bCurrentTargetBraking = true;
            }

            m_hunterPhysics.Update(dt);

            m_pNode->SetPosition(m_pNode->GetPosition() + Vec3(m_hunterPhysics.GetVelocity().x, m_hunterPhysics.GetVelocity().y, 0.0f) * dt);
        }

    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::UpdateSearching(float dt)
    {
        //ENV_LOG_INFO("UpdateSearching");

        Vec3 vecDistance = m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition();
        float distance = glm::length(vecDistance);
        m_bCurrentTargetBraking = false;
        if (distance < m_radius)
        {
            m_attackDir = glm::normalize(m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());
            SetState(HUNTER_STATE_ATTACKINGTARGET);
        }

        m_hunterPhysics.Update(dt);
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::UpdateAttackingTarget(float dt)
    {
        //ENV_LOG_INFO("UpdateAttackingTarget");

        Vec3 pos = m_pNode->GetDerivedPosition();

        Vec3 attackDir = m_pCharacter->GetNode()->GetDerivedPosition() - pos;
        Vec2 veloDir = glm::normalize(m_hunterPhysics.GetVelocity());

        float angle = glm::dot(attackDir, Vec3(veloDir.x, veloDir.y, 0.0f));
        if (m_hunterPhysics.GetVelocityScalar() <= 5.0f)
            angle = 0.0f;

        //pos += VECTOR3(m_hunterPhysics.GetVelocity().x * dt, m_hunterPhysics.GetVelocity().y * dt, 0);
        Vec2 curVelocity = m_hunterPhysics.GetVelocity() * dt;
        VECTOR2FIXED pos_fixed(pos.x * (float)CELL_SCALE, pos.y * (float)CELL_SCALE);
        VECTOR2FIXED velocity_fixed(curVelocity.x * (float)CELL_SCALE, curVelocity.y * (float)CELL_SCALE);

        CTerrainCollidable collidable;
        collidable.height = 26;
        collidable.width = 26;
        collidable.position = pos_fixed;
        collidable.velocity = velocity_fixed;
        CollisionState res = gEngine->GetWorldMgr()->GetLevelMgr()->ComputeMovement(collidable);

        pos = Vec3((float)res.pos.x / (float)CELL_SCALE, (float)res.pos.y / (float)CELL_SCALE, m_pNode->GetPosition().z);

        if (res.bHitCeiling)
        {
            Vec2 velo = m_hunterPhysics.GetVelocity();
            if (velo.y > 0.0f)
                velo.y *= 0.2f;
            m_hunterPhysics.SetVelocity(velo);
        }
        if (res.bOnFloor)
        {
            Vec2 velo = m_hunterPhysics.GetVelocity();
            if (velo.y < 0.0f)
                velo.y *= 0.2f;
            m_hunterPhysics.SetVelocity(velo);
        }
        if (res.bHitWallFromleft)
        {
            Vec2 velo = m_hunterPhysics.GetVelocity();
            if (velo.x > 0.0f)
                velo.x *= 0.2f;
            m_hunterPhysics.SetVelocity(velo);
        }
        if (res.bHitWallFromRight)
        {
            Vec2 velo = m_hunterPhysics.GetVelocity();
            if (velo.x < 0.0f)
                velo.x *= 0.2f;
            m_hunterPhysics.SetVelocity(velo);
        }

        m_collisionState = res;


        Vec3 vecDistance = m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition();
        float distance = glm::length(vecDistance);

        if (!m_bCurrentTargetBraking && angle < 0.0f && m_hunterPhysics.GetVelocityScalar() > m_targetVelocity)
        {
            if (m_bTargetBraking)
                m_bCurrentTargetBraking = true;
        }
        if (m_bCurrentTargetBraking)
        {
            Vec2 velo = glm::normalize(m_hunterPhysics.GetVelocity());

            if (glm::dot(velo, Vec2(attackDir)) < 0.0f)
                velo = -velo;

            m_hunterPhysics.SetGravity(true, velo * m_acceleration * distance / 150.0f);

            float velocityScalar = m_hunterPhysics.GetVelocityScalar();

            if (velocityScalar < 30.0f)
            {
                m_hunterPhysics.SetGravity(false, Vec2(0, 0));
                SetState(HUNTER_STATE_SEARCHING);
            }
        }
        else
        {
            Vec3 projPoint(0.0f, 0.0f, 0.0f);
            Vec3 gravity(0.0f, 0.0f, 0.0f);
            float alpha = 0.0f;
            if (m_hunterPhysics.GetVelocityScalar() > 1.0f)
            {
                float param = ComputePointOnLineProjection(m_pCharacter->GetNode()->GetDerivedPosition(),
                                                           m_pNode->GetDerivedPosition(), m_attackDir);

                if (abs(param) > 0.05)
                {
                    projPoint = m_pNode->GetDerivedPosition() + param * m_attackDir;

                    gravity = Vec3(m_pCharacter->GetNode()->GetDerivedPosition() - projPoint);
                    if (glm::length(gravity) > 0.1)
                    {
                        gravity = glm::normalize(gravity);
                        alpha = m_aiming;
                    }

                }

            }



            Vec2 dir(glm::normalize(m_attackDir));
            dir = glm::normalize(alpha * Vec2(gravity) + (1.0f - alpha) * dir);
            m_hunterPhysics.SetGravity(true, Vec2(dir) * m_acceleration);

            if (m_hunterPhysics.GetVelocityScalar() > m_targetVelocity * 10)
                m_hunterPhysics.SetGravity(false, Vec2(0.0f, 0.0f));
        }


        m_hunterPhysics.Update(dt);
        m_pNode->SetPosition(pos);
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::SetState(HUNTER_STATE state)
    {
        m_currentState = state;
        m_stateTimer.Reset();
    }

    //-----------------------------------------------------------------------------------
    void CHunterBaseEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CMeshEntity::AddToRenderQueue(dt, queue);

    }

    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CHuntingHunterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_HUNTINGHUNTER;

    //-----------------------------------------------------------------------------------
    CHuntingHunterEntity::CHuntingHunterEntity()
        : m_pParticleSys(nullptr)
        , m_pLight(nullptr)
        , m_state(HUNTINGHUNTER_STATE_ENABLED)
        , m_currentStateTime(-1.0f)
    {
        RegisterEvent("[HuntingHunter] Enable", HUNTINGHUNTER_EVENT_ENABLE);
        RegisterEvent("[HuntingHunter] Disable", HUNTINGHUNTER_EVENT_DISABLE);
        RegisterEvent("[HuntingHunter] Spawn", HUNTINGHUNTER_EVENT_SPAWN);
        RegisterEvent("[HuntingHunter] Vanish", HUNTINGHUNTER_EVENT_VANISH);

        SetMetaTypeID(CHuntingHunterEntity::ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CHuntingHunterEntity::~CHuntingHunterEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pLight);
        gEngine->GetEntityMgr()->RemoveEntity(m_pParticleSys);
    }

    //-----------------------------------------------------------------------------------
    void CHuntingHunterEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case HUNTINGHUNTER_EVENT_ENABLE:
        {
            m_bGhost = false;

            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(true); });

            m_pParticleSys->SetRenderParticles(true);

            m_pLight->GetLight()->SetRenderInGame(true);

            m_state = HUNTINGHUNTER_STATE_ENABLED;
        }
        break;

        case HUNTINGHUNTER_EVENT_DISABLE:
        {
            m_bGhost = true;

            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });

            m_pLight->GetLight()->SetRenderInGame(false);

            m_pParticleSys->SetRenderParticles(false);

            m_state = HUNTINGHUNTER_STATE_DISABLED;

        }
        break;

        case HUNTINGHUNTER_EVENT_SPAWN:

            m_currentStateTime = 0.5f;

            m_bGhost = false;

            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(true); });

            m_pParticleSys->SetRenderParticles(true);

            m_pLight->GetLight()->SetRenderInGame(true);


            m_state = HUNTINGHUNTER_STATE_SPAWNING;

            break;

        case HUNTINGHUNTER_EVENT_VANISH:

            m_currentStateTime = 0.5f;

            m_bGhost = true;

            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });

            m_pLight->GetLight()->SetRenderInGame(false);

            m_pParticleSys->SetRenderParticles(false);

            m_state = HUNTINGHUNTER_STATE_VANISHING;


            break;

        default:
            CHunterBaseEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CHuntingHunterEntity::OnGameModeChanged(bool bGameMode)
    {
        CHunterBaseEntity::OnGameModeChanged(bGameMode);

        m_pNode->SetScale(HUNTINGHUNTER_SCALE);
    }

    //-----------------------------------------------------------------------------------
    void CHuntingHunterEntity::InitializeAtCreation()
    {
        CHunterBaseEntity::InitializeAtCreation();

        if (!m_pParticleSys)
        {
            m_pParticleSys = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
            m_pParticleSys->LoadFromTemplate("HUNTER_CORE");
            m_pParticleSys->SetPickable(false);
            m_pParticleSys->SetAutoDelete(false);
            m_pParticleSys->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -1.0f));
            m_pNode->AttachChildNode(m_pParticleSys->GetNode());
        }

        if (!m_pLight)
        {
            m_pLight = static_cast<CLightSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHTSPRITE));
            m_pLight->SetRenderGame(false);
            m_pLight->SetRenderEditor(false);
            m_pLight->SetPickable(true);
            m_pLight->SetAutoDelete(false);

            m_pLight->GetLight()->SetInitialEnergy(7.0f);
            m_pLight->GetLight()->SetInitialDiffuseColor(Vec4(1.0f, 0.2f, 0.0f, 1.0f));

            m_pLight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -40.0f));
            m_pNode->AttachChildNode(m_pLight->GetNode());
        }
    }

    //-----------------------------------------------------------------------------------
    void CHuntingHunterEntity::InitializeAtLoad()
    {
        CHunterBaseEntity::InitializeAtLoad();

        m_pParticleSys->LoadFromTemplate("HUNTER_CORE");

        if (!m_pLight)
        {
            m_pLight = static_cast<CLightSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHTSPRITE));
            m_pLight->SetRenderGame(false);
            m_pLight->SetRenderEditor(false);
            m_pLight->SetPickable(true);
            m_pLight->SetAutoDelete(false);

            m_pLight->GetLight()->SetInitialEnergy(7.0f);
            m_pLight->GetLight()->SetInitialDiffuseColor(Vec4(1.0f, 0.2f, 0.0f, 1.0f));

            m_pLight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -40.0f));
            m_pNode->AttachChildNode(m_pLight->GetNode());
        }
    }

    //-----------------------------------------------------------------------------------
    void CHuntingHunterEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CHuntingHunterEntity::Update);

        if (!GetGameMode())
        {
            CHunterBaseEntity::Update(dt);
        }
        else
        {
            if (m_state == HUNTINGHUNTER_STATE_DISABLED)
            {
                CMeshEntity::Update(dt);
            }
            else if (m_state == HUNTINGHUNTER_STATE_ENABLED)
            {
                CHunterBaseEntity::Update(dt);
            }
            else if (m_state == HUNTINGHUNTER_STATE_SPAWNING)
            {
                CMeshEntity::Update(dt);

                m_currentStateTime -= dt;

                float alpha = 1.0f - m_currentStateTime / 0.5f;

                m_pNode->SetScale(HUNTINGHUNTER_SCALE * alpha);

                if (m_currentStateTime < 0.0f)
                    OnInput(HUNTINGHUNTER_EVENT_ENABLE, EventParameter());
            }
            else if (m_state == HUNTINGHUNTER_STATE_VANISHING)
            {
                CMeshEntity::Update(dt);

                m_currentStateTime -= dt;

                float alpha = m_currentStateTime / 0.5f;

                m_pNode->SetScale(HUNTINGHUNTER_SCALE * alpha);

                if (m_currentStateTime < 0.0f)
                    OnInput(HUNTINGHUNTER_EVENT_DISABLE, EventParameter());
            }
        }


    }

    namespace
    {
        const float BLINK_TIME = 0.25f;
    }
    
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CExplodingHunterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_EXPLODINGHUNTER;

    //-----------------------------------------------------------------------------------
    CExplodingHunterEntity::CExplodingHunterEntity()
        : m_pExplosion(nullptr)
        , m_pHunterCore(nullptr)
        , m_lifeTime(1.0f)
        , m_currentBlinkTime(BLINK_TIME)
        , m_bBlinkAppear(false)
    {
        SetMetaTypeID(CExplodingHunterEntity::ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CExplodingHunterEntity::~CExplodingHunterEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pExplosion);
        gEngine->GetEntityMgr()->RemoveEntity(m_pHunterCore);
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::InitializeAtCreation()
    {
        CHunterBaseEntity::InitializeAtCreation();

        SetMeshFilename("SawbladeNew01.x");

        m_pNode->SetScale(Vec3(0.4f, 0.4f, 0.4f));

        m_pExplosion = static_cast<CExplosionEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLOSION));
        m_pExplosion->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_pExplosion->SetRenderEditor(false);
        m_pExplosion->SetRenderGame(false);
        m_pExplosion->SetPickable(false);
        m_pExplosion->SetAutoDelete(false);
        m_pExplosion->SetRadius(60.0f);

        m_pHunterCore = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
        m_pHunterCore->LoadFromTemplate("HUNTER_CORE2");
        m_pHunterCore->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_pHunterCore->SetRenderEditor(false);
        m_pHunterCore->SetRenderGame(false);
        m_pHunterCore->SetPickable(false);
        m_pHunterCore->SetAutoDelete(false);
        m_pHunterCore->GetNode()->SetScale(Vec3(2.5f));

        m_pNode->AttachChildNode(m_pHunterCore->GetNode());

        m_lifeTime = GetRandomFloat(3.5f - 0.2f, 3.5f + 0.2f);

        m_sprite.SetMaterialFilename("Sprites/laser_mine.png");
        m_sprite.AttachNode(&m_spriteNode);
        m_sprite.SetColor(Vec4(0.0f, 0.5f, 1.0f, 1.0f));
        m_spriteNode.SetScale(Vec3(0.2f, 0.2f, 0.2f));

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::InitializeAtLoad()
    {
        CHunterBaseEntity::InitializeAtLoad();

        m_sprite.SetMaterialFilename("Sprites/laser_mine.png");
        m_sprite.AttachNode(&m_spriteNode);
        m_sprite.SetColor(Vec4(0.0f, 0.5f, 1.0f, 1.0f));
        m_spriteNode.SetScale(Vec3(0.2f, 0.2f, 0.2f));

        m_pExplosion->SetRadius(60.0f);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::OnGameModeChanged(bool bGameMode)
    {
        CHunterBaseEntity::OnGameModeChanged(bGameMode);

        SetGhost(false);
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CHunterBaseEntity::AddToRenderQueue(dt, queue);

        if (m_currentState != HUNTER_STATE_INACTIVE)
            queue.m_spriteRenderObjects.push_back(m_sprite.GetRenderObject());
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CExplodingHunterEntity::Update);

        CHunterBaseEntity::Update(dt);

        m_lifeTimer.Update(dt);

        if (m_lifeTimer.GetTime() > m_lifeTime)
        {
            m_pExplosion->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + Vec3(0.0f, 0.0f, -25.0f));
            m_pExplosion->SetEnabled();

            SetGhost(true);
            SetState(HUNTER_STATE_INACTIVE);

            m_sprite.SetAlpha(0.0f);

            m_lifeTimer.Reset();
            m_lifeTimer.Stop();

        }
        else
        {
            m_spriteNode.SetPosition(m_pNode->GetDerivedPosition() + Vec3(0.0f, 0.0f, 0.0f));

            m_currentBlinkTime -= dt;
            if (m_currentBlinkTime < 0.0f)
            {
                m_bBlinkAppear = !m_bBlinkAppear;
                m_currentBlinkTime = BLINK_TIME;
            }
            else
            {
                float alpha = m_currentBlinkTime / BLINK_TIME;
                if (m_bBlinkAppear)
                    alpha = 1.0f - alpha;

                m_sprite.SetAlpha(alpha);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::UpdateAttackingTarget(float dt)
    {
        CHunterBaseEntity::UpdateAttackingTarget(dt);

         if (!m_pCharacter)
            return;

        Vec3 targetDir = m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition();
        float targetDistance = glm::length(targetDir);

        if (targetDistance < 70.0f)
        {
            m_pExplosion->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + Vec3(0.0f, 0.0f, -15.0f));
            m_pExplosion->SetEnabled();

            m_sprite.SetAlpha(0.0f);

            SetGhost(true);
            SetState(HUNTER_STATE_INACTIVE);
        }
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::UpdateInactive(float dt)
    {
        CHunterBaseEntity::UpdateInactive(dt);

        m_pHunterCore->SetRenderParticles(false);

        if (m_stateTimer.GetTime() > 2.0f && !GetPersistent())
            gEngine->GetEntityMgr()->KillEntity(this);
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEntity::SetPersistent(bool bSet)
    {
        m_bPersistent = bSet;
        if (!m_bPersistent && m_pExplosion)
            m_pExplosion->SetPersistent(false);
    }

    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CExplodingHunterEmitterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_EXPLODINGHUNTEREMITTER;

    //-----------------------------------------------------------------------------------
    CExplodingHunterEmitterEntity::CExplodingHunterEmitterEntity()
        : m_accumTime(100000.0f)
        , m_emitRate(1.8f)
        , m_bActive(false)
        , m_pCharacter(nullptr)
        , m_radius(600.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CExplodingHunterEmitterEntity::~CExplodingHunterEmitterEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CExplodingHunterEmitterEntity::Update);

        CMeshEntity::Update(dt);

        if (!m_pCharacter || !GetGameMode())
            return;


        float distance = glm::length(m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());

        if (distance < m_radius && !gEngine->GetWorldMgr()->GetLevelMgr()->IsRayIntersecting(m_pNode->GetDerivedPosition(), m_pCharacter->GetNode()->GetDerivedPosition(), 0, 0))
        {
            if (!m_bActive)
            {
                //m_accumTime = 10000.0f;
            }

            m_bActive = true;
        }
        else
            m_bActive = false;

        m_accumTime += dt;

        /*if(m_accumTime <= 0.2f)
        {
        CQuaternion quat = m_pNode->GetOrientation();
        quat.AddPitch(2 * dt);
        m_pNode->SetOrientation(quat);
        }*/

        if (!m_bActive)
            return;


        if (m_accumTime >= m_emitRate)
        {
            OnEmitExplodingHunter();

            m_accumTime = 0.0f;
        }
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntity::OnEmitExplodingHunter()
    {
        CExplodingHunterEntity* pHunter = static_cast<CExplodingHunterEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLODINGHUNTER));
        pHunter->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
        pHunter->SetTargetPosition(m_pNode->GetDerivedPosition() + m_pNode->GetDerivedRight() * 100.0f);
        pHunter->SetState(CHunterBaseEntity::HUNTER_STATE_FLYINGTOTARGET);
        pHunter->SetPersistent(false);
        pHunter->SetRadius(3000.0f);
        pHunter->SetLifeTime(2.3f);
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("HunterEmitter.x");

        m_emitRate = 2.0f;
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_accumTime = 10000.0f;

        m_bActive = false;

        m_pCharacter = gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_CHARACTER3D);
    }



    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CExplodingHunterEmitterEntityT2::ENTITY_META_TYPEID = ENTITY_TYPEID_EXPLODINGHUNTEREMITTERT2;

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntityT2::InitializeAtCreation()
    {
        CExplodingHunterEmitterEntity::InitializeAtCreation();

        m_radius = 600.0f;
    }

    //-----------------------------------------------------------------------------------
    void CExplodingHunterEmitterEntityT2::OnEmitExplodingHunter()
    {
        if (!m_pCharacter)
            return;

        float distance = glm::length(m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());

        for (int i = -1; i < 2; ++i)
        {
            CExplodingHunterEntity* pHunter = static_cast<CExplodingHunterEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLODINGHUNTER));
            pHunter->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
            pHunter->GetNode()->SetScale(Vec3(0.5f, 0.5f, 0.5f));
            pHunter->SetTargetPosition(m_pNode->GetDerivedPosition() + m_pNode->GetDerivedRight() * 100.0f + (float)i * m_pNode->GetDerivedUp() * 50.0f);
            pHunter->SetState(CHunterBaseEntity::HUNTER_STATE_FLYINGTOTARGET);
            pHunter->SetPersistent(false);
            pHunter->SetTargetBraking(false);
            pHunter->SetRadius(3000.0f);
            pHunter->SetAiming(0.0f);
            pHunter->SetLifeTime(distance / 250.0f);
        }
    }

} // env