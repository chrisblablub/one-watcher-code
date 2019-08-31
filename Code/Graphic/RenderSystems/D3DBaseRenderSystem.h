/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef D3D_RENDER_SYSTEM_H
#define D3D_RENDER_SYSTEM_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Core/StateBlock.h>
#include <Graphic/Core/RenderQualityParams.h>
#include <Graphic/Core/RenderSystemParams.h>
#include <Graphic/Renderer/BaseRenderer.h>

namespace env
{
    class CRenderDevice;

    class CBaseRenderSystem
    {
    public:
        typedef std::map< std::string, CBaseRenderer* > mapRenderers;

        CBaseRenderSystem();
        CBaseRenderSystem(const CBaseRenderSystem&) = delete;
        CBaseRenderSystem& operator=(const CBaseRenderSystem&) = delete;

        bool Initialize(const RenderQualityParams& renderQualityParams, const RenderSystemParams& renderSysParams);
        void OnLostDevice();
        void OnResetDevice();
        void SetRenderQueue(RenderQueue* pQueue);
        bool CreateDevice();
        void AddRenderer(const std::string& strName, CBaseRenderer* pRenderer);
        CBaseRenderer* GetRenderer(const std::string& strName);
        void RemoveRenderer(const std::string& strName);
        void OnLevelChanged();
        RenderQualityParams& GetRenderQualityParams() { return m_renderQualityParams; }

        void BeginScene();
        void EndScene();

        bool InitializeRenderers();
        bool ResetDevice(const RenderQualityParams& videoParams);
        bool ResetDevice();

        CRenderDevice* GetRenderDevice() const { return m_pRenderDevice; }

    protected:

        ~CBaseRenderSystem();

        bool ReadDeviceSettings();
        bool CheckDeviceSettings();

        RenderSystemParams  m_renderSysParams;
        RenderQualityParams m_renderQualityParams;

        mapRenderers m_renderers;

        RenderQueue*   m_pRenderQueue;
        CRenderDevice* m_pRenderDevice;
    };
} // env 
#endif // D3D_RENDER_SYSTEM_H 