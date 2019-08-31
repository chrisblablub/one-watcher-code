#include <Common/Core/Log.h>
#include <Common/Core/StringUtility.h>

namespace env
{
    CLog* gLog = nullptr;

    //-----------------------------------------------------------------------------------
    CLog::CLog(bool append, std::string logFile)
        : m_logFile(logFile)
    {
        //RedirectIOToConsole();

        if (!append)
            std::ofstream ofstr(m_logFile.c_str());

        m_traceLevel = 0;

        std::ofstream ofstr(m_logFile.c_str(), std::ios_base::app);
        std::ostringstream osstr;
        std::string strTime = GetCurTime();
        ofstr << "-------------------------------------------------------------" << std::endl;
        ofstr << "Beginning new log: Engine started at " << GetCurTime() << std::endl;
        ofstr << "-------------------------------------------------------------" << std::endl;
        ofstr << "=============================================================" << std::endl;
        ofstr << osstr.str();
        std::cout << osstr.str().c_str();
    }

    //-----------------------------------------------------------------------------------
    void CLog::LogString(const std::string& strMsg, const char* file, const unsigned int line, const char* function,
        LOGTYPE type, int traceLevel)
    {
        if (traceLevel <= m_traceLevel)
        {
            std::string strType;
            switch (type)
            {
            case LOGTYPE_ERROR:
                strType = "E"; break;
            case LOGTYPE_WARNING:
                strType = "W"; break;
            case LOGTYPE_INFO:
                strType = "I"; break;
            case LOGTYPE_TRACE:
                strType = "T"; break;
            }

            std::string strFileName = GetTruncatedString(file, 32);
            std::string strFunctionName = GetTruncatedString(function, 32);

            std::ofstream ofstr(m_logFile.c_str(), std::ios_base::app);
            std::ostringstream osstr;
            std::string strTime = GetCurTime();
            osstr << strTime << " " << strFileName << "\t " << strFunctionName << "\t L" << line << "\t " << strType << " " << strMsg << "\n";
            ofstr << osstr.str();
            std::cout << osstr.str();
            ENV_PLATFORM_WINDOWS_ONLY(OutputDebugString(osstr.str().c_str()));
        }
    }

    //-----------------------------------------------------------------------------------
    std::string CLog::GetCurTime()
    {
        std::time_t time = std::time(nullptr);

        std::stringstream str;
        str << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

        return str.str();
    }

    //-----------------------------------------------------------------------------------
    void CLog::LogString(const std::string& strMsg)
    {
        std::ofstream ofstr(m_logFile.c_str(), std::ios_base::app);
        std::ostringstream osstr;
        std::string strTime = GetCurTime();
        osstr << strMsg << "\n";
        ofstr << osstr.str();
        std::cout << osstr.str();
        ENV_PLATFORM_WINDOWS_ONLY(OutputDebugString(osstr.str().c_str()));
    }
} // env
