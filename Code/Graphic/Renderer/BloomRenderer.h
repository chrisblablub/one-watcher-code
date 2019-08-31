/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BLOOM_RENDERER_H
#define BLOOM_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/RenderTarget.h>
#include <Graphic/Core/Quad.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    class CRenderDevice;
    class CVertexDeclaration;
    class CGaussBlurRenderer;

    class ENV_DECL_ALIGN_16 CBloomRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CBloomRenderer(CRenderDevice* pDevice, CGaussBlurRenderer* pGaussBlurRenderer);
        virtual ~CBloomRenderer();

        virtual bool Initialize();
        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet);

        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:

        void ComputeSampleOffsetsDownscale4x4(int width, int height, std::vector<Vec2>& sampleOffsets);
        void ComputeTextureCoords(int sourceWidth, int sourceHeight, RECT* pSrcRect, CRenderTarget* pTarget, RECT* pTargetRect, TextureCoordRect* pCoords);

    private:

        CRenderDevice* m_pDevice;

        std::unique_ptr<CVertexDeclaration> m_pBloomQuadVertexDecl;

        CShader* m_pShaderBloomBlurVertical;
        CShader* m_pShaderBloomBlurVerticalAndUpscale;
        CShader* m_pShaderBloomBlurHorizontal;
        CShader* m_pShaderBloomDownscale;
        CShader* m_pShaderBloomDownscaleAndMask;
        CShader* m_pShaderBloomUpscale;
        CShader* m_pShaderBloomDarkPass;
        CShader* m_pShaderBloomBrightPass;
        CShader* m_pShaderLightrays;

        ENV_DECL_ALIGN_16 struct ConstBufBloomDarkPass
        {
            float m_bloomScale;
            int m_bLightRays;
        };

        ENV_DECL_ALIGN_16 struct ConstBufLightRaysPS
        {
            Vec3   m_lightRaysColor;
            float  m_lightRaysDecay;
            Vec2   m_lightRaysOrigin;
            float  m_lightRaysIllumDecay;
            int   m_bLightRaysOcclusionMask;

        };

        ENV_DECL_ALIGN_16 struct ConstBufBloomBrightpass
        {
            Vec4 m_pixelSize;
            Vec4 m_threshold;
        };

        CConstantBufferCPU<Vec4> m_constBufBloomBlurVerticalPS;
        CConstantBufferCPU<Vec4> m_constBufBloomBlurHorizontalPS;

        CConstantBufferCPU<Vec4> m_constBufBloomBlurBrightpassPS;

        CConstantBufferCPU<ConstBufBloomDarkPass> m_constBufBloomDarkpassPS;

        CConstantBufferCPU<ConstBufLightRaysPS> m_constBufLightRaysPS;

        std::vector<CRenderTarget*> m_renderTargetDownscale;
        std::vector<CRenderTarget*> m_renderTargetDownscaleTmp;
        std::unique_ptr<CRenderTarget> m_pRT_Downscale_Tmp;
        std::unique_ptr<CRenderTarget> m_pRT_LightRays;

        CGPUQuad m_quad;
        CGPUQuad m_quadDownscale;

        int m_croppedWidth;
        int m_croppedHeight;
    };

} // env

#endif // BLOOM_RENDERER_H