/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef POST_PROCESS_RENDERER_H
#define POST_PROCESS_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/Quad.h>


namespace env
{
    class CFullscreenTextureRenderer;
    class CRenderDevice;
    class CTextureManaged;
    class CVertexDeclaration;

    class ENV_DECL_ALIGN_16 CPostProcessRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CPostProcessRenderer(CRenderDevice* pDevice, CFullscreenTextureRenderer* pFullscreenRenderer);
        
        virtual ~CPostProcessRenderer();
        virtual bool Initialize();
        
        void RenderCartoon(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderPostProcessing(RenderQueue& renderQueue, RenderSet& renderSet);
        
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:

        Matrix4x4 ComputeColorGrading(RenderQueue& renderQueue);

        bool InitShader(CShader* pShader);

    private:
        CRenderDevice* m_pDevice;
        CFullscreenTextureRenderer* m_pFullscreenRenderer;

        std::unique_ptr<CVertexDeclaration> m_pPostProcessVertexDecl;

        CShader* m_pCartoonShader;
        CShader* m_pPostProcessShader;

        ENV_DECL_ALIGN_16 struct ConstBufCartoonPS
        {
            Vec3   m_charPos;
            float  m_silhouetteIntensity;
            Vec3   m_silhouetteColor;
            float  m_fogRange;
            Vec3   m_fogColor;
            float  m_fogStart;
            Vec2   m_silhouetteRadiusOpacity;
            Vec2   m_texCoordDXDY;
        };

        ENV_DECL_ALIGN_16 struct ConstBufPostProcessPS
        {
            Matrix4x4 m_colorGrading;
            float m_posterizationAmount;
        };

        CConstantBufferCPU<Matrix4x4> m_constBufCartoonVS;
        CConstantBufferCPU<ConstBufCartoonPS> m_constBufCartoonPS;

        CConstantBufferCPU<Matrix4x4> m_constBufPostProcessVS;
        CConstantBufferCPU<ConstBufPostProcessPS> m_constBufPostProcessPS;
         
        CTextureManaged* m_pWaterNMap;
        CTextureManaged* m_pCombs;

        CGPUQuad m_screenQuad;
    };
} // env

#endif // POST_PROCESS_RENDERER_H