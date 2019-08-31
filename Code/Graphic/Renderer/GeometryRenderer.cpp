#include <Graphic/Renderer/GeometryRenderer.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/TextureSampler.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Renderer/FullscreenTextureRenderer.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/Core/Material.h>

namespace env
{


    //-----------------------------------------------------------------------------------
    CGeometryRenderer::CGeometryRenderer(CRenderDevice* pDevice,
                                         CFullscreenTextureRenderer* pFSTextureRenderer)
        : m_pDevice(pDevice)
        , m_pShaderGeometryForward(nullptr)
        , m_pShaderGeometryNormal(nullptr)
        , m_pShaderGeometryTransmissive(nullptr)
        , m_pShaderGeometryCharacter(nullptr)
        , m_pShaderGeometryCharacterDistort(nullptr)
        , m_pShaderGeometrySnow(nullptr)
        , m_pShaderGeometryWet(nullptr)
        , m_pShaderGeometryOutline(nullptr)
        , m_pShaderGeometryMotionBlur(nullptr)
        , m_pShaderTerrainSelection(nullptr)
        , m_pShaderTerrainForward(nullptr)
        , m_pShaderTerrainNormal(nullptr)
        , m_pShaderTerrainSnow(nullptr)
        , m_pShaderTerrainWet(nullptr)
        , m_pShaderTerrainBorder(nullptr)
        , m_pWaterNormalMap(nullptr)
        , m_pBorderTexture(nullptr)
        , m_pWaterSpecularityMap(nullptr)
        , m_pSnowTexture(nullptr)
        , m_pTerrainSelectionDepth(nullptr)
        , m_time(0.0f) {}

    //-----------------------------------------------------------------------------------
    CGeometryRenderer::~CGeometryRenderer()
    {
        gShaderMgr->Release(m_pShaderTerrainBorder);
        gShaderMgr->Release(m_pShaderGeometryMotionBlur);
        gShaderMgr->Release(m_pShaderGeometryOutline);
        gShaderMgr->Release(m_pShaderGeometryForward);
        gShaderMgr->Release(m_pShaderGeometryTransmissive);
        gShaderMgr->Release(m_pShaderGeometryCharacterDistort);
        gShaderMgr->Release(m_pShaderGeometryCharacter);
        gShaderMgr->Release(m_pShaderGeometryNormal);
        gShaderMgr->Release(m_pShaderGeometrySnow);
        gShaderMgr->Release(m_pShaderGeometryWet);
        gShaderMgr->Release(m_pShaderTerrainSelection);
        gShaderMgr->Release(m_pShaderTerrainForward);
        gShaderMgr->Release(m_pShaderTerrainNormal);
        gShaderMgr->Release(m_pShaderTerrainSnow);
        gShaderMgr->Release(m_pShaderTerrainWet);

        gTextureMgr->Release(m_pBorderTexture);
        gTextureMgr->Release(m_pWaterNormalMap);
        gTextureMgr->Release(m_pWaterSpecularityMap);
        gTextureMgr->Release(m_pSnowTexture);
    }

    //-----------------------------------------------------------------------------------
    bool CGeometryRenderer::Initialize()
    {
        m_pShaderGeometryForward = gShaderMgr->AddGetPtr("GeometryForward");
        m_pShaderGeometryNormal = gShaderMgr->AddGetPtr("GeometryNormal");
        m_pShaderGeometrySnow = gShaderMgr->AddGetPtr("GeometrySnow");
        m_pShaderGeometryWet = gShaderMgr->AddGetPtr("GeometryWet");
        m_pShaderGeometryTransmissive = gShaderMgr->AddGetPtr("GeometryTransmissive");
        m_pShaderGeometryCharacter = gShaderMgr->AddGetPtr("GeometryCharacter");
        m_pShaderGeometryCharacterDistort = gShaderMgr->AddGetPtr("GeometryCharacterDistort");
        m_pShaderGeometryOutline = gShaderMgr->AddGetPtr("GeometryOutline");
        m_pShaderGeometryMotionBlur = gShaderMgr->AddGetPtr("GeometryMotionBlur");
        m_pShaderTerrainSelection = gShaderMgr->AddGetPtr("TerrainSelection");
        m_pShaderTerrainForward = gShaderMgr->AddGetPtr("TerrainForward");
        m_pShaderTerrainNormal = gShaderMgr->AddGetPtr("TerrainNormal");
        m_pShaderTerrainSnow = gShaderMgr->AddGetPtr("TerrainSnow");
        m_pShaderTerrainWet = gShaderMgr->AddGetPtr("TerrainWet");
        m_pShaderTerrainBorder = gShaderMgr->AddGetPtr("TerrainBorder");

        if (nullptr == m_pShaderGeometryForward ||
            nullptr == m_pShaderGeometryNormal ||
            nullptr == m_pShaderGeometrySnow ||
            nullptr == m_pShaderGeometryWet ||
            nullptr == m_pShaderGeometryTransmissive ||
            nullptr == m_pShaderGeometryCharacter ||
            nullptr == m_pShaderGeometryCharacterDistort ||
            nullptr == m_pShaderGeometryOutline ||
            nullptr == m_pShaderGeometryMotionBlur ||
            nullptr == m_pShaderTerrainSelection ||
            nullptr == m_pShaderTerrainForward ||
            nullptr == m_pShaderTerrainNormal ||
            nullptr == m_pShaderTerrainSnow ||
            nullptr == m_pShaderTerrainWet ||
            nullptr == m_pShaderTerrainBorder)
        {
            return false;
        }

        m_pShaderGeometryNormal->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryNormal->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);

        m_pShaderGeometryWet->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryWet->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);
        //m_pShaderGeometryWet->LinkConstantBufferCPU("cb_GeometryWetPS", m_constBufGeometryWetPS);

        m_pShaderGeometrySnow->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometrySnow->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);

        m_pShaderGeometryTransmissive->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryTransmissive->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);

        m_pShaderGeometryForward->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryForward->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);

        m_pShaderGeometryOutline->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryOutline->LinkConstantBufferCPU("cb_GeometryOutline", m_constBufGeometryOutlinePS);

        m_pShaderTerrainNormal->LinkConstantBufferCPU("cb_TerrainVS", m_constBufTerrainNormalVS);
        m_pShaderTerrainNormal->LinkConstantBufferCPU("cb_TerrainPS", m_constBufTerrainNormalPS);

        m_pShaderTerrainWet->LinkConstantBufferCPU("cb_TerrainVS", m_constBufTerrainNormalVS);
        m_pShaderTerrainWet->LinkConstantBufferCPU("cb_TerrainPS", m_constBufTerrainNormalPS);

        m_pShaderTerrainSnow->LinkConstantBufferCPU("cb_TerrainVS", m_constBufTerrainNormalVS);
        m_pShaderTerrainSnow->LinkConstantBufferCPU("cb_TerrainPS", m_constBufTerrainNormalPS);

        m_pShaderTerrainForward->LinkConstantBufferCPU("cb_TerrainVS", m_constBufTerrainNormalVS);

        m_pShaderTerrainSelection->LinkConstantBufferCPU("cb_TerrainVS", m_constBufTerrainNormalVS);

        m_pShaderTerrainBorder->LinkConstantBufferCPU("cb_TerrainBorderVS", m_constBufTerrainBorderVS);
        m_pShaderTerrainBorder->LinkConstantBufferCPU("cb_TerrainBorderPS", m_constBufTerrainBorderPS);

        m_pShaderGeometryCharacterDistort->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryCharacterDistort->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);
        m_pShaderGeometryCharacterDistort->LinkConstantBufferCPU("cb_GeometryCharPS", m_constBufGeometrCharPS);

        m_pShaderGeometryCharacter->LinkConstantBufferCPU("cb_GeometryVS", m_constBufGeometryNormalVS);
        m_pShaderGeometryCharacter->LinkConstantBufferCPU("cb_GeometryPS", m_constBufGeometryNormalPS);
        m_pShaderGeometryCharacter->LinkConstantBufferCPU("cb_GeometryCharPS", m_constBufGeometrCharPS);

        m_pTerrainSelectionDepth = m_pDevice->CreateRenderTarget(128, 128, TEXTUREFORMAT_TYPE_R32F, true);

        m_pLevelVertexDeclaration = m_pDevice->CreateVertexDeclaration(TerrainVertex::VertexElements, sizeof(TerrainVertex), m_pShaderTerrainNormal);
        m_pLevelBorderVertexDeclaration = m_pDevice->CreateVertexDeclaration(TerrainBorderVertex::VertexElements, sizeof(TerrainBorderVertex), m_pShaderTerrainBorder);
        m_pLevelForwardVertexDeclaration = m_pDevice->CreateVertexDeclaration(TerrainVertex::VertexElements, sizeof(TerrainVertex), m_pShaderTerrainForward);
        m_pGeometryVertexDeclaration = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderGeometryNormal);
        m_pGeometryForwardVertexDeclaration = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderGeometryForward);

        if (nullptr == m_pLevelVertexDeclaration ||
            nullptr == m_pLevelBorderVertexDeclaration ||
            nullptr == m_pLevelForwardVertexDeclaration ||
            nullptr == m_pGeometryVertexDeclaration ||
            nullptr == m_pGeometryForwardVertexDeclaration)
        {
            return false;
        }

        gTextureMgr->SetAllocPolicy(CTextureMgr::AllocPolicyType(gTextureMgr->GetAllocPolicy(), false));

        m_pWaterNormalMap = gTextureMgr->AddGetPtr("water_nmap01.png");
        m_pWaterSpecularityMap = gTextureMgr->AddGetPtr("water_specular01.png");
        m_pSnowTexture = gTextureMgr->AddGetPtr("Terrain/snow01.dds");

        gTextureMgr->SetAllocPolicy(CTextureMgr::AllocPolicyType(gTextureMgr->GetAllocPolicy(), true));


        if (nullptr == m_pWaterNormalMap ||
            nullptr == m_pWaterSpecularityMap ||
            nullptr == m_pSnowTexture)
        {
            return false;
        }

        m_pWaterNormalMap->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        m_pWaterSpecularityMap->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        m_pSnowTexture->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());


        m_initialStateBlock.SetNumStates(m_pDevice, 4, 11);
        m_initialStateBlock.AddSamplerState(0, SAMPLERSTATE_TYPE_SRGBTEXTURE, true);
        m_initialStateBlock.AddSamplerState(2, SAMPLERSTATE_TYPE_SRGBTEXTURE, true);
        m_initialStateBlock.AddSamplerState(3, SAMPLERSTATE_TYPE_SRGBTEXTURE, true);

        m_initialStateBlock.AddRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_initialStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
        m_initialStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
        m_initialStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);


        m_levelTerrainStateBlock.SetNumStates(m_pDevice, 2);

        m_levelTerrainStateBlock.AddRenderState(RENDERSTATE_TYPE_ALPHATESTENABLE, false);
        m_levelTerrainStateBlock.AddRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_TERRAIN);


        m_finalStateBlock.SetNumStates(m_pDevice, 0, 3);

        m_finalStateBlock.AddSamplerState(0, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_finalStateBlock.AddSamplerState(2, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_finalStateBlock.AddSamplerState(3, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::ClearRenderTargets()
    {
        m_pDevice->Clear(Vec4(0, 0, 0, 0), CLEAR_TYPE_STENCIL | CLEAR_TYPE_TARGET | CLEAR_TYPE_ZBUFFER, 1.0f, 0);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::SetupDeferredRendering(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_numMeshTrianglesRendered = 0;
        m_numMeshInstancesRendered = 0;
        m_numMeshTypesRendered = 0;
        m_numTerrainTrianglesRendered = 0;
        m_numBorderTrianglesRendered = 0;

        m_initialStateBlock.Apply();

        m_time += renderQueue.timeElapsedFrame;        

        m_pDevice->SetRenderTarget(0, renderSet.m_pRenderTargetColors);
        m_pDevice->SetRenderTarget(1, renderSet.m_pRenderTargetNormals);
        m_pDevice->SetRenderTarget(2, renderSet.m_pRenderTargetDepths);
        m_pDevice->SetRenderTarget(3, renderSet.m_pRenderTargetEmissive);

        m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);

        m_pDevice->SetTextureSampler(6, m_pSnowTexture->GetTextureSampler());
        m_pDevice->SetTexture(6, m_pSnowTexture);
        m_pDevice->SetTextureSampler(2, m_pWaterNormalMap->GetTextureSampler());
        m_pDevice->SetTexture(2, m_pWaterNormalMap);
        m_pDevice->SetTextureSampler(4, m_pWaterSpecularityMap->GetTextureSampler());
        m_pDevice->SetTexture(4, m_pWaterSpecularityMap);

        m_pDevice->Clear(Vec4(0, 0, 0, 0.25f), CLEAR_TYPE_STENCIL | CLEAR_TYPE_TARGET | CLEAR_TYPE_ZBUFFER, 1.0f, 0);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::CleanupDeferredRendering(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHATESTENABLE, false);

        // Render meshes without z-buffering and without lighting
        //
        m_pDevice->SetRenderTarget(1, nullptr);
        m_pDevice->SetRenderTarget(2, nullptr);
        m_pDevice->SetRenderTarget(3, nullptr);

        m_finalStateBlock.Apply();
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderGeometryOutline(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.bRenderMeshes)
        {
            Matrix4x4 scaling = glm::scale(Matrix4x4(1.0f), Vec3(1.0f, 1.0f, 1.0f));

            m_pDevice->ResetToDefaultRenderStates();

            m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
            m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
            m_pDevice->SetTexture(4, renderSet.m_pCurrentColorSource->GetTexture());
            m_pDevice->SetTextureSampler(4, renderSet.m_pCurrentColorSource->GetTextureSampler());
            m_pDevice->SetTexture(5, m_pWaterNormalMap);
            m_pDevice->SetTextureSampler(5, m_pWaterNormalMap->GetTextureSampler());
            m_pDevice->SetTexture(6, renderSet.m_pRenderTargetNormals->GetTexture());
            m_pDevice->SetTextureSampler(6, renderSet.m_pRenderTargetNormals->GetTextureSampler());
            m_pDevice->SetShader(m_pShaderGeometryOutline);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_GEOMETRY);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_FRONT);

            Render(renderQueue, m_pGeometryVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_OUTLINE], scaling);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_BACK);

            m_pDevice->SetTexture(4, nullptr);
            m_pDevice->SetTexture(6, nullptr);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderGeometryMotionBlur(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderGeometryMotionBlur);

        if (renderQueue.bRenderMeshes)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
            m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
            m_pDevice->SetShader(m_pShaderGeometryMotionBlur);

            m_pDevice->SetTexture(0, nullptr);
            m_pDevice->SetTexture(4, nullptr);
            m_pDevice->SetTexture(6, renderSet.m_pCurrentColorSource->GetTexture());
            m_pDevice->SetTextureSampler(6, renderSet.m_pCurrentColorSource->GetTexture()->GetTextureSampler());

            m_pDevice->Clear(Vec4(0.0f, 0.0f, 0.0f, 0.0f), CLEAR_TYPE_STENCIL);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);

            RenderMotionBlur(renderQueue, m_pGeometryVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_DEFERRED], Matrix4x4(1.0f));

            m_pDevice->SetTexture(6, nullptr);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderGeometryTransmissive(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderGeometryTransmissive);

        if (!renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_TRANSMISSIVE].empty())
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
            m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
            m_pDevice->SetTexture(5, renderSet.m_pCurrentColorSource->GetTexture());
            m_pDevice->SetTextureSampler(5, CDefaultTextureSampler::GetLinearWrapSampler());
            m_pDevice->SetTexture(4, renderSet.m_pRenderTargetDepths->GetTexture());
            m_pDevice->SetTextureSampler(4, CDefaultTextureSampler::GetLinearWrapSampler());
            m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
            m_pDevice->SetShader(m_pShaderGeometryTransmissive);

            Render(renderQueue, m_pGeometryVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_TRANSMISSIVE], Matrix4x4(1.0f));

            m_pDevice->SetTexture(5, nullptr);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderGeometryDeferred(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderGeometryDeferred);

        if (renderQueue.bRenderMeshes)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetShader(GetGeometryShader(renderQueue));
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_REPLACE);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);

            Render(renderQueue, m_pGeometryVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_DEFERRED], Matrix4x4(1.0f));
        }
    }

    void CGeometryRenderer::RenderGeometryCharacter(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderGeometryCharacter);

        if (renderQueue.bRenderMeshes &&
            renderQueue.m_pCharacter3DRenderObject)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetShader(m_pShaderGeometryCharacter);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_REPLACE);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);

            //if (renderQueue.m_pCharacter3DRenderObject)
            {
                m_constBufGeometrCharPS.GetDataForWriting().m_charMtrl = Vec4(renderQueue.m_pCharacter3DRenderObject->m_charMtrl, 0);
                m_constBufGeometrCharPS.GetDataForWriting().m_charParams = renderQueue.m_pCharacter3DRenderObject->m_charParams;

                Render(renderQueue, m_pGeometryVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_CHARACTER], Matrix4x4(1.0f), true/*, true, false*/);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderGeometryCharacterDistort(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderGeometryCharacter);

        if (renderQueue.bRenderMeshes)
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetShader(m_pShaderGeometryCharacterDistort);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
            m_pDevice->SetTextureSampler(5, CDefaultTextureSampler::GetLinearWrapSampler());
            m_pDevice->SetTexture(6, renderSet.m_pCurrentColorSource->GetTexture());
            m_pDevice->SetTextureSampler(6, renderSet.m_pCurrentColorSource->GetTextureSampler());
            m_pDevice->SetTexture(5, nullptr);

            if (renderQueue.m_pCharacter3DRenderObject)
            {
                m_constBufGeometrCharPS.GetDataForWriting().m_charMtrl = Vec4(renderQueue.m_pCharacter3DRenderObject->m_charMtrl, 0);
                m_constBufGeometrCharPS.GetDataForWriting().m_charParams = renderQueue.m_pCharacter3DRenderObject->m_charParams;
            }

            Render(renderQueue, m_pGeometryVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_CHARACTER], Matrix4x4(1.0f), true/*, true, false*/);

            m_pDevice->SetTexture(4, nullptr);
            m_pDevice->SetTexture(6, nullptr);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderGeometryForward(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderGeometryForward);

        if (!renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_FORWARD].empty())
        {
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
            m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
            m_pDevice->SetDepthStencilTarget(renderSet.m_pForwardBackBuffer);
            m_pDevice->Clear(Vec4(0), CLEAR_TYPE_ZBUFFER | CLEAR_TYPE_STENCIL);
            m_pDevice->SetShader(m_pShaderGeometryForward);

            Render(renderQueue, m_pGeometryForwardVertexDeclaration.get(), renderQueue.m_meshInstances[MESH_RENDER_CATEGORY_FORWARD], Matrix4x4(1.0f));

            m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
        }

        if (renderQueue.m_pLevelGridRenderObject->m_pWireframeVB)
        {
            m_constBufTerrainNormalVS.GetDataForWriting().m_viewProjection = renderQueue.m_pCameraRenderObject->m_mtxViewProjection;
            m_constBufTerrainNormalVS.GetDataForWriting().m_view = renderQueue.m_pCameraRenderObject->m_mtxView;

            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_FILLMODE, FILLMODE_TYPE_WIREFRAME);
            m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
            m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
            m_pDevice->SetShader(m_pShaderTerrainForward);
            m_pDevice->SetVertexBuffer(0, 0, renderQueue.m_pLevelGridRenderObject->m_pWireframeVB);
            m_pDevice->SetVertexDeclaration(m_pLevelForwardVertexDeclaration.get());
            m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_LINELIST, 0, renderQueue.m_pLevelGridRenderObject->m_numCellsX + 1 + renderQueue.m_pLevelGridRenderObject->m_numCellsY + 1);
        }
    }

    //-----------------------------------------------------------------------------------
    //     void CGeometryRenderer::RenderDeferred(RenderQueue& renderQueue, RenderSet& renderSet)
    //     {
    //         SetupDeferredRendering(renderQueue, renderSet);
    // 
    //         RenderGeometryDeferred(renderQueue, renderSet);
    // 
    //         RenderLevelTerrain(renderQueue, renderSet);
    // 
    //         RenderLevelTerrainBorder(renderQueue, renderSet);
    // 
    //         RenderLevelTerrainPreview(renderQueue, renderSet);
    // 
    //         RenderLevelTerrainSelection(renderQueue, renderSet);
    // 
    //         CleanupDeferredRendering(renderQueue, renderSet);
    //     }

    //-----------------------------------------------------------------------------------
    CShader* CGeometryRenderer::GetGeometryShader(RenderQueue& renderQueue) const
    {
        CShader* pGeometryShader = nullptr;

        switch (renderQueue.m_pEnvironmentRenderObject->m_weather)
        {
        case SCENARIO_WEATHER_NORMAL:
            pGeometryShader = m_pShaderGeometryNormal;
            break;

        case SCENARIO_WEATHER_RAIN:
            pGeometryShader = m_pShaderGeometryNormal;//m_pShaderGeometryWet;
            break;

        case SCENARIO_WEATHER_SNOW:
            pGeometryShader = m_pShaderGeometrySnow;
            break;
        }

        return pGeometryShader;
    }

    //-----------------------------------------------------------------------------------
    CShader* CGeometryRenderer::GetTerrainShader(RenderQueue& renderQueue) const
    {
        CShader* pTerrainShader = nullptr;

        switch (renderQueue.m_pEnvironmentRenderObject->m_weather)
        {
        case SCENARIO_WEATHER_NORMAL:
            pTerrainShader = m_pShaderTerrainNormal;
            break;

        case SCENARIO_WEATHER_RAIN:
            pTerrainShader = m_pShaderTerrainNormal;
            break;

        case SCENARIO_WEATHER_SNOW:
            pTerrainShader = m_pShaderTerrainSnow;//m_pShaderTerrainSnow;
            break;
        }

        return pTerrainShader;
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::BindMesh(RenderQueue& renderQueue, MeshRenderObject* pMesh, CVertexDeclaration* pVertexDeclaration)
    {
        ENV_ASSERT_DBG(pMesh->m_pIndexBuffer);
        ENV_ASSERT_DBG(pMesh->m_pVertexBuffer);

        m_pDevice->SetIndexBuffer(pMesh->m_pIndexBuffer);
        m_pDevice->SetVertexDeclaration(pVertexDeclaration);
        m_pDevice->SetVertexBuffer(0, 0, pMesh->m_pVertexBuffer);

        if (renderQueue.m_pEnvMapRenderObject)
        {
//            m_uniformHasEnvMap.SetData((int)(renderQueue.m_pEnvMapRenderObject->m_pEnvMap != nullptr));

            m_pDevice->SetTextureSampler(4, CDefaultTextureSampler::GetLinearMipWrapSampler());
            m_pDevice->SetTexture(4, renderQueue.m_pEnvMapRenderObject->m_pEnvMap);
        }
        else
        {
            m_pDevice->SetTexture(4, nullptr);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::DrawInstance(RenderQueue& renderQueue, MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance, const Matrix4x4& mtxScaling, const Matrix4x4& mtxViewProj, const Matrix4x4& mtxView)
    {
        CMaterial& material = *pInstance->m_pMaterial;

        if (material.HasTexture())
        {
            m_pDevice->SetTextureSampler(0, material.GetTexture()->GetTextureSampler());
            m_pDevice->SetTexture(0, material.GetTexture());
        }
        if (material.HasNormalMap())
        {
            m_pDevice->SetTextureSampler(1, material.GetNormalMap()->GetTextureSampler());
            m_pDevice->SetTexture(1, material.GetNormalMap());
        }
        if (material.HasSpecularMap())
        {
            m_pDevice->SetTextureSampler(2, material.GetSpecularMap()->GetTextureSampler());
            m_pDevice->SetTexture(2, material.GetSpecularMap());
        }
        if (material.HasEmissiveMap())
        {
            m_pDevice->SetTextureSampler(3, material.GetEmissiveMap()->GetTextureSampler());
            m_pDevice->SetTexture(3, material.GetEmissiveMap());
        }

        m_constBufGeometryNormalPS.GetDataForWriting().m_bHasNormalMap = material.HasNormalMap();
        m_constBufGeometryNormalPS.GetDataForWriting().m_bHasSpecularMap = material.HasSpecularMap();
        m_constBufGeometryNormalPS.GetDataForWriting().m_bHasEmissiveMap = material.HasEmissiveMap();
        m_constBufGeometryNormalPS.GetDataForWriting().m_bHasEnvMap = renderQueue.m_pEnvMapRenderObject != nullptr;
        m_constBufGeometryNormalPS.GetDataForWriting().m_bHasTexture = material.HasTexture();
        m_constBufGeometryNormalPS.GetDataForWriting().m_diffuseMtrl = Vec4(material.GetDiffuseColor() * material.GetDiffuseEnergy(), 0.0f);
        m_constBufGeometryNormalPS.GetDataForWriting().m_emissiveMtrl = Vec4(material.GetEmissiveColor() * material.GetEmissiveEnergy(), 0.0f);
        m_constBufGeometryNormalPS.GetDataForWriting().m_envMtrl = Vec4(material.GetEnvColor(), material.GetEnvReflectivity());

        m_constBufGeometryNormalVS.GetDataForWriting().m_model = mtxScaling * pMesh->m_mtxTransform * pInstance->m_mtxTransform;
        m_constBufGeometryNormalVS.GetDataForWriting().m_viewProjection = mtxViewProj;
        m_constBufGeometryNormalVS.GetDataForWriting().m_view = mtxView;

        m_constBufGeometryOutlinePS.GetDataForWriting() = Vec4(material.GetOutlineColor(), 0.0f);

        m_pDevice->DrawIndexedPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 0, pMesh->m_numVertices, 0, pMesh->m_numFaces);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::Render(RenderQueue& renderQueue,
                                   CVertexDeclaration* pVertexDeclaration,
                                   mapMeshInstanceRenderObjects& meshInstances,
                                   const Matrix4x4& mtxScaling,
                                   bool stencilFlagDirectionalLighting,
                                   bool stencilFlagPostProcessing,
                                   bool stencilPostProcessing)
    {
        for (mapMeshInstanceRenderObjects::iterator itMeshData = meshInstances.begin(); itMeshData != meshInstances.end(); ++itMeshData)
        {
            MeshRenderObject* pMesh = itMeshData->first;

            if (!pMesh)
                continue;

            listMeshInstanceRenderObjects& instances = itMeshData->second;

            BindMesh(renderQueue, pMesh, pVertexDeclaration);

            for (listMeshInstanceRenderObjects::iterator itInstances = instances.begin(); itInstances != instances.end(); ++itInstances)
            {
                MeshInstanceRenderObject* pInstance = *itInstances;

                if (stencilFlagDirectionalLighting &&
                    stencilFlagPostProcessing)
                {
                    unsigned char stencilRef = pInstance->m_pMaterial->GetDirectionalLighting() ? STENCILREF_DEFERRED_GEOMETRY : STENCILREF_DEFERRED_GEOMETRY_NODIRECTIONAL;
                    stencilRef |= stencilPostProcessing ? 0 : (1 << 4);

                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, stencilRef);
                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILWRITEMASK, 0xFF);
                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREADMASK, 0xFF);
                }
                else if (stencilFlagDirectionalLighting == true &&
                         stencilFlagPostProcessing == false)
                {
                    unsigned char stencilRef = pInstance->m_pMaterial->GetDirectionalLighting() ? STENCILREF_DEFERRED_GEOMETRY : STENCILREF_DEFERRED_GEOMETRY_NODIRECTIONAL;
                    stencilRef |= 0;

                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, stencilRef);
                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILWRITEMASK, 0xFF);
                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREADMASK, 0xFF);
                }
                else if (stencilFlagDirectionalLighting == false &&
                         stencilFlagPostProcessing == true)
                {
                    unsigned char stencilRef = stencilPostProcessing ? (1 << 4) : 0;

                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, stencilRef);
                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILWRITEMASK, 0xFF);
                    m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREADMASK, 0xFF);
                }

                DrawInstance(renderQueue, pMesh, pInstance, mtxScaling, renderQueue.m_pCameraRenderObject->m_mtxViewProjection, renderQueue.m_pCameraRenderObject->m_mtxView);
            }
        }

        m_pDevice->SetTexture(4, 0);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderMotionBlur(RenderQueue& renderQueue,
                                             CVertexDeclaration* pVertexDeclaration,
                                             mapMeshInstanceRenderObjects& meshInstances,
                                             const Matrix4x4& mtxScaling)
    {
        return;
        for (mapMeshInstanceRenderObjects::iterator itMeshData = meshInstances.begin(); itMeshData != meshInstances.end(); ++itMeshData)
        {
            MeshRenderObject* pMesh = itMeshData->first;

            m_pDevice->SetIndexBuffer(pMesh->m_pIndexBuffer);
            m_pDevice->SetVertexDeclaration(pVertexDeclaration);
            m_pDevice->SetVertexBuffer(0, 0, pMesh->m_pVertexBuffer);

            listMeshInstanceRenderObjects& instances = itMeshData->second;

            for (listMeshInstanceRenderObjects::iterator itInstances = instances.begin(); itInstances != instances.end(); ++itInstances)
            {
                MeshInstanceRenderObject* pInstance = *itInstances;

                if (!pInstance->m_pMaterial->GetMotionBlur())
                    continue;

                m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, pInstance->m_pMaterial->GetDirectionalLighting() ? STENCILREF_DEFERRED_GEOMETRY : STENCILREF_DEFERRED_GEOMETRY_NODIRECTIONAL);

//                 m_uniformModel.SetData(mtxScaling * pMesh->m_mtxTransform * pInstance->m_mtxTransform);
//                 m_uniformPrevModel.SetData(mtxScaling * pMesh->m_mtxTransform * pInstance->m_mtxPrevTransform);
//                 m_uniformMotionBlurIntensity.SetData(pInstance->m_pMaterial->GetMotionBlurScaling() * renderQueue.timeElapsedFrame * 60.0f);

                m_pDevice->DrawIndexedPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 0, pMesh->m_numVertices, 0, pMesh->m_numFaces);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    //     void CGeometryRenderer::RenderForward(RenderQueue& renderQueue, RenderSet& renderSet)
    //     {
    //         if (!renderQueue.m_meshInstancesForward.empty())
    //         {
    //             m_pDevice->ResetToDefaultRenderStates();
    //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
    //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
    //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
    //             m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
    //             m_pDevice->SetShader(m_pShaderGeometryForward);
    // 
    //             Render(renderQueue, m_pGeometryForwardVertexDeclaration, false);
    //         }
    // 
    //         if (renderQueue.m_pLevelGridRenderObject->GetTerrainWireframeVertexBuffer())
    //         {
    //             m_pDevice->SetShader(m_pShaderTerrainForward);
    //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_FILLMODE, FILLMODE_TYPE_WIREFRAME);
    //             m_pDevice->SetVertexBuffer(0, 0, renderQueue.m_pLevelGridRenderObject->GetTerrainWireframeVertexBuffer());
    //             m_pDevice->SetVertexDeclaration(m_pLevelForwardVertexDeclaration);
    //             m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_LINELIST, 0, renderQueue.m_pLevelGridRenderObject->GetNumCellsX() + 1 + renderQueue.m_pLevelGridRenderObject->GetNumCellsY() + 1);
    //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_FILLMODE, FILLMODE_TYPE_SOLID);
    //         }
    //     }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderLevelTerrainPreview(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (!renderQueue.m_pLevelGridRenderObject->m_pMaterial || !renderQueue.m_pLevelGridRenderObject->m_previewBuffer)
            return;

        m_pDevice->ResetToDefaultRenderStates();  
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_TERRAIN_PREVIEW);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);

        m_pDevice->SetTextureSampler(0, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetTexture()->GetTextureSampler());
        m_pDevice->SetTexture(0, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetTexture());

        if (renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap())
        {
            m_pDevice->SetTextureSampler(1, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap()->GetTextureSampler());
            m_pDevice->SetTexture(1, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap());
        }

        if (renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap())
        {
            m_pDevice->SetTextureSampler(2, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap()->GetTextureSampler());
            m_pDevice->SetTexture(2, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap());
        }

        if (renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap())
        {
            m_pDevice->SetTextureSampler(3, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap()->GetTextureSampler());
            m_pDevice->SetTexture(3, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap());
        }


        m_pDevice->SetVertexDeclaration(m_pLevelVertexDeclaration.get());
        m_pDevice->SetVertexBuffer(0, 0, renderQueue.m_pLevelGridRenderObject->m_previewBuffer->m_pVB);
        m_pDevice->SetShader(GetTerrainShader(renderQueue));

//         m_uniformModel.SetData(Matrix4x4(1.0f));
//         m_uniformDiffuseMtrl.SetData(Vec3(1.0f, 0.5f, 0.5f));
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZFUNC, COMPARISON_TYPE_GREATEREQUAL);
        m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, renderQueue.m_pLevelGridRenderObject->m_previewBuffer->m_numTerrainTriangles);

//        m_uniformDiffuseMtrl.SetData(Vec3(1.0f, 1.0f, 1.0f));
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZFUNC, COMPARISON_TYPE_LESS);
        m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, renderQueue.m_pLevelGridRenderObject->m_previewBuffer->m_numTerrainTriangles);

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, renderQueue.m_pLevelGridRenderObject->m_offsetPrevPatternVertices, renderQueue.m_pLevelGridRenderObject->m_numPrevPatternTriangles);

    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderLevelTerrainBorder(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderLevelTerrainBorder);

        if (!renderQueue.bRenderLevelBorder ||
            !renderQueue.m_pLevelGridRenderObject->m_pBorderVB)
        {
            return;
        }

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetTexture(2, 0);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_TERRAIN);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);


        const std::string& strBorderTexture = renderQueue.m_pEnvironmentRenderObject->m_strBorderTexture;
        if (m_pBorderTexture && m_pBorderTexture->GetName() != strBorderTexture)
        {
            gTextureMgr->Release(m_pBorderTexture->GetName());
            m_pBorderTexture = NULL;
        }

        if (!strBorderTexture.empty() && !m_pBorderTexture)
        {
            m_pBorderTexture = gTextureMgr->AddGetPtr(strBorderTexture);
            m_pBorderTexture->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        }

        if (m_pBorderTexture)
        {
            m_pDevice->SetTextureSampler(0, m_pBorderTexture->GetTextureSampler());
            m_pDevice->SetTexture(0, m_pBorderTexture);
        }
        
        m_constBufTerrainBorderPS.GetDataForWriting().m_diffuseMtrl = Vec4(renderQueue.m_pEnvironmentRenderObject->m_borderColor, 0.0f);
        m_constBufTerrainBorderPS.GetDataForWriting().m_bHasPrecomputedLighting = renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_FINAL ||
                                                                        renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_PRECOMPUTEDONLY;

        m_constBufTerrainBorderVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxViewProjection;

        m_pDevice->SetVertexDeclaration(m_pLevelBorderVertexDeclaration.get());
        m_pDevice->SetVertexBuffer(0, 0, renderQueue.m_pLevelGridRenderObject->m_pBorderVB);
        m_pDevice->SetShader(m_pShaderTerrainBorder);
        m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, renderQueue.m_pLevelGridRenderObject->m_numBorderTriangles);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderLevelTerrain(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        ENV_SCOPED_GPU_PROFILING(GEOMETRY_RENDERER, RenderLevelTerrain);

        if (!renderQueue.bRenderLevelTerrain)
            return;

        if (renderQueue.m_pLevelGridRenderObject->m_numCellsX == 0 || renderQueue.m_pLevelGridRenderObject->m_numCellsY == 0)
            return;

        m_constBufTerrainNormalPS.GetDataForWriting().m_diffuseMtrl = Vec4(1.0f, 1.0f, 1.0f, 0.0f);
        m_constBufTerrainNormalPS.GetDataForWriting().m_bHasTexture = true;
        m_constBufTerrainNormalPS.GetDataForWriting().m_bHasPrecomputedLighting = renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_FINAL ||
                                                                        renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_PRECOMPUTEDONLY;

        m_levelTerrainStateBlock.Apply();

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHAREF, 1);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_TERRAIN);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);


        m_pDevice->SetVertexDeclaration(m_pLevelVertexDeclaration.get());

        //m_numTerrainTrianglesRendered = renderQueue.m_pLevelGridRenderObject->m_numTerrainTriangles;
        m_numTerrainTrianglesRendered = 0;
        //if (m_numTerrainTrianglesRendered > 0)
        {
            if (renderQueue.m_pLevelGridRenderObject->m_pMaterial)
            {
                
                m_constBufTerrainNormalPS.GetDataForWriting().m_bHasNormalMap = renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap() ? 1 : 0;
                m_constBufTerrainNormalPS.GetDataForWriting().m_bHasSpecularMap = renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap() ? 1 : 0;
                m_constBufTerrainNormalPS.GetDataForWriting().m_bHasEmissiveMap = renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap() ? 1 : 0;
                m_constBufTerrainNormalPS.GetDataForWriting().m_emissiveMtrl = Vec4((renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveColor() *
                                                                           renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveEnergy()), 0.0f);
                m_constBufTerrainNormalPS.GetDataForWriting().m_envMtrl = Vec4(renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEnvColor(),
                                                                     renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEnvReflectivity());

                m_pDevice->SetTextureSampler(0, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetTexture()->GetTextureSampler());
                m_pDevice->SetTexture(0, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetTexture());

                if (renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap())
                {
                    m_pDevice->SetTextureSampler(1, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap()->GetTextureSampler());
                    m_pDevice->SetTexture(1, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetNormalMap());
                }

                if (renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap())
                {
                    m_pDevice->SetTextureSampler(2, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap()->GetTextureSampler());
                    m_pDevice->SetTexture(2, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetSpecularMap());
                }

                if (renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap())
                {
                    m_pDevice->SetTextureSampler(3, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap()->GetTextureSampler());
                    m_pDevice->SetTexture(3, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetEmissiveMap());
                }
            }

            {
                m_pDevice->SetTexture(4, m_pWaterNormalMap);
                m_pDevice->SetTextureSampler(4, m_pWaterNormalMap->GetTextureSampler());

                m_pDevice->SetTexture(6, m_pWaterSpecularityMap);
                m_pDevice->SetTextureSampler(6, m_pWaterSpecularityMap->GetTextureSampler());
            }

            if (renderQueue.m_pEnvMapRenderObject)
            {
                m_pDevice->SetTextureSampler(5, CDefaultTextureSampler::GetLinearMipWrapSampler());
                m_pDevice->SetTexture(5, renderQueue.m_pEnvMapRenderObject->m_pEnvMap);
            }

            m_constBufTerrainNormalVS.GetDataForWriting().m_viewProjection = renderQueue.m_pCameraRenderObject->m_mtxViewProjection;
            m_constBufTerrainNormalVS.GetDataForWriting().m_view = renderQueue.m_pCameraRenderObject->m_mtxView;

            m_pDevice->SetShader(GetTerrainShader(renderQueue));

            for (auto& terrainCellBuffer : renderQueue.m_pLevelGridRenderObject->m_terrainBuffers)
            {
                m_numTerrainTrianglesRendered += terrainCellBuffer.m_numTerrainTriangles;

                if (terrainCellBuffer.m_numTerrainTriangles > 0)
                {
                    m_pDevice->SetIndexBuffer(terrainCellBuffer.m_pIB);
                    m_pDevice->SetVertexBuffer(0, 0, terrainCellBuffer.m_pVB);
                    m_pDevice->DrawIndexedPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 0, 0, 0, terrainCellBuffer.m_numTerrainTriangles);
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::RenderLevelTerrainSelection(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (!renderQueue.m_pLevelGridRenderObject->m_selectionBuffer || 
            !renderQueue.m_pLevelGridRenderObject->m_pMaterial)
            return;

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_TERRAIN_SELECTION);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_ALWAYS);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_REPLACE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);


        m_pDevice->SetTexture(0, renderQueue.m_pLevelGridRenderObject->m_pMaterial->GetTexture());
        m_pDevice->SetShader(m_pShaderTerrainSelection);
        m_pDevice->SetVertexDeclaration(m_pLevelVertexDeclaration.get());
        m_pDevice->SetVertexBuffer(0, 0, renderQueue.m_pLevelGridRenderObject->m_selectionBuffer->m_pVB);
        m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, renderQueue.m_pLevelGridRenderObject->m_selectionBuffer->m_numTerrainTriangles);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::OnLostDevice()
    {
        m_initialStateBlock.OnLostDevice();
        m_finalStateBlock.OnLostDevice();
        m_levelTerrainStateBlock.OnLostDevice();

        m_pTerrainSelectionDepth->OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_initialStateBlock.OnResetDevice();
        m_finalStateBlock.OnResetDevice();
        m_levelTerrainStateBlock.OnResetDevice();

        m_pTerrainSelectionDepth->SetSize(screenWidth, screenHeight);
        m_pTerrainSelectionDepth->OnResetDevice();
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::OnLevelChanged()
    {
    }

    //-----------------------------------------------------------------------------------
    void CGeometryRenderer::DrawMeshRenderObject(CRenderDevice* pDevice, MeshRenderObject& meshRenderObject)
    {
        pDevice->SetVertexBuffer(0, 0, meshRenderObject.m_pVertexBuffer);
        pDevice->SetIndexBuffer(meshRenderObject.m_pIndexBuffer);
        pDevice->DrawIndexedPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 0, meshRenderObject.m_numVertices, 0, meshRenderObject.m_numFaces);
    }

} // env 
