#include <Engine/Entity/MagnetEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/ParticleSystemEntity.h>

namespace env
{
    ENTITY_TYPEID CMagnetEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MAGNET;

    //-----------------------------------------------------------------------------------
    CMagnetEntity::CMagnetEntity()
        : m_speed(1000.0f)
        , m_time(0.15f)
        , m_pParticles(nullptr)
        , m_pSound(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
		m_bForceUpdate = true;
    }

    //-----------------------------------------------------------------------------------
    CMagnetEntity::~CMagnetEntity()
    {
        if (m_pParticles)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticles->GetName());
            m_pParticles = nullptr;
        }

        gEngine->GetEntityMgr()->RemoveEntity(m_pSound);
    }

    //-----------------------------------------------------------------------------------
    void CMagnetEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        RemoveCircles();
        RemovePolygons();

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-20.0f, -32.0f, 0.0f));
        p.AddPoint(Vec3(-20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, -32.0f, 0.0f));
        AddPolygon(p);

        CParticleSystemEntity* pEntity = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
        pEntity->LoadFromTemplate("BEAM_T1");
        pEntity->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
        pEntity->SetAutoDelete(false);
        pEntity->SetPickable(false);
        pEntity->SetRenderEditor(false);
        pEntity->SetRenderGame(false);
        m_pNode->AttachChildNode(pEntity->GetNode());

        m_pParticles = pEntity;

        SetRenderGame(false);

        m_bGhost = false;

        SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CMagnetEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        RemoveCircles();
        RemovePolygons();

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-20.0f, -32.0f, 0.0f));
        p.AddPoint(Vec3(-20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, 55.0f, 0.0f));
        p.AddPoint(Vec3(20.0f, -32.0f, 0.0f));
        AddPolygon(p);

        static_cast<CParticleSystemEntity*>(m_pParticles)->LoadFromTemplate("BEAM_T1");
        m_pParticles->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
        m_pParticles->SetPickable(false);
        CParticleSystemEntity* pParticles = static_cast<CParticleSystemEntity*>(m_pParticles);
        pParticles->SetRenderEditor(false);
        pParticles->SetRenderGame(false);

        //m_time = 0.15f;
        //m_speed = 1300.0f;

        m_bGhost = false;
        SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CMagnetEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->LoadSound("Entities/Magnet.wav");
        m_pSound->SetVolume(0.4f);
        m_pSound->SetPeakVolumeDistance(200.0f);
        m_pSound->SetAudibleDistance(300);
    }

    //-----------------------------------------------------------------------------------
    void CMagnetEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    //-----------------------------------------------------------------------------------
    void CMagnetEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }



    ENTITY_TYPEID CMagnetClusterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MAGNETCLUSTER;

    //-----------------------------------------------------------------------------------
    CMagnetClusterEntity::CMagnetClusterEntity()
        : m_offsetRadius(100.0f)
        , m_version(0)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CMagnetClusterEntity::~CMagnetClusterEntity()
    {
        for (int i = 0; i < m_magnets.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_magnets[i]->GetName());
        }
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        m_magnets.resize(3);

        CMeshEntity::SetMeshFilename("MagnetCluster.x");

        for (int i = 0; i < m_magnets.size(); ++i)
        {
            m_magnets[i] = static_cast<CMagnetEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MAGNET));
            m_magnets[i]->SetAutoDelete(false);
            GetMeshInstanceNode(0)->AttachChildNode(m_magnets[i]->GetNode());
        }

        UpdateMagnets();

        RemoveCircles();
        CCircle c;
        c.SetRadius(80.0f);
        AddCircle(c);
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        if (m_version < 2)
        {
            for (int i = 0; i < m_magnets.size(); ++i)
            {
                GetMeshInstanceNode(0)->AttachChildNode(m_magnets[i]->GetNode());
            }
        }
        //CMeshEntity::SetMeshFilename("AcceleratorCluster01.x");

        RemoveCircles();
        CCircle c;
        c.SetRadius(80.0f);
        AddCircle(c);
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::UpdateMagnets()
    {
        Vec3 magnetOffset(0.0f, 1.0f, 0.0f);

        for (int i = 0; i < m_magnets.size(); ++i)
        {
            float roll = ((2 * PI) / 3.0f) * i;
            Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), roll, Vec3(0.0f, 0.0f, 1.0f)));

            magnetOffset = Vec3(Vec4(Vec3(0.0f, 1.0f, 0.0f), 0.0f) * mtxRot);

            magnetOffset *= m_offsetRadius;

            m_magnets[i]->GetNode()->SetPosition(magnetOffset);
            m_magnets[i]->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, roll));
            //m_pMeshInstanceNode->AttachChildNode(m_magnets[i]->GetNode());
        }
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::SetOffsetRadius(float radius)
    {
        m_offsetRadius = radius;

        UpdateMagnets();
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        ApplyTransform(m_pNode->GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CMagnetClusterEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        for (int i = 0; i < m_magnets.size(); ++i)
        {
            GetMeshInstanceNode(0)->AttachChildNode(m_magnets[i]->GetNode());
        }
    }



} // env