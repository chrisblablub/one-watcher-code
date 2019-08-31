
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    ENTITY_TYPEID CMeshEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MESH;

    //-----------------------------------------------------------------------------------
    CMeshEntity::CMeshEntity()
        : CEntity()
        , m_pMeshInstance(nullptr)
        , m_pMeshInstanceNode(nullptr)
        , m_rotationSpeedZ(0.0f)
        , m_rotationSpeedY(0.0f)
        , m_rotationSpeedX(0.0f)
        , m_diffuseTime(-1.0f)
        , m_diffuseDuration(0.0f)
        , m_emissiveTime(-1.0f)
        , m_emissiveDuration(0.0f)
        , m_version(0)
        , m_bInitFromTemplate(false)
        , m_pSound(nullptr)
    {
        SetMetaTypeID(CMeshEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Mesh] Show", MESH_EVENT_SHOW);
        RegisterEvent("[Mesh] Hide", MESH_EVENT_HIDE);
        RegisterEvent("[Mesh] SetDiffuseColor", MESH_EVENT_SET_DIFFUSE_COLOR);
        RegisterEventParameter(MESH_EVENT_SET_DIFFUSE_COLOR, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(MESH_EVENT_SET_DIFFUSE_COLOR, "Color", CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 0), Vec4(0), Vec4(1, 1, 1, 100));

        RegisterEvent("[Mesh] SetEmissiveColor", MESH_EVENT_SET_EMISSIVE_COLOR);
        RegisterEventParameter(MESH_EVENT_SET_EMISSIVE_COLOR, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT);
        RegisterEventParameter(MESH_EVENT_SET_EMISSIVE_COLOR, "Color", CBaseParam::PARAMETER_TYPE_VECTOR4, "[Format] = RGBA", Vec4(0, 0, 0, 0), Vec4(0), Vec4(1, 1, 1, 100));
    }

    //-----------------------------------------------------------------------------------
    CMeshEntity::~CMeshEntity()
    {
        DestroyMeshInstancesAndNodes();

        gEngine->GetSceneMgr()->RemoveNode(m_pNode);
        gEngine->GetSceneMgr()->RemoveNode(m_pMeshInstanceNode);
        gEngine->GetEntityMgr()->RemoveEntity(m_pSound);
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::DestroyMeshInstancesAndNodes()
    {
        for (const auto& pNode : m_meshInstanceNodes)
            gEngine->GetSceneMgr()->RemoveNode(pNode);

        m_meshInstanceNodes.clear();
        m_materialsInitial.clear();

        gEngine->GetSceneMgr()->RemoveNode(m_pMeshInstanceNode);

        m_pMeshInstanceNode = nullptr;
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::InitializeAtCreation()
    {
        CEntity::InitializeAtCreation();

        m_pNode = gEngine->GetSceneMgr()->CreateSceneNode();
        gEngine->GetSceneMgr()->GetRootNode()->AttachChildNode(m_pNode);

        m_pMeshInstanceNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pNode->AttachChildNode(m_pMeshInstanceNode);

        Vec3 vOrigin, vDirection, vIntersection;
        CCamera* pCamera = gEngine->GetWorldMgr()->GetActiveCamera();
        vOrigin = pCamera->GetAttachedNode()->GetDerivedPosition();
        vDirection = pCamera->GetAttachedNode()->GetDerivedDirection();

        float dist = 0.0f;
        glm::intersectRayPlane(vOrigin, vDirection, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), dist);

        m_pNode->SetPosition(vIntersection);

        SetMeshFilename("dummy.x");

        InitDebugName();
        SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::OnEntityCloned(CEntity& entity)
    {
        ENV_ASSERT_DBG(entity.GetMetaTypeId() == GetMetaTypeId());

        CEntity::OnEntityCloned(entity);

        CMeshEntity& meshEntity = static_cast<CMeshEntity&>(entity);

        ENV_ASSERT_DBG(false && "TODO: fix this! store mesh container name in mesh entity");
        SetMeshFilename(meshEntity.GetMeshInstanceContainer().GetMeshInstance(0)->GetAttachedMeshContainerName());

        m_bInitFromTemplate = meshEntity.m_bInitFromTemplate;
        m_strTemplate = meshEntity.m_strTemplate;

        const CMeshInstanceContainer& subMeshInstContainer = GetMeshInstanceContainer();
        const CMeshInstanceContainer& otherSubMeshInstContainer = meshEntity.GetMeshInstanceContainer();

        for (size_t i = 0; i < subMeshInstContainer.GetNumMeshInstances(); ++i)
        {
            subMeshInstContainer.GetMeshInstance(i)->SetRotationSpeedX(otherSubMeshInstContainer.GetMeshInstance(i)->GetRotationSpeedX());
            subMeshInstContainer.GetMeshInstance(i)->SetRotationSpeedY(otherSubMeshInstContainer.GetMeshInstance(i)->GetRotationSpeedY());
            subMeshInstContainer.GetMeshInstance(i)->SetRotationSpeedZ(otherSubMeshInstContainer.GetMeshInstance(i)->GetRotationSpeedZ());
            subMeshInstContainer.GetMeshInstance(i)->GetMaterial() = otherSubMeshInstContainer.GetMeshInstance(i)->GetMaterial();
        }
    }


    //-----------------------------------------------------------------------------------
    void CMeshEntity::InitializeAtLoad()
    {
        CEntity::InitializeAtLoad();

        // Patch CSubMeshInstance member to CSubMeshInstanceCounter
        if (m_version == 0)
        {
            if (m_pMeshInstance)
            {
                std::string strMeshContainerName = m_pMeshInstance->GetAttachedMeshContainerName();

                CMeshContainer* pMeshContainer = gMeshMgr->AddGetPtr(strMeshContainerName);

                SetMeshFilename(pMeshContainer->GetName());

                gMeshMgr->Release(strMeshContainerName);
            }

            gEngine->GetSceneMgr()->RemoveMeshInstance(m_pMeshInstance);
            gEngine->GetSceneMgr()->RemoveNode(m_pMeshInstanceNode);

            m_pMeshInstance = nullptr;
            m_pMeshInstanceNode = nullptr;
        }

        ENV_ASSERT_DBG(m_pMeshInstance == nullptr);
        ENV_ASSERT_DBG(m_pMeshInstanceNode == nullptr);

        if (m_bInitFromTemplate)
            LoadTemplate(m_strTemplate, false);

        InitDebugName();
        SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::SaveTemplate(const std::string& strFilename, const std::string& strName)
    {
        CScript script;

        script["meshContainer"] = m_meshInstContainer.GetMeshContainer();
        script["meshContainerTemplateInit"] = m_bInitFromTemplate;

        for(unsigned int i = 0; i < m_meshInstContainer.GetNumMeshInstances(); ++i)
        {
            CMeshInstance* pMeshInst = m_meshInstContainer.GetMeshInstance(i);
            
            CScript& meshInstScript = *script.AddSubscript(pMeshInst->GetAttachedSubMeshName());

            meshInstScript["rotationSpeed"] = pMeshInst->GetRotationSpeed();

            CScript& meshInstMaterialScript = *meshInstScript.AddSubscript("material");
            
            pMeshInst->GetMaterial().SaveTemplate(meshInstMaterialScript);
        }
        
        script.SaveScript(strFilename);

        m_strTemplate = strName;
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::LoadTemplate(const std::string& strFilename, bool bInitMaterial)
    {
        CScript* pScript = gScriptMgr->AddGetPtr(strFilename);

        if (!pScript)
            return;

        m_strTemplate = strFilename;

        if (pScript->Exists("meshContainer"))
        {
            SetMeshFilename((*pScript)["meshContainer"]);
        }

        if (pScript->Exists("meshContainerTemplateInit"))
            m_bInitFromTemplate = (*pScript)["meshContainerTemplateInit"];

        for (unsigned int i = 0; i < m_meshInstContainer.GetNumMeshInstances(); ++i)
        {
            CMeshInstance* pMeshInst = m_meshInstContainer.GetMeshInstance(i);

            if (!pScript->SubscriptExists(pMeshInst->GetAttachedSubMeshName()))
                continue;

            CScript& meshInstScript = *pScript->GetSubscript(pMeshInst->GetAttachedSubMeshName());

            if (meshInstScript.Exists("rotationSpeed"))
                pMeshInst->SetRotationSpeed(meshInstScript["rotationSpeed"]);

            if (!meshInstScript.SubscriptExists("material"))
                continue;

            CScript& meshInstMaterialScript = *meshInstScript.GetSubscript("material");

            pMeshInst->GetMaterial().LoadTemplate(meshInstMaterialScript);
        }

        gScriptMgr->Release(strFilename);
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::InitDebugName()
    {
        m_pNode->SetDebugName(GetName());

        for (unsigned int i = 0; i < m_meshInstanceNodes.size(); ++i)
        {
            m_meshInstanceNodes[i]->SetDebugName(GetName() + "#" + m_meshInstContainer.GetMeshInstance(i)->GetAttachedSubMeshName());
        }
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::SetRotationSpeedZ(float rot)
    {
        m_meshInstContainer.CallForEach([&](CMeshInstance* pMeshInst) { pMeshInst->SetRotationSpeedZ(rot); });
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::SetRotationSpeedY(float rot)
    {
        m_meshInstContainer.CallForEach([&](CMeshInstance* pMeshInst) { pMeshInst->SetRotationSpeedY(rot); });
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::SetRotationSpeedX(float rot)
    {
        m_meshInstContainer.CallForEach([&](CMeshInstance* pMeshInst) { pMeshInst->SetRotationSpeedX(rot); });
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        if (eventId == MESH_EVENT_SHOW)
        {
            m_meshInstContainer.CallForEach([](const auto& pMeshInstance) { pMeshInstance->GetMaterial().SetRender(true);});
        }
        else if (eventId == MESH_EVENT_HIDE)
        {
            m_meshInstContainer.CallForEach([](const auto& pMeshInstance) { pMeshInstance->GetMaterial().SetRender(false);});
        }
        else if (eventId == MESH_EVENT_SET_EMISSIVE_COLOR)
        {
            float duration = 0.0f;
            Vec4 color = Vec4(0);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", duration);

            if (eventParameter.IsParameterSet("Color"))
                eventParameter.GetParameter("Color", color);

            for (size_t i = 0; i < m_meshInstContainer.GetNumMeshInstances(); ++i)
            {
                if (m_materialsInitial.size() <= i)
                {
                    m_materialsInitial.push_back(m_meshInstContainer.GetMeshInstance(i)->GetMaterial());
                    m_materialsBegin.push_back(m_meshInstContainer.GetMeshInstance(i)->GetMaterial());
                    m_materialsEnd.push_back(m_meshInstContainer.GetMeshInstance(i)->GetMaterial());
                }

                m_materialsBegin[i].SetEmissiveColor(m_meshInstContainer.GetMeshInstance(i)->GetMaterial().GetEmissiveColor());
                m_materialsBegin[i].SetEmissiveEnergy(m_meshInstContainer.GetMeshInstance(i)->GetMaterial().GetEmissiveEnergy());

                m_materialsEnd[i].SetEmissiveColor(Vec3(color));
                m_materialsEnd[i].SetEmissiveEnergy(color.w);
            }

            m_emissiveDuration = duration;
            m_emissiveTime = duration;
        }
        else if (eventId == MESH_EVENT_SET_DIFFUSE_COLOR)
        {
            float duration = 0.0f;
            Vec4 color = Vec4(0);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", duration);

            if (eventParameter.IsParameterSet("Color"))
                eventParameter.GetParameter("Color", color);

            for (size_t i = 0; i < m_meshInstContainer.GetNumMeshInstances(); ++i)
            {
                if (m_materialsInitial.size() <= i)
                {
                    m_materialsInitial.push_back(m_meshInstContainer.GetMeshInstance(i)->GetMaterial());
                    m_materialsBegin.push_back(m_meshInstContainer.GetMeshInstance(i)->GetMaterial());
                    m_materialsEnd.push_back(m_meshInstContainer.GetMeshInstance(i)->GetMaterial());
                }

                m_materialsBegin[i].SetDiffuseColor(m_meshInstContainer.GetMeshInstance(i)->GetMaterial().GetDiffuseColor());
                m_materialsBegin[i].SetDiffuseEnergy(m_meshInstContainer.GetMeshInstance(i)->GetMaterial().GetDiffuseEnergy());

                m_materialsEnd[i].SetDiffuseColor(Vec3(color));
                m_materialsEnd[i].SetDiffuseEnergy(color.w);
            }

            m_diffuseDuration = duration;
            m_diffuseTime = duration;
        }
        else 
        {
            CEntity::OnInput(eventId, eventParameter);
        }
    }

    //-----------------------------------------------------------------------------------
    bool CMeshEntity::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
    {
        if (!m_pNode || !m_meshInstContainer.GetNumMeshInstances())
            return false;

        *ppNode = m_pNode;

        CMeshContainer* pMeshContainer = m_meshInstContainer.GetMeshInstance(0)->GetAttachedMeshContainer();

        bool bCollision = false;

        if (pMeshContainer && pMeshContainer->GetBoundingVolume())
        {
            CBoundingVolume pVolume(*pMeshContainer->GetBoundingVolume());

            pVolume.Transform(m_pNode->GetDerivedRST());
            if (intersection::RayIntersectsBB(pVolume, vRayOrigin, vRayDirection, &distance))
                bCollision = true;
        }

        return bCollision;
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::SetMeshFilename(const std::string& strName)
    {
        if (m_meshInstContainer.GetNumMeshInstances() ||
            m_pMeshInstanceNode ||
            !m_meshInstanceNodes.empty())
        {
            DestroyMeshInstancesAndNodes();
        }

       ENV_ASSERT_DBG(!m_pMeshInstanceNode);
       ENV_ASSERT_DBG(m_meshInstanceNodes.empty());

        m_meshInstContainer.InitFromMeshContainer(strName);
        m_meshInstanceNodes.resize(m_meshInstContainer.GetNumMeshInstances());

        for (unsigned int i = 0; i < m_meshInstanceNodes.size(); ++i)
        {
            m_meshInstanceNodes[i] = gEngine->GetSceneMgr()->CreateSceneNode();
            m_meshInstContainer.GetMeshInstance(i)->AttachNode(m_meshInstanceNodes[i]);

            m_pNode->AttachChildNode(m_meshInstanceNodes[i]);
        }

        InitDebugName();
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::OnGameModeChanged(bool bGameMode)
    {
        CEntity::OnGameModeChanged(bGameMode);

        m_diffuseDuration = -1.0f;
        m_diffuseTime = -1.0f;
        m_emissiveDuration = -1.0f;
        m_emissiveTime = -1.0f;

        for (const auto& node : m_meshInstanceNodes)
            node->SetOrientation(CQuaternion());

        if (bGameMode)
        {
            if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
                m_meshInstContainer.CallForEach([](const auto& pMeshInstance) { pMeshInstance->GetMaterial().RemoveRenderCategory(MESH_RENDER_CATEGORY_OUTLINE);});
        }

        for (size_t i = 0; i < m_materialsInitial.size(); ++i)
            *m_meshInstContainer.GetMeshInstance(i)->GetMaterialPtr() = m_materialsInitial[i];

        m_materialsInitial.clear();
        m_materialsBegin.clear();
        m_materialsEnd.clear();
    }

    //-----------------------------------------------------------------------------------
    void CMeshEntity::Update(float dt)
    {
        CEntity::Update(dt);

        if (!m_pNode)
            return;

        ENV_SCOPED_ACCUM_CPU_PROFILING(Entity, CMeshEntity::Update);

        if (m_diffuseTime >= 0.0f)
        {
            float alpha = 1.0f;
            
            if (m_diffuseDuration > 0.0f)
                alpha = m_diffuseTime / m_diffuseDuration;

            for (size_t i = 0; i < m_materialsBegin.size(); ++i)
            {
                Vec3 diffColor = Lerp(m_materialsEnd[i].GetDiffuseColor(), m_materialsBegin[i].GetDiffuseColor(), alpha);
                float diffEnergy = Lerp(m_materialsEnd[i].GetDiffuseEnergy(), m_materialsBegin[i].GetDiffuseEnergy(), alpha);

                m_meshInstContainer.GetMeshInstance(i)->GetMaterial().SetDiffuseColor(diffColor);
                m_meshInstContainer.GetMeshInstance(i)->GetMaterial().SetDiffuseEnergy(diffEnergy);
            }

            m_diffuseTime -= dt;
        }

        if (m_emissiveTime >= 0.0f)
        {
            float alpha = 1.0f;
            
            if (m_emissiveDuration > 0.0f)
                alpha = m_emissiveTime / m_emissiveDuration;

            for (size_t i = 0; i < m_materialsInitial.size(); ++i)
            {
                Vec3 diffColor = Lerp(m_materialsEnd[i].GetEmissiveColor(), m_materialsBegin[i].GetEmissiveColor(), alpha);
                float diffEnergy = Lerp(m_materialsEnd[i].GetEmissiveEnergy(), m_materialsBegin[i].GetEmissiveEnergy(), alpha);

                m_meshInstContainer.GetMeshInstance(i)->GetMaterial().SetEmissiveColor(diffColor);
                m_meshInstContainer.GetMeshInstance(i)->GetMaterial().SetEmissiveEnergy(diffEnergy);
            }

            m_emissiveTime -= dt;
        }


        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
            if (gEngine->GetEntityMgr()->GetSelectionOutline())
                m_meshInstContainer.CallForEach([](CMeshInstance* pSubMeshInst) { pSubMeshInst->GetMaterial().AddRenderCategory(MESH_RENDER_CATEGORY_OUTLINE); });
            else
                m_meshInstContainer.CallForEach([](CMeshInstance* pSubMeshInst) { pSubMeshInst->GetMaterial().RemoveRenderCategory(MESH_RENDER_CATEGORY_OUTLINE); });

            if (gEngine->GetInput()->IsKeyDown(VK_CONTROL))
            {
                CScenario* pScenario = gEngine->GetWorldMgr()->GetScenario();

                if (gEngine->GetInput()->IsKeyJustDown('1'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(0)); });
                else if (gEngine->GetInput()->IsKeyJustDown('2'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(1)); });
                else if (gEngine->GetInput()->IsKeyJustDown('3'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(2)); });
                else if (gEngine->GetInput()->IsKeyJustDown('4'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(3)); });
                else if (gEngine->GetInput()->IsKeyJustDown('5'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(4)); });
                else if (gEngine->GetInput()->IsKeyJustDown('6'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(5)); });
                else if (gEngine->GetInput()->IsKeyJustDown('7'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(6)); });
                else if (gEngine->GetInput()->IsKeyJustDown('8'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(7)); });
                else if (gEngine->GetInput()->IsKeyJustDown('9'))
                    m_meshInstContainer.CallForEach([&](const auto& subMeshInst) { subMeshInst->GetMaterial().SetDiffuseColor(pScenario->GetTerrainDiffuseColor(8)); });
            }
        }
        else
        {
            m_meshInstContainer.CallForEach([](CMeshInstance* pSubMeshInst) { pSubMeshInst->GetMaterial().RemoveRenderCategory(MESH_RENDER_CATEGORY_OUTLINE); });
        }
    }
}