#include <Engine/Entity/SpikeBallEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CSpikeBallEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SPIKEBALL;

    //-----------------------------------------------------------------------------------
    CSpikeBallEntity::CSpikeBallEntity()
        : m_pMeshInner(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[SpikeBall] Start", SPIKEBALL_EVENT_START);
        RegisterEvent("[SpikeBall] Stop", SPIKEBALL_EVENT_STOP);
    }

    //-----------------------------------------------------------------------------------
    CSpikeBallEntity::~CSpikeBallEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshInner);
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

//         SetMeshFilename("SpikeBall.x");
//         SetRotationSpeedZ(-1.3f);
//         GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(UndoGamma(RGBByte2Float(135, 255, 135))); });
//         GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetEmissiveColor(UndoGamma(RGBByte2Float(19, 255, 19))); });
//         GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetEnvColor(UndoGamma(RGBByte2Float(144, 255, 157))); });
//         GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseEnergy(1.0f); });
//         GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetEmissiveEnergy(5.0f); });
//         GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetEnvReflectivity(0.3f); });

        InitMeshes();

        RemovePolygons();
        RemoveCircles();

        AddCircle(CCircle(75.0));

        CQuaternion quat = m_pNode->GetOrientation();
        quat.SetYaw(PI * 0.5f);
        m_pNode->SetOrientation(quat);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        InitMeshes();

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->SetVolume(0.31f);
        m_pSound->SetPeakVolumeDistance(500);
        m_pSound->SetAudibleDistance(300);
        m_pSound->LoadSound("Entities/Spikeball.wav");
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

        CSpikeBallEntity& other = static_cast<CSpikeBallEntity&>(entity);
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::InitMeshes()
    {
        //SetMeshFilename("SpikeBallOuter.x");

        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshInner);

        m_pMeshInner = nullptr;
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        ApplyTransform(GetNode()->GetDerivedRST(), GetNode()->GetDerivedScale().x);
    }

    //-----------------------------------------------------------------------------------
    void CSpikeBallEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case SPIKEBALL_EVENT_START:
            break;

        case SPIKEBALL_EVENT_STOP:
            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }
} // env>