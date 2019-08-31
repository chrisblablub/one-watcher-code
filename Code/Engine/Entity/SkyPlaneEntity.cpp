#include <Engine/Entity/SkyPlaneEntity.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CSkyPlaneEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SKYPLANE;

    //-----------------------------------------------------------------------------------
    CSkyPlaneEntity::CSkyPlaneEntity()
        : m_color(1.0f, 1.0f, 1.0f),
        m_alpha(0.8f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CSkyPlaneEntity::~CSkyPlaneEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CSkyPlaneEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("skyplane.x");

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    //-----------------------------------------------------------------------------------
    void CSkyPlaneEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
        
        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    //-----------------------------------------------------------------------------------
    void CSkyPlaneEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);
        
        m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + Vec3(0.0f, 250.0f, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CSkyPlaneEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_skyPlaneRenderObject.m_meshInstances.clear();
        GetMeshInstanceContainer().GetMeshInstanceRenderObjects(m_skyPlaneRenderObject.m_meshInstances);

        m_skyPlaneRenderObject.m_alpha = m_alpha;

        renderQueue.m_pSkyPlaneRenderObject = &m_skyPlaneRenderObject;
    }

    //-----------------------------------------------------------------------------------
    void CSkyPlaneEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }
} // env
