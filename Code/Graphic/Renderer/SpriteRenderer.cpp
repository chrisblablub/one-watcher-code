#include <Graphic/Renderer/SpriteRenderer.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/VertexDeclarations.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    CSpriteRenderer::CSpriteRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pSpriteShader(nullptr) {}

    //-----------------------------------------------------------------------------------
    CSpriteRenderer::~CSpriteRenderer()
    {
        gShaderMgr->Release(m_pSpriteShader);
    }

    //-----------------------------------------------------------------------------------
    bool CSpriteRenderer::Initialize()
    {
        m_pSpriteShader = gShaderMgr->AddGetPtr("Sprites");
        if (!m_pSpriteShader)
            return false;

        m_pSpriteShader->LinkConstantBufferCPU("cb_SpritesVS", m_constBufSpritesVS);
        m_pSpriteShader->LinkConstantBufferCPU("cb_SpritesPS", m_constBufSpritesPS);

        m_pSpriteVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pSpriteShader);

        m_screenQuad.Initialize(m_pDevice);

        return (m_pSpriteShader && m_pSpriteVertexDecl);
    }

    //-----------------------------------------------------------------------------------
    void CSpriteRenderer::OnLostDevice()
    {
    }

    //-----------------------------------------------------------------------------------
    void CSpriteRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        Vec2 pixelsToTexels = CGPUQuad::GetPixelsToTexelsOffset(screenWidth, screenHeight);

        RenderCoordRect renderCoords;
        renderCoords.m_left += pixelsToTexels.x;
        renderCoords.m_right += pixelsToTexels.x;
        renderCoords.m_top += pixelsToTexels.y;
        renderCoords.m_bottom += pixelsToTexels.y;

        m_screenQuad.UpdateVertexBuffer();
    }

    //-----------------------------------------------------------------------------------
    void CSpriteRenderer::RenderTextSprites(RenderQueue& renderQueue, RenderSet& renderSet, listSpriteRenderObjects& textSprites)
    {
        Render(textSprites, renderQueue, renderSet, SPRITE_PIPELINE_AFTER_FADE, false);
    }

    //-----------------------------------------------------------------------------------
    void CSpriteRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet, SPRITE_PIPELINE pipeline, bool bZEnable)
    {
        if (!renderQueue.bRenderParticles)
            return;

        Render(renderQueue.m_spriteRenderObjects, renderQueue, renderSet, pipeline, bZEnable);
    }

    //-----------------------------------------------------------------------------------
    Matrix4x4 GetProjectionMatrix(RenderQueue& renderQueue, bool bScreenSpace)
    {
        if (bScreenSpace)
        {
            Vec2 refernceSize(1920.0f, 1080.0f);
            Vec2 resolutionScaling((float)renderQueue.m_pCameraRenderObject->m_screenWidth / refernceSize.x, (float)renderQueue.m_pCameraRenderObject->m_screenHeight / refernceSize.y);

            if (resolutionScaling.x < resolutionScaling.y)
            {
                // Use smaller scaling factor & clamp to [0.25, 2.0f]
                resolutionScaling.x = std::min(resolutionScaling.x, 2.0f);
                resolutionScaling.x = std::max(resolutionScaling.x, 0.5f);
                resolutionScaling.y = resolutionScaling.x;
            }
            else
            {
                // Use smaller scaling factor & clamp to [0.5, 2.0f]
                resolutionScaling.y = std::min(resolutionScaling.y, 2.0f);
                resolutionScaling.y = std::max(resolutionScaling.y, 0.5f);
                resolutionScaling.x = resolutionScaling.y;
            }

            Vec3 spriteResolutionScaling(1.0f / (float)(renderQueue.m_pCameraRenderObject->m_screenWidth * 0.5f),
                                         1.0f / (float)(renderQueue.m_pCameraRenderObject->m_screenHeight * 0.5f),
                                         1.0f);

            return glm::transpose(glm::scale(Matrix4x4(1.0f), Vec3(resolutionScaling.x * spriteResolutionScaling.x, resolutionScaling.y * spriteResolutionScaling.y, 1.0f)));
        }
        else
        {
            return renderQueue.m_pCameraRenderObject->m_mtxViewProjection;
        }
    }

    //-----------------------------------------------------------------------------------
    void CSpriteRenderer::Render(listSpriteRenderObjects& sprites, RenderQueue& renderQueue, RenderSet& renderSet, SPRITE_PIPELINE pipeline, bool bZEnable)
    {
        if (!renderQueue.bRenderParticles)
            return;

        struct
        {
            bool operator()(SpriteRenderObject& pLeft, SpriteRenderObject& pRight)
            {
                return pLeft.m_mtxRST[2][3] > pRight.m_mtxRST[2][3];
            }
        } spriteSorter;

        //CProfiler prof(false);
        //prof.Start();
        sprites.sort(spriteSorter);
        //prof.Stop();

        //float f = prof.GetTime();
        m_pDevice->ResetToDefaultRenderStates();
        //        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, 1);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_NOTEQUAL);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_CULLMODE, CULLMODE_TYPE_NONE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);


        //         if (!bApplyFade)
        //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRGBWRITEENABLE, true);

        m_pDevice->SetVertexDeclaration(m_pSpriteVertexDecl.get());
        m_pDevice->SetShader(m_pSpriteShader);

        m_screenQuad.BindQuad();

        for (listSpriteRenderObjects::iterator it = sprites.begin(); it != sprites.end(); ++it)
        {
            SpriteRenderObject* pSprite = &*it;
            if (!pSprite->m_pTexture)
                continue;

            if (pSprite->m_pipeline != pipeline)
                continue;

            if (pSprite->m_bAlphaBlending)
                m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            else
                m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, pSprite->m_bDepthTest);

            // The sprites projection matrix: either scaling for screenspace for projection
            Matrix4x4 mtxProjection = GetProjectionMatrix(renderQueue, pSprite->m_bScreenSpace);
            Matrix4x4 mtxScreenSpaceScale = Matrix4x4(1.0f);

            if (pSprite->m_bScreenSpace)
                m_constBufSpritesVS.GetDataForWriting().m_worldViewProj = pSprite->m_mtxRSTPre * mtxProjection * pSprite->m_mtxRST;
            else
                m_constBufSpritesVS.GetDataForWriting().m_worldViewProj = pSprite->m_mtxRST * mtxProjection;

            m_constBufSpritesVS.GetDataForWriting().m_texRect = pSprite->m_texRect;

            m_constBufSpritesPS.GetDataForWriting() = pSprite->m_color;

            m_pDevice->SetTextureSampler(0, CDefaultTextureSampler::GetLinearMipClampSampler()/*pSprite->m_pTexture->GetTextureSampler()*/);
            m_pDevice->SetTexture(0, pSprite->m_pTexture);

            m_screenQuad.DrawQuad();
        }
    }

} // env