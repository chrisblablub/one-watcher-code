/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GAUSS_BLUR_RENDERER_H
#define GAUSS_BLUR_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/Quad.h>

namespace env
{
    class CRenderDevice;
    class CVertexDeclaration;

    class ENV_DECL_ALIGN_16 CGaussBlurRenderer : public CBaseRenderer
    {
    public:
        ENV_DECL_ALIGN_16 struct ConstBufGaussBlur
        {
            float m_sampleOffsets[16];
            float m_sampleWeights[16];
        };

        ENV_DEFINE_ALLOC_ALIGNED_16

        CGaussBlurRenderer(CRenderDevice* pDevice);
        virtual ~CGaussBlurRenderer();

        virtual bool Initialize();
        void Render(int targetWidth, int targetHeight, float intensity = 1.0f);

        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:
        float ComputeGaussianDistribution(float x, float y, float rho);
        void ComputeGaussBlurSamples(unsigned long dwD3DTexWidth, unsigned long dwD3DTexHeight, std::vector<Vec4>& avTexCoordOffset,
                                        std::vector<Vec4>& avSampleWeight, float fMultiplier);

    private:
        CRenderDevice* m_pDevice;

        CShader* m_pGaussBlurShader;

        CConstantBufferCPU<ConstBufGaussBlur> m_constBufSamples;
        CGPUQuad m_quad;

        std::unique_ptr<CVertexDeclaration> m_pVertexDecl;
    };
} // env

#endif // GAUSS_BLUR_RENDERER_H