#include <Engine/Scene/MeshInstance.h>
#include <Engine/Core/BoundingBoxDataCache.h>
#include <Engine/Core/Engine.h>
#include <Engine/Math/Intersection.h>
#include <Engine/Scene/SceneMgr.h>
#include <Graphic/Core/Mesh.h>
#include <Common/Core/BoundingVolume.h>

namespace env
{

    //-----------------------------------------------------------------------------------
    CMeshInstance::CMeshInstance()
        : m_pMeshContainer(nullptr)
        , m_bShowBBox(false)
        , m_mtxPrevTransform(1.0f) {}

    //-----------------------------------------------------------------------------------
    CMeshInstance::CMeshInstance(const std::string& strName)
        : m_pMeshContainer(nullptr)
        , m_bShowBBox(false)
        , m_strName(strName)
        , m_mtxPrevTransform(1.0f) {}

    //-----------------------------------------------------------------------------------
    CMeshInstance::~CMeshInstance()
    {
        CleanUp();
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::CleanUp()
    {
        if (m_pMeshContainer)
        {
            for (int i = 0; i < m_boundingBoxesDataCache.size(); ++i)
            {
                ENV_DELETE(m_boundingBoxesDataCache[i]);
            }

            gMeshMgr->Release(m_pMeshContainer->GetName());
            m_pMeshContainer = NULL;
        }

        m_strMeshContainer.clear();
        m_strSubMesh.clear();
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::InitializeAtLoad()
    {
        if (!m_strMeshContainer.empty())
        {
            AttachMeshContainer(m_strMeshContainer, m_strSubMesh, false);
        }
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::DetachMeshContainer()
    {
        CleanUp();
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::AttachMeshContainer(const std::string& strMeshContainer, const std::string& strSubMesh, bool bInitMaterial)
    {
        //ENV_ASSERT_DBG(m_pMeshContainer == nullptr);

        std::string strMeshContainerTemp = strMeshContainer;
        std::string strSubMeshTemp = strSubMesh;

        DetachMeshContainer();

        if (!strMeshContainerTemp.empty())
        {
            m_strMeshContainer = strMeshContainerTemp;
            m_strSubMesh = strSubMeshTemp;

            if (m_strMeshContainer == "character.x")
            {
                bool bFuckYou = true;
            }
            m_pMeshContainer = gMeshMgr->AddGetPtr(m_strMeshContainer);

            // Copy material from mesh container
            // * if the mesh instance has just been created (not loaded) or
            // * if the mesh instance has just been loaded WITHOUT the material (backwards comp)
            if (bInitMaterial || !m_material.HasBeenSerialized())
                m_material = *GetAttachedSubMesh()->GetMaterial();
            else if (GetAttachedSubMesh())
            {
                
                CMaterial& material = *GetAttachedSubMesh()->GetMaterial();

                if (material.GetTexture())
                {
                    m_material.LoadTexture(material.GetTexture()->GetName(),
                                           CDefaultTextureSampler::GetSamplerName(material.GetTexture()->GetTextureSampler()),
                                           material.GetTexture()->IsSRGB());
                }
                
                if (material.GetNormalMap())
                {
                    m_material.LoadNormalMap(material.GetNormalMap()->GetName(),
                                             CDefaultTextureSampler::GetSamplerName(material.GetNormalMap()->GetTextureSampler()),
                                             material.GetNormalMap()->IsSRGB());
                }

                if (material.GetEmissiveMap())
                {
                    m_material.LoadEmissiveMap(material.GetEmissiveMap()->GetName(),
                                               CDefaultTextureSampler::GetSamplerName(material.GetEmissiveMap()->GetTextureSampler()),
                                               material.GetEmissiveMap()->IsSRGB());
                }

                if (material.GetSpecularMap())
                {
                    m_material.LoadSpecularMap(material.GetSpecularMap()->GetName(),
                                               CDefaultTextureSampler::GetSamplerName(material.GetSpecularMap()->GetTextureSampler()),
                                               material.GetSpecularMap()->IsSRGB());
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    CMesh* CMeshInstance::GetAttachedSubMesh()
    {
        return m_pMeshContainer->GetMesh(m_strSubMesh);
    }

    //-----------------------------------------------------------------------------------
    CMeshContainer* CMeshInstance::GetAttachedMeshContainer()
    {
        return m_pMeshContainer;
    }

    //-----------------------------------------------------------------------------------
    const std::string& CMeshInstance::GetAttachedMeshContainerName()
    {
        return m_strMeshContainer;
    }

    //-----------------------------------------------------------------------------------
    const std::string& CMeshInstance::GetAttachedSubMeshName()
    {
        return m_strSubMesh;
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::ShowBBox(bool bShow)
    {
        if (!m_pMeshContainer)
            return;

        m_bShowBBox = bShow;

        for (int i = 0; i < m_boundingBoxesDataCache.size(); ++i)
        {
            ENV_DELETE(m_boundingBoxesDataCache[i]);
        }

        if (GetMaterial().GetRenderCategory() == MESH_RENDER_CATEGORY_FORWARD)
            return;

        int numBBoxes = m_pMeshContainer->GetMeshes().size() + 1;

        if (m_bShowBBox)
        {
            m_boundingBoxesDataCache.resize(numBBoxes);

            int i = 0;
            for (mapSubMeshes::iterator it = m_pMeshContainer->GetMeshes().begin(); it != m_pMeshContainer->GetMeshes().end(); ++it, ++i)
            {
                CBoundingVolume* pBox = it->second->GetBoundingVolume();

                Matrix4x4 mtxId(1.0f);
                pBox->Transform(mtxId);

                m_boundingBoxesDataCache[i] = new CBoundingBoxDataCache();
                m_boundingBoxesDataCache[i]->Initialize(*pBox);
            }


            CBoundingVolume* pBox = m_pMeshContainer->GetBoundingVolume();

            Matrix4x4 mtxId(1.0f);;
            pBox->Transform(mtxId);

            m_boundingBoxesDataCache[i] = new CBoundingBoxDataCache();
            m_boundingBoxesDataCache[i]->Initialize(*pBox);
        }
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::GetInstanceAndMeshesRenderObject(InstanceAndMeshesRenderObject& renderObject)
    {
        if (m_pMeshContainer)
        {
            UpdateMeshInstanceRenderObject();

            mapSubMeshes& meshes = m_pMeshContainer->GetMeshes();
            const auto& it = meshes.find(m_strSubMesh);

            ENV_ASSERT_DBG(it != meshes.end());

            renderObject.m_InstanceRenderObject = GetMeshInstanceRenderObject();
            renderObject.m_pMeshRenderObject    = it->second->GetMeshRenderObject();
        }
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::UpdateMeshInstanceRenderObject()
    {
        if (m_pMeshContainer == nullptr)
            return;

        m_instanceRenderData.m_mtxTransform = m_pNode->GetDerivedRST();
        m_instanceRenderData.m_mtxPrevTransform = m_mtxPrevTransform;
        m_instanceRenderData.m_pMaterial = &m_material;
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::Update(float dt)
    {
        bool bUpdateOrientation = false;

        if (!m_pNode)
            return;

        CQuaternion quat = m_pNode->GetOrientation();

        if (abs(m_rotSpeedX.z) > 0.0f)
        {
            quat.AddRoll(m_rotSpeedX.z * dt);
            bUpdateOrientation = true;
        }

        if (abs(m_rotSpeedX.x) > 0.0f)
        {
            quat.AddPitch(m_rotSpeedX.x * dt);
            bUpdateOrientation = true;
        }

        if (abs(m_rotSpeedX.y) > 0.0f)
        {
            quat.AddYaw(m_rotSpeedX.y * dt);
            bUpdateOrientation = true;
        }

        if (bUpdateOrientation)
            m_pNode->SetOrientation(quat);


        Matrix4x4 mtxTransform = m_pNode->GetDerivedRST();

        m_instanceRenderData.m_mtxTransform = mtxTransform;
        m_instanceRenderData.m_mtxPrevTransform = m_mtxPrevTransform;
        m_instanceRenderData.m_pMaterial = &m_material;
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstance::AddToRenderQueue(RenderQueue& renderQueue, float dt)
    {
        if (!m_pNode)
        {
            ENV_LOG_WARNING("Mesh instance is not attached to a scene node!");
            return;
        }

        Matrix4x4 mtxTransform = m_pNode->GetDerivedRST();

        if (m_pMeshContainer)
        {
            mapSubMeshes& meshes = m_pMeshContainer->GetMeshes();

            const auto& it = meshes.find(m_strSubMesh);

            if (it != meshes.end())
            {
                for (unsigned int k = 0; k < MESH_RENDER_MODE_COUNT; ++k)
                {
                    MESH_RENDER_CATEGORY renderMode = gMeshRenderCategories[k];

                    MeshRenderObject* p = it->second->GetMeshRenderObject();

                    if (GetMaterial().GetRenderCategoryMask() & RenderCategory2Bitmask(renderMode))
                    {
                        renderQueue.m_meshInstances[renderMode][p].push_back(&m_instanceRenderData);
                    }
                }
            }
        }

        m_mtxPrevTransform = mtxTransform;

        if (m_bShowBBox)
        {
            for (int i = 0; i < m_boundingBoxesDataCache.size(); ++i)
            {
                m_boundingBoxesDataCache[i]->Transform(mtxTransform);
                renderQueue.m_vector3Caches.push_back(m_boundingBoxesDataCache[i]);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    const std::string& CMeshInstance::GetName()
    {
        return m_strName;
    }

    //-----------------------------------------------------------------------------------
    CMeshInstanceContainer::~CMeshInstanceContainer()
    {
        gEngine->GetSceneMgr()->UnregisterMeshInstanceContainer(this);

        Destroy();
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstanceContainer::Destroy()
    {
        for (auto& pMeshInst : m_meshInstances)
            gEngine->GetSceneMgr()->RemoveMeshInstance(pMeshInst);

        m_meshInstances = std::vector<CMeshInstance*>();
        m_indexNameMapping.clear();
        m_strMeshContainer.clear();
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstanceContainer::InitFromMeshContainer(const std::string& strMeshContainer)
    {
        if (m_strMeshContainer == strMeshContainer)
            return;

        Destroy();

        m_strMeshContainer = strMeshContainer;

        CMeshContainer* pMeshContainer = gMeshMgr->AddGetPtr(m_strMeshContainer);

        for (const auto& it : pMeshContainer->GetMeshes())
        {
            CMeshInstance* pSubMeshInstance = gEngine->GetSceneMgr()->CreateMeshInstances(m_strMeshContainer, it.first);

            AddMeshInstance(pSubMeshInstance);
        }

        gMeshMgr->Release(m_strMeshContainer);

        gEngine->GetSceneMgr()->RegisterMeshInstanceContainer(this);
    }

    //-----------------------------------------------------------------------------------
    void CMeshInstanceContainer::AddMeshInstance(CMeshInstance* pSubMeshInstance)
    {
        const std::string& strName = pSubMeshInstance->GetAttachedSubMeshName();

        ENV_ASSERT_DBG(m_indexNameMapping.find(strName) == m_indexNameMapping.end());

        m_meshInstances.push_back(pSubMeshInstance);

        m_indexNameMapping[strName] = m_meshInstances.size() - 1;
    }

    //-----------------------------------------------------------------------------------
    CMeshInstance* CMeshInstanceContainer::GetMeshInstance(const std::string& strSubMesh)  const
    { 
        const auto& it = m_indexNameMapping.find(strSubMesh);

        if (it == m_indexNameMapping.end())
            return nullptr;

        return GetMeshInstance(it->second);
    }

    //-----------------------------------------------------------------------------------
    CMeshInstance* CMeshInstanceContainer::GetMeshInstance(unsigned int subMeshInstanceIndex) const
    { 
        ENV_ASSERT_DBG(subMeshInstanceIndex < m_meshInstances.size());

        return m_meshInstances[subMeshInstanceIndex]; 
    }
}