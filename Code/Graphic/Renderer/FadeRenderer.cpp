#include <Graphic/Renderer/FadeRenderer.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/Material.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CFadeRenderer::CFadeRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pShader(nullptr)
        , m_pMaterial(nullptr)
        , m_pStripesVB(nullptr)
        , m_pStripesShader(nullptr)
        , m_numStripes(0) {}

    //-----------------------------------------------------------------------------------
    CFadeRenderer::~CFadeRenderer()
    {
        gShaderMgr->Release(m_pStripesShader);
        gShaderMgr->Release(m_pShader);
        gShaderMgr->Release(m_pShaderFadeScreen);

        gMaterialMgr->Release(m_pMaterial);
    }

    //-----------------------------------------------------------------------------------
    bool CFadeRenderer::Initialize()
    {
        m_pShader = gShaderMgr->AddGetPtr("Fade");
        m_pShaderFadeScreen = gShaderMgr->AddGetPtr("FadeScreen");
        m_pStripesShader = gShaderMgr->AddGetPtr("Stripes");

        if (nullptr == m_pShader ||
            nullptr == m_pShaderFadeScreen ||
            nullptr == m_pStripesShader)
        {
            return false;
        }

        m_pShader->LinkConstantBufferCPU("cb_FadePS", m_constBufFadePS);
        m_pShaderFadeScreen->LinkConstantBufferCPU("cb_FadeScreenPS", m_constBufFadeScreenPS);

        m_pVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShader);
        m_pStripesDecl = m_pDevice->CreateVertexDeclaration(PosColorVertex::VertexElements, sizeof(PosColorVertex), m_pStripesShader);

        m_pMaterial = gMaterialMgr->AddGetPtr("fade01.png");

        if (!m_pMaterial ||
            !m_pMaterial->GetTexture())
        {
            return false;
        }

        m_pMaterial->GetTexture()->SetSRGB(false);
        m_pMaterial->GetTexture()->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());

        screenQuad.Initialize(m_pDevice);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CFadeRenderer::RenderFadeScreen(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        CRenderTarget* pSource = renderSet.m_pCurrentColorTarget;

        m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);

        m_constBufFadeScreenPS.GetDataForWriting().m_fadeScreenColorBot = renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot;
        m_constBufFadeScreenPS.GetDataForWriting().m_fadeScreenColorTop = renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop;

        m_pDevice->SetVertexDeclaration(m_pVertexDecl.get());
        m_pDevice->SetShader(m_pShaderFadeScreen);
        screenQuad.RenderQuad();
    }

    //-----------------------------------------------------------------------------------
    void CFadeRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        CRenderTarget* pSource = renderSet.m_pCurrentColorSource;

        m_pDevice->SetRenderTarget(0, renderSet.m_pBackBuffer);
        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetTexture(0, pSource->GetTexture());
        m_pDevice->SetTextureSampler(2, renderSet.m_pRenderTargetDepths->GetTextureSampler());
        m_pDevice->SetTexture(2, renderSet.m_pRenderTargetDepths->GetTexture());
        m_pDevice->SetTextureSampler(1, m_pMaterial->GetTexture()->GetTextureSampler());
        m_pDevice->SetTexture(1, m_pMaterial->GetTexture());


        float phi = renderQueue.m_pCameraRenderObject->m_frustum.GetFieldOfView() * 0.5f;
        float t = -renderQueue.m_pCameraRenderObject->m_position.z * std::tan(phi);
        float r = t * renderQueue.m_pCameraRenderObject->m_frustum.GetAspectRatio();

        Matrix4x4 mtxOrthoProj;
        ComputeProjectionMatrixOrtho(mtxOrthoProj, -r, r, t, -t,
                                     renderQueue.m_pCameraRenderObject->m_near, renderQueue.m_pCameraRenderObject->m_far);

        m_constBufFadePS.GetDataForWriting().m_fadeScreenColorBot = renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorBot;
        m_constBufFadePS.GetDataForWriting().m_fadeScreenColorTop = renderQueue.m_fadeScreenRenderObject.m_fadeScreenColorTop;

        // Fade bars
        if (renderQueue.m_pFadeBarsRenderObject)
        {
            m_constBufFadePS.GetDataForWriting().m_type = (int)renderQueue.m_pFadeBarsRenderObject->fadeBarsType;
            m_constBufFadePS.GetDataForWriting().m_fadeBarsColor = renderQueue.m_pFadeBarsRenderObject->fadeColor;
            m_constBufFadePS.GetDataForWriting().m_fadeBarsAmount = renderQueue.m_pFadeBarsRenderObject->fadeBarsAmount;
            m_constBufFadePS.GetDataForWriting().m_scalingOffset = m_scalingOffset;
        }
        else
        {
            m_constBufFadePS.GetDataForWriting().m_fadeBarsAmount = 0.0f;
        }

        // Fade frame
        if (renderQueue.m_fadeFrameRenderObject.m_fadeFrame.x > 0.0f ||
            renderQueue.m_fadeFrameRenderObject.m_fadeFrame.y > 0.0f ||
            renderQueue.m_fadeFrameRenderObject.m_fadeFrame.z > 0.0f ||
            renderQueue.m_fadeFrameRenderObject.m_fadeFrame.w > 0.0f)
        {
            Vec4 fadeFrame = renderQueue.m_fadeFrameRenderObject.m_fadeFrame;
            fadeFrame.x *= 0.5f;
            fadeFrame.y = 1.0f - fadeFrame.y * 0.5f;

            fadeFrame.z *= 0.5f;
            fadeFrame.w = 1.0f - fadeFrame.w * 0.5f;

            m_constBufFadePS.GetDataForWriting().m_fadeFrameAmount = fadeFrame;
            m_constBufFadePS.GetDataForWriting().m_fadeFrameColorBot = renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorBot;
            m_constBufFadePS.GetDataForWriting().m_fadeFrameColorTop = renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorTop;

            m_fadeFrameColorBotPrevFrame = renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorBot;
            m_fadeFrameColorTopPrevFrame = renderQueue.m_fadeFrameRenderObject.m_fadeFrameColorTop;

            m_bFadeFramePrevFrame = true;
        }
        else
        {
            m_constBufFadePS.GetDataForWriting().m_fadeFrameAmount = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
            m_constBufFadePS.GetDataForWriting().m_fadeFrameColorBot = Vec4(0.0f);
            m_constBufFadePS.GetDataForWriting().m_fadeFrameColorTop = Vec4(0.0f);
            
            m_bFadeFramePrevFrame = false;
        }

        m_pDevice->SetVertexDeclaration(m_pVertexDecl.get());
        m_pDevice->SetShader(m_pShader);
        screenQuad.RenderQuad();
    }

    //-----------------------------------------------------------------------------------
    void CFadeRenderer::RenderStripes(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (m_numStripes != renderQueue.m_stripeRenderObject.m_stripes.size())
        {
            ENV_DELETE(m_pStripesVB);

            m_numStripes = renderQueue.m_stripeRenderObject.m_stripes.size();

            if (m_numStripes)
            {
                m_pStripesVB = m_pDevice->CreateVertexBuffer(sizeof(PosColorVertex) * 6 * m_numStripes, USAGE_TYPE_VERTEXINDEXBUFFERS);
            }
        }

        if (m_pStripesVB == nullptr)
            return;

        PosColorVertex* pVertices = static_cast<PosColorVertex*>(m_pStripesVB->Lock(0, 0, 0));

        unsigned int numStripesRendered = 0;

        for (Stripe& stripe : renderQueue.m_stripeRenderObject.m_stripes)
        {
            float currentStripeTime   = renderQueue.m_stripeRenderObject.m_stripesAmount * renderQueue.m_stripeRenderObject.m_stripeTime;
            float currentStripeInterp = Clamp((currentStripeTime - stripe.m_timeSpawnStart) / stripe.m_timeSpawnDuration, 0.0f, 1.0f);

            currentStripeInterp = 1.0f - currentStripeInterp;
            currentStripeInterp = currentStripeInterp * currentStripeInterp * currentStripeInterp;
            currentStripeInterp = 1.0f - currentStripeInterp;

            float currentStripeWidth  = Lerp(stripe.m_startWidth, stripe.m_endWidth, currentStripeInterp * currentStripeInterp * currentStripeInterp);

            Vec2 v0 = Lerp(stripe.m_startTopPos, stripe.m_endTopPos, currentStripeInterp);
            Vec2 v1 = Lerp(stripe.m_startTopPos, stripe.m_endTopPos, currentStripeInterp);
            Vec2 v2 = Lerp(stripe.m_startBotPos, stripe.m_endBotPos, currentStripeInterp);
            Vec2 v3 = Lerp(stripe.m_startBotPos, stripe.m_endBotPos, currentStripeInterp);

            v0.x -= currentStripeWidth;
            v1.x += currentStripeWidth;
            v2.x += currentStripeWidth;
            v3.x -= currentStripeWidth;

            Vec4 stripeTopColor = Lerp(stripe.m_startTopColor, stripe.m_endTopColor, currentStripeInterp);
            Vec4 stripeBotColor = Lerp(stripe.m_startBotColor, stripe.m_endBotColor, currentStripeInterp);

            (*(pVertices + 0)).p = Vec4(v0, 0.0f, 1.0f);
            (*(pVertices + 1)).p = Vec4(v1, 0.0f, 1.0f);
            (*(pVertices + 2)).p = Vec4(v3, 0.0f, 1.0f);
            (*(pVertices + 3)).p = Vec4(v3, 0.0f, 1.0f);
            (*(pVertices + 4)).p = Vec4(v1, 0.0f, 1.0f);
            (*(pVertices + 5)).p = Vec4(v2, 0.0f, 1.0f);

            (*(pVertices + 0)).color = Vec4(stripeTopColor);
            (*(pVertices + 1)).color = Vec4(stripeTopColor);
            (*(pVertices + 2)).color = Vec4(stripeBotColor);
            (*(pVertices + 3)).color = Vec4(stripeBotColor);
            (*(pVertices + 4)).color = Vec4(stripeTopColor);
            (*(pVertices + 5)).color = Vec4(stripeBotColor);

            pVertices += 6;

            ++numStripesRendered;
        }

        m_pStripesVB->Unlock();

        if (numStripesRendered)
        {
            m_pDevice->SetRenderTarget(0, renderSet.m_pBackBuffer);
            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_NONE);
            m_pDevice->SetTexture(0, renderSet.m_pCurrentColorSource->GetTexture());
            m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearClampSampler());
            m_pDevice->SetTexture(1, renderSet.m_pRenderTargetBlurBilateral->GetTexture());
            m_pDevice->SetTextureSampler(1, CDefaultTextureSampler::GetLinearClampSampler());
            m_pDevice->SetVertexDeclaration(m_pStripesDecl.get());
            m_pDevice->SetShader(m_pStripesShader);
            m_pDevice->SetVertexBuffer(0, 0, m_pStripesVB);
            m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, numStripesRendered * 2);
        }
    }

    //-----------------------------------------------------------------------------------
    void CFadeRenderer::OnLostDevice()
    {

    }

    //-----------------------------------------------------------------------------------
    void CFadeRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        RenderCoordRect renderCoords;

#ifdef D3D9
        Vec2 pixelsToTexels = CGPUQuad::GetPixelsToTexelsOffset(screenWidth, screenHeight);

        renderCoords.m_left += pixelsToTexels.x;
        renderCoords.m_right += pixelsToTexels.x;
        renderCoords.m_top += pixelsToTexels.y;
        renderCoords.m_bottom += pixelsToTexels.y;
#endif

        screenQuad.SetRenderRect(renderCoords);
        screenQuad.UpdateVertexBuffer();

        unsigned int width = m_pMaterial->GetTexture()->GetWidth();
        unsigned int height = m_pMaterial->GetTexture()->GetHeight();

        m_scalingOffset.x = ((float)screenWidth / width) * 1.5f;
        m_scalingOffset.y = ((float)screenHeight / height) * 1;
        m_scalingOffset.z = width * 0.9f / (float)screenWidth;
        m_scalingOffset.w = height * 0.9f / (float)screenHeight;
    }

} // env