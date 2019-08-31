
#include <Engine/Entity/BombEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/TerrainMgr.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    ENTITY_TYPEID CBombEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_BOMB;

    CBombEntity::CBombEntity()
        : m_state(BOMB_STATE_INACTIVE),
        m_pExplosion(NULL),
        m_currentLifeTime(0.0f),
        m_impulse(0.0f),
        m_lifeTime(1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    CBombEntity::~CBombEntity()
    {
        if (m_pExplosion)
            gEngine->GetEntityMgr()->RemoveEntity(m_pExplosion->GetName());
    }

    bool CBombEntity::HasCollision(CEntity& e)
    {
        if (m_state != BOMB_STATE_EXPLODING)
            return false;

        if (e.GetMetaTypeId() == ENTITY_TYPEID_BOMB && CGeometryCollidable::HasCollision(e))
        {
            CBombEntity& otherBomb = static_cast<CBombEntity&>(e);
            if (otherBomb.GetState() != BOMB_STATE_EXPLODING)
            {
                otherBomb.SetState(BOMB_STATE_EXPLODING);
            }
            return true;
        }

        return false;
    }

    void CBombEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Mine.x");

        CCircle c;
        c.SetRadius(10.0f);
        AddCircle(c);



        m_pExplosion = static_cast<CExplosionEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLOSION));
        m_pExplosion->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_pExplosion->SetRenderEditor(false);
        m_pExplosion->SetRenderGame(false);
        m_pExplosion->SetPickable(false);
        m_pExplosion->SetAutoDelete(false);
        //m_pNode->AttachChildNode(m_pExplosion->GetNode());

        SetEnabled(false);

    }

    void CBombEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_bGhost = false;
    }


    void CBombEntity::UpdateBombLoading(float dt)
    {
        /*if(m_stateTimer.GetTime() >= 2.75f)
        {
        m_pExplosion->Explode();

        SetState(BOMB_STATE_EXPLODING);

        return;
        }*/

        float alpha = m_currentLifeTime / m_lifeTime;

        SetRotationSpeedZ(alpha * 3);

        CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

        CTerrainCollidable c;
        c.height = 20.0f;
        c.width = 20.0f;
        c.position = VECTOR2FIXED(Convert2Fixed(Vec2(m_pNode->GetPosition())));
        c.velocity = VECTOR2FIXED(Convert2Fixed(m_bombPhysics.GetVelocity() * dt));

        CollisionState collisionState = pLevel->ComputeMovement(c);

        Vec2 velocity = m_bombPhysics.GetVelocity();
        if (collisionState.bOnFloor)
        {
            ComputeReflectedVector(velocity, velocity, Vec2(0.0f, 1.0f));
            velocity *= 0.5f;
        }
        else if (collisionState.bHitCeiling)
        {
            ComputeReflectedVector(velocity, velocity, Vec2(0.0f, -1.0f));
            velocity *= 0.5f;
        }
        else if (collisionState.bHitWallFromleft)
        {
            ComputeReflectedVector(velocity, velocity, Vec2(-1.0f, 0.0f));
            velocity *= 0.5f;
        }
        else if (collisionState.bHitWallFromRight)
        {
            ComputeReflectedVector(velocity, velocity, Vec2(1.0f, 0.0f));
            velocity *= 0.5f;
        }

        m_bombPhysics.SetVelocity(velocity);

        Vec2 posFloat = Convert2Float(collisionState.pos);
        Vec3 newPos(posFloat.x, posFloat.y, 0.0f);
        m_pNode->SetPosition(newPos);

        ApplyTransform(m_pNode->GetDerivedRST());

        m_pExplosion->GetNode()->SetPosition(newPos);
    }

    void CBombEntity::UpdateBombExploding(float dt)
    {
        m_bombPhysics.Reset();


        //m_pExplosion->SetRenderParticles(true);
        //m_pExplosion->SetPauseParticles(false);

        /*if(m_stateTimer.GetTime() > 0.5f)
        {
        SetEnabled(false);

        return;
        }*/

        m_pExplosion->GetNode()->SetPosition(m_pNode->GetDerivedPosition());

        if (m_stateTimer.GetTime() < 0.3f)
        {
            float alpha = m_stateTimer.GetTime() / 0.3f;


            Vec3 scaling(1.0f, 1.0f, 1.0f);
            scaling *= (1.0f - alpha);

            m_pNode->SetScale(scaling);
            ApplyTransform(m_pNode->GetDerivedRST(), scaling.x);
        }
        else
        {
            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });
            ApplyTransform(m_pNode->GetDerivedRST(), 0.0f);
        }



    }

    void CBombEntity::SetState(BOMB_STATE state)
    {
        m_stateTimer.Reset();
        m_state = state;

        if (m_state == BOMB_STATE_EXPLODING)
            m_pExplosion->SetEnabled(true);
    }


    void CBombEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CBombEntity::Update);

        CMeshEntity::Update(dt);

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
            return;

        if (!m_bEnabled)
            return;

        m_currentLifeTime += dt;

        m_stateTimer.Update(dt);

        m_bombPhysics.Update(dt);


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
            ENV_ASSERT_DBG(false);
            break;
        }

    }

    void CBombEntity::SetEnabled(bool bEnable)
    {
        m_bEnabled = bEnable;

        m_stateTimer.Reset();

        if (!bEnable)
        {
            //m_pExplosion->SetRenderParticles(false);
            //m_pExplosion->SetPauseParticles(true);

            m_pExplosion->SetEnabled(false);
            
            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });
        }

        SetRotationSpeedZ(0.0f);

        m_pNode->SetScale(Vec3(1.0f, 1.0f, 1.0f));

        GetMeshInstanceContainer().CallForEach([&](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(bEnable); });

        m_bGhost = !bEnable;

        m_bombPhysics.Reset();
        m_bombPhysics.SetGravity(true, Vec2(0.0f, -500.0f));
        m_bombPhysics.ApplyImpulse(m_impulse * Vec2(m_pNode->GetDerivedUp()), 0.2f);

        m_currentLifeTime = 0.0f;

        //SetState(BOMB_STATE_LOADING);
    }

    void CBombEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        SetEnabled(false);
    }









    ENTITY_TYPEID CBombEntityGun::ENTITY_META_TYPEID = ENTITY_TYPEID_BOMBGUN;

    CBombEntityGun::CBombEntityGun()
        : m_fireAccum(0.0f),
        m_fireDelay(0.0f),
        m_fireDelayMin(3.5f),
        m_lifeTimeMin(3.0f),
        m_impulseMin(5000.0f),
        m_fireDelayMax(3.5f),
        m_lifeTimeMax(3.0f),
        m_impulseMax(2500.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CBombEntityGun::~CBombEntityGun()
    {
        for (int i = 0; i < m_bombs.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_bombs[i]->GetName());
        }
    }

    void CBombEntityGun::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Streamer01.x");

        UpdateBombs();
    }

    void CBombEntityGun::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();


    }

    void CBombEntityGun::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CBombEntityGun::Update);

        CMeshEntity::Update(dt);

        if (!GetGameMode())
            return;

        m_fireAccum += dt;

        m_currentRotatationTime -= dt;

        if (m_currentRotatationTime < 0.0f)
        {
            SetRotationSpeedY(0.0f);
        }


        // Update active bombs
        //
        for (int i = 0; i < m_currentNumActiveBombs; ++i)
        {
            CBombEntity* p = m_bombs[i];
            if (p->GetCurrentLifetime() >= (p->GetLifeTime() + EXPLOSION_TIME))
            {
                // Disable a bomb if it has exploded. It is moved back to 
                // vector slot 'm_currentNumActiveBombs'.
                //
                p->SetEnabled(false);
                //p->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
                //p->SetRenderParticles(false);

                // Disable bomb
                m_bombs[i] = m_bombs[m_currentNumActiveBombs - 1];
                m_bombs[m_currentNumActiveBombs - 1] = p;

                --m_currentNumActiveBombs;
            }
            else if (p->GetCurrentLifetime() >= p->GetLifeTime() && m_bombs[i]->GetState() != CBombEntity::BOMB_STATE_EXPLODING)
            {
                // Explode the bomb
                //
                m_bombs[i]->SetState(CBombEntity::BOMB_STATE_EXPLODING);
                //m_bombs[i]->SetLifetime(m_lifeTimeMin);
            }
        }


        // Emit new bomb
        if (m_fireAccum >= m_fireDelay)
        {
            if (m_currentNumActiveBombs < m_bombs.size())
            {
                // Activate bomb
                //m_bombs[m_currentNumActiveBombs]->SetRenderParticles(true);
                m_bombs[m_currentNumActiveBombs]->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
                m_bombs[m_currentNumActiveBombs]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
                m_bombs[m_currentNumActiveBombs]->SetImpulse(GetRandomFloat(m_impulseMin, m_impulseMax));
                m_bombs[m_currentNumActiveBombs]->SetLifetime(GetRandomFloat(m_lifeTimeMin, m_lifeTimeMax));
                m_bombs[m_currentNumActiveBombs]->SetEnabled(true);
                m_bombs[m_currentNumActiveBombs]->SetState(CBombEntity::BOMB_STATE_LOADING);


                ++m_currentNumActiveBombs;

                SetRotationSpeedY(5.0f);

                m_currentRotatationTime = 0.25f;
            }

            m_fireAccum = 0.0f;

            m_fireDelay = GetRandomFloat(m_fireDelayMin, m_fireDelayMax);
        }

    }

    void CBombEntityGun::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentNumActiveBombs = 0;

        m_fireAccum = 0.0f;

        m_currentRotatationTime = -1.0f;

        m_fireDelay = GetRandomFloat(m_fireDelayMin, m_fireDelayMax);

        SetRotationSpeedY(0.0f);

    }

    void CBombEntityGun::UpdateBombs()
    {
        /* Creates as many bomb entities as required according to lifetime, speed etc.
           Bomb entites are serialized, and used in a queue. If a bomb has exploded, it gets
           deactivated and moved to the end of the queue. If a new bomb is emitted, one of
           the inactive bombs is enabled. This prevents the system from creating new entities
           during level time.

           Slot 0 -> m_currentNumActiveBombs - 1 contain currently active bombs.*/

        int numBombs = (int)((m_lifeTimeMax + EXPLOSION_TIME) / m_fireDelayMin) + 1;

        for (int i = 0; i < m_bombs.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_bombs[i]->GetName());
        }

        m_bombs.resize(numBombs);

        for (int i = 0; i < m_bombs.size(); ++i)
        {
            m_bombs[i] = static_cast<CBombEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_BOMB));
            m_bombs[i]->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
            m_bombs[i]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
            m_bombs[i]->SetAutoDelete(false);
            m_bombs[i]->SetPickable(false);
            m_bombs[i]->SetEnabled(false);
            m_bombs[i]->SetImpulse(m_impulseMin);
        }

        m_currentNumActiveBombs = 0;
    }





} // env