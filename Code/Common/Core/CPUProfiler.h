/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CPU_PROFILER_H
#define CPU_PROFILER_H

#include <Common/Common.h>
#include <Common/Core/Profiler.h>
#include <Common/Core/ProfilingMgr.h>

namespace env
{
    class CCPUStackedProfiler : public CProfiler
    {
    public:
        CCPUStackedProfiler(const std::string& strGroup);
        ~CCPUStackedProfiler();

        ENV_FORCEINLINE void Begin(const std::string& strName)
        {
            m_bProfiling = true;

            CProfilingMgr::GetInstance().PushProfiler(m_strGroup, strName);

            m_beginTimestamp = CPUClock::GetTimestamp();

            m_strName = strName;
        }

        ENV_FORCEINLINE void End()
        {
            if (!m_bProfiling)
                return;

            m_time = CPUClock::ConvertToMilliSecs(m_beginTimestamp);

            CProfilingMgr::GetInstance().PopProfiler(m_strGroup, m_time);

            m_bProfiling = false;

            m_strName.clear();
        }
        
    private:

        bool m_bProfiling;

        TIME_STAMP m_beginTimestamp;

        float m_time;
    };

    class CCPUAccumProfiler : public CProfiler
    {
    public:
        CCPUAccumProfiler(const std::string& strGroup);
        ~CCPUAccumProfiler();

        ENV_FORCEINLINE void Begin(const std::string& strName)
        {
            m_bProfiling = CProfilingMgr::GetInstance().GetProfiling(m_strGroup);

            if (!m_bProfiling)
                return;

            m_beginTimestamp = CPUClock::GetTimestamp();

            m_strName = strName;
        }

        ENV_FORCEINLINE void End()
        {
            if (!m_bProfiling)
                return;

            m_time = CPUClock::ConvertToMilliSecs(m_beginTimestamp);

            CProfilingMgr::GetInstance().AddAccumProfilingResult(m_strGroup, m_strName, m_time);

            m_bProfiling = false;

            m_strName.clear();
        }

    private:

        bool m_bProfiling;

        TIME_STAMP m_beginTimestamp;

        float m_time;
    };
    
} // env

#endif // CPU_PROFILER_H
