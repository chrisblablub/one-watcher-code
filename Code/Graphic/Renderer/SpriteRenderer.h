/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/Quad.h>

namespace env
{
    class CRenderDevice;
    class CVertexDeclaration;

    class ENV_DECL_ALIGN_16 CSpriteRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CSpriteRenderer(CRenderDevice* pDevice);
        
        virtual ~CSpriteRenderer();
        virtual bool Initialize();
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        void Render(RenderQueue& renderQueue, RenderSet& renderSet, SPRITE_PIPELINE pipeline = SPRITE_PIPELINE_BEFORE_POSTPROCESSING, bool ZEnable = true);
        void RenderTextSprites(RenderQueue& renderQueue, RenderSet& renderSet, listSpriteRenderObjects& textSprites);

        void Render(listSpriteRenderObjects& sprites, RenderQueue& renderQueue, RenderSet& renderSet, SPRITE_PIPELINE pipeline, bool bZEnable);

    private:

        CGPUQuad m_screenQuad;
        CGenericGPUQuad<PosColorTexVertex> m_screenQuadLensFlare;

        CRenderDevice* m_pDevice;

        std::unique_ptr<CVertexDeclaration> m_pSpriteVertexDecl;

        CShader* m_pSpriteShader;

        ENV_DECL_ALIGN_16 struct ConstBufSpritesVS
        {
            Matrix4x4 m_worldViewProj;
            Vec4      m_texRect;
        };

        CConstantBufferCPU<ConstBufSpritesVS> m_constBufSpritesVS;
        CConstantBufferCPU<Vec4> m_constBufSpritesPS;
    };

    class CSpriteRenderPass : public CDeferredRendererRenderPass<CSpriteRenderer>
    {
    public:
        CSpriteRenderPass(SPRITE_PIPELINE pipeline) 
            : CDeferredRendererRenderPass<CSpriteRenderer>()
            , m_pipeline(pipeline) {}

        virtual void Init(const std::string& strName, CSpriteRenderer* pRenderer)
        {
            CDeferredRendererRenderPass<CSpriteRenderer>::Init(strName, pRenderer);
        }

        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet)
        {
            m_pRenderer->Render(renderQueue, renderSet, m_pipeline, true);
        }

    private:

        SPRITE_PIPELINE m_pipeline;
    };
} // env
#endif // SPRITERENDERER_H