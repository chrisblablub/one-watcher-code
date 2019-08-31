/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FULLSCREEN_TEXTURE_RENDERER_H
#define FULLSCREEN_TEXTURE_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Quad.h>
#include <Graphic/Core/Shader.h>
#include <Graphic/Core/StateBlock.h>


namespace env
{
    class CRenderDevice;
    class CVertexDeclaration;
    class CRenderTarget;

    class CFullscreenTextureRenderer : public CBaseRenderer
    {
    public:

        CFullscreenTextureRenderer(CRenderDevice* pDevice);

        virtual ~CFullscreenTextureRenderer();
        virtual bool Initialize();
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        CRenderDevice* GetDevice() const { return m_pDevice; }

        void Render(CRenderTarget* pSource, RenderQueue& renderQueue, RenderSet& renderSet, CRenderTarget* pTarget = nullptr, int mipLevel = 0);
        void Render(RenderQueue& renderQueue);

    private:

        CRenderDevice* m_pDevice;
        CShader* m_pShaderFullscreenTexture;
        CShader* m_pShaderFullscreenColor;
        std::unique_ptr<CVertexDeclaration> m_pVertexDeclFSTexture;
        std::unique_ptr<CVertexDeclaration> m_pVertexDeclFSColor;

        CGPUQuad m_quad;
    };

    class CFullScreenTextureRenderPass
    {
    public:
        CFullScreenTextureRenderPass()
            : m_pRenderer(nullptr) {}

        void Initialize(CFullscreenTextureRenderer* pRenderer) 
        { 
            m_pRenderer = pRenderer; 
        }

        void Render(RenderQueue& renderQueue, RenderSet& renderSet);

        CStateBlock& GetRenderStates() { return m_renderStates; }

    private:
        CFullscreenTextureRenderer* m_pRenderer;

        CStateBlock m_renderStates;
    };

} // env

#endif // FULLSCREEN_TEXTURE_RENDERER_H