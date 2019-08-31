/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PROFILING_MGR_H
#define PROFILING_MGR_H

#include <Common/Common.h>
#include <Common/Core/Singleton.h>
#include <Common/Core/CPUClock.h>

namespace env
{
    const unsigned int NUM_PROFILING_SAMPLES = 30;

    struct ProfilingData
    {
        ProfilingData()
            : m_time(0.0f), m_bDone(false) {}

        ProfilingData(float t, const std::string& str)
            : m_time(t), strName(str), m_bDone(false) {}

        float m_time;

        std::string strName;

        bool m_bDone;

        ENV_FORCEINLINE bool operator<(const ProfilingData& rhs) const
        {
            return m_time < rhs.m_time;
        }
    };

    struct ProfilingTree
    {
        ProfilingTree() {}

        ProfilingTree(const ProfilingData& data)
            : m_profilingData(data) {}

        ProfilingData m_profilingData;

        std::list< ProfilingTree > m_profilingStack;
    };

    struct ProfilingTreeHead
    {
        std::list< ProfilingTree > m_profilingStack;
    };

    class CProfilingMgr : public CSingleton< CProfilingMgr >
    {
        friend class CSingleton< CProfilingMgr >;

    public:
       
        ~CProfilingMgr();

        void BeginProfilingFrame();
        void EndProfilingFrame(float deltaTime);
        void PushProfiler(const std::string& strGroup, const std::string& strName);
        void PopProfiler(const std::string& strGroup, float time);
        void SetFrameProfilingGroup(const std::string& strGroup);

        ENV_FORCEINLINE void AddAccumProfilingResult(const std::string& strGroup, const std::string& strName, float time) { m_profilingAccum[strGroup][strName] += time; }
        ENV_FORCEINLINE bool GetProfiling()                            const { return !m_strProfilingGroup.empty(); }
        ENV_FORCEINLINE bool GetProfilingFinished()                    const { return !m_strProfilingGroup.empty() && m_samples == NUM_PROFILING_SAMPLES; }
        ENV_FORCEINLINE bool GetProfiling(const std::string& strGroup) const { return m_strProfilingGroup == strGroup && m_samples <= NUM_PROFILING_SAMPLES; }

    protected:

        CProfilingMgr();

    private:

        void LogResultsAccum(const std::string& strGroup, float frameTime);
        void LogResultsScoped(const std::string& strGroup, float frameTime);
        void LogRecursive(ProfilingTree& tree, const std::string& strIndentation, float accumProfilingTimes);

        float GetAccumProfilingTime(ProfilingTree& tree);

        bool PushProfilerRecursive(const std::string& strGroup, const std::string& strName, ProfilingTree& tree);
        bool PopProfilerRecursive(const std::string& strGroup, float time, ProfilingTree& tree);

        typedef std::pair< float, std::string > pairProfilingResult;
        typedef std::list< pairProfilingResult > listProfilingGroupResults;
        typedef std::map< std::string, listProfilingGroupResults > mapProfilingResults;

        mapProfilingResults m_results;

        std::string m_strProfilingGroup;
        std::map< std::string, ProfilingTreeHead >              m_profilingStacked;
        std::map< std::string, std::map< std::string, float > > m_profilingAccum;

        unsigned long m_samples;
    };

} // env

#endif // PROFILING_MGR_H
