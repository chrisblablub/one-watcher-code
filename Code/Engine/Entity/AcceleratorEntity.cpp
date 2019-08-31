#include <Engine/Entity/AcceleratorEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CAcceleratorEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ACCELERATOR;

    //-----------------------------------------------------------------------------------
    CAcceleratorEntity::CAcceleratorEntity()
        : m_strength(900.0f)
        , m_duration(0.15f)
        , m_pParticles(nullptr)
        , m_bDisabled(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Accelerator] SetDisabled", ACCELERATOR_EVENT_SETDISABLED);
        RegisterEventParameter(ACCELERATOR_EVENT_SETDISABLED, "Disabled", CBaseParam::PARAMETER_TYPE_BOOL, true);

        RegisterEvent("[Accelerator] SetBlueParticles", ACCELERATOR_EVENT_SETBLUEPARTICLES);
    }

    //-----------------------------------------------------------------------------------
    CAcceleratorEntity::~CAcceleratorEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pParticles);
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Accelerator.x");

        RemoveCircles();
        RemovePolygons();

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-20.0f, -32.0f, 0.0f));
        p.AddPoint(Vec3(-20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, -32.0f, 0.0f));
        AddPolygon(p);

        m_pParticles = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
        m_pParticles->LoadFromTemplate("BEAM_T2");
        m_pParticles->GetNode()->SetPosition(Vec3(0.0f, 10.0f, 0.0f));
        m_pParticles->SetAutoDelete(false);
        m_pParticles->SetPickable(false);
        m_pParticles->SetRender(false);

        m_pNode->AttachChildNode(m_pParticles->GetNode());

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        RemoveCircles();
        RemovePolygons();

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-20.0f, -32.0f, 0.0f));
        p.AddPoint(Vec3(-20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, -32.0f, 0.0f));
        AddPolygon(p);

        //CParticleSystemEntity* pParticles = static_cast< CParticleSystemEntity* >(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
        m_pParticles->LoadFromTemplate("BEAM_T2");
        m_pParticles->GetNode()->SetPosition(Vec3(0.0f, 10.0f, 0.0f));
        //pParticles->GetNode()->SetPosition(VECTOR3(0.0f, 0.0f, 0.0f));
        //pParticles->SetAutoDelete(false);
        //pParticles->SetPickable(false);
        //m_pNode->AttachChildNode(pParticles->GetNode());

        //m_pParticles = pParticles;

        m_bGhost = false;
    }

    void CAcceleratorEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->LoadSound("Entities/Accelerator.wav");
        m_pSound->SetVolume(0.4f);
        m_pSound->SetPeakVolumeDistance(200.0f);
        m_pSound->SetAudibleDistance(300);
    }

    //-----------------------------------------------------------------------------------
    Vec3 CAcceleratorEntity::GetForce()
    {
        return m_pNode->GetDerivedUp();
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        SetGhost(m_bDisabled);

        m_pParticles->SetEmitNewParticles(!m_bDisabled);

        ApplyTransform(m_pNode->GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }




    void CAcceleratorEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case ACCELERATOR_EVENT_SETDISABLED:
            if (eventParameter.IsParameterSet("Disabled"))
                eventParameter.GetParameter("Disabled", m_bDisabled);
            break;

        case ACCELERATOR_EVENT_SETBLUEPARTICLES:
            m_pParticles->LoadFromTemplate("BEAM_T1");
            m_pParticles->SetEmitNewParticles(!m_bDisabled);
            break;
        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CAcceleratorClusterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ACCELERATORCLUSTER;

    //-----------------------------------------------------------------------------------
    CAcceleratorClusterEntity::CAcceleratorClusterEntity()
        : m_offsetRadius(95.0f)
        , m_version(0)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CAcceleratorClusterEntity::~CAcceleratorClusterEntity()
    {
        for (int i = 0; i < m_magnets.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_magnets[i]);
        }
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::SetOffsetRadius(float radius)
    {
        m_offsetRadius = radius;

        UpdateAccelerators();
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::UpdateAccelerators()
    {
        Vec3 magnetOffset(0.0f, 1.0f, 0.0f);

        for (int i = 0; i < m_magnets.size(); ++i)
        {
            float roll = ((2 * PI) / 3.0f) * i;
            Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), roll, Vec3(0.0f, 0.0f, 1.0f)));
            magnetOffset = Vec3(Vec4(0.0f, 1.0f, 0.0f, 0.0f) * mtxRot);

            magnetOffset *= m_offsetRadius;

            m_magnets[i]->GetNode()->SetPosition(magnetOffset);
            m_magnets[i]->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, roll));
        }
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        m_magnets.resize(3);

        CMeshEntity::SetMeshFilename("AcceleratorCluster.x");

        for (int i = 0; i < m_magnets.size(); ++i)
        {
            m_magnets[i] = static_cast<CAcceleratorEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_ACCELERATOR));
            m_magnets[i]->SetAutoDelete(false);
            GetMeshInstanceNode(0)->AttachChildNode(m_magnets[i]->GetNode());
        }

        UpdateAccelerators();

        RemoveCircles();
        CCircle c;
        c.SetRadius(80.0f);
        AddCircle(c);
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        if (m_version < 2)
        {
            for (int i = 0; i < m_magnets.size(); ++i)
            {
                GetMeshInstanceNode(0)->AttachChildNode(m_magnets[i]->GetNode());
            }
        }

        RemoveCircles();
        CCircle c;
        c.SetRadius(80.0f);
        AddCircle(c);

        UpdateAccelerators();
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        ApplyTransform(m_pNode->GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CAcceleratorClusterEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        for (int i = 0; i < m_magnets.size(); ++i)
        {
            GetMeshInstanceNode(0)->AttachChildNode(m_magnets[i]->GetNode());
        }
    }



    ENTITY_TYPEID CAcceleratorBossEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ACCELERATOR_BOSS;

    CAcceleratorBossEntity::CAcceleratorBossEntity()
    {
        SetMetaTypeID(CAcceleratorBossEntity::ENTITY_META_TYPEID);
    }

    CAcceleratorBossEntity::~CAcceleratorBossEntity()
    {

    }

    void CAcceleratorBossEntity::InitializeAtCreation()
    {
        CAcceleratorEntity::InitializeAtCreation();
    }

    void CAcceleratorBossEntity::InitializeAtLoad()
    {
        CAcceleratorEntity::InitializeAtLoad();
    }

    void CAcceleratorBossEntity::Update(float dt)
    {
        CAcceleratorEntity::Update(dt);
    }

    void CAcceleratorBossEntity::OnGameModeChanged(bool bGameMode)
    {
        CAcceleratorEntity::OnGameModeChanged(bGameMode);
    }

    void CAcceleratorBossEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        if (eventId == 0)
            m_bDisabled = true;
        else
            CAcceleratorEntity::OnInput(eventId, eventParameter);
    }
} // env