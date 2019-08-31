/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENVIRONMENT_RENDERER_H
#define ENVIRONMENT_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/StateBlock.h>
#include <Graphic/Core/Shader.h>

namespace env
{
    class CTextureManaged;
    class CRenderDevice;
    class CVertexDeclaration;
    class CVertexBuffer;

    class ENV_DECL_ALIGN_16 CEnvironmentRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CEnvironmentRenderer(CRenderDevice* pDevice);
        
        virtual ~CEnvironmentRenderer();
        virtual bool Initialize();
        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet);
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        void Reload(bool bForceReload = false);
        void SetupEnvRendering(RenderQueue& renderQueue, RenderSet& renderSet);
        void CleanupEnvRendering(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderEnergyField(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderHalo(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderSkybox(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderSkyDome(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderPlanet(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderSkyPlane(RenderQueue& renderQueue, RenderSet& renderSet);

    private:

        ENV_DECL_ALIGN_16 struct ConstBufEnvDefaultVS
        {
            Matrix4x4 m_model;
            Matrix4x4 m_modelInvTrans;
            Matrix4x4 m_worldViewProj;
        };

        ENV_DECL_ALIGN_16 struct ConstBufEnergyFieldPS
        {
            Vec4 m_fogColor;
            Vec4 m_skyColor;
            Vec4 m_energyFieldColor;
            float m_time;
        };
       
        ENV_DECL_ALIGN_16 struct ConstBufHaloPS
        {
            float m_time;
            float m_intensityLeft;
            float m_intensityRight;
        };

        ENV_DECL_ALIGN_16 struct ConstBufPlanetPS
        {
            Vec3  m_fogColor;
            float m_time;
            Vec3  m_energyFieldColor;
            float m_cloudIntensity;
        };

        ENV_DECL_ALIGN_16 struct ConstBufSkyDomePS
        {
            Vec3  m_fogColor;
            float m_time;
            Vec3  m_skyColor;
            float m_energyFieldIntensity;
        };

        ENV_DECL_ALIGN_16 struct ConstBufSkyPlanePS
        {
            Vec3  m_skyColor;
            float m_time;
            float m_cloudIntensity;
        };

        CConstantBufferCPU<ConstBufEnvDefaultVS> m_constBufEnvVS;
        CConstantBufferCPU<ConstBufEnergyFieldPS> m_constBufEnergyFieldPS;

        CConstantBufferCPU<ConstBufHaloPS> m_constBufHaloPS;

        CConstantBufferCPU<ConstBufPlanetPS> m_constBufPlanetPS;

        CConstantBufferCPU<Matrix4x4> m_constBufSkyBoxWVPVS;

        CConstantBufferCPU<ConstBufSkyDomePS> m_constBufSkyDomePS;

        CConstantBufferCPU<ConstBufSkyPlanePS> m_constBufSkyPlanePS;

        template<typename ModelMtxLambda, typename WVPMtxLambda, typename SetConstBufLambda>
        void RenderMeshes(mapMeshInstanceRenderObjects& meshes, CVertexDeclaration* pVertexDeclaratrion, RenderQueue& renderQueue, bool useMeshTexture,
                          ModelMtxLambda getModelMatrix, WVPMtxLambda getWVPMatrix, SetConstBufLambda setConstBuf);

        CRenderDevice* m_pDevice;
        CVertexBuffer* m_pSkyboxVB;

        CStateBlock m_setupStateBlock;
        CStateBlock m_resetStateBlock;

        std::unique_ptr<CVertexDeclaration> m_pEnvEnergyFieldVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pEnvPlanetVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pEnvSkyDomeVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pEnvSkyBoxVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pEnvSkyPlaneVertexDecl;
        std::unique_ptr<CVertexDeclaration> m_pEnvHaloVertexDecl;

        CShader* m_pShaderEnvEnergyField;
        CShader* m_pShaderEnvPlanet;
        CShader* m_pShaderEnvSkyDome;
        CShader* m_pShaderEnvSkyBox;
        CShader* m_pShaderEnvSkyPlane;
        CShader* m_pShaderEnvHalo;

        float m_time;

        CTextureManaged* m_pSkyDomeEnergy;
        CTextureManaged* m_pSkyDomeEnergyIntro;
        CTextureManaged* m_pSkyDomeEnergyAlpha;
        CTextureManaged* m_pCombs;
        CTextureManaged* m_pWater;
        CTextureManaged* m_pClouds;
        CTextureManaged* m_pPlanetStructure;
    };

} // env
#endif // ENVIRONMENT_RENDERER_H 
