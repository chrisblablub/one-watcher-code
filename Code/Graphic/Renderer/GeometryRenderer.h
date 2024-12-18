/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GEOMETRY_RENDERER_H
#define GEOMETRY_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>
#include <Graphic/Core/StateBlock.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/Texture.h>

namespace env
{
    struct RenderQueue;

    class CFullscreenTextureRenderer;
    class CRenderDevice;
    class CVertexDeclaration;

    // Used for lighting
    const int STENCILREF_DEFERRED_GEOMETRY = 6;
    const int STENCILREF_DEFERRED_GEOMETRY_NODIRECTIONAL = 8;
    const int STENCILREF_DEFERRED_TERRAIN = 3;
    const int STENCILREF_DEFERRED_TERRAIN_BORDER = 4;
    const int STENCILREF_DEFERRED_TERRAIN_SELECTION = 5;
    const int STENCILREF_DEFERRED_TERRAIN_PREVIEW = 5;

    // Used for motion blur (separately during the very last pass after clearing previous stencil)
    const int STENCILREF_DEFERRED_GEOMETRY_MOTIONBLUR = 1;

    class ENV_DECL_ALIGN_16 CGeometryRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CGeometryRenderer(CRenderDevice* pDevice, CFullscreenTextureRenderer* pFSTextureRenderer);

        virtual ~CGeometryRenderer();
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);
        virtual void OnLevelChanged();
        virtual bool Initialize();

        void SetupDeferredRendering(RenderQueue& renderQueue, RenderSet& renderSet);
        void CleanupDeferredRendering(RenderQueue& renderQueue, RenderSet& renderSet);

        void RenderGeometryDeferred(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryDeferredLayer0(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryTransmissive(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryForward(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryOutline(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryMotionBlur(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryCharacter(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderGeometryCharacterDistort(RenderQueue& renderQueue, RenderSet& renderSet);

        void ClearRenderTargets();

        void RenderLevelTerrainPreview(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderLevelTerrainBorder(RenderQueue& renderQueue, RenderSet& renderSet);
        
        void RenderLevelTerrain(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderLevelTerrainLayer0(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderLevelTerrainInternal(RenderQueue& renderQueue, RenderSet& renderSet, bool bLayer0);
        void RenderLevelTerrainSelection(RenderQueue& renderQueue, RenderSet& renderSet);


        int GetNumMeshTrianglesRendered() const { return m_numMeshTrianglesRendered; }
        int GetNumTerrainTrianglesRendered() const { return m_numTerrainTrianglesRendered; }
        int GetNumBorderTrianglesRendered() const { return m_numBorderTrianglesRendered; }

        static void DrawMeshRenderObject(CRenderDevice* pDevice, MeshRenderObject& meshRenderObject);

    private:

        CShader* GetGeometryShader(RenderQueue& renderQueue) const;
        CShader* GetTerrainShader(RenderQueue& renderQueue) const;

        void Render(RenderQueue& renderQueue,
                    CVertexDeclaration* pVertexDeclaration,
                    mapMeshInstanceRenderObjects& meshInstances,
                    const Matrix4x4& mtxScaling = Matrix4x4(1.0f),
                    bool stencilFlagDirectionalLighting = true,
                    bool stencilFlagPostProcessing = false,
                    bool stencilPostProcessing = true);

        void RenderMotionBlur(RenderQueue& renderQueue,
                              CVertexDeclaration* pVertexDeclaration,
                              mapMeshInstanceRenderObjects& meshInstances,
                              const Matrix4x4& mtxScaling = Matrix4x4(1.0f));

        void BindMesh(RenderQueue& renderQueue, MeshRenderObject* pMesh, CVertexDeclaration* pVertexDeclaration);

        void DrawInstance(RenderQueue& renderQueue, MeshRenderObject* pMesh, MeshInstanceRenderObject* pInstance, const Matrix4x4& mtxScaling, const Matrix4x4& mtxViewProj, const Matrix4x4& mtxView);
    private:

        CStateBlock m_initialStateBlock;
        CStateBlock m_levelTerrainStateBlock;
        CStateBlock m_finalStateBlock;

        CRenderDevice* m_pDevice;
        std::unique_ptr<CVertexDeclaration> m_pLevelVertexDeclaration;
        std::unique_ptr<CVertexDeclaration> m_pLevelBorderVertexDeclaration;
        std::unique_ptr<CVertexDeclaration> m_pGeometryVertexDeclaration;
        std::unique_ptr<CVertexDeclaration> m_pGeometryForwardVertexDeclaration;
        std::unique_ptr<CVertexDeclaration> m_pLevelForwardVertexDeclaration;

        CTextureManaged* m_pWaterNormalMap;
        CTextureManaged* m_pWaterSpecularityMap;
        CTextureManaged* m_pBorderTexture;
        CTextureManaged* m_pSnowTexture;

        CShader* m_pShaderGeometryForward;
        CShader* m_pShaderGeometryNormal;
        CShader* m_pShaderGeometryWet;
        CShader* m_pShaderGeometrySnow;
        CShader* m_pShaderGeometryTransmissive;
        CShader* m_pShaderGeometryCharacter;
        CShader* m_pShaderGeometryCharacterDistort;
        CShader* m_pShaderGeometryOutline;
        CShader* m_pShaderGeometryMotionBlur;
        CShader* m_pShaderTerrainSelection;
        CShader* m_pShaderTerrainForward;
        CShader* m_pShaderTerrainNormal;
        CShader* m_pShaderTerrainWet;
        CShader* m_pShaderTerrainSnow;
        CShader* m_pShaderTerrainBorder;

        ENV_DECL_ALIGN_16 struct ConstBufGeometryVS
        {
            Matrix4x4 m_view;
            Matrix4x4 m_viewProjection;
            Matrix4x4 m_model;
        };

        ENV_DECL_ALIGN_16 struct ConstBufGeometryNormalPS
        {
            Vec4 m_diffuseMtrl;
            Vec4 m_emissiveMtrl;
            Vec4 m_envMtrl;
            int m_bHasNormalMap;
            int m_bHasSpecularMap;
            int m_bHasEmissiveMap;
            int m_bHasTexture;
            int m_bHasEnvMap;
        };

        ENV_DECL_ALIGN_16 struct ConstBufTerrainNormalPS
        {
            Vec4 m_diffuseMtrl;
            Vec4 m_emissiveMtrl;
            Vec4 m_envMtrl;
            int m_bHasNormalMap;
            int m_bHasSpecularMap;
            int m_bHasEmissiveMap;
            int m_bHasTexture;
            int m_bHasEnvMap;
            int m_bHasPrecomputedLighting;
            float m_time;
        };

        ENV_DECL_ALIGN_16 struct ConstBufTerrainBorderPS
        {
            Vec4 m_diffuseMtrl;
            int m_bHasPrecomputedLighting;
        };

        ENV_DECL_ALIGN_16 struct ConstBufTerrainVS
        {
            Matrix4x4 m_viewProjection;
            Matrix4x4 m_view;
        };

        ENV_DECL_ALIGN_16 struct ConstBufGeometryCharPS
        {
            Vec4 m_charMtrl;
            Vec2 m_charParams;
        };

        CConstantBufferCPU<ConstBufGeometryVS> m_constBufGeometryNormalVS;
        CConstantBufferCPU<ConstBufGeometryNormalPS> m_constBufGeometryNormalPS;

        CConstantBufferCPU<Vec4> m_constBufGeometryOutlinePS;

        CConstantBufferCPU<Vec4> m_constBufGeometryWetPS;

        CConstantBufferCPU<ConstBufGeometryCharPS> m_constBufGeometrCharPS;

        CConstantBufferCPU<ConstBufTerrainVS> m_constBufTerrainNormalVS;
        CConstantBufferCPU<ConstBufTerrainNormalPS> m_constBufTerrainNormalPS;

        CConstantBufferCPU<Matrix4x4> m_constBufTerrainBorderVS;
        CConstantBufferCPU<ConstBufTerrainBorderPS> m_constBufTerrainBorderPS;

        
        float m_time;

        int m_numMeshTrianglesRendered;
        int m_numTerrainTrianglesRendered;
        int m_numBorderTrianglesRendered;
        int m_numMeshTypesRendered;
        int m_numMeshInstancesRendered;

        std::shared_ptr<CRenderTarget> m_pTerrainSelectionDepth;
    };
} // env
#endif // GEOMETRY_RENDERER_H 