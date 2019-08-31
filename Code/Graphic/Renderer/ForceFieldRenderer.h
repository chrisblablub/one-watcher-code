/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>

namespace env
{
    class CForceFieldEntity;
    class CTextureManaged;
    class CVertexBuffer;
    class CVertexDeclaration;
    class CRenderDevice;

    class ENV_DECL_ALIGN_16 CForceFieldRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CForceFieldRenderer(CRenderDevice* pDevice);
        virtual ~CForceFieldRenderer();

        virtual bool Initialize();
        virtual void PreRender(RenderQueue& renderQueue, RenderSet& renderSet);
        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet);
        virtual void PostRender(RenderQueue& renderQueue, RenderSet& renderSet);
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        //void Render(float dt, const MATRIX& mtxVP, bool bUpdate, CRenderTarget* pDepthNormals, CRenderTarget* pSource);
       
//         void AddForceField(const std::string& strName, CForceFieldEntity* pForceField);
//         void RemoveForceField(const std::string& strName);

        virtual void OnLevelChanged();

    protected:
        void InitBuffer(RenderQueue& renderQueue);

        std::string m_strName;

        CVertexBuffer* m_pVB;
        CRenderDevice* m_pDevice;
        std::unique_ptr<CVertexDeclaration> m_pVertexDeclaration;

        ENV_DECL_ALIGN_16 struct ConstBufForcefieldPS
        {
            Vec4 m_timeDirection;
            Vec4 m_rotMatrix;
            Vec2 m_direction;
            Vec2 m_invScreenSize;
            Vec3 m_color;
        };

        CConstantBufferCPU<Matrix4x4> m_constBufForcefieldVS;
        CConstantBufferCPU<ConstBufForcefieldPS> m_constBufForcefieldPS;


        CShader* m_pShader;

        CTextureManaged* m_pWaterSpecularityMap;
        CTextureManaged* m_pWaterNormalMap;
        CTextureManaged* m_pForceField;

        float m_time;
    };
}
