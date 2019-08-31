#include <Common/Core/CPUProfiler.h>
#include <Common/Core/ProfilingMgr.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CCPUStackedProfiler::CCPUStackedProfiler(const std::string& strGroup)
        : CProfiler(strGroup)
        , m_time(0.0f)
        , m_bProfiling(false) {}

    //-----------------------------------------------------------------------------------
    CCPUStackedProfiler::~CCPUStackedProfiler()
    {
        End();
    }

    //-----------------------------------------------------------------------------------
    CCPUAccumProfiler::CCPUAccumProfiler(const std::string& strGroup)
        : CProfiler(strGroup)
        , m_time(0.0f)
        , m_bProfiling(false) {}

    //-----------------------------------------------------------------------------------
    CCPUAccumProfiler::~CCPUAccumProfiler()
    {
        End();
    }
} // env

