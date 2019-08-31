#include <Common/Core/ProfilingMgr.h>
#include <Common/Core/StringUtility.h>

namespace env
{
    const unsigned int OFFSET = 50;

    //-----------------------------------------------------------------------------------
    inline void LogParagraphSplitterSingle()
    {
        ENV_LOG_INFO("---------------------------------------------------");
    }

    //-----------------------------------------------------------------------------------
    inline void LogParagraphSplitterDouble()
    {
        ENV_LOG_INFO("===================================================");
    }

    //-----------------------------------------------------------------------------------
    inline void LogHeader(const std::string& strName, const std::string& strGroup)
    {
        ENV_LOG_INFO(std::string(""));

        // LogParagraphSplitterSingle();

        std::stringstream stream;
        stream.str(std::string());
        stream << strName << strGroup;
        
        ENV_LOG_INFO(stream.str());

        LogParagraphSplitterDouble();
    }

    //-----------------------------------------------------------------------------------
    inline std::string GetLineSplitter() { return std::string(" | "); }

    //-----------------------------------------------------------------------------------
    template< class T >
    std::string GetFormattedNumberString(const T& n, const std::string& strUnit = std::string())
    {
        std::stringstream strOuput;
        strOuput << std::setprecision(4) << std::fixed << std::setw(6) << std::setfill('0') << n << " " << strUnit;

        return strOuput.str();
    }


    //-----------------------------------------------------------------------------------
    CProfilingMgr::CProfilingMgr()
        : m_samples(0)
    {
//         PushProfiler("testGroup", "event01");
// 
//         PushProfiler("testGroup", "event02");
// 
//         PushProfiler("testGroup", "event03");
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PushProfiler("testGroup", "event04");
// 
//         PushProfiler("testGroup", "event05");
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PushProfiler("testGroup", "event06");
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PushProfiler("testGroup", "event07");
// 
//         PushProfiler("testGroup", "event08");
// 
//         PushProfiler("testGroup", "event09");
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         PopProfiler("testGroup", 0.0f);
// 
//         Print("testGroup");
    }

    //-----------------------------------------------------------------------------------
    CProfilingMgr::~CProfilingMgr()
    {
    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::LogResultsScoped(const std::string& strGroup, float frameTime)
    {
        if (m_profilingStacked.find(strGroup) == m_profilingStacked.end() ||
            m_profilingStacked.find(strGroup)->second.m_profilingStack.empty())
        {
            return;
        }

        float accumProfilingTimes = 0.0f;
        
        for (std::list< ProfilingTree >::iterator it = m_profilingStacked[strGroup].m_profilingStack.begin(); it != m_profilingStacked[strGroup].m_profilingStack.end(); ++it)
        {
            ProfilingTree& subtree = *it;

            accumProfilingTimes += GetAccumProfilingTime(subtree);
        }

        LogHeader("PROFILING: SCOPED", strGroup);

        for (std::list< ProfilingTree >::iterator it = m_profilingStacked[strGroup].m_profilingStack.begin(); it != m_profilingStacked[strGroup].m_profilingStack.end(); ++it)
        {
            ProfilingTree& subtree = *it;

            LogRecursive(subtree, "", accumProfilingTimes);
        }

        float accumProfilingPercent = accumProfilingTimes / (frameTime * 10.0f);

        LogParagraphSplitterDouble();

        ENV_LOG_INFO(GetTruncatedString("PROFILINGTIME ", OFFSET, '_')
                     + GetFormattedNumberString(accumProfilingTimes, std::string("ms")) + GetLineSplitter()
                     + GetFormattedNumberString(1.0f / accumProfilingTimes, std::string("fps")) + GetLineSplitter()
                     + GetFormattedNumberString(accumProfilingPercent, std::string("%")));

        ENV_LOG_INFO(GetTruncatedString("FRAMETIME ", OFFSET, '_')
                     + GetFormattedNumberString(frameTime * 1000.0f, std::string("ms")) + GetLineSplitter()
                     + GetFormattedNumberString(1.0f / frameTime, std::string("fps")));

        LogParagraphSplitterSingle();
    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::LogRecursive(ProfilingTree& tree, const std::string& strIndentation, float accumProfilingTimes)
    {
        ENV_LOG_INFO(GetTruncatedString(strIndentation + tree.m_profilingData.strName + " ", OFFSET, '_')
                     + GetFormattedNumberString(tree.m_profilingData.m_time, std::string("ms")) + GetLineSplitter()
                     + GetFormattedNumberString(tree.m_profilingData.m_time / accumProfilingTimes * 100.0f, std::string("%")));

        for (std::list< ProfilingTree >::iterator it = tree.m_profilingStack.begin(); it != tree.m_profilingStack.end(); ++it)
        {
            LogRecursive(*it, strIndentation + "    ", accumProfilingTimes);
        }

    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::LogResultsAccum(const std::string& strGroup, float frameTime)
    {
        if (m_profilingAccum.find(strGroup) == m_profilingAccum.end())
            return;

        LogHeader("PROFILING: ACCUMULATED", strGroup);

        float accumProfilingResults = 0.0f;

        std::map< std::string, float >& resultsAccum = m_profilingAccum[strGroup];

        for (std::map< std::string, float >::iterator it = resultsAccum.begin(); it != resultsAccum.end(); ++it)
        {
            accumProfilingResults += it->second;

            ENV_LOG_INFO(GetTruncatedString(it->first + " ", OFFSET, '_')
                         + GetFormattedNumberString(it->second, std::string("ms")));
        }

        float accumProfilingPercent = accumProfilingResults / (frameTime * 10.0f);

        LogParagraphSplitterDouble();

        ENV_LOG_INFO(GetTruncatedString("PROFILINGTIME ", OFFSET, '_')
                     + GetFormattedNumberString(accumProfilingResults, std::string("ms")) + GetLineSplitter()
                     + GetFormattedNumberString(1.0f / accumProfilingResults, std::string("fps")) + GetLineSplitter()
                     + GetFormattedNumberString(accumProfilingPercent, std::string("%")));

        ENV_LOG_INFO(GetTruncatedString("FRAMETIME ", OFFSET, '_')
                     + GetFormattedNumberString(frameTime * 1000.0f, std::string("ms")) + GetLineSplitter()
                     + GetFormattedNumberString(1.0f / frameTime, std::string("fps")));

        LogParagraphSplitterSingle();
    }

    //-----------------------------------------------------------------------------------
    float CProfilingMgr::GetAccumProfilingTime(ProfilingTree& tree)
    {
        float accum = tree.m_profilingData.m_time;
        
        for (std::list< ProfilingTree >::iterator it = tree.m_profilingStack.begin(); it != tree.m_profilingStack.end(); ++it)
        {
            accum += GetAccumProfilingTime(*it);
        }

        return accum;
    }


    //-----------------------------------------------------------------------------------
    void CProfilingMgr::SetFrameProfilingGroup(const std::string& strGroup)
    {
        m_strProfilingGroup = strGroup;
    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::PushProfiler(const std::string& strGroup, const std::string& strName)
    {
        if (m_profilingStacked[strGroup].m_profilingStack.empty())
        {
            m_profilingStacked[strGroup].m_profilingStack.push_back(ProfilingTree(ProfilingData(0.0f, strName)));
        }
        else
        {
            // We only need to check the LAST element in the queue - all previous elements must have been processed already
            ProfilingTree& subTree = m_profilingStacked[strGroup].m_profilingStack.back();

            if (subTree.m_profilingData.m_bDone)
            {
                m_profilingStacked[strGroup].m_profilingStack.push_back(ProfilingTree(ProfilingData(0.0f, strName)));
            }
            else
            {
                PushProfilerRecursive(strGroup, strName, m_profilingStacked[strGroup].m_profilingStack.back());
            }
            
        }
    }

    //-----------------------------------------------------------------------------------
    bool CProfilingMgr::PushProfilerRecursive(const std::string& strGroup, const std::string& strName, ProfilingTree& tree)
    {
        if (tree.m_profilingData.m_bDone)
        {
            return false;
        }
        else
        {
            if (tree.m_profilingStack.empty())
            {
                tree.m_profilingStack.push_back(ProfilingTree(ProfilingData(0.0f, strName)));
                return true;
            }
            else
            {
                if (!PushProfilerRecursive(strGroup, strName, tree.m_profilingStack.back()))
                {
                    tree.m_profilingStack.push_back(ProfilingTree(ProfilingData(0.0f, strName)));
                    return true;
                }
                else
                {
                    return true;
                }
            }

        }
    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::PopProfiler(const std::string& strGroup, float time)
    {
        if (m_profilingStacked[strGroup].m_profilingStack.empty())
        {
            return;
        }
        else
        {
            PopProfilerRecursive(strGroup, time, m_profilingStacked[strGroup].m_profilingStack.back());
        }
    }

    //-----------------------------------------------------------------------------------
    bool CProfilingMgr::PopProfilerRecursive(const std::string& strGroup, float time, ProfilingTree& tree)
    {
        if (tree.m_profilingStack.empty())
        {
            tree.m_profilingData.m_bDone = true;
            tree.m_profilingData.m_time = time;

            return true;
        }
        else
        {
            ProfilingTree& subTree = tree.m_profilingStack.back();

            if (!subTree.m_profilingData.m_bDone)
            {
                return (PopProfilerRecursive(strGroup, time, subTree));
            }
            else
            {
                tree.m_profilingData.m_bDone = true;

                return true;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::BeginProfilingFrame()
    {
    }

    //-----------------------------------------------------------------------------------
    void CProfilingMgr::EndProfilingFrame(float deltaTime)
    {
        if (m_strProfilingGroup.empty())
            return;

//         ++m_samples;
// 
//         if (m_samples <= NUM_PROFILING_SAMPLES)
//             return;

        LogResultsScoped(m_strProfilingGroup, deltaTime);
        LogResultsAccum(m_strProfilingGroup, deltaTime);

        m_strProfilingGroup.clear();
        m_results.clear();
        m_profilingAccum.clear();
        m_profilingStacked.clear();

        m_samples = 0;

        

//         std::string strIndentation("    ");
// 
//         for (mapProfilingResults::iterator it = m_results.begin(); it != m_results.end(); ++it)
//         {
//             listProfilingGroupResults& groupResults = it->second;
// 
//             groupResults.sort();
// 
//             float frameTime = deltaTime * 1000.0f;
//             float accumProfilingTimes = 0.0f;
// 
//             for (listProfilingGroupResults::iterator jt = groupResults.begin(); jt != groupResults.end(); ++jt)
//             {
//                 accumProfilingTimes += (*jt).first;
//             }
// 
//             std::stringstream str;
//             str << std::endl;
//             str << "---------------------------------------------------" << std::endl;
//             str << "PROFILING RESULTS OF: " << it->first << std::endl;
//             str << "===================================================" << std::endl;
// 
//             for (listProfilingGroupResults::iterator jt = groupResults.begin(); jt != groupResults.end(); ++jt)
//             {
//                 float percent = (*jt).first / accumProfilingTimes * 100.0f;
// 
//                 std::stringstream strLineTime;
//                 strLineTime.precision(4);
//                 strLineTime << (*jt).first << "ms";
//                 
//                 std::stringstream strLinePercent;
//                 strLinePercent << percent << "%";
// 
//                 str << strIndentation
//                     << GetTruncatedString((*jt).second + ":", 24)
//                     << GetTruncatedString(strLineTime.str(), 18)
//                     << GetTruncatedString(strLinePercent.str(), 10) << std::endl;
//             }
// 
// 
//             float accumPercent = accumProfilingTimes / frameTime * 100.0f;
// 
//             std::stringstream strFrameTime;
//             strFrameTime.precision(4);
//             strFrameTime << frameTime << "ms";
// 
//             std::stringstream strFPS;
//             strFPS.precision(4);
//             strFPS << (1.0f / deltaTime) << "fps";
// 
//             std::stringstream strProfilingTime;
//             strProfilingTime.precision(4);
//             strProfilingTime << accumProfilingTimes << "ms";
// 
//             std::stringstream strProfilingPercent;
//             strProfilingPercent.precision(4);
//             strProfilingPercent << accumPercent << "%";
// 
//             str << "===================================================" << std::endl;
//             str << strIndentation << GetTruncatedString("FRAMETIME:", 24) << GetTruncatedString(strFrameTime.str(), 18) << GetTruncatedString(strFPS.str(), 14) << std::endl;
//             str << strIndentation << GetTruncatedString("PROFILINGTIME:", 24) << GetTruncatedString(strProfilingTime.str(), 18) << GetTruncatedString(strProfilingPercent.str(), 14) << std::endl;
//             str << "-------------------------------------------------- - " << std::endl;
//             str << std::endl;
// 
//             ENV_LOG_INFO(str.str());
//         }
    }

} // env

