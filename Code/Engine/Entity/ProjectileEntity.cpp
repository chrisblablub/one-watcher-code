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
    ENTITY_TYPEID CProjectileEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PROJECTILE;

    //-----------------------------------------------------------------------------------
    CProjectileEntity::CProjectileEntity()
        : m_Speed(500.0f)
        , m_IsCurvedPath(false)
        , m_bTriggered(false)
        , m_bInitial(true)
        , m_FixedTargetPosition(0.0f, 0.0f, 0.0f)
        , m_TargetEntity(nullptr)
        , m_UseFixedTargetPosition(false)
        , m_pExplosion(nullptr)
        , m_Agility(2.0f)
        , m_Damage(1)
        , m_currentLifetime(10.0f)
        , m_currentKilltime(-1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CProjectileEntity::~CProjectileEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pExplosion);
        gEngine->GetEntityMgr()->RemoveEntity(m_pSound);
    }

    //-----------------------------------------------------------------------------------
    void CProjectileEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();
        SetMeshFilename("Missile.x");
        float scale = 1.0f;
        GetNode()->SetScale(Vec3(scale, scale, scale));

        m_pExplosion = static_cast<CExplosionEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLOSION));
        m_pExplosion->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_pExplosion->GetNode()->SetScale(Vec3(1.3f, 1.3f, 1.3f));
        m_pExplosion->SetRenderEditor(false);
        m_pExplosion->SetRenderGame(false);
        m_pExplosion->SetPickable(false);
        m_pExplosion->SetAutoDelete(false);
        m_pExplosion->SetRadius(110.0f);

        m_bInitial = true;
    }

    //-----------------------------------------------------------------------------------
    void CProjectileEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_bInitial = true;
    }

    //-----------------------------------------------------------------------------------
    void CProjectileEntity::SetupSounds()
    {

    }

    //-----------------------------------------------------------------------------------
    void CProjectileEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

        CBossEntity& other = static_cast<CBossEntity&>(entity);
    }

    //-----------------------------------------------------------------------------------
    void CProjectileEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (dt == 0.0f)
        {
            return;
        }

        if (m_TargetEntity == nullptr)
        {
            return;
        }


        //         if (m_currentKilltime >= 0.0f)
        //         {
        //             GetMeshInstanceContainer().CallForEach([&](CMeshInstance* pMeshInst)
        //             {
        //                 pMeshInst->GetMaterial().SetDiffuseColor(Vec3(1.0f, 0.0f, 0.0f));
        //                 pMeshInst->GetMaterial().SetDiffuseEnergy(5.0f);
        //                 pMeshInst->GetMaterial().SetEmissiveColor(Vec3(1.0f, 0.0f, 0.0f));
        //                 pMeshInst->GetMaterial().SetEmissiveEnergy(5.0f);
        //             });
        // 
        // 
        //             if (m_currentKilltime >= 0.0f &&
        //                 m_currentKilltime - dt < 0.0f)
        //             {
        //                 GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
        // 
        //                 if (m_currentKilltime < 0.0f)
        //                     gEngine->GetEntityMgr()->KillEntity(this);
        //             }
        // 
        //             m_currentKilltime -= dt;
        // 
        //             return;
        //         }

        if (m_currentKilltime >= 0.0f)
        {
            GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });

            m_currentKilltime -= dt;

            if (m_currentKilltime < 0.0f)
                gEngine->GetEntityMgr()->KillEntity(this);


            return;
        }

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetAttachedNode()->SetOrientation(CQuaternion(0.0f, PI * 0.5f, 0.0f)); });

        if (m_currentKilltime < 0.0f)
        {
            for (unsigned int i = 0; i < m_secondaryTargets.size(); ++i)
            {
                CEntity* p = m_secondaryTargets[i];

                CGeometryCollidable c;
                c.AddCircle(CCircle(m_pNode->GetDerivedPosition(), 100.0f));

                if (c.HasCollision(*p))
                {
                    CWeaponEntity* pWeapon = static_cast<CWeaponEntity*>(p);

                    pWeapon->DoDamage();

                    Explode();
                    return;

                    break;
                }
            }
        }



        Vec3 targetPosition = m_UseFixedTargetPosition ? m_FixedTargetPosition : m_TargetEntity->GetNode()->GetDerivedPosition();
        Vec3 currentPosition = GetNode()->GetDerivedPosition();

        if (m_TargetEntity)
        {
            if (currentPosition.z > 300.0f)
            {
                targetPosition.y += 500.0f;
            }
        }


        Vec3 direction = targetPosition - currentPosition;

        float dist = glm::length(direction);

        if (dist < 200.0f)
        {
            m_bTriggered = true;
        }



        if (dist < 30.0f)
        {
            Explode();
            return;
        }

        if (dist > 400.0f && m_bTriggered)
        {
            Explode();
            return;
        }

        direction = glm::normalize(direction);

        CSmoothRotation::ApplyRotation(*GetNode(), direction, direction, m_Agility, dt);

        m_currentLifetime -= dt;

        if (m_currentLifetime < 0.0f)
        {
            Explode();
            return;
        }

        CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

        if (pLevel->IsRayIntersecting(currentPosition, currentPosition + direction, nullptr))
        {
            Explode();
            return;
        }

        GetNode()->Translate(direction * dt * m_Speed);
    }

    void CProjectileEntity::Explode()
    {
        if (m_currentKilltime >= 0.0f)
            return;

        m_pExplosion->GetNode()->SetPosition(GetNode()->GetDerivedPosition());
        m_pExplosion->SetEnabled(true);

        m_currentKilltime = 4.0f;
    }

    void CProjectileEntity::SetCurvedPath(bool isCurvedPath)
    {
        m_IsCurvedPath = isCurvedPath;
    }

    void CProjectileEntity::SetFixedTargetPosition(Vec3 targetPosition)
    {
        m_UseFixedTargetPosition = true;
        m_FixedTargetPosition = targetPosition;
    }

    void CProjectileEntity::SetTargetEntity(CEntity* targetEntity)
    {
        m_TargetEntity = targetEntity;
    }

    void CProjectileEntity::OnGameModeChanged(bool bGameMode)
    {
        //if (bGameMode == false) {
        if (!m_bInitial)
            gEngine->GetEntityMgr()->KillEntity(this);

        m_bInitial = false;
        //}

        //m_directionPrevFrame = m_pNode->GetDirection();

    }








    ENTITY_TYPEID CProjectileGunEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PROJECTILE_GUN;

    CProjectileGunEntity::CProjectileGunEntity()
        : m_pTarget(nullptr)
        , m_bDisabled(false)
        , m_pFan(nullptr)
        , m_lifeTime(8.0f)
        , m_fireRate(0.35f)
        , m_fireAccumTime(-1.0f)
        , m_speed(500.0f)
        , m_agility(2.0f)
        , m_numProjectiles(0)
    {
        SetMetaTypeID(CProjectileGunEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Projectile Gun] SetEnabled", PROJECTILE_GUN_EVENT_SET_ENABLED);
        RegisterEventParameter(PROJECTILE_GUN_EVENT_SET_ENABLED, "Enabled", CBaseParam::PARAMETER_TYPE_BOOL, (bool)true, (bool)false, (bool)true);

        RegisterEvent("[Projectile Gun] SetFireRate", PROJECTILE_GUN_EVENT_SET_FIRE_RATE);
        RegisterEventParameter(PROJECTILE_GUN_EVENT_SET_FIRE_RATE, "FireRate", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 5.0f);

        RegisterEvent("[Projectile Gun] SetLifetime", PROJECTILE_GUN_EVENT_SET_LIFETIME);
        RegisterEventParameter(PROJECTILE_GUN_EVENT_SET_LIFETIME, "ProjectileLifetime", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100.0f);

        RegisterEvent("[Projectile Gun] SetSpeed", PROJECTILE_GUN_EVENT_SET_SPEED);
        RegisterEventParameter(PROJECTILE_GUN_EVENT_SET_SPEED, "ProjectileSpeed", CBaseParam::PARAMETER_TYPE_FLOAT, 500.0f, 0.0f, 10000.0f);

        RegisterEvent("[Projectile Gun] SetAgility", PROJECTILE_GUN_EVENT_SET_AGILITY);
        RegisterEventParameter(PROJECTILE_GUN_EVENT_SET_AGILITY, "ProjectileAgility", CBaseParam::PARAMETER_TYPE_FLOAT, 2.0f, 0.0f, 100.0f);
    }

    CProjectileGunEntity::~CProjectileGunEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pFan);
    }

    void CProjectileGunEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

#ifndef ENV_FINAL
        if (!gEngine->GetEntityMgr()->IsValid(m_pTarget))
            m_pTarget = nullptr;
#endif

        if (!m_pTarget)
            return;

        GetMeshInstanceContainer().GetMeshInstance(0)->GetAttachedNode()->SetOrientation(CQuaternion(0.0f, -PI * 0.5f, 0.0f));

        if (m_bDisabled)
            return;


        if (m_fireAnimTime < 0.0f)
        {
            Vec3 targetDir = m_pTarget->GetNode()->GetDerivedPosition() - GetNode()->GetDerivedPosition();

            CSmoothRotation::ApplyRotation(*GetNode(), glm::normalize(targetDir), m_agility, dt);
        }

        m_projectileTime -= dt;

        // Fire
        //if (m_numProjectiles > 0)
        {
            float fireTime = 1.0f / m_fireRate;

            m_fireAccumTime += dt;

            if (m_fireAccumTime > fireTime)
            {
                CProjectileEntity* projectile = static_cast<CProjectileEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PROJECTILE));
                projectile->GetNode()->SetPosition(GetNode()->GetDerivedPosition());
                projectile->GetNode()->SetOrientation(GetNode()->GetDerivedOrientation());
                projectile->SetTargetEntity(m_pTarget);
                projectile->SetSpeed(m_speed);
                projectile->SetLifetime(m_lifeTime);
                projectile->SetAgility(m_agility);

                for (CEntity* pEntity : m_secondaryTargets)
                {
                    projectile->AddSecondaryTarget(pEntity);
                }

                m_projectileTime = m_lifeTime;

                m_fireAccumTime = 0.0f;

                m_fireAnimTime = 0.25f;

                --m_numProjectiles;
            }
        }


        if (m_fireAnimTime >= 0.0f)
        {
            CQuaternion orientation = m_pFan->GetNode()->GetOrientation();
            orientation.AddRoll(5.0f * dt);
            m_pFan->GetNode()->SetOrientation(orientation),

                m_fireAnimTime -= dt;
        }
    }

    void CProjectileGunEntity::FireProjectiles(int num)
    {
        m_numProjectiles = num;

        m_fireAccumTime = 100000.0f;
    }


    void CProjectileGunEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("StreamerBig.x");

        Initialize();
    }

    void CProjectileGunEntity::Initialize()
    {
        if (m_pFan == nullptr)
        {
            m_pFan = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pFan->SetMeshFilename("FanHeavy01.x");
            m_pFan->SetAutoDelete(false);
            m_pFan->SetDeletable(false);
            m_pFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pFan->GetNode()->SetOrientation(CQuaternion(PI, 0.0f, PI));
            m_pNode->AttachChildNode(m_pFan->GetNode());
        }

        m_pFan->GetNode()->SetScale(Vec3(0.8f, 0.8f, 0.8f));
    }

    void CProjectileGunEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        Initialize();
    }

    void CProjectileGunEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);
    }

    void CProjectileGunEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_pTarget = gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_CHARACTER3D);

        gEngine->GetEntityMgr()->GetEntities(m_secondaryTargets, ENTITY_TYPEID_WEAPON);

        m_bDisabled = true;

        m_fireAnimTime = -1.0f;
        m_fireAccumTime = 0.0f;
        m_fireRate = 0.5f;
        m_speed = 500.0f;

        m_numProjectiles = 0;

        m_projectileTime = 0.0f;
    }

    void CProjectileGunEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case PROJECTILE_GUN_EVENT_SET_ENABLED:
        {
            bool bEnabled = true;

            if (eventParameter.IsParameterSet("Enabled"))
                eventParameter.GetParameter("Enabled", bEnabled);

            m_bDisabled = !bEnabled;

            m_fireAccumTime = 100000.0f;

        } break;

        case PROJECTILE_GUN_EVENT_SET_FIRE_RATE:
        {
            if (eventParameter.IsParameterSet("FireRate"))
                eventParameter.GetParameter("FireRate", m_fireRate);
        } break;

        case PROJECTILE_GUN_EVENT_SET_LIFETIME:
        {
            if (eventParameter.IsParameterSet("ProjectileLifetime"))
                eventParameter.GetParameter("ProjectileLifetime", m_lifeTime);
        } break;

        case PROJECTILE_GUN_EVENT_SET_SPEED:
        {
            if (eventParameter.IsParameterSet("ProjectileSpeed"))
                eventParameter.GetParameter("ProjectileSpeed", m_speed);
        } break;

        case PROJECTILE_GUN_EVENT_SET_AGILITY:
        {
            if (eventParameter.IsParameterSet("ProjectileAgility"))
                eventParameter.GetParameter("ProjectileAgility", m_agility);
        } break;



        default:
            CMeshEntity::OnInput(eventId, eventParameter);
        }
    }


} // env>