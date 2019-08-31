/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef RAIN_RENDERER_H
#define RAIN_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/Quad.h>

namespace env
{
    class CRenderDevice;
    class CTextureManaged;

    class CRainRenderer : public CBaseRenderer
    {
    public:
        CRainRenderer(CRenderDevice* pDevice);
        virtual ~CRainRenderer();

        virtual bool Initialize();

        void Render(CRenderTarget* pSource, CRenderTarget* pNormals, CRenderTarget* pDepth,
                    const Matrix4x4& mtxCamPMatrix, const Matrix4x4& mtxCamVMatrix,
                    const Vec3& camPos, float refractionStrength,
                    const Vec2& invScreenSize,
                    float dt);

        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:
        CRenderDevice* m_pDevice;

        CShader* m_pShader;

        CGPUQuad m_renderQuad;

        CTextureManaged* m_pRainTexture;


    };
} // env 
#endif // RAIN_RENDERER_H 