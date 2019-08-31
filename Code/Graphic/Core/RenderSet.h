/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef RENDER_SET_H
#define RENDER_SET_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    class CRenderTarget;

    struct RenderSet
    {
        RenderSet()
            : m_pRenderTargetColors(nullptr)
            , m_pRenderTargetNormals(nullptr)
            , m_pRenderTargetDepths(nullptr)
            , m_pRenderTargetEmissive(nullptr)
            , m_pRenderTargetPostProcess01(nullptr)
            , m_pRenderTargetPostProcess02(nullptr)
            , m_pRenderTargetOcclusionMask(nullptr)
            , m_pRenderTargetDepthMapOrtho(nullptr)
            , m_pCurrentColorTarget(nullptr)
            , m_pCurrentColorSource(nullptr)
            , m_pBackBuffer(nullptr)
            , m_pForwardBackBuffer(nullptr)
            , m_pBloom(nullptr)
            , m_pLightRays(nullptr) {}

        RenderSet(const RenderSet&) = delete;
        RenderSet& operator=(const RenderSet&) = delete;

        CRenderTarget* m_pRenderTargetColors;
        CRenderTarget* m_pRenderTargetNormals;
        CRenderTarget* m_pRenderTargetDepths;
        CRenderTarget* m_pRenderTargetEmissive;
        CRenderTarget* m_pRenderTargetPostProcess01;
        CRenderTarget* m_pRenderTargetPostProcess02;
        CRenderTarget* m_pRenderTargetOcclusionMask;
        CRenderTarget* m_pRenderTargetDepthMapOrtho;
        CRenderTarget* m_pRenderTargetBlurBilateral;
        CRenderTarget* m_pBloom;
        CRenderTarget* m_pLightRays;
        CRenderTarget* m_pCurrentColorTarget;
        CRenderTarget* m_pCurrentColorSource;
        CRenderTarget* m_pBackBuffer;
        CRenderTarget* m_pForwardBackBuffer;

        static void SwapRendertargets(CRenderTarget** p1, CRenderTarget** p2)
        {
            CRenderTarget* pTmp = *p1;
            *p1 = *p2;
            *p2 = pTmp;
        }
    };

} // env
#endif // RENDER_SET_H