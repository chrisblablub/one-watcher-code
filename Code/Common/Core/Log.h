/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#ifndef LOG_H
#define LOG_H

#include <Common/Common.h>
#include <Common/Core/Singleton.h>

#include <Common/Core/Macros.h>
#define LOG_FILE "Log.txt"

namespace env 
{
    static const unsigned short MAX_CONSOLE_LINES = 500;

    ENV_PLATFORM_WINDOWS_ONLY(void RedirectIOToConsole(););

    class CLog
    {
    public:
        enum LOGTYPE
        {
            LOGTYPE_ERROR = 0,
            LOGTYPE_WARNING,
            LOGTYPE_INFO,
            LOGTYPE_TRACE
        };

        CLog(bool append = false, std::string logFile = std::string("Log.txt"));

        void LogString(const std::string& strMsg, const char* file, const unsigned int line, const char* function,
            LOGTYPE type, int traceLevel = 0);

        void LogString(const std::string& strMsg);

    private:
        
        std::string GetCurTime();

    protected:
        int m_traceLevel;
        std::string m_logFile;
    };

    extern CLog* gLog;

    #ifndef ENV_LOG 
    #define ENV_LOG(String, type, traceLvl) if(gLog) gLog->LogString((String), __FILE__, __LINE__, __FUNCTION__, type, traceLvl) 
    #endif

    #ifndef ENV_LOG_INFO
    #define ENV_LOG_INFO(Info) ENV_LOG(Info, CLog::LOGTYPE_INFO, 0)
    #endif


    #ifndef ENV_LOG_INFO_DBG
    #define ENV_LOG_INFO_DBG(strInfo) ENV_LOG(strInfo, CLog::LOGTYPE_INFO, 0)
    #endif

    #ifndef ENV_LOG_ERROR
    #define ENV_LOG_ERROR(Error) ENV_LOG(Error, CLog::LOGTYPE_ERROR, 0)
    #endif

    #ifndef ENV_LOG_ERROR_SIMPLE
    #define ENV_LOG_ERROR_SIMPLE(Error) if(gLog) gLog->LogString(Error) 
    #endif

    #ifndef ENV_LOG_WARNING
    #define ENV_LOG_WARNING(Warning) ENV_LOG(Warning, CLog::LOGTYPE_WARNING, 0)
    #endif

    #ifndef ENV_LOG_WARNING_DBG
    #define ENV_LOG_WARNING_DBG(Warning) ENV_LOG(Warning, CLog::LOGTYPE_WARNING, 0)
    #endif

    #ifndef ENV_LOG_EXC
    #define ENV_LOG_EXC(x) ENV_LOG_ERROR(x.what());
    #endif
}

#endif // LOG_H