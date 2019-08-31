/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHTING_RENDERER_H
#define LIGHTING_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/StateBlock.h>
#include <Graphic/Core/Quad.h>
#include <Graphic/Core/Shader.h>


namespace env
{
    class CLight;

    class CVertexDeclaration;
    class CRenderDevice;
    class CTextureManaged;
    class CFullscreenTextureRenderer;
    class CMeshContainer;
    class CMesh;

    class ENV_DECL_ALIGN_16 CLightingRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CLightingRenderer(CRenderDevice* pDevice);
        
        virtual ~CLightingRenderer();
        virtual bool Initialize();
        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet);
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:

        CRenderDevice* m_pDevice;

        std::unique_ptr<CVertexDeclaration> m_pOmniVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pDirectionalVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pUnlitVertexDecl;

        CShader* m_pShaderLightingOmni;
        CShader* m_pShaderLightingDirectional;
        CShader* m_pShaderLightingReflection;
        CShader* m_pShaderLightingEmissive;
        CShader* m_pShaderLightingUnlit;

        ENV_DECL_ALIGN_16 struct ConstBufDirectionalPS
        {
            Vec4 m_diffuse;
            Vec4 m_specular;
            Vec4 m_direction;
            Vec2 m_invScreenSize;
            int m_bSpecular;
        };

        ENV_DECL_ALIGN_16 struct ConstBufOmniVS
        {
            Matrix4x4 m_view;
            Matrix4x4 m_worldViewProj;
        };

        ENV_DECL_ALIGN_16 struct ConstBufOmniPS
        {
            Vec4 m_position;
            Vec4 m_diffuse;
            Vec4 m_specular;
            Vec2 m_invScreenSize;
            float m_radius;
            int m_bSpecular;
        };

        CConstantBufferCPU<Matrix4x4> m_constBufDirectionalVS;
        CConstantBufferCPU<ConstBufDirectionalPS> m_constBufDirectionalPS;

        CConstantBufferCPU<Matrix4x4> m_constBufEmissiveVS;
        CConstantBufferCPU<Vec4> m_constBufEmissivePS;

        CConstantBufferCPU<ConstBufOmniVS> m_constBufOmniVS;
        CConstantBufferCPU<ConstBufOmniPS> m_constBufOmniPS;

        CConstantBufferCPU<Matrix4x4> m_constBufReflectionVS;
        CConstantBufferCPU<Vec4> m_constBufReflectionPS;

        CConstantBufferCPU<Matrix4x4> m_constBufUnlitVS;
        CConstantBufferCPU<Vec4> m_constBufUnlitPS;

        CMeshContainer* m_pSphere;
        CMesh* m_pSphereMesh;

        CGPUQuad m_renderQuad;
    };

} // env
#endif // LIGHTING_RENDERER_H 