/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef DEFERRED_RENDER_SYSTEM_H
#define DEFERRED_RENDER_SYSTEM_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/RenderSystems/D3DBaseRenderSystem.h>
#include <Graphic/Core/RenderSet.h>

namespace env
{
    class CRenderSystem : public CBaseRenderSystem
    {
    public:
        CRenderSystem();
        ~CRenderSystem();

        bool Initialize(const RenderQualityParams& videoParams, const RenderSystemParams& renderSysParams);
        bool InitializeRenderers();

        void Render(float time, float dt);
        void RenderLoadingScreen(RenderQueue& renderQueue);
        void PreRender();
        void OnLostDevice();
        void OnResetDevice();
        void OnLevelChanged();

    protected:

        bool CreateRenderers();

        bool InitializeRenderTargets();

        void AddRenderPass(CRenderPassBase* pRenderPass);
        void ResetRenderPasses();

        void Clear(RenderQueue& renderQueue, RenderSet& renderSet);
        void ClearBackBuffer(RenderQueue& renderQueue, RenderSet& renderSet);

        void SwapRenderTargets(RenderQueue& renderQueue, RenderSet& renderSet);
        void SwapCharIntermediate(RenderQueue& renderQueue, RenderSet& renderSet);

        void RenderCurrentColorTarget(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderDebugDataCaches(RenderQueue& renderQueue, RenderSet& renderSet);
        void RenderLightraysMask(RenderQueue& renderQueue, RenderSet& renderSet);

    private:

        std::vector<CRenderPassBase*> m_currentRenderPasses;
        std::vector<std::unique_ptr<CRenderTarget>> m_renderTargets;

        RenderSet m_renderSet;

        struct RenderSystemImpl;

        std::unique_ptr<RenderSystemImpl> m_impl;

        inline RenderSystemImpl& GetImpl() const { return *(m_impl.get()); }
    };

    extern CRenderSystem* gRenderSystem;
} // env 

#endif // DEFERRED_RENDER_SYSTEM_H 