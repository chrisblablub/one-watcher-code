/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PROFILER_H
#define PROFILER_H

#include <Common/Common.h>
#include <Common/Core/Macros.h>
#include <Common/Core/ProfilingMgr.h>

namespace env
{
    class CProfiler
    {
    public:
        CProfiler(const std::string& strGroup)
            : m_strGroup(strGroup) {}

        ENV_FORCEINLINE bool IsProfiling() const
        {
            return CProfilingMgr::GetInstance().GetProfiling(m_strGroup);
        }

        ENV_FORCEINLINE const std::string& GetName() const { return m_strName; }

    protected:
        
        std::string m_strName;
        std::string m_strGroup;
    };

} // env

#endif // PROFILER_H
