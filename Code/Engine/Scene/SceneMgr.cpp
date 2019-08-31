#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/BoundingBoxDataCache.h>
#include <Engine/Math/Intersection.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/LightEntity.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/SunEntity.h>
#include <Graphic/RenderSystems/RenderSystem.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CSceneMgr::CSceneMgr()
        : m_pSelectedLight(nullptr)
        , m_pSelectedMeshInstance(nullptr)
        , m_pRootNode(nullptr)
        , m_boundingBoxesVisible(false)
        , m_bGameMode(true)
        , m_idLights(0)
        , m_idMeshInstances(0)
        , m_idNodes(0)
    {
        SetRootNode(CreateSceneNode("rootNode"));
    }

    //-----------------------------------------------------------------------------------
    CSceneMgr::~CSceneMgr()
    {
        Reset(false);

        //RemoveNode("rootNode");
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::Reset(bool bCreateRootNode)
    {
        m_boundingBoxesVisible = false;
        m_bGameMode = true;

        for (mapMeshInstances::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); ++it)
        {
            CMeshInstance* pInstance = it->second;
            ENV_DELETE(pInstance);
        }
        m_meshInstances.clear();


        for (mapLights::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
        {
            CLight* pSpotLight = it->second;
            ENV_DELETE(pSpotLight);
        }
        m_lights.clear();


        for (mapScenNodes::iterator it = m_sceneNodes.begin(); it != m_sceneNodes.end(); ++it)
        {
            ENV_DELETE(it->second);
        }

        m_sceneNodes.clear();
        m_sceneNodeParents.clear();

        m_sceneNodesSerialize.clear();
        m_meshInstancesSerialize.clear();
        m_lightsSerialize.clear();
        m_lightNodesSerialize.clear();
        m_meshInstanceNodesSerialize.clear();

        if (bCreateRootNode)
            SetRootNode(CreateSceneNode("rootNode"));
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::InitializeAtLoad()
    {
        for (mapMeshInstances::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); ++it)
        {
            CMeshInstance* p = it->second;
            p->InitializeAtLoad();
        }

        for (mapLights::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
        {
            it->second->InitializeAtLoad();
        }

        //m_meshInstances.erase("mesh#368");

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // MONSTER HACK! REMOVE UNUSED LIGHT TYPES AUTOMATICALLY.
        // REMOVE FOR RELEASE!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //
        //
        //
        const CEntityMgr::mapEntities& entities = gEngine->GetEntityMgr()->GetEntities();

        for (mapLights::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
        {
            bool bUnusedLight = true;
            for (CEntityMgr::mapEntities::const_iterator jt = entities.begin(); jt != entities.end(); ++jt)
            {
                if (jt->second->GetMetaTypeId() == ENTITY_TYPEID_LIGHT)
                {
                    CLightEntity* pL = (CLightEntity*)jt->second;
                    if (pL->GetLight() == it->second)
                    {
                        bUnusedLight = false;
                        break;
                    }
                }
                else if (jt->second->GetMetaTypeId() == ENTITY_TYPEID_SIMPLELIGHT)
                {
                    CSimpleLightEntity* pL = (CSimpleLightEntity*)jt->second;

                    if (pL->GetLight() == it->second)
                    {
                        bUnusedLight = false;
                        break;
                    }
                }
                else if (jt->second->GetMetaTypeId() == ENTITY_TYPEID_LIGHTSPRITE)
                {
                    CLightSpriteEntity* pL = (CLightSpriteEntity*)jt->second;

                    if (pL->GetLight() == it->second)
                    {
                        bUnusedLight = false;
                        break;
                    }
                }
                if (jt->second->GetMetaTypeId() == ENTITY_TYPEID_SUN)
                {
                    CSunEntity* pL = (CSunEntity*)jt->second;

                    if (pL->GetLight() == it->second)
                    {
                        bUnusedLight = false;
                        break;
                    }
                }


            }

            if (bUnusedLight)
            {
                m_lights.erase(it);
                if (m_lights.empty())
                    break;
                else
                    it = m_lights.begin();
            }
        }
        // END MONSTER HACK!
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::Update(float dt)
    {
        for (mapMeshInstances::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); ++it)
        {
            CMeshInstance* p = it->second;

            p->Update(dt);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::AddToRenderQueue(RenderQueue& renderQueue, float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Engine, CSceneMgr);

        {
            ENV_SCOPED_CPU_PROFILING(Engine, CSceneMgr::MeshInstances);

            int numMeshInstancesRendered = 0;
            for (mapMeshInstances::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); ++it)
            {
                CMeshInstance* p = it->second;

                if ((!m_bGameMode && p->GetMaterial().GetRenderInEditor()) || (m_bGameMode && p->GetMaterial().GetRenderInGame()))
                {
                    CBoundingVolume* pVol = p->GetAttachedMeshContainer()->GetBoundingVolume();

                    bool bIntersectingFrustum = false;
                    if (pVol && p->GetAttachedNode())
                    {
                        if (intersection::FrustumIntersectsBB(renderQueue.m_pCameraRenderObject->m_frustum, *pVol, p->GetAttachedNode()->GetDerivedRST()) != intersection::INTERSECTION_OUTSIDE)
                        {
                            bIntersectingFrustum = true;

                            p->AddToRenderQueue(renderQueue, dt);
                        }
                    }

                    if (bIntersectingFrustum)
                    {
                        ++numMeshInstancesRendered;
                    }
                }
            }
        }

        int numPointLightsRendered = 0;
        int numPointLights = 0;

        {
            ENV_SCOPED_CPU_PROFILING(Engine, CSceneMgr::Lights);

            for (mapLights::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
            {
                CLight* p = it->second;
                if (p->GetEnergy() < 0.05f)
                    continue;

                if ((!m_bGameMode && p->GetRenderInEditor()) || (m_bGameMode && p->GetRenderInGame()))
                {
                    if (p->GetType() == LT_POINT)
                    {
                        ++numPointLights;
                        if (intersection::FrustumIntersectsSphere(renderQueue.m_pCameraRenderObject->m_frustum, p->GetAttachedNode()->GetDerivedPosition(), p->GetRadius()))
                        {
                            p->AddToRenderQueue(renderQueue);
                            ++numPointLightsRendered;
                        }
                    }
                    else if (p->GetType() == LT_SPOT)
                    {
                        //ENV_ASSERT_DBG(false && "NO LONGER SUPPORTED");

    //                     ++numSpotLights;
    // 
    //                     CBoundingVolume* pBoundingVolume = p->GetSpotBoundingVolume();
    // 
    //                     if (!pBoundingVolume || intersection::FrustumIntersectsBB(renderQueue.m_pCameraRenderObject->m_frustum, *pBoundingVolume,
    //                         p->GetAttachedNode()->GetDerivedRST()) != intersection::INTERSECTION_OUTSIDE)
    //                     {
    //                         p->AddToRenderQueue(renderQueue);
    //                         ++numSpotLightsRendered;
    //                     }
                    }
                    else
                        p->AddToRenderQueue(renderQueue);
                }
            }
        }

//         std::stringstream str2;
//         str2 << "Num MeshInstances Rendered: " << numMeshInstancesRendered << " / " << m_meshInstances.size() << std::endl;
//         ENV_LOG_INFO(str2.str());
// 
//         std::stringstream str1;
//         str1 << "Num Point lights Rendered: " << numPointLightsRendered << " / " << numPointLights << std::endl;
//         ENV_LOG_INFO(str1.str());
// 
//         std::stringstream str3;
//         str3 << "Num Spot lights Rendered: " << numSpotLightsRendered << " / " << numSpotLights << std::endl;
//         ENV_LOG_INFO(str3.str());

    }

    //-----------------------------------------------------------------------------------
    CLight* CSceneMgr::CreateLight(const std::string& strName)
    {
        ENV_ASSERT_DBG(!strName.empty());

        CLight* pLight = new CLight(strName, Vec4(1.0f, 0.8f, 0.0f, 1.0f), Vec4(0.6f, 0.6f, 0.6f, 0.6f), 3.5f);
        pLight->InitializeAtCreation();
        pLight->SetShadowSamples(1);
        pLight->SetShadowSoftness(0.1f);
        pLight->SetShadowJitter(1.0f);
        pLight->SetAttenuationQuadratic(0.000000f);
        pLight->SetInitialSpecularColor(Vec4(0.0f, 0.0f, 0.0f, 0.0f));
        pLight->SetSpotInnerAngle(PI * 0.20f);
        pLight->SetSpotOuterAngle(PI * 0.25f);
        m_lights[strName] = pLight;
        return pLight;
    }

    //-----------------------------------------------------------------------------------
    CLight* CSceneMgr::GetLight(const std::string& strName)
    {
        ENV_ASSERT_DBG(!strName.empty());

        mapLights::iterator it = m_lights.find(strName);
        if (it == m_lights.end())
            return NULL;

        return it->second;
    }

    //-----------------------------------------------------------------------------------
    CLight* CSceneMgr::CreateLight()
    {
        std::stringstream str;
        str << "light#" << m_idLights++;
        return CreateLight(str.str());
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::SetRootNode(CSceneNode* pSceneNode)
    {
        m_pRootNode = pSceneNode;
        m_pRootNode->Serialize();
    }

    //-----------------------------------------------------------------------------------
    CSceneNode* CSceneMgr::CreateSceneNode(const std::string& strNodeName, const std::string& strAttachTo)
    {
        ENV_ASSERT_DBG(!strNodeName.empty());
        mapScenNodes::iterator it = m_sceneNodes.find(strNodeName);
        ENV_ASSERT_DBG(it == m_sceneNodes.end());
        if (it != m_sceneNodes.end())
        {
            ENV_LOG_WARNING("Node '" + strNodeName + "' already exists.");
            return it->second;
        }
        else
        {
            CSceneNode* pNode = new CSceneNode(strNodeName);
            if (!strAttachTo.empty())
            {
                if (m_sceneNodes.find(strAttachTo) != m_sceneNodes.end())
                {
                    CSceneNode* pParent = m_sceneNodes[strAttachTo];
                    pParent->AttachChildNode(pNode);
                }
                else
                {
                    ENV_LOG_WARNING("Cannot attach node to '" + strAttachTo + "': Node does not exist.");
                }
            }

            m_sceneNodes[strNodeName] = pNode;

            return pNode;
        }
    }

    //-----------------------------------------------------------------------------------
    CSceneNode* CSceneMgr::CreateSceneNode()
    {
        std::stringstream str;
        str << "node#" << m_idNodes++;
        return CreateSceneNode(str.str());
    }

    //-----------------------------------------------------------------------------------
    CSceneNode* CSceneMgr::GetSceneNode(const std::string& strName) const
    {
        mapScenNodes::const_iterator it = m_sceneNodes.find(strName);
        if (it == m_sceneNodes.end())
            return NULL;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    bool CSceneMgr::RemoveNode(CSceneNode* pNode)
    {
        if (pNode == nullptr)
            return false;

        return RemoveNode(pNode->GetName());
    }

    //-----------------------------------------------------------------------------------
    bool CSceneMgr::RemoveNode(const std::string& strNode)
    {
        if (strNode == "rootNode")
        {
            ENV_LOG_ERROR("rootNode cannot be deleted.");
            return false;
        }

        mapScenNodes::iterator it = m_sceneNodes.find(strNode);

        if (it == m_sceneNodes.end())
        {
            ENV_LOG_ERROR("Cannot not delete node '" + strNode + "': Node does not exist.");

            return false;
        }

        CSceneNode* pNode = it->second;

        m_sceneNodes.erase(strNode);
        ENV_DELETE(pNode);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RegisterMeshInstanceContainer(CMeshInstanceContainer* pContainer)
    {
        ENV_ASSERT_DBG(pContainer);
        ENV_ASSERT_DBG(!pContainer->GetMeshContainer().empty());

        m_meshInstanceContainers[pContainer->GetMeshContainer()].insert(pContainer);
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::SetGameMode(bool bGameMode) 
    { 
        m_bGameMode = bGameMode; 

        for (auto& it : m_lights)
        {
            it.second->OnGameModeChanged();
        }
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::UnregisterMeshInstanceContainer(CMeshInstanceContainer* pContainer)
    {
        const auto& it = m_meshInstanceContainers.find(pContainer->GetMeshContainer());

        if (it == m_meshInstanceContainers.end())
            return;

        it->second.erase(pContainer);
    }

    //----------------------------------------------------------------------------------
    void CSceneMgr::UpdateMeshInstanceContainers()
    {
        for (const auto& it : m_meshInstanceContainers)
        {
            for (const auto& jt : it.second)
            {
                std::string strMeshContainer = jt->GetMeshContainer();

                CMeshContainer* p = gMeshMgr->AddGetPtr(strMeshContainer);

                if (p->HasChanged())
                {
                    jt->Destroy();
                    jt->InitFromMeshContainer(strMeshContainer);

                    p->SetChanged(false);
                }

                gMeshMgr->Release(p);
            }
        }
    }

//     //-----------------------------------------------------------------------------------
//     void CSceneMgr::CreateMeshInstances(mapMeshInstances& meshInstances, const std::string& strInstanceName, const std::string& strMesh)
//     {
//         ENV_ASSERT_DBG(!strInstanceName.empty());
//         ENV_ASSERT_DBG(!strMesh.empty());
//         ENV_ASSERT_DBG(meshInstances.empty());
// 
//         mapMeshInstances::iterator it = m_meshInstances.find(strInstanceName);
//         if (it != m_meshInstances.end())
//         {
//             ENV_LOG_ERROR("Mesh instance '" + strInstanceName + "' already exists.");
//             return it->second;
//         }
// 
//         CMeshContainer* pMeshContainer = gMeshMgr->AddGetPtr(strMesh);
// 
//         mapMeshes& meshes = pMeshContainer->GetMeshes();
// 
//         for (CMesh* pMesh : meshes)
//         {
//             CMeshInstance* pInstance = new CMeshInstance(strInstanceName);
//             pInstance->AttachMeshContainer(gMeshMgr->AddGetPtr(strMesh));
//             m_meshInstances[strInstanceName] = pInstance;
//         }
// 
//         gMeshMgr->Release(strMesh);
//     }

    //-----------------------------------------------------------------------------------
    CMeshInstance* CSceneMgr::CreateMeshInstances(const std::string& strMeshContainer, const std::string& strMesh)
    {
        CMeshContainer* pMeshContainer = gMeshMgr->AddGetPtr(strMeshContainer);

        mapSubMeshes& meshes = pMeshContainer->GetMeshes();

        CMesh* pSubMesh = meshes[strMesh];

        std::stringstream strInstanceName;
        strInstanceName << strMeshContainer << "_" << strMesh << "#" << m_idMeshInstances++;

        CMeshInstance* pInstance = new CMeshInstance(strInstanceName.str());
        pInstance->AttachMeshContainer(strMeshContainer, strMesh);
        m_meshInstances[strInstanceName.str()] = pInstance;
         
        gMeshMgr->Release(strMeshContainer);

        return pInstance;
    }

    //-----------------------------------------------------------------------------------
    CMeshInstance* CSceneMgr::GetMeshInstance(const std::string& strInstance)
    {
        mapMeshInstances::iterator it = m_meshInstances.find(strInstance);
        if (it == m_meshInstances.end())
            return NULL;

        return it->second;
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RemoveMeshInstance(const std::string& strName)
    {
        mapMeshInstances::iterator it = m_meshInstances.find(strName);
        if (it == m_meshInstances.end())
        {
            ENV_LOG_WARNING("Cannot remove mesh instance because it does not exist.");
        }
        else
        {
            if (m_pSelectedMeshInstance && m_pSelectedMeshInstance->GetName() == strName)
                m_pSelectedMeshInstance = NULL;

            //if(it->second->GetAttachedNode())
            //	it->second->GetAttachedNode()->DetachObject();

            //meshMgr->Release(it->second->GetAttachedMeshName());
            ENV_DELETE(it->second);
            m_meshInstances.erase(it);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RemoveMeshInstance(CMeshInstance* pMeshInstance)
    {
        if (pMeshInstance == nullptr)
            return;

        RemoveMeshInstance(pMeshInstance->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RemoveMeshInstanceWithMesh(const std::string& strMeshName)
    {
        for (mapMeshInstances::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); ++it)
        {
            CMeshInstance* p = it->second;

            if (p->GetAttachedMeshContainerName() == strMeshName)
            {
                it = m_meshInstances.erase(it);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::SetBoundingBoxesVisible(bool bShow)
    {
        m_boundingBoxesVisible = bShow;

        for (mapMeshInstances::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); ++it)
        {
            it->second->ShowBBox(m_boundingBoxesVisible);
        }
    }

    //-----------------------------------------------------------------------------------
    bool CSceneMgr::AreBoundingBoxesVisible()
    {
        return m_boundingBoxesVisible;
    }

    //-----------------------------------------------------------------------------------
    bool CSceneMgr::IsValid(CSceneNode* pNode)
    {
        for (mapScenNodes::iterator it = m_sceneNodes.begin(); it != m_sceneNodes.end(); ++it)
        {
            if (it->second == pNode)
                return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RemoveMeshInstanceSelected()
    {
        if (!m_pSelectedMeshInstance)
            return;

        RemoveMeshInstance(m_pSelectedMeshInstance->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RemoveLight(const std::string& strName)
    {
        mapLights::iterator it = m_lights.find(strName);
        if (it == m_lights.end())
        {
            ENV_LOG_WARNING("Cannot remove light because it does not exist.");
        }
        else
        {
            if (m_pSelectedLight && m_pSelectedLight->GetName() == strName)
                m_pSelectedLight = NULL;

            if (it->second->GetAttachedNode())
                it->second->GetAttachedNode()->DetachFromParent();

            ENV_DELETE(it->second);
            m_lights.erase(it);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::RemoveLightSelected()
    {
        if (!m_pSelectedLight)
            return;

        RemoveLight(m_pSelectedLight->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::PrepareToSave()
    {
    }

    //-----------------------------------------------------------------------------------
    void CSceneMgr::Offset(float x, float y)
    {
        const CSceneNode::mapChildNodes& childNodes = m_pRootNode->GetChildNodes();

        for (CSceneNode::mapChildNodes::const_iterator it = childNodes.begin(); it != childNodes.end(); ++it)
        {
            CSceneNode* pNode = it->second;
            const Vec3& pos = pNode->GetPosition();
            pNode->SetPosition(pos + Vec3(x, y, 0.0f));
        }
    }

}
