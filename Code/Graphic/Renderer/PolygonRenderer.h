/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef POLYGON_RENDERER_H
#define POLYGON_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/Shader.h>


namespace env
{
    class CRenderDevice;
    class CDataCacheRenderer;

    class ENV_DECL_ALIGN_16 CPolygonRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CPolygonRenderer();
        ~CPolygonRenderer();

        void Render(CDataCacheRenderer* pRenderer, const Matrix4x4& mtxViewProj);

        virtual bool Initialize();
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        CShader* GetShader() const { return m_pShader; }

    private:
        CShader* m_pShader;
        CConstantBufferCPU<Matrix4x4> m_constBufPolygonVS;
    };

} // env
#endif // POLYGON_RENDERER_H 