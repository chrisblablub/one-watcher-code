/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/Shader.h>

namespace env
{
    class CSpriteRenderer;
    class CRenderDevice;
    class CFont;

    class CTextRenderer : public CBaseRenderer
    {
    public:
        typedef std::map< std::string, CFont* > mapFonts;

        CTextRenderer(CSpriteRenderer* pSpriteRenderer);
        virtual ~CTextRenderer();

        virtual bool Initialize();

        void Render(RenderQueue& queue, RenderSet& renderSet);

        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:

        CSpriteRenderer* m_pSpriteRenderer;

        int m_screenWidth;
        int m_screenHeight;

    };
} // namespace
#endif // TEXT_RENDERER_H 