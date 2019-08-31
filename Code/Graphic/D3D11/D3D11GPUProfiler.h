/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_GPU_PROFILER_H
#define D3D11_GPU_PROFILER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/Core/APITypes.h>
#include <Common/Core/Profiler.h>

namespace env
{
    class CGPUProfiler : public CProfiler
    {
    public:
        CGPUProfiler(const std::string& strGroup);

        virtual ~CGPUProfiler();
        ENV_FORCEINLINE virtual void Begin(const std::string& strName);
        ENV_FORCEINLINE virtual void End();

    protected:

        void Flush();

        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
        ID3D11Query* m_pQueryBegin;
        ID3D11Query* m_pQueryEnd;
        ID3D11Query* m_pQueryDisjoint;

        float m_time;

        bool m_bProfiling;

        //TIME_STAMP m_beginTimestamp;
    };
} // env

#endif // D3D11_GPU_PROFILER_H
#endif // ENV_GRAPHIC_API_D3D11
