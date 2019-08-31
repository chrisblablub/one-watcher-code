/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_GPU_PROFILER_H
#define D3D9_GPU_PROFILER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/Core/APITypes.h>
#include <Common/Core/Profiler.h>

namespace env
{
    class CD3D9GPUProfiler : public CProfiler
    {
    public:
        CD3D9GPUProfiler(const std::string& strGroup);

        virtual ~CD3D9GPUProfiler();
        virtual void Begin(const std::string& strName);
        virtual void End();

    protected:

        IDirect3DDevice9* m_pDevice;
        IDirect3DQuery9*  m_pEvent;

        float m_time;

        bool m_bProfiling;

        LONGLONG m_start;
        LONGLONG m_stop;
        LONGLONG m_freq;
    };

    typedef CD3D9GPUProfiler CGPUProfiler;
} // env

#endif // D3D9_GPU_PROFILER_H
#endif // D3D9