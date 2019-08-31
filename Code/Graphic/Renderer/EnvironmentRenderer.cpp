#include <Graphic/Renderer/EnvironmentRenderer.h>
#include <Graphic/Renderer/GeometryRenderer.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/Texture.h>
#include <Graphic/Core/Material.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CEnvironmentRenderer::CEnvironmentRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pShaderEnvEnergyField(nullptr)
        , m_pShaderEnvPlanet(nullptr)
        , m_pShaderEnvSkyBox(nullptr)
        , m_pShaderEnvSkyDome(nullptr)
        , m_pShaderEnvSkyPlane(nullptr)
        , m_pSkyboxVB(nullptr)
        , m_pSkyDomeEnergy(nullptr)
        , m_pSkyDomeEnergyAlpha(nullptr)
        , m_pWater(nullptr)
        , m_pCombs(nullptr)
        , m_pClouds(nullptr)
        , m_pPlanetStructure(nullptr)
        , m_pSkyDomeEnergyIntro(nullptr)
        , m_time(0.0f)
    {
    }

    //-----------------------------------------------------------------------------------
    CEnvironmentRenderer::~CEnvironmentRenderer()
    {
        gShaderMgr->Release(m_pShaderEnvEnergyField);
        gShaderMgr->Release(m_pShaderEnvPlanet);
        gShaderMgr->Release(m_pShaderEnvSkyDome);
        gShaderMgr->Release(m_pShaderEnvSkyBox);
        gShaderMgr->Release(m_pShaderEnvSkyPlane);
        gShaderMgr->Release(m_pShaderEnvHalo);

        gTextureMgr->Release(m_pSkyDomeEnergyAlpha);
        gTextureMgr->Release(m_pSkyDomeEnergyIntro);
        gTextureMgr->Release(m_pClouds);
        gTextureMgr->Release(m_pPlanetStructure);
        gTextureMgr->Release(m_pCombs);
        gTextureMgr->Release(m_pWater);
        gTextureMgr->Release(m_pSkyDomeEnergy);

        ENV_DELETE(m_pSkyboxVB);
    }

    //-----------------------------------------------------------------------------------
    bool CEnvironmentRenderer::Initialize()
    {
        m_pShaderEnvEnergyField = gShaderMgr->AddGetPtr("EnvEnergyField");
        m_pShaderEnvPlanet = gShaderMgr->AddGetPtr("EnvPlanet");
        m_pShaderEnvSkyDome = gShaderMgr->AddGetPtr("EnvSkyDome");
        m_pShaderEnvSkyBox = gShaderMgr->AddGetPtr("EnvSkyBox");
        m_pShaderEnvSkyPlane = gShaderMgr->AddGetPtr("EnvSkyPlane");
        m_pShaderEnvHalo = gShaderMgr->AddGetPtr("EnvHalo");

        if (nullptr == m_pShaderEnvEnergyField ||
            nullptr == m_pShaderEnvPlanet ||
            nullptr == m_pShaderEnvSkyDome ||
            nullptr == m_pShaderEnvSkyBox ||
            nullptr == m_pShaderEnvSkyPlane ||
            nullptr == m_pShaderEnvHalo)
        {
            return false;
        }

        m_pShaderEnvEnergyField->LinkConstantBufferCPU("cb_EnvVS", m_constBufEnvVS);
        m_pShaderEnvEnergyField->LinkConstantBufferCPU("cb_EnergyFieldPS", m_constBufEnergyFieldPS);

        m_pShaderEnvHalo->LinkConstantBufferCPU("cb_EnvVS", m_constBufEnvVS);
        m_pShaderEnvHalo->LinkConstantBufferCPU("cb_HaloPS", m_constBufHaloPS);

        m_pShaderEnvPlanet->LinkConstantBufferCPU("cb_EnvVS", m_constBufEnvVS);
        m_pShaderEnvPlanet->LinkConstantBufferCPU("cb_PlanetPS", m_constBufPlanetPS);

        m_pShaderEnvSkyBox->LinkConstantBufferCPU("cb_SkyBoxVS", m_constBufSkyBoxWVPVS);

        m_pShaderEnvSkyDome->LinkConstantBufferCPU("cb_EnvVS", m_constBufEnvVS);
        m_pShaderEnvSkyDome->LinkConstantBufferCPU("cb_SkyDomePS", m_constBufSkyDomePS);

        m_pShaderEnvSkyPlane->LinkConstantBufferCPU("cb_EnvVS", m_constBufEnvVS);
        m_pShaderEnvSkyPlane->LinkConstantBufferCPU("cb_SkyPlanePS", m_constBufSkyPlanePS);

        m_pSkyDomeEnergy = gTextureMgr->AddGetPtr("SkyDomeEnergy.png");
        m_pSkyDomeEnergyIntro = gTextureMgr->AddGetPtr("SkyDomeEnergyIntro.png");
        m_pSkyDomeEnergyAlpha = gTextureMgr->AddGetPtr("SkyDomeEnergyAlpha.png");
        m_pCombs = gTextureMgr->AddGetPtr("Combs.png");
        m_pPlanetStructure = gTextureMgr->AddGetPtr("planetStructure.dds");
        m_pClouds = gTextureMgr->AddGetPtr("Skies/clouds.dds");
        m_pWater = gTextureMgr->AddGetPtr("water_nmap01.png");

        ENV_ASSERT_DBG(nullptr != m_pSkyDomeEnergy);
        ENV_ASSERT_DBG(nullptr != m_pSkyDomeEnergyIntro);
        ENV_ASSERT_DBG(nullptr != m_pSkyDomeEnergyAlpha);
        ENV_ASSERT_DBG(nullptr != m_pCombs);
        ENV_ASSERT_DBG(nullptr != m_pPlanetStructure);
        ENV_ASSERT_DBG(nullptr != m_pClouds);
        ENV_ASSERT_DBG(nullptr != m_pWater);

        m_pEnvEnergyFieldVertexDecl = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderEnvEnergyField);
        m_pEnvPlanetVertexDecl = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderEnvPlanet);
        m_pEnvSkyBoxVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShaderEnvSkyBox);
        m_pEnvSkyPlaneVertexDecl = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderEnvSkyPlane);
        m_pEnvSkyDomeVertexDecl = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderEnvSkyDome);
        m_pEnvHaloVertexDecl = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderEnvHalo);

        m_pSkyDomeEnergy->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pSkyDomeEnergyIntro->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pSkyDomeEnergyAlpha->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pCombs->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pPlanetStructure->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pClouds->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pWater->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pWater->SetSRGB(false);

        m_setupStateBlock.SetNumStates(m_pDevice, 11);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILREF, 0);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_EQUAL);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_KEEP);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_KEEP);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        m_setupStateBlock.AddRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);

        m_resetStateBlock.SetNumStates(m_pDevice, 2, 5);
        m_resetStateBlock.AddRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
        m_resetStateBlock.AddRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, true);


        return true;
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::Reload(bool bForceReload)
    {
        if (m_pSkyboxVB)
            return;

        m_pSkyboxVB = m_pDevice->CreateVertexBuffer(sizeof(PosTexVertex) * 36, USAGE_TYPE_VERTEXINDEXBUFFERS);
        //m_pDevice->GetDevice()->CreateVertexBuffer(sizeof(VertexPT) * 36, USAGE_TYPE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pSkyboxVB, NULL);

        PosTexVertex* pSkyBox = NULL;
        pSkyBox = static_cast<PosTexVertex*>(m_pSkyboxVB->Lock(0, 0, 0));

        Vec3 vertices[8];
        vertices[0] = Vec3(-1.0f, -1.0f, -1.0f);
        vertices[1] = Vec3(-1.0f, -1.0f, 1.0f);
        vertices[2] = Vec3(1.0f, -1.0f, 1.0f);
        vertices[3] = Vec3(1.0f, -1.0f, -1.0f);

        vertices[4] = Vec3(-1.0f, 1.0f, -1.0f);
        vertices[5] = Vec3(-1.0f, 1.0f, 1.0f);
        vertices[6] = Vec3(1.0f, 1.0f, 1.0f);
        vertices[7] = Vec3(1.0f, 1.0f, -1.0f);

        // left
        pSkyBox[0].p = vertices[0];
        pSkyBox[1].p = vertices[4];
        pSkyBox[2].p = vertices[1];

        pSkyBox[3].p = vertices[1];
        pSkyBox[4].p = vertices[4];
        pSkyBox[5].p = vertices[5];

        // front
        pSkyBox[6].p = vertices[1];
        pSkyBox[7].p = vertices[5];
        pSkyBox[8].p = vertices[6];

        pSkyBox[9].p = vertices[1];
        pSkyBox[10].p = vertices[6];
        pSkyBox[11].p = vertices[2];

        // right
        pSkyBox[12].p = vertices[2];
        pSkyBox[13].p = vertices[6];
        pSkyBox[14].p = vertices[7];

        pSkyBox[15].p = vertices[2];
        pSkyBox[16].p = vertices[7];
        pSkyBox[17].p = vertices[3];

        // back
        pSkyBox[18].p = vertices[3];
        pSkyBox[19].p = vertices[7];
        pSkyBox[20].p = vertices[4];

        pSkyBox[21].p = vertices[3];
        pSkyBox[22].p = vertices[4];
        pSkyBox[23].p = vertices[0];

        // top
        pSkyBox[24].p = vertices[5];
        pSkyBox[25].p = vertices[4];
        pSkyBox[26].p = vertices[7];

        pSkyBox[27].p = vertices[5];
        pSkyBox[28].p = vertices[7];
        pSkyBox[29].p = vertices[6];

        // bottom
        pSkyBox[30].p = vertices[0];
        pSkyBox[31].p = vertices[1];
        pSkyBox[32].p = vertices[3];

        pSkyBox[33].p = vertices[3];
        pSkyBox[34].p = vertices[1];
        pSkyBox[35].p = vertices[2];

        for (int i = 0; i < 36; i++)
            pSkyBox[i].t = Vec2(0.0f, 0.0f);

        m_pSkyboxVB->Unlock();
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::OnLostDevice()
    {
        m_setupStateBlock.OnLostDevice();
        m_resetStateBlock.OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_setupStateBlock.OnResetDevice();
        m_resetStateBlock.OnResetDevice();

        Reload(true);
    }


    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        SetupEnvRendering(renderQueue, renderSet);

        RenderSkyDome(renderQueue, renderSet);

        RenderHalo(renderQueue, renderSet);

        RenderSkybox(renderQueue, renderSet);

        RenderEnergyField(renderQueue, renderSet);

        RenderPlanet(renderQueue, renderSet);

        RenderSkyPlane(renderQueue, renderSet);

        CleanupEnvRendering(renderQueue, renderSet);
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::SetupEnvRendering(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pEnvironmentRenderObject == nullptr)
        {
            return;
        }

        m_time += renderQueue.timeElapsedFrame;

        Reload();

        Vec3 sunColor(1, 1, 1);

        if (renderQueue.m_pSunRenderObject)
        {
            sunColor = renderQueue.m_pSunRenderObject->m_diffuseColor;
        }

        m_pDevice->SetRenderTarget(0, renderSet.m_pRenderTargetPostProcess01);
        m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::CleanupEnvRendering(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_resetStateBlock.Apply();
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::RenderSkyPlane(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pSkyPlaneRenderObject)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_NONE);
            m_pDevice->SetTextureSampler(1, m_pSkyDomeEnergy->GetTextureSampler());
            m_pDevice->SetTexture(1, m_pSkyDomeEnergy);
            m_pDevice->SetTextureSampler(2, m_pWater->GetTextureSampler());
            m_pDevice->SetTexture(2, m_pWater);
            m_pDevice->SetShader(m_pShaderEnvSkyPlane);
            m_pDevice->SetVertexDeclaration(m_pEnvSkyPlaneVertexDecl.get());

            m_setupStateBlock.Apply();

            m_constBufSkyPlanePS.GetDataForWriting().m_cloudIntensity = renderQueue.m_pSkyPlaneRenderObject->m_alpha;
            m_constBufSkyPlanePS.GetDataForWriting().m_skyColor = renderQueue.m_pEnvironmentRenderObject->m_skyColor;
            m_constBufSkyPlanePS.GetDataForWriting().m_time = m_time;

            auto setConstBufVS = [&](const Matrix4x4& mtxModel, const Matrix4x4& mtxModelInvTrans, const Matrix4x4& mtxWVP)
            {
                m_constBufEnvVS.GetDataForWriting().m_model = mtxModel;
                m_constBufEnvVS.GetDataForWriting().m_modelInvTrans = mtxModelInvTrans;
                m_constBufEnvVS.GetDataForWriting().m_worldViewProj = mtxWVP;
            };

            mapMeshInstanceRenderObjects& meshes = renderQueue.m_pSkyPlaneRenderObject->m_meshInstances;

            RenderMeshes(meshes, m_pEnvSkyPlaneVertexDecl.get(), renderQueue, true,
                         [](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return pMesh->m_mtxTransform * pInstance->m_mtxTransform; },
                         [&](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return renderQueue.m_pCameraRenderObject->m_mtxViewProjection; },
                         setConstBufVS);

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_BACK);
        }
    }

    //-----------------------------------------------------------------------------------
    template<typename ModelMtxLambda, typename WVPMtxLambda, typename SetConstBufLambda>
    void CEnvironmentRenderer::RenderMeshes(mapMeshInstanceRenderObjects& meshes, CVertexDeclaration* pVertexDeclaratrion, RenderQueue& renderQueue, bool useMeshTexture,
                                            ModelMtxLambda getModelMatrix, WVPMtxLambda getWVPMatrix, SetConstBufLambda setConstBuf)
    {
        for (mapMeshInstanceRenderObjects::iterator itMeshData = meshes.begin(); itMeshData != meshes.end(); ++itMeshData)
        {
            MeshRenderObject* pMesh = itMeshData->first;

            if (!pMesh)
                continue;

            listMeshInstanceRenderObjects& instances = itMeshData->second;

            ENV_ASSERT_DBG(pMesh->m_pIndexBuffer);
            ENV_ASSERT_DBG(pMesh->m_pVertexBuffer);

            m_pDevice->SetVertexDeclaration(pVertexDeclaratrion);

            for (listMeshInstanceRenderObjects::iterator itInstances = instances.begin(); itInstances != instances.end(); ++itInstances)
            {
                MeshInstanceRenderObject* pInstance = *itInstances;

                if (!pInstance->m_pMaterial)
                    continue;

                if (useMeshTexture)
                    m_pDevice->SetTexture(0, pInstance->m_pMaterial->GetTexture());

                Matrix4x4 mtxModel = getModelMatrix(pMesh, pInstance);
                Matrix4x4 mtxModelInvTrans = glm::transpose(glm::inverse((mtxModel)));

                setConstBuf(mtxModel, mtxModelInvTrans, getWVPMatrix(pMesh, pInstance));

                CGeometryRenderer::DrawMeshRenderObject(m_pDevice, *pMesh);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::RenderSkyDome(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pSkyDomeRenderObject)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetTextureSampler(1, m_pSkyDomeEnergy->GetTextureSampler());
            m_pDevice->SetTexture(1, m_pSkyDomeEnergy);
            m_pDevice->SetTextureSampler(2, m_pWater->GetTextureSampler());
            m_pDevice->SetTexture(2, m_pWater);
            m_pDevice->SetTextureSampler(3, m_pSkyDomeEnergyAlpha->GetTextureSampler());
            m_pDevice->SetTexture(3, m_pSkyDomeEnergyAlpha);
            m_pDevice->SetShader(m_pShaderEnvSkyDome);
            m_pDevice->SetVertexDeclaration(m_pEnvSkyDomeVertexDecl.get());

            if (renderQueue.m_pSkyDomeRenderObject->m_pSkyTexture)
            {
                m_pDevice->SetTextureSampler(0, renderQueue.m_pSkyDomeRenderObject->m_pSkyTexture->GetTextureSampler());
                m_pDevice->SetTexture(0, renderQueue.m_pSkyDomeRenderObject->m_pSkyTexture);
            }

            m_setupStateBlock.Apply();

            m_constBufSkyDomePS.GetDataForWriting().m_skyColor = renderQueue.m_pEnvironmentRenderObject->m_skyColor;
            m_constBufSkyDomePS.GetDataForWriting().m_fogColor = renderQueue.m_pEnvironmentRenderObject->m_fogColor;
            m_constBufSkyDomePS.GetDataForWriting().m_energyFieldIntensity = renderQueue.m_pEnvironmentRenderObject->m_energyFieldIntensity;
            m_constBufSkyDomePS.GetDataForWriting().m_time = m_time;

            auto setConstBufVS = [&](const Matrix4x4& mtxModel, const Matrix4x4& mtxModelInvTrans, const Matrix4x4& mtxWVP)
            {
                m_constBufEnvVS.GetDataForWriting().m_model = mtxModel;
                m_constBufEnvVS.GetDataForWriting().m_modelInvTrans = mtxModelInvTrans;
                m_constBufEnvVS.GetDataForWriting().m_worldViewProj = mtxWVP;
            };

            mapMeshInstanceRenderObjects& meshes = renderQueue.m_pSkyDomeRenderObject->m_meshInstances;

            RenderMeshes(meshes, m_pEnvSkyDomeVertexDecl.get(), renderQueue, false,
                         [](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return pMesh->m_mtxTransform; },
                         [&](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return pInstance->m_mtxTransform * renderQueue.m_pCameraRenderObject->m_mtxViewProjection; },
                         setConstBufVS);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::RenderPlanet(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pPlanetRenderObject)
        {
            m_pDevice->ResetToDefaultRenderStates();

            m_pDevice->SetTextureSampler(1, m_pSkyDomeEnergy->GetTextureSampler());
            m_pDevice->SetTexture(1, m_pSkyDomeEnergy);
            m_pDevice->SetTextureSampler(2, m_pWater->GetTextureSampler());
            m_pDevice->SetTexture(2, m_pWater);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
            m_pDevice->SetTextureSampler(1, m_pPlanetStructure->GetTextureSampler());
            m_pDevice->SetTexture(1, m_pPlanetStructure);
            m_pDevice->SetTextureSampler(2, m_pClouds->GetTextureSampler());
            m_pDevice->SetTexture(2, m_pClouds);
            m_pDevice->SetTextureSampler(3, m_pWater->GetTextureSampler());
            m_pDevice->SetTexture(3, m_pWater);
            m_pDevice->SetShader(m_pShaderEnvPlanet);
            m_pDevice->SetVertexDeclaration(m_pEnvPlanetVertexDecl.get());

            m_setupStateBlock.Apply();

            m_constBufPlanetPS.GetDataForWriting().m_fogColor = renderQueue.m_pEnvironmentRenderObject->m_fogColor;
            m_constBufPlanetPS.GetDataForWriting().m_cloudIntensity = renderQueue.m_pEnvironmentRenderObject->m_cloudIntensity;
            m_constBufPlanetPS.GetDataForWriting().m_energyFieldColor = renderQueue.m_pPlanetRenderObject->m_color;
            m_constBufPlanetPS.GetDataForWriting().m_time = m_time;

            auto setConstBufVS = [&](const Matrix4x4& mtxModel, const Matrix4x4& mtxModelInvTrans, const Matrix4x4& mtxWVP)
            {
                m_constBufEnvVS.GetDataForWriting().m_model = mtxModel;
                m_constBufEnvVS.GetDataForWriting().m_modelInvTrans = mtxModelInvTrans;
                m_constBufEnvVS.GetDataForWriting().m_worldViewProj = mtxWVP;
            };

            mapMeshInstanceRenderObjects& meshes = renderQueue.m_pPlanetRenderObject->m_meshInstances;

            const Vec3& camPos = renderQueue.m_pCameraRenderObject->m_position;
            Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), Vec3(camPos.x, camPos.y, camPos.z)));
            RenderMeshes(meshes, m_pEnvPlanetVertexDecl.get(), renderQueue, true,
                         [&](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return pMesh->m_mtxTransform * pInstance->m_mtxTransform *  mtxTranslation * renderQueue.m_pCameraRenderObject->m_mtxView; },
                         [&](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return renderQueue.m_pCameraRenderObject->m_mtxProjection; },
                         setConstBufVS);

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::RenderSkybox(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pSkyBoxRenderObject == nullptr)
            return;

        m_pDevice->ResetToDefaultRenderStates();

        m_pDevice->SetTextureSampler(1, m_pSkyDomeEnergy->GetTextureSampler());
        m_pDevice->SetTexture(1, m_pSkyDomeEnergy);
        m_pDevice->SetTextureSampler(2, m_pWater->GetTextureSampler());
        m_pDevice->SetTexture(2, m_pWater);

        m_setupStateBlock.Apply();

        m_constBufSkyBoxWVPVS.GetDataForWriting() = renderQueue.m_pSkyBoxRenderObject->m_mtxTransform * renderQueue.m_pCameraRenderObject->m_mtxViewProjection;

        m_pDevice->SetVertexDeclaration(m_pEnvSkyBoxVertexDecl.get());
        m_pDevice->SetVertexBuffer(0, 0, m_pSkyboxVB);
        m_pDevice->SetShader(m_pShaderEnvSkyBox);
        m_pDevice->SetTextureSampler(0, renderQueue.m_pSkyBoxRenderObject->m_pSkyTexture->GetTextureSampler());
        m_pDevice->SetTexture(0, renderQueue.m_pSkyBoxRenderObject->m_pSkyTexture);
        m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 12);

    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::RenderEnergyField(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pEnergyFieldRenderObject)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_setupStateBlock.Apply();

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
            m_pDevice->SetTextureSampler(1, m_pSkyDomeEnergy->GetTextureSampler());
            m_pDevice->SetTexture(1, m_pSkyDomeEnergy);
            m_pDevice->SetTextureSampler(2, m_pWater->GetTextureSampler());
            m_pDevice->SetTexture(2, m_pWater);
            m_pDevice->SetTextureSampler(3, m_pCombs->GetTextureSampler());
            m_pDevice->SetTexture(3, m_pCombs);
            m_pDevice->SetShader(m_pShaderEnvEnergyField);

            m_constBufEnergyFieldPS.GetDataForWriting().m_fogColor = Vec4(renderQueue.m_pEnvironmentRenderObject->m_fogColor, 0);
            m_constBufEnergyFieldPS.GetDataForWriting().m_skyColor = Vec4(renderQueue.m_pEnvironmentRenderObject->m_skyColor, 0);
            m_constBufEnergyFieldPS.GetDataForWriting().m_energyFieldColor = Vec4(1, 0, 0, 0);
            m_constBufEnergyFieldPS.GetDataForWriting().m_time = m_time;

            auto setConstBufVS = [&](const Matrix4x4& mtxModel, const Matrix4x4& mtxModelInvTrans, const Matrix4x4& mtxWVP)
            {
                m_constBufEnvVS.GetDataForWriting().m_model = mtxModel;
                m_constBufEnvVS.GetDataForWriting().m_modelInvTrans = mtxModelInvTrans;
                m_constBufEnvVS.GetDataForWriting().m_worldViewProj = mtxWVP;
            };

            mapMeshInstanceRenderObjects& meshes = renderQueue.m_pPlanetRenderObject->m_meshInstances;

            RenderMeshes(meshes, m_pEnvEnergyFieldVertexDecl.get(), renderQueue, false,
                         [](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return pMesh->m_mtxTransform * pInstance->m_mtxTransform; },
                         [&](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return renderQueue.m_pCameraRenderObject->m_mtxViewProjection; },
                         setConstBufVS);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEnvironmentRenderer::RenderHalo(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (!renderQueue.m_pHaloRenderObject)
            return;

        m_pDevice->ResetToDefaultRenderStates();
        m_setupStateBlock.Apply();
        m_pDevice->SetTextureSampler(1, m_pSkyDomeEnergyIntro->GetTextureSampler());
        m_pDevice->SetTexture(1, m_pSkyDomeEnergyIntro);
        m_pDevice->SetTextureSampler(2, m_pWater->GetTextureSampler());
        m_pDevice->SetTexture(2, m_pWater);
//         m_pDevice->SetTextureSampler(3, m_pSkyDomeEnergyAlpha->GetTextureSampler());
//         m_pDevice->SetTexture(3, m_pSkyDomeEnergyAlpha);

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_SRGBTEXTURE, true);
        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_MAXANISOTROPY, 8);
        m_pDevice->SetSamplerState(1, SAMPLERSTATE_TYPE_MAXANISOTROPY, 8);
        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_MIPFILTER, TEXTUREFILTER_TYPE_LINEAR);
        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_MAGFILTER, TEXTUREFILTER_TYPE_LINEAR);
        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_MINFILTER, TEXTUREFILTER_TYPE_ANISOTROPIC);
        m_pDevice->SetSamplerState(1, SAMPLERSTATE_TYPE_MIPFILTER, TEXTUREFILTER_TYPE_LINEAR);
        m_pDevice->SetSamplerState(1, SAMPLERSTATE_TYPE_MAGFILTER, TEXTUREFILTER_TYPE_LINEAR);
        m_pDevice->SetSamplerState(1, SAMPLERSTATE_TYPE_MINFILTER, TEXTUREFILTER_TYPE_ANISOTROPIC);
        m_pDevice->SetSamplerState(2, SAMPLERSTATE_TYPE_MIPFILTER, TEXTUREFILTER_TYPE_LINEAR);
        m_pDevice->SetSamplerState(2, SAMPLERSTATE_TYPE_MAGFILTER, TEXTUREFILTER_TYPE_LINEAR);
        m_pDevice->SetSamplerState(2, SAMPLERSTATE_TYPE_MINFILTER, TEXTUREFILTER_TYPE_ANISOTROPIC);

        m_pDevice->SetShader(m_pShaderEnvHalo);

        m_constBufHaloPS.GetDataForWriting().m_intensityLeft = renderQueue.m_pHaloRenderObject->m_haloIntensityLeft;
        m_constBufHaloPS.GetDataForWriting().m_intensityRight = renderQueue.m_pHaloRenderObject->m_haloIntensityRight;
        m_constBufHaloPS.GetDataForWriting().m_time = m_time;

        auto setConstBufVS = [&](const Matrix4x4& mtxModel, const Matrix4x4& mtxModelInvTrans, const Matrix4x4& mtxWVP)
        {
            m_constBufEnvVS.GetDataForWriting().m_model = mtxModel;
            m_constBufEnvVS.GetDataForWriting().m_modelInvTrans = mtxModelInvTrans;
            m_constBufEnvVS.GetDataForWriting().m_worldViewProj = mtxWVP;
        };

        mapMeshInstanceRenderObjects& meshes = renderQueue.m_pHaloRenderObject->m_meshInstances;

        RenderMeshes(meshes, m_pEnvHaloVertexDecl.get(), renderQueue, true,
                     [](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return pMesh->m_mtxTransform * pInstance->m_mtxTransform; },
                     [&](MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance) { return renderQueue.m_pCameraRenderObject->m_mtxViewProjection; },
                     setConstBufVS);
    }
}