
#include <Engine/Entity/EnergyFieldEntity.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CEnergyFieldEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ENERGYFIELD;

    CEnergyFieldEntity::CEnergyFieldEntity()
        : m_energyColor(1.0f, 1.0f, 1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CEnergyFieldEntity::~CEnergyFieldEntity()
    {
    }

    void CEnergyFieldEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("energyField.x");

    }

    void CEnergyFieldEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
    }

    void CEnergyFieldEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    void CEnergyFieldEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        //m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + VECTOR3(0, -1200, 2500));
    }

    void CEnergyFieldEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_energyFieldRenderObject.m_meshInstances.clear();
        GetMeshInstanceContainer().GetMeshInstanceRenderObjects(m_energyFieldRenderObject.m_meshInstances);

        ENV_ASSERT_DBG(false && "TODO: Implement fix for sub mesh instance container");

        renderQueue.m_pEnergyFieldRenderObject = &m_energyFieldRenderObject;
    }

    void CEnergyFieldEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }
} // env
