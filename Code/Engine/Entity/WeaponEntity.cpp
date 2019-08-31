#include <Engine/Entity/WeaponEntity.h>
#include <Engine/Entity/ProjectileEntity.h>
#include <Engine/Entity/BossEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CWeaponEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_WEAPON;

    //-----------------------------------------------------------------------------------
    CWeaponEntity::CWeaponEntity()
        : m_Charge(0.0f)
        , m_MaxCharge(5.0f)
        , m_ChargingRange(400.0f)
        , m_pLaserNode(nullptr)
        , m_pParticles(nullptr)
        , m_bDisabled(false)
        , m_bActive(false)
        , m_bJustFired(true)
        , m_bossLaserTime(-1.0f)
        , m_bossLaserDuration(3.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CWeaponEntity::~CWeaponEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pParticles);
        gEngine->GetSceneMgr()->RemoveNode(m_pLaserNode);
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        if (m_pLaserNode == nullptr)
        {
            m_pLaserNode = gEngine->GetSceneMgr()->CreateSceneNode();
            m_pNode->AttachChildNode(m_pLaserNode);

            m_laserSprite.AttachNode(m_pLaserNode);
            m_laserSprite.SetMaterialFilename("Sprites/laser.mtrl");
        }

        InitSprites();

        InitParticles();

        RemoveCircles();
        AddCircle(CCircle(58.0f));
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        if (m_pLaserNode == nullptr)
        {
            m_pLaserNode = gEngine->GetSceneMgr()->CreateSceneNode();
            m_pNode->AttachChildNode(m_pLaserNode);
        }

        InitSprites();

        InitParticles();

        RemoveCircles();
        AddCircle(CCircle(58.0f));
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

		m_pSound->SetVolume(2.0f);
        m_pSound->SetPeakVolumeDistance(500);
        m_pSound->SetAudibleDistance(700);
        m_pSound->LoadSound("Entities/Magnet.wav");
        m_pSound->SetVolume(0.5f);
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::InitParticles()
    {
        if (m_pParticles == nullptr)
        {
            m_pParticles = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
            m_pParticles->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pParticles->SetDeletable(false);
            m_pParticles->SetAutoDelete(false);
            m_pParticles->LoadFromTemplate("WEAPON");
            m_pNode->AttachChildNode(m_pParticles->GetNode());
        }
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::InitSprites()
    {
        if (m_pLaserNode == nullptr)
        {
            m_pLaserNode = gEngine->GetSceneMgr()->CreateSceneNode();
            m_pNode->AttachChildNode(m_pLaserNode);
        }

        m_laserSprite.AttachNode(m_pLaserNode);
        m_laserSprite.SetMaterialFilename("Sprites/laser.mtrl");
        m_laserSprite.SetDepthTest(true);
        m_laserSprite.SetColor(Vec4(1.0f, 0.25f, 0.0f, 1.4f));

        m_loadingSpriteNode.SetScale(Vec3(0.4f, 0.4f, 0.4f));
        m_loadingSprite.AttachNode(&m_loadingSpriteNode);
        m_loadingSprite.SetMaterialFilename("Sprites/weapon_loading.png");
        m_loadingSprite.SetDepthTest(false);
        m_loadingSprite.SetAlpha(0.45f);
        m_loadingSprite.SetAlphaBlending(true);
        m_loadingSprite.SetColor(Vec3(0.25f, 0.5f, 1.0f));

    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);
    }

    void CWeaponEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        if (!m_bDisabled && m_bActive)
        {
            queue.m_spriteRenderObjects.push_back(m_laserSprite.GetRenderObject());
        }

        if (!m_bDisabled && m_Charge >= 0.0f)
        {
            queue.m_spriteRenderObjects.push_back(m_loadingSprite.GetRenderObject());
        }
    }

    void CWeaponEntity::DoDamage()
    {
        m_Charge -= 1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (m_bDisabled)
            return;

        m_loadingSpriteNode.SetPosition(m_pNode->GetDerivedPosition());

        float chargeFactor = m_Charge / m_MaxCharge;

        m_loadingSpriteNode.SetScale(Vec3(0.45f, 0.45f, 0.45f) * chargeFactor);

        Vec3 weaponPosition = m_pNode->GetDerivedPosition();

        if (m_Charge <= 0.0f)
        {
            m_Charge = -1.0f;

            CBossEntity* boss = static_cast<CBossEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_BOSS));

            if (boss)
            {
                m_bossLaserTime -= dt;

                if (m_bJustFired)
                    boss->SetState(CBossEntity::BOSS_STATE_HIT);

                m_bJustFired = false;

                float alpha = Clamp(m_bossLaserTime / m_bossLaserDuration, 0.0f, 1.0f);

                //m_pSound->SetVolume(alpha * 6.0f);

                GetMeshInstanceNode(0)->SetScale(Vec3(1.0f) * alpha);
                m_pParticles->GetNode()->SetScale(Vec3(1.0f) * alpha);

                if (m_bossLaserTime < 0.0f)
                {
                    m_bDisabled = true;
                    m_bActive = false;

                    m_pParticles->SetEmitNewParticles(false);

                    GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });

                    OnFired(0);
                }
            }
        }

        SetGhost(m_bDisabled);

        ApplyTransform(m_pNode->GetDerivedST());
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::FireAtBoss()
    {
        CProjectileEntity* projectile = static_cast<CProjectileEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PROJECTILE));
        projectile->GetNode()->SetPosition(GetNode()->GetDerivedPosition());

        CBossEntity* boss = static_cast<CBossEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_BOSS));
        projectile->SetTargetEntity(boss);
    }

    //-----------------------------------------------------------------------------------
    void CWeaponEntity::OnGameModeChanged(bool bGameMode)
    {
        m_Charge = 0.0f;

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(true); });

        m_bossLaserTime = m_bossLaserDuration;

        m_bDisabled = false;
        m_bActive = false;
        m_bJustFired = true;

        m_Charge = m_MaxCharge;

        m_pParticles->SetEmitNewParticles(true);

        SetGhost(false);

        GetMeshInstanceNode(0)->SetScale(Vec3(1.0f));
        m_pParticles->GetNode()->SetScale(Vec3(1.0f));
    }
} // env>