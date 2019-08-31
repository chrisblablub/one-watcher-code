#include <Graphic/Renderer/ParticleSystemRenderer.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/VertexDeclaration.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/Material.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CParticleSystemRenderer::CParticleSystemRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice) {}

    //-----------------------------------------------------------------------------------
    CParticleSystemRenderer::~CParticleSystemRenderer()
    {
        if (m_pShader)
            gShaderMgr->Release(m_pShader->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemRenderer::OnLostDevice()
    {
        m_pShader->OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_pShader->OnResetDevice();
    }

    //-----------------------------------------------------------------------------------
    bool CParticleSystemRenderer::Initialize()
    {
        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CParticleSystemRenderer::InitEffect(const std::string& strEffectFile)
    {
        m_pShader = gShaderMgr->AddGetPtr(strEffectFile);

        if (!m_pShader)
            return false;

        m_pShader->LinkConstantBufferCPU("cb_gVP", m_constBufParticleVS);

        return true;
    }


    //-----------------------------------------------------------------------------------
    bool CParticleSystemColorRenderer::Initialize()
    {
        if (!InitEffect("ParticleColor"))
            return false;

        m_pColorVertexDecl = m_pDevice->CreateVertexDeclaration(CParticleColorGPU::VertexElements, sizeof(CParticleColorGPU), m_pShader);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemColorRenderer::Render(const listParticleSystemRenderObjects& particleSystemRenderObjects,
                                              const Matrix4x4& mtxViewProj)
    {
        bool bZEnable = true;

        m_numParticlesRendered = 0;

        if (particleSystemRenderObjects.empty())
            return;

        m_constBufParticleVS.GetDataForWriting() = mtxViewProj;

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_NONE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, bZEnable);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);


        for (listParticleSystemRenderObjects::const_iterator it = particleSystemRenderObjects.begin(); it != particleSystemRenderObjects.end(); ++it)
        {
            const ParticleSystemRenderObject* pParticleRenderObject = *it;

            ENV_ASSERT_DBG(pParticleRenderObject != nullptr);
            ENV_ASSERT_DBG(pParticleRenderObject->m_pVertexBuffer != nullptr);
            ENV_ASSERT_DBG(pParticleRenderObject->m_numTotalParticlesAlive > 0);

            if (pParticleRenderObject == nullptr ||
                pParticleRenderObject->m_pVertexBuffer == nullptr ||
                pParticleRenderObject->m_numTotalParticlesAlive == 0)
            {
                continue;
            }

            m_numParticlesRendered += pParticleRenderObject->m_numTotalParticlesAlive;


            if (nullptr != pParticleRenderObject->m_pMaterial &&
                nullptr != pParticleRenderObject->m_pMaterial->GetTexture())
            {
                m_pDevice->SetTextureSampler(6, pParticleRenderObject->m_pMaterial->GetTexture()->GetTextureSampler());
                m_pDevice->SetTexture(6, pParticleRenderObject->m_pMaterial->GetTexture());
            }

            m_pDevice->SetShader(m_pShader);
            m_pDevice->SetVertexDeclaration(m_pColorVertexDecl.get());
            m_pDevice->SetVertexBuffer(0, 0, pParticleRenderObject->m_pVertexBuffer);
            m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, pParticleRenderObject->m_numTotalParticlesAlive * 2);
        }

        m_pDevice->SetSamplerState(6, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_BACK);
        m_pDevice->SetTexture(2, NULL);
    }



    //-----------------------------------------------------------------------------------
    void CParticleSystemColorRenderer::RenderDefault(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        Render(renderQueue.m_particleSystems["Default"],
               renderQueue.m_pCameraRenderObject->m_mtxViewProjection);
    }

    //-----------------------------------------------------------------------------------
    bool CParticleSystemDistortRenderer::Initialize()
    {
        if (!InitEffect("ParticleDistort"))
            return false;

        m_pDistortVertexDecl = m_pDevice->CreateVertexDeclaration(CParticleDistortGPU::VertexElements, sizeof(CParticleDistortGPU), m_pShader);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemDistortRenderer::Render(const RenderSet& renderSet,
                                                const listParticleSystemRenderObjects& particleSystemRenderObjects,
                                                const Matrix4x4& mtxViewProj)
    {
        bool bZEnable = true;

        m_numParticlesRendered = 0;

        if (particleSystemRenderObjects.empty())
            return;

        m_constBufParticleVS.GetDataForWriting() = mtxViewProj;

        m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
        m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetTextureSampler(5, renderSet.m_pCurrentColorSource->GetTexture()->GetTextureSampler());
        m_pDevice->SetTexture(5, renderSet.m_pCurrentColorSource->GetTexture());
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_NONE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, bZEnable);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);

        for (listParticleSystemRenderObjects::const_iterator it = particleSystemRenderObjects.begin(); it != particleSystemRenderObjects.end(); ++it)
        {
            const ParticleSystemRenderObject* pParticleRenderObject = *it;

            ENV_ASSERT_DBG(pParticleRenderObject != nullptr);
            ENV_ASSERT_DBG(pParticleRenderObject->m_pVertexBuffer != nullptr);
            ENV_ASSERT_DBG(pParticleRenderObject->m_numTotalParticlesAlive > 0);

            if (pParticleRenderObject == nullptr ||
                pParticleRenderObject->m_pVertexBuffer == nullptr ||
                pParticleRenderObject->m_numTotalParticlesAlive == 0)
            {
                continue;
            }

            m_numParticlesRendered += pParticleRenderObject->m_numTotalParticlesAlive;

            if (nullptr != pParticleRenderObject->m_pMaterial &&
                nullptr != pParticleRenderObject->m_pMaterial->GetTexture())
            {
                m_pDevice->SetTextureSampler(6, pParticleRenderObject->m_pMaterial->GetTexture()->GetTextureSampler());
                m_pDevice->SetTexture(6, pParticleRenderObject->m_pMaterial->GetTexture());
            }

            m_pDevice->SetShader(m_pShader);
            m_pDevice->SetVertexDeclaration(m_pDistortVertexDecl.get());
            m_pDevice->SetVertexBuffer(0, 0, pParticleRenderObject->m_pVertexBuffer);
            m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, pParticleRenderObject->m_numTotalParticlesAlive * 2);
        }

        m_pDevice->SetSamplerState(6, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_BACK);
        m_pDevice->SetTexture(5, NULL);
    }

    //-----------------------------------------------------------------------------------
    void CParticleSystemDistortRenderer::RenderDefault(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        Render(renderSet,
               renderQueue.m_particleSystems["Distort"],
               renderQueue.m_pCameraRenderObject->m_mtxViewProjection);
    }
} // env