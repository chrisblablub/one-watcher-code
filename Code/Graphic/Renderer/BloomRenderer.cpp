#include <Graphic/Renderer/BloomRenderer.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/RenderObjects/BloomRenderObject.h>
#include <Graphic/RenderObjects/LightRaysRenderObject.h>
#include <Graphic/Renderer/GaussBlurRenderer.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
namespace env
{
    //-----------------------------------------------------------------------------------
    CBloomRenderer::CBloomRenderer(CRenderDevice* pDevice, CGaussBlurRenderer* pGaussBlurRenderer)
        : m_pDevice(pDevice)
        , m_pShaderBloomBlurVertical(nullptr)
        , m_pShaderBloomBlurVerticalAndUpscale(nullptr)
        , m_pShaderBloomBlurHorizontal(nullptr)
        , m_pShaderBloomDownscaleAndMask(nullptr)
        , m_pShaderBloomUpscale(nullptr)
        , m_pShaderBloomDownscale(nullptr)
        , m_pShaderBloomDarkPass(nullptr)
        , m_pShaderBloomBrightPass(nullptr)
        , m_pShaderLightrays(nullptr) {}

    //-----------------------------------------------------------------------------------
    CBloomRenderer::~CBloomRenderer()
    {
        for (unsigned int i = 0; i < m_renderTargetDownscale.size(); ++i)
        {
            ENV_DELETE(m_renderTargetDownscale[i]);
        }

        for (unsigned int i = 0; i < m_renderTargetDownscaleTmp.size(); ++i)
        {
            ENV_DELETE(m_renderTargetDownscaleTmp[i]);
        }

        gShaderMgr->Release(m_pShaderBloomBlurVerticalAndUpscale);
        gShaderMgr->Release(m_pShaderBloomBlurVertical);
        gShaderMgr->Release(m_pShaderBloomBlurHorizontal);
        gShaderMgr->Release(m_pShaderBloomDownscaleAndMask);
        gShaderMgr->Release(m_pShaderBloomUpscale);
        gShaderMgr->Release(m_pShaderBloomDownscale);
        gShaderMgr->Release(m_pShaderBloomDarkPass);
        gShaderMgr->Release(m_pShaderBloomBrightPass);
        gShaderMgr->Release(m_pShaderLightrays);
    }

    //-----------------------------------------------------------------------------------
    bool CBloomRenderer::Initialize()
    {
        m_quad.Initialize(m_pDevice);
        m_quadDownscale.Initialize(m_pDevice);

        m_pShaderBloomBlurVertical = gShaderMgr->AddGetPtr("BloomBlurVertical");
        m_pShaderBloomBlurVerticalAndUpscale = gShaderMgr->AddGetPtr("BloomBlurVerticalAndUpscale");
        m_pShaderBloomBlurHorizontal = gShaderMgr->AddGetPtr("BloomBlurHorizontal");
        m_pShaderBloomDownscale = gShaderMgr->AddGetPtr("BloomDownscale");
        m_pShaderBloomDownscaleAndMask = gShaderMgr->AddGetPtr("BloomDownscaleAndMask");
        m_pShaderBloomUpscale = gShaderMgr->AddGetPtr("BloomUpscale");
        m_pShaderBloomDarkPass = gShaderMgr->AddGetPtr("BloomDarkPass");
        m_pShaderBloomBrightPass = gShaderMgr->AddGetPtr("BloomBrightPass");
        m_pShaderLightrays = gShaderMgr->AddGetPtr("Lightrays");

        if (nullptr == m_pShaderBloomBlurVertical ||
            nullptr == m_pShaderBloomBlurVerticalAndUpscale ||
            nullptr == m_pShaderBloomBlurHorizontal ||
            nullptr == m_pShaderBloomDownscale ||
            nullptr == m_pShaderBloomDownscaleAndMask ||
            nullptr == m_pShaderBloomUpscale ||
            nullptr == m_pShaderBloomDarkPass ||
            nullptr == m_pShaderBloomBrightPass ||
            nullptr == m_pShaderLightrays)
        {
            return false;
        }

        m_pShaderBloomBlurVertical->LinkConstantBufferCPU("cb_BloomBlur", m_constBufBloomBlurVerticalPS);
        m_pShaderBloomBlurVerticalAndUpscale->LinkConstantBufferCPU("cb_BloomBlur", m_constBufBloomBlurVerticalPS);
        m_pShaderBloomBlurHorizontal->LinkConstantBufferCPU("cb_BloomBlur", m_constBufBloomBlurHorizontalPS);
        m_pShaderBloomBrightPass->LinkConstantBufferCPU("cb_BloomBrightpass", m_constBufBloomBlurBrightpassPS);
        m_pShaderBloomDarkPass->LinkConstantBufferCPU("cb_BloomDarkpass", m_constBufBloomDarkpassPS);
        m_pShaderLightrays->LinkConstantBufferCPU("cb_LightRaysPS", m_constBufLightRaysPS);

        m_pBloomQuadVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShaderBloomBlurVertical);

        if (nullptr == m_pBloomQuadVertexDecl)
        {
            return false;
        }

        m_renderTargetDownscale.resize(3);
        m_renderTargetDownscaleTmp.resize(3);

        for (unsigned int i = 0; i < m_renderTargetDownscale.size(); ++i)
        {
            unsigned int downscale = 1 << (i + 1);
            auto renderTarget = m_pDevice->CreateRenderTarget(m_croppedWidth / downscale, m_croppedHeight / downscale, TEXTUREFORMAT_TYPE_R11G11B10);
            auto renderTargetTmp = m_pDevice->CreateRenderTarget(m_croppedWidth / downscale, m_croppedHeight / downscale, TEXTUREFORMAT_TYPE_R11G11B10);

            m_renderTargetDownscale[i] = renderTarget.get();
            m_renderTargetDownscale[i]->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());

            m_renderTargetDownscaleTmp[i] = renderTargetTmp.get();
            m_renderTargetDownscaleTmp[i]->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());

            renderTarget.release();
            renderTargetTmp.release();
        }

        m_pRT_Downscale_Tmp = m_pDevice->CreateRenderTarget(m_croppedWidth / 2, m_croppedHeight / 2, TEXTUREFORMAT_TYPE_A16B16G16R16F);
        m_pRT_Downscale_Tmp->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());

        m_pRT_LightRays = m_pDevice->CreateRenderTarget(m_croppedWidth / 2, m_croppedHeight / 2, TEXTUREFORMAT_TYPE_A16B16G16R16F);
        m_pRT_LightRays->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CBloomRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        const RenderQualityParams& videoParams = gRenderSystem->GetRenderQualityParams();

        static int numBlurPasses = 3;

        if (videoParams.bloomQuality == 1)
            numBlurPasses = 3;
        else if (videoParams.bloomQuality == 2)
            numBlurPasses = 2;
        else if (videoParams.bloomQuality == 3)
            numBlurPasses = 1;
        else
            numBlurPasses = 0;

#ifndef ENV_FINAL
        if (gEngine->GetInput()->IsKeyJustDown(VK_F5))
            numBlurPasses = 3;
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F6))
            numBlurPasses = 2;
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F7))
            numBlurPasses = 1;
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F8))
            numBlurPasses = 0;
#endif // ENV_FINAL

        CRenderTarget* pCurrentSource = renderSet.m_pCurrentColorSource;
        CRenderTarget* pIntermediateTex = renderSet.m_pCurrentColorTarget;

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);

        // Downscale to 1/2
        m_pDevice->SetShader(m_pShaderBloomDownscaleAndMask);
        m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
        m_pDevice->SetTexture(0, pCurrentSource->GetTexture());
        m_pDevice->SetTextureSampler(1, CDefaultTextureSampler::GetLinearClampSampler());
        m_pDevice->SetTexture(1, renderSet.m_pRenderTargetColors->GetTexture());
        m_pDevice->SetRenderTarget(0, m_pRT_Downscale_Tmp.get());
        m_pDevice->SetDepthStencilTarget(nullptr);
        m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_pRT_Downscale_Tmp->GetWidth(), m_pRT_Downscale_Tmp->GetHeight());
        m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
        m_quadDownscale.RenderQuad();

        // Brightpass
        m_constBufBloomBlurBrightpassPS.GetDataForWriting().x = renderQueue.m_pBloomRenderObject->m_bloomThreshold;

        m_pDevice->SetShader(m_pShaderBloomBrightPass);
        m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
        m_pDevice->SetTexture(0, m_pRT_Downscale_Tmp->GetTexture());
        m_pDevice->SetRenderTarget(0, m_renderTargetDownscale[0]);
        m_pDevice->SetDepthStencilTarget(nullptr);
        m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_renderTargetDownscale[0]->GetWidth(), m_renderTargetDownscale[0]->GetHeight());
        m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
        m_quadDownscale.RenderQuad();

        // Downscale to 1/4 & lower
        for (unsigned int i = 1; i < m_renderTargetDownscale.size(); ++i)
        {
            m_pDevice->SetShader(m_pShaderBloomDownscale);
            m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
            m_pDevice->SetTexture(0, m_renderTargetDownscale[i - 1]->GetTexture());
            m_pDevice->SetRenderTarget(0, m_renderTargetDownscale[i]);
            m_pDevice->SetDepthStencilTarget(nullptr);
            m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_renderTargetDownscale[i]->GetWidth(), m_renderTargetDownscale[i]->GetHeight());
            m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
            m_quadDownscale.RenderQuad();
        }

        int maxDownscaleIndex = m_renderTargetDownscale.size() - 1;

        numBlurPasses = std::min(numBlurPasses, maxDownscaleIndex + 1);

        // Upscale & gauss blur rendertargets
        for (int i = maxDownscaleIndex; i > maxDownscaleIndex - numBlurPasses; --i)
        {
            m_constBufBloomBlurVerticalPS.GetDataForWriting().x = 0.0f;
            m_constBufBloomBlurVerticalPS.GetDataForWriting().y = 1.0f / m_renderTargetDownscale[i]->GetHeight();
            m_constBufBloomBlurHorizontalPS.GetDataForWriting().x = 1.0f / m_renderTargetDownscale[i]->GetWidth();
            m_constBufBloomBlurHorizontalPS.GetDataForWriting().y = 0.0f;

            m_pDevice->SetShader(m_pShaderBloomBlurHorizontal);
            m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
            m_pDevice->SetTexture(0, m_renderTargetDownscale[i]->GetTexture());
            m_pDevice->SetRenderTarget(0, m_renderTargetDownscaleTmp[i]);
            m_pDevice->SetDepthStencilTarget(nullptr);
            m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_renderTargetDownscaleTmp[i]->GetWidth(), m_renderTargetDownscaleTmp[i]->GetHeight());
            m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
            m_quadDownscale.RenderQuad();

            if (i == maxDownscaleIndex)
            {
                m_pDevice->SetShader(m_pShaderBloomBlurVertical);
                m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
                m_pDevice->SetTexture(0, m_renderTargetDownscaleTmp[i]->GetTexture());
                m_pDevice->SetRenderTarget(0, m_renderTargetDownscale[i]);
                m_pDevice->SetDepthStencilTarget(nullptr);
                m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_renderTargetDownscale[i]->GetWidth(), m_renderTargetDownscale[i]->GetHeight());
                m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
                m_quadDownscale.RenderQuad();
            }
            else
            {
                m_pDevice->SetShader(m_pShaderBloomBlurVerticalAndUpscale);
                m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
                m_pDevice->SetTexture(0, m_renderTargetDownscaleTmp[i]->GetTexture());
                m_pDevice->SetTextureSampler(1, CDefaultTextureSampler::GetLinearClampSampler());
                m_pDevice->SetTexture(1, m_renderTargetDownscale[i + 1]->GetTexture());
                m_pDevice->SetRenderTarget(0, m_renderTargetDownscale[i]);
                m_pDevice->SetDepthStencilTarget(nullptr);
                m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_renderTargetDownscale[i]->GetWidth(), m_renderTargetDownscale[i]->GetHeight());
                m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
                m_quadDownscale.RenderQuad();
            }
        }

        CRenderTarget* pBloomResult = m_renderTargetDownscale[0];

        // Upscale non gauss-blurred rendertargets
        for (int i = maxDownscaleIndex - std::max(1, numBlurPasses); i >= 0; --i)
        {
            m_pDevice->SetShader(m_pShaderBloomUpscale);
            m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
            m_pDevice->SetTextureSampler(1, CDefaultTextureSampler::GetLinearClampSampler());

            if (i == (maxDownscaleIndex - std::max(1, numBlurPasses)))
            {
                m_pDevice->SetTexture(0, m_renderTargetDownscale[i + 1]->GetTexture());
                m_pDevice->SetTexture(1, m_renderTargetDownscale[i]->GetTexture());
            }
            else
            {
                m_pDevice->SetTexture(0, m_renderTargetDownscaleTmp[i + 1]->GetTexture());
                m_pDevice->SetTexture(1, m_renderTargetDownscale[i]->GetTexture());
            }
            
            m_pDevice->SetRenderTarget(0, m_renderTargetDownscaleTmp[i]);
            m_pDevice->SetDepthStencilTarget(nullptr);
            m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_renderTargetDownscaleTmp[i]->GetWidth(), m_renderTargetDownscaleTmp[i]->GetHeight());
            m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
            m_quadDownscale.RenderQuad();

            pBloomResult = m_renderTargetDownscaleTmp[0];
        }


        // LIGHT RAYS
        if (renderQueue.m_pLightRaysRenderObject && videoParams.bLightRays)
        {
            m_constBufBloomDarkpassPS.GetDataForWriting().m_bLightRays = renderQueue.m_pLightRaysRenderObject->m_bLightRays;

            if (renderQueue.m_pLightRaysRenderObject->m_bLightRaysOrigin)
            {
                Vec4 lightRaysPosition(0.0f, 0.0f, 0.0f, 1.0f);
                lightRaysPosition = Vec4(renderQueue.m_pLightRaysRenderObject->m_lightRaysOrigin, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxViewProjection;

                lightRaysPosition.x /= lightRaysPosition.w;
                lightRaysPosition.y /= lightRaysPosition.w;

                Vec2 screenLightRaysPosition(lightRaysPosition.x * 0.5f + 0.5f, lightRaysPosition.y * -0.5f + 0.5f);

                m_constBufLightRaysPS.GetDataForWriting().m_lightRaysOrigin = screenLightRaysPosition;
            }
            else
            {
                m_constBufLightRaysPS.GetDataForWriting().m_lightRaysOrigin = Vec2(0.5f, 0.5f);
            }

            m_constBufLightRaysPS.GetDataForWriting().m_bLightRaysOcclusionMask = renderQueue.m_pLightRaysRenderObject->m_bLightRaysOcclusionMask;
            m_constBufLightRaysPS.GetDataForWriting().m_lightRaysColor = renderQueue.m_pLightRaysRenderObject->m_lightRaysColor;
            m_constBufLightRaysPS.GetDataForWriting().m_lightRaysIllumDecay = renderQueue.m_pLightRaysRenderObject->m_lightRaysIllumDecay;
            m_constBufLightRaysPS.GetDataForWriting().m_lightRaysDecay = renderQueue.m_pLightRaysRenderObject->m_lightRaysDecay;

           // m_pDevice->BeginPerformanceMarker("LIGHTRAYS");
            m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_pRT_LightRays->GetWidth(), m_pRT_LightRays->GetHeight());
            m_pDevice->SetRenderTarget(0, m_pRT_LightRays.get());
            m_pDevice->SetTextureSampler(0, m_pRT_Downscale_Tmp->GetTextureSampler());
            m_pDevice->SetTexture(0, m_pRT_Downscale_Tmp->GetTexture());
            m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
            m_pDevice->SetShader(m_pShaderLightrays);
            m_quad.RenderQuad();
            //m_pDevice->EndPerformanceMarker();

            // Set texture sampler for subsequent render passes
            m_pDevice->SetTextureSampler(4, m_pRT_LightRays->GetTextureSampler());
            m_pDevice->SetTexture(4, m_pRT_LightRays->GetTexture());
        }
        else
        {
            m_constBufBloomDarkpassPS.GetDataForWriting().m_bLightRays = false;
            
            m_pDevice->SetTexture(4, nullptr);
        }

        //renderSet.m_pBloom = m_pRT_1_16_BrightPass.get();
        renderSet.m_pLightRays = m_pRT_LightRays.get();

        m_constBufBloomDarkpassPS.GetDataForWriting().m_bloomScale = renderQueue.m_pBloomRenderObject->m_bloomScale;
        
        m_pDevice->SetViewport(Vec2(0.0f, 0.0f), pIntermediateTex->GetWidth(), pIntermediateTex->GetHeight());
        m_pDevice->SetRenderTarget(0, pIntermediateTex);
        m_pDevice->SetTextureSampler(0, pCurrentSource->GetTextureSampler());
        m_pDevice->SetTexture(0, pCurrentSource->GetTexture());
        m_pDevice->SetTextureSampler(1, pBloomResult->GetTextureSampler());
        m_pDevice->SetTexture(1, pBloomResult->GetTexture());
        m_pDevice->SetVertexDeclaration(m_pBloomQuadVertexDecl.get());
        m_pDevice->SetShader(m_pShaderBloomDarkPass);
        m_quad.RenderQuad();
    }

    //-----------------------------------------------------------------------------------
    void CBloomRenderer::OnLostDevice()
    {
        for (unsigned int i = 0; i < m_renderTargetDownscale.size(); ++i)
        {
            m_renderTargetDownscale[i]->OnLostDevice();
            m_renderTargetDownscaleTmp[i]->OnLostDevice();
        }

        m_pRT_Downscale_Tmp->OnLostDevice();
        m_pRT_LightRays->OnLostDevice();
    }

    void CBloomRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_croppedWidth = screenWidth - screenWidth % 8;
        m_croppedHeight = screenHeight - screenHeight % 8;

        for (unsigned int i = 0; i < m_renderTargetDownscale.size(); ++i)
        {
            unsigned int downscale = 1 << i;
            m_renderTargetDownscale[i]->SetSize(m_croppedWidth / downscale, m_croppedHeight / downscale);
            m_renderTargetDownscale[i]->OnResetDevice();

            m_renderTargetDownscaleTmp[i]->SetSize(m_croppedWidth / downscale, m_croppedHeight / downscale);
            m_renderTargetDownscaleTmp[i]->OnResetDevice();
        }

        m_pRT_Downscale_Tmp->SetSize(m_croppedWidth / 2, m_croppedHeight / 2);
        m_pRT_Downscale_Tmp->OnResetDevice();

        m_pRT_LightRays->SetSize(m_croppedWidth / 2, m_croppedHeight / 2);
        m_pRT_LightRays->OnResetDevice();

        RenderCoordRect renderCoords;
        TextureCoordRect texCoords;

        m_quad.SetRenderRect(renderCoords);
        m_quadDownscale.SetTextureRect(texCoords);
        m_quad.UpdateVertexBuffer();
    }

    //-----------------------------------------------------------------------------------
    void CBloomRenderer::ComputeSampleOffsetsDownscale4x4(int width, int height, std::vector<Vec2>& sampleOffsets)
    {
        sampleOffsets.resize(16);

        float tU = 1.0f / width;
        float tV = 1.0f / height;

        int index = 0;
        for (int y = 0; y < 4; ++y)
        {
            for (int x = 0; x < 4; ++x)
            {
                sampleOffsets[index].x = (x - 1.5f) * tU * 4.0f;
                sampleOffsets[index].y = (y - 1.5f) * tV * 4.0f;

                index++;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CBloomRenderer::ComputeTextureCoords(int sourceWidth, int sourceHeight, RECT* pSrcRect, CRenderTarget* pTarget, RECT* pTargetRect, TextureCoordRect* pCoords)
    {
        float tU, tV;

        pCoords->m_left = 0.0f;
        pCoords->m_top = 0.0f;
        pCoords->m_right = 1.0f;
        pCoords->m_bottom = 1.0f;

        // If not using the complete source surface, adjust the coordinates
        if (pSrcRect != NULL)
        {
            // Get destination texture description
            int srcWidth = sourceWidth,
                srcHeight = sourceHeight;

            tU = 1.0f / srcWidth;
            tV = 1.0f / srcHeight;

            pCoords->m_left += pSrcRect->left * tU;
            pCoords->m_top += pSrcRect->top * tV;
            pCoords->m_right -= (srcWidth - pSrcRect->right) * tU;
            pCoords->m_bottom -= (srcHeight - pSrcRect->bottom) * tV;
        }

        // If not drawing to the complete destination surface, adjust the coordinates
        if (pTargetRect != NULL)
        {
            // Get source texture description
            int targetWidth = pTarget->GetWidth(),
                targetHeight = pTarget->GetHeight();

            // These delta values are the distance between source texel centers in 
            // texture address space
            tU = 1.0f / targetWidth;
            tV = 1.0f / targetHeight;

            pCoords->m_left -= pTargetRect->left * tU;
            pCoords->m_top -= pTargetRect->top * tV;
            pCoords->m_right += (targetWidth - pTargetRect->right) * tU;
            pCoords->m_bottom += (targetHeight - pTargetRect->bottom) * tV;
        }
    }
} // env