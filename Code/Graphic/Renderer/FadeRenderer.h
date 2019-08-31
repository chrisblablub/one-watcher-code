/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FADE_RENDERER_H
#define FADE_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/Quad.h>


namespace env
{
    class CRenderDevice;
    class CVertexDeclaration;
    class CTextureManaged;
    class CManagedMaterial;

    class ENV_DECL_ALIGN_16 CFadeRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CFadeRenderer(CRenderDevice* pDevice);
        virtual ~CFadeRenderer();

        virtual bool Initialize();

        void Render(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderFadeScreen(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderStripes(RenderQueue& renderQueue, RenderSet& renderSet);
        //void Render(CRenderTarget* pSource, CRenderTarget* pDepths, RenderQueue& queue, const MATRIX& mtxCamInvPMatrix);

        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:
        CRenderDevice* m_pDevice;

        std::unique_ptr<CVertexDeclaration> m_pVertexDecl;

        CShader* m_pShader;
        CShader* m_pShaderFadeScreen;

        ENV_DECL_ALIGN_16 struct ConstBufFadePS
        {
            Vec4  m_fadeScreenColorTop;
            Vec4  m_fadeScreenColorBot;
            Vec4  m_fadeFrameColorTop;
            Vec4  m_fadeFrameColorBot;
            Vec4  m_fadeBarsColor;
            Vec4  m_fadeFrameAmount;
            Vec4  m_scalingOffset;
            float m_fadeBarsAmount;
            int   m_type;
        };

        ENV_DECL_ALIGN_16 struct ConstBufFadeScreenPS
        {
            Vec4 m_fadeScreenColorTop;
            Vec4 m_fadeScreenColorBot;
        };

        CConstantBufferCPU<ConstBufFadePS> m_constBufFadePS;
        CConstantBufferCPU<ConstBufFadeScreenPS> m_constBufFadeScreenPS;


        CManagedMaterial* m_pMaterial;

        CGPUQuad screenQuad;

        Vec4 m_scalingOffset;

        Vec4 m_fadeScreenColorTopPrevFrame;
        Vec4 m_fadeScreenColorBotPrevFrame;

        Vec4 m_fadeFrameColorTopPrevFrame;
        Vec4 m_fadeFrameColorBotPrevFrame;

        bool m_bFadeFramePrevFrame;
        bool m_bFadeScreenPrevFrame;

        unsigned int m_numStripes;
        CVertexBuffer* m_pStripesVB;
        std::unique_ptr<CVertexDeclaration> m_pStripesDecl;
        CShader* m_pStripesShader;
    };
} // env 

#endif // FADE_RENDERER_H 