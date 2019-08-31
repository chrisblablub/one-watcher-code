#include <Graphic/Renderer/FullscreenTextureRenderer.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CFullscreenTextureRenderer::CFullscreenTextureRenderer(CRenderDevice* pDevice)
        : m_pShaderFullscreenTexture(nullptr)
        , m_pShaderFullscreenColor(nullptr)
        , m_pDevice(pDevice) {}

    //-----------------------------------------------------------------------------------
    CFullscreenTextureRenderer::~CFullscreenTextureRenderer()
    {
        if (m_pShaderFullscreenColor)
            gShaderMgr->Release(m_pShaderFullscreenColor->GetName());

        if (m_pShaderFullscreenTexture)
            gShaderMgr->Release(m_pShaderFullscreenTexture->GetName());
    }

    //-----------------------------------------------------------------------------------
    bool CFullscreenTextureRenderer::Initialize()
    {
        m_pShaderFullscreenTexture = gShaderMgr->AddGetPtr("FullscreenTexture");
        m_pShaderFullscreenColor = gShaderMgr->AddGetPtr("FullscreenColor");

        m_pVertexDeclFSTexture = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShaderFullscreenTexture);
        m_pVertexDeclFSColor = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShaderFullscreenColor);

        m_quad.Initialize(m_pDevice);

        if (!m_pShaderFullscreenTexture ||
            !m_pShaderFullscreenColor)
        {
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CFullscreenTextureRenderer::Render(CRenderTarget* pTexture,  
                                            RenderQueue& renderQueue, 
                                            RenderSet& renderSet, 
                                            CRenderTarget* pTarget,
                                            int mipLevel)
    {
        if (pTarget)
        {
            m_pDevice->SetRenderTarget(0, pTarget);
        }

        // Currently not supported
        ENV_ASSERT_DBG(mipLevel == 0);

        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_pDevice->SetTexture(0, pTexture->GetTexture());

        m_pDevice->SetVertexDeclaration(m_pVertexDeclFSTexture.get());
        m_pDevice->SetShader(m_pShaderFullscreenTexture);
        m_quad.RenderQuad();
        m_pDevice->SetTexture(5, NULL);
    }

    //-----------------------------------------------------------------------------------
    void CFullscreenTextureRenderer::Render(RenderQueue& renderQueue)
    {
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);

        m_pDevice->SetVertexDeclaration(m_pVertexDeclFSColor.get());
        m_pDevice->SetShader(m_pShaderFullscreenColor);
        m_quad.RenderQuad();
    }

    //-----------------------------------------------------------------------------------
    void CFullscreenTextureRenderer::OnLostDevice()
    {
    }

    //-----------------------------------------------------------------------------------
    void CFullscreenTextureRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        RenderCoordRect renderCoords;

#ifdef D3D9
        Vec2 pixelsToTexels = CGPUQuad::GetPixelsToTexelsOffset(screenWidth, screenHeight);

        renderCoords.m_left += pixelsToTexels.x;
        renderCoords.m_right += pixelsToTexels.x;
        renderCoords.m_top += pixelsToTexels.y;
        renderCoords.m_bottom += pixelsToTexels.y;
#endif // D3D9

        m_quad.SetRenderRect(renderCoords);
        m_quad.UpdateVertexBuffer();

        //m_quad.OnResetDevice(screenWidth, screenHeight);
    }

    //-----------------------------------------------------------------------------------
    void CFullScreenTextureRenderPass::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_pRenderer->GetDevice()->ResetToDefaultRenderStates();
        m_pRenderer->GetDevice()->SetDepthStencilTarget(renderSet.m_pBackBuffer);
        
        m_renderStates.Apply();

        m_pRenderer->Render(renderSet.m_pCurrentColorSource, renderQueue, renderSet, renderSet.m_pCurrentColorTarget);
    }
} // env 
