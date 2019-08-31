/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/RenderObjects/ParticleSystemRenderObject.h>

namespace env
{
    class CParticleSystemEntity;
    class CRenderDevice;
    class CVertexDeclaration;

    class CParticleSystemRenderer : public CBaseRenderer
    {
    public:

        CParticleSystemRenderer(CRenderDevice* pDevice);

        virtual ~CParticleSystemRenderer();
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);
        virtual bool Initialize();

        inline unsigned int GetNumParticlesRendered() const { return m_numParticlesRendered; }

    protected:

        bool InitEffect(const std::string& strEffectFile);

    protected:

        CRenderDevice* m_pDevice;
        CShader* m_pShader;

        CConstantBufferCPU<Matrix4x4> m_constBufParticleVS;

        unsigned int m_numParticlesRendered;
    };

    class ENV_DECL_ALIGN_16 CParticleSystemColorRenderer : public CParticleSystemRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16;

        CParticleSystemColorRenderer(CRenderDevice* pDevice)
            : CParticleSystemRenderer(pDevice) {}

        virtual bool Initialize() override;

        void RenderDefault(RenderQueue& renderQueue, RenderSet& renderSet);
        void Render(const listParticleSystemRenderObjects& particleSystemRenderObjects,
                    const Matrix4x4& mtxViewProj);
    private:

        std::unique_ptr<CVertexDeclaration> m_pColorVertexDecl;


    };


    class ENV_DECL_ALIGN_16 CParticleSystemDistortRenderer : public CParticleSystemRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16;

        CParticleSystemDistortRenderer(CRenderDevice* pDevice)
            : CParticleSystemRenderer(pDevice) {}

        virtual bool Initialize() override;

        void RenderDefault(RenderQueue& renderQueue, RenderSet& renderSet);
        void Render(const RenderSet& renderSet, 
                    const listParticleSystemRenderObjects& particleSystemRenderObjects,
                    const Matrix4x4& mtxViewProj);

    private:

        std::unique_ptr<CVertexDeclaration> m_pDistortVertexDecl;
    };

}