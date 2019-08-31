#include <Engine/Entity/PlanetEntity.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CPlanetEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PLANET;

    CPlanetEntity::CPlanetEntity()
        : m_energyColor(1.0f, 1.0f, 1.0f),
        m_bHalo(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CPlanetEntity::~CPlanetEntity()
    {
    }

    void CPlanetEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("planet3.x");

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });


        SetHalo(m_bHalo);


    }

    void CPlanetEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        SetHalo(m_bHalo);

        /*m_sprite.SetMaterialFilename("Sprites/earth_halo.png");
        m_sprite.SetBillboard(false);
        m_sprite.AttachNode(&m_spriteNode);
        m_spriteNode.SetScale(VECTOR3(5.7, 4.6, 1.0f));*/

    }

    void CPlanetEntity::SetHalo(bool bHalo)
    {
        m_bHalo = bHalo;

        if (m_bHalo)
        {
            m_sprite.SetMaterialFilename("Sprites/earth_halo.png");
            m_sprite.SetBillboard(true);
            m_sprite.AttachNode(&m_spriteNode);
            m_spriteNode.SetScale(Vec3(5.7, 4.6, 1.0f));
            //m_pNode->SetPosition(VECTOR3(0, -1720, 2500));
        }
        else
        {
            m_sprite.Reset();
        }
    }

    bool CPlanetEntity::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
    {
        Vec3 transformedRayOrigin(vRayOrigin),
            transformedRayDirection(vRayDirection);

        transformedRayOrigin += gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition();

        return CMeshEntity::HasCollisionWithRay(transformedRayOrigin, transformedRayDirection, distance, ppNode);
    }

    void CPlanetEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    void CPlanetEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        const Vec3& scale = m_pNode->GetScale();

        m_spriteNode.SetScale(Vec3(scale.x * 5.7f, scale.y * 4.6f, scale.z * 4.0f));

        // Offset for planet in chapter 4
        //m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + VECTOR3(0, 1720, 2500));

        // Offset for planet in chapter 2 & 3
        //m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + VECTOR3(0, -1200, 2500));

        m_spriteNode.SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + m_pNode->GetPosition() + Vec3(0, 0, 700));
    }

    void CPlanetEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_planetRenderObject.m_meshInstances.clear();
        GetMeshInstanceContainer().GetMeshInstanceRenderObjects(m_planetRenderObject.m_meshInstances);

        m_planetRenderObject.m_color = m_energyColor;

        renderQueue.m_pPlanetRenderObject = &m_planetRenderObject;

        if (m_bHalo)
        {
            renderQueue.m_spriteRenderObjects.push_back(m_sprite.GetRenderObject());
        }
    }

    void CPlanetEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }












    ENTITY_TYPEID CUniversePlanetEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_UNIVERSEPLANET;

    CUniversePlanetEntity::CUniversePlanetEntity()
        //: m_energyColor(1.0f, 1.0f, 1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CUniversePlanetEntity::~CUniversePlanetEntity()
    {
    }

    void CUniversePlanetEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("planetNew.x");

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    void CUniversePlanetEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
    }

    void CUniversePlanetEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(false); });
    }

    void CUniversePlanetEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        //m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition() + VECTOR3(0, -1200, 2500));
    }

    void CUniversePlanetEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
//        renderQueue.pUniversePlanet = this;
    }

    void CUniversePlanetEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);
    }
} // env
