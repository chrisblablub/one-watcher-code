/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CHARACTER_RENDERER_H
#define CHARACTER_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/RenderTarget.h>
#include <Graphic/Core/Quad.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    const float CELL_STRETCH_X = 1.2f;
    const float CELL_OFFSET_X = 40.0f * 1.1f;

    const float CELL_EVEN_SIZE_Y = 32.0f * 1.1f;
    const float CELL_EVEN_SIZE_X = CELL_EVEN_SIZE_Y * CELL_STRETCH_X;

    const float CELL_EVEN_DISTANCE_Y = CELL_EVEN_SIZE_Y * 2.0f;
    const float CELL_EVEN_DISTANCE_X = CELL_EVEN_SIZE_X * 2.0f + CELL_OFFSET_X;

    class CRenderDevice;
    class CSpriteRenderer;
    class CFullscreenTextureRenderer;
    class CVertexDeclaration;

    class ENV_DECL_ALIGN_16 CCharacterRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CCharacterRenderer(CRenderDevice* pDevice, 
                           CSpriteRenderer* pSpriteRenderer);
        virtual ~CCharacterRenderer();

        void RenderCells(RenderQueue& renderQueue, RenderSet& renderSet);

        void RenderCharacter(RenderQueue& renderQueue, float dt, CSpriteRenderer* pSpriteRenderer, CRenderTarget* pTarget, CFullscreenTextureRenderer* pFsRenderer);

        void RenderBackground(RenderQueue& renderQueue, RenderSet& renderSet);

        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet);

        virtual bool Initialize();
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        virtual void OnLevelChanged();

    private:

        void UpdateQuadTransform(RenderQueue& renderQueue, const Vec2& charScreenPos);
        void UpdateRenderTargetSize(RenderQueue& renderQueue);

        void ComputeCharacterFrustum(RenderQueue& renderQueue, float& l, float& r, float& t, float& b);
    private:

        ENV_DECL_ALIGN_16 struct ConstBufLevelBackgroundVS
        {
            Matrix4x4 m_viewProj;
            Vec4 m_position;
            Vec4 m_alpha;
        };

        CConstantBufferCPU<ConstBufLevelBackgroundVS> m_constBufCellsVS;
        CConstantBufferCPU<Matrix4x4> m_constBufCharVS;
        CConstantBufferCPU<Vec4> m_constBufCharPS;

        Vec4 m_scaling;
        Vec4 m_currentScaling;
        Vec2 m_currentOffsetOpacity;
        Vec2 m_currentEnergyDir;

        Vec2 m_charPrevVelocity;
        Vec3 m_charPrevPos;

        float m_currentStandingDistortionTime;

        std::shared_ptr<CRenderTarget> m_pCharacterRT;
        CSpriteRenderer* m_pSpriteRenderer;

        CGPUQuad screenQuad;

        std::unique_ptr<CVertexDeclaration> m_pCharacterVertexDecl;

        CShader* m_pCharacterShader;

        CTextureManaged* m_pWaterNormalMap;

        CRenderDevice* m_pDevice;

        struct Cell
        {
            Cell(int x, int y, float t, float lt, float ft, bool bEven, int seq)
                : cellX(x), cellY(y), emitTime(t), lifeTime(lt), fadeTime(ft),
                alpha(0.0f), bEven(bEven), sequence(seq),
                glowAlpha(0.0f),
                bFadeIn(true), bFadeOut(false) {}

            int cellX, cellY;

            float lifeTime;
            float emitTime;
            float fadeTime;
            float alpha;
            float glowAlpha;

            bool bEven;
            int sequence;

            bool bFadeIn;
            bool bFadeOut;
        };

        float m_time;

        std::list< Cell > m_evenCells;

         CVertexBuffer* m_pVBBackground;
         CShader* m_pBackgroundShader;
         CTextureManaged* m_pTextureBackground;
//         CMatrixUniform m_uniformVP;
//         CVec3Uniform m_uniformCharPos;
//         CVec3Uniform m_uniformPosition;
//         CVec2Uniform m_uniformTexCoordOffset;
//         CVec2Uniform m_uniformAlpha;
//         CVec2Uniform m_uniformInvScreenSize;
// 
//         CShader* m_pCharBackgroundShader;
// 
//         CGPUQuad m_screenQuad;
//         CVertexDeclaration* m_pCharacterBackgroundVertexDecl;
//         CMatrixUniform m_uniformCamInvPMatrix;
//         CMatrixUniform m_uniformCamInvVMatrix;

    };

//    typedef CRenderPass< CCharacterRenderer > CCharacterRenderPass;
} /* end */

#endif /* CHARACTER_RENDERER_H */